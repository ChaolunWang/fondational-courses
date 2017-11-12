//-----------------main.cpp-------------------------
/* This is an implementation of a producer/consumer problem
   in which the pthread was used for thread creation and 
   syncronization
   Programmed by: Chaolun Wang
              at: 04/04/2016
*/

#include "buffer.h"
#include <stdlib.h>    
#include <time.h>  
#include <signal.h>
#include <unistd.h>
#include <string.h>

const int PRODUCER_SLEEP_TIME=3;
const int CONSUMER_SLEEP_TIME=3;


void *produce();                             //producer
void *consume();                             //consumer


void sig_handler(int signo)                  //signal handler to catch signal control+C
{
  if (signo == SIGINT)
    stop=1;
}



int
main(int argc, char *argv[]) {
    /*signal handeling*/
    if (signal(SIGINT, sig_handler) == SIG_ERR){
        printf("can't catch SIGINT\n");
    }
    /*Get command line arguments argv[1],argv[2],argv[3] */ // 1) how long to sleep 2) number of producer 3)num of consumer
    if (argc!=4){
        printf("Wrong number of parameters\n");
        exit(-1);
    }
    int numpd=atoi(argv[2]);
    int numcs=atoi(argv[3]);
    int sleeptime=atoi(argv[1]);

    if (numpd<=0 || numcs<=0 || sleeptime<0){
        printf("Invalid parameters\n");
        exit(-1);
    }

    int i, rt;

    /* Initialize buffer and thread condition*/
    pthread_t producer[numpd];
    pthread_t consumer[numcs];
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    initialize();
   

   /* Create producer thread(s) */
    for(i=0; i<numpd; ++i){
        rt=pthread_create(&producer[i], &attr, produce, NULL);
        if(rt){
            printf("error with producer thread creation\n");
            exit(-1);
        }
    }

    /* Create consumer thread(s) */
    for(i=0; i<numcs; ++i){
        rt=pthread_create(&consumer[i], &attr, consume, NULL);
        if(rt){
            printf("error with consumer thread creation\n");
            exit(-1);
        }
    }

    /* Sleep */
    if(!stop){
        sleep(sleeptime);
        stop=1;
    }

    /*Termination and Exit */
 
   
    for(i=0; i<numpd; ++i){                                         //join producer threads
        rt=pthread_join(producer[i], NULL);
        if(rt){
            printf("error with producer thread joining\n");
            exit(-1);
        }
    }    

    stop=0;                                                        //set the stop flag back to 0 ,CRITICAL STEP

    for(i=0; i<numcs; ++i){                                        //put -1 into buffer to terminate consumers
        rt=insert_item(-1);
        if(rt==-1){
            printf("error with stopping consumer thread\n");  
            exit(-1);
        }     
    }

    for(i=0; i<numcs; ++i){                                       //join consumer threads
        rt=pthread_join(consumer[i], NULL);
        if(rt){
            printf("error with consumer thread joining\n");
            exit(-1);
        }
    }    

    pthread_attr_destroy(&attr);

    pthread_exit(NULL);
    return 0;
}



void *produce()                                                  //can terminate when receiving 1 from stop flag
{
   int item;
   int sleep_time;
   srand (time(NULL)+pthread_self());
   while(1)
   {
       sleep_time= rand()%(PRODUCER_SLEEP_TIME);
       sleep(sleep_time);
       item = rand()%(RAND_MAX);
       
       if(insert_item(item)==0){
           if(!stop)
               printf("producer produced %d\n", item);
       }else{
           printf("error to insert item");
       }
       if(stop)
           break;
   }
   pthread_exit(NULL);
}

void *consume()                                                  //can terminate when receiving -1 from buffer
{
   int item;
   int sleep_time;
   srand (time(NULL)+pthread_self());
   while(1)
   {
       sleep_time= rand()%(CONSUMER_SLEEP_TIME);
       sleep(sleep_time);
       if(remove_item(&item)==0){
           if(item==-1)
               break;
           else
               printf("consumer consumed %d\n", item);
       }else{
            printf("error to remove item");
       }

   }
   pthread_exit(NULL);
}


