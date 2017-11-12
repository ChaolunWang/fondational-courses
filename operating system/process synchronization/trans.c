//--------------------------------------trans.c------------------------
/*
   This is a c program which can transfer the data form one file to another.
   This program is implemented by branching the process and separate their function
   into reading and writing. Inter process communication is realized by shared
   memory. Inter process communication is realized by two pipes connect child and 
   parent process.
   programmed by: Chaolun Wang
              at: 03/17/2016
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

const int SIZE = 4096;

int
main(int argc, char *argv[])
{
	int pipeptc[2], pipectp[2];                                        //initialize two pipelines
	int cpid, blocknum=1, blocklen, receivednum , receivedlen ;
	char *name = "SM";
	char c;
	int shm, i;
	void *ptr;
	if(argc !=3){
		printf("Wrong number of file\n");
		exit(EXIT_FAILURE);
	}
    
	if(pipe(pipeptc)==-1 || pipe(pipectp)==-1){                         //create two pipelines
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	
	cpid=fork();
	if(cpid==-1){
		perror("fork");
		exit(EXIT_FAILURE);
	}
	
	if(cpid==0){                                                     //child
	    close(pipeptc[1]);                                           //close unecessary pipe end
		close(pipectp[0]); 
		FILE *out;                                                   //creat file object, open file
	    out=fopen(argv[2], "w");
	
		shm = shm_open(name, O_RDONLY, 0666);                        //create shared memory
	    if (shm == -1) {
		    perror("shared memory");
		    exit(EXIT_FAILURE);
	    }
	
	    ptr = mmap(0,SIZE, PROT_READ, MAP_SHARED, shm, 0);           //map shared memory
	    if (ptr == MAP_FAILED) {
		    perror("maping");
		    exit(EXIT_FAILURE);
	    }
		
		while (1){			  
        read(pipeptc[0], &receivednum, 8);                            //read information about shared memory
		read(pipeptc[0], &receivedlen, 8);

	    for(i=0; i<receivedlen; ++i){                                 //output
		    fputc(((char*)ptr)[i], out);
		}

		write(pipectp[1], &receivednum, 8);                           //sent feedback to parent
 
        if (receivednum==0)
            break;			
		}
		
		close(pipeptc[0]);                              
		close(pipectp[1]); 
		fclose(out);
        exit(EXIT_SUCCESS);		
	
	} else{                                       		                     //parent section
		
	    FILE *in;
	    in=fopen(argv[1], "r");
        if(!in){
		    perror("Invalid input file");
		    exit(EXIT_FAILURE);			
		}
		close(pipeptc[0]);	                                                 //close unecessary pipe ends
		close(pipectp[1]);


	    shm = shm_open(name, O_CREAT | O_RDWR, 0666);                        //open shared memory
	    if (shm == -1) {
		    perror("shared memory");
		    exit(EXIT_FAILURE);
	    }
		
	    ftruncate(shm,SIZE);                                                 //makesure the size is 4KB
		ptr = mmap(0,SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm, 0);   //map the memory
		if (ptr == MAP_FAILED) {
		    perror("maping");
		    exit(EXIT_FAILURE);
	    }
		
		for(blocklen=0; blocklen<SIZE; ++blocklen){                          //starting step
		    c=fgetc(in);
			if(c==EOF)
				break;
			((char*)ptr)[blocklen]=c;   
		}		
	
        write(pipeptc[1], &blocknum, 8);              
		write(pipeptc[1], &blocklen, 8);
		
	    while (1){
            read(pipectp[0], &blocknum, 8);                                //waiting to get feedback form child
					
			for(blocklen=0; blocklen<SIZE; ++blocklen){
				c=fgetc(in);
			    if(c==EOF)                                                 //check if file is empty
				    break;
			    *(char*)(ptr+blocklen)=c;
		    }
			
            if(blocklen==0)                                                //if file empty, break
			    break;							
			++blocknum;
	
		    write(pipeptc[1], &blocknum, 8);                              //senting the information about memory to child
		    write(pipeptc[1], &blocklen, 8);
		}
		blocknum=0;
		write(pipeptc[1], &blocknum, 8);                                    //termination step
		write(pipeptc[1], &blocklen, 8);
        read(pipectp[0], &blocknum, 8);	
		
		ptr=0;                                                             //set ptr=0;
		
		if(blocknum==0){
		    close(pipeptc[1]);	
		    close(pipectp[0]);
            wait(NULL);
		    fclose(in);
            exit(EXIT_SUCCESS);
	    } else{
			perror("process termination");
			exit(EXIT_FAILURE);
		}	     		
	}
	return 0;
}


