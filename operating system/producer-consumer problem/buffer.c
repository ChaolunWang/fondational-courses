/* buffer.cpp */
/* Programmed by: Chaolun Wang
              at: 03/24/2016
*/
#include "buffer.h"

/* the buffer */
buffer_item buffer[BUFFER_SIZE];                               //declear buffer as global variable



int insert_item(buffer_item item){                             //function to insert item into buffer

    if(end>=BUFFER_SIZE || end<0)                              //check if the insertion is valid
        return -1;
    sem_wait(&empty);                                          //to mutual execlusively operate on buffer
    sem_wait(&mutex);

    if(!stop){
        buffer[end]=item;
        if(end<BUFFER_SIZE-1)
            end++;
        else
            end=0;
        sem_post(&mutex);
        sem_post(&full);
    }else{                                                     //the condition that the program is ready to terminate. 
        sem_post(&mutex);
        sem_post(&empty);                                      //do nothing at this point.
    }

    return 0;
}

int remove_item(buffer_item *item){                           //function to remove item
    if(begin>=BUFFER_SIZE || begin<0)                         //check if the deletion is valid
        return -1;
    sem_wait(&full);                                          //to mutual execlsively operate on buffer
    sem_wait(&mutex);

    *item=buffer[begin];
    if(begin<BUFFER_SIZE-1)
        begin++;    
    else
        begin=0;
    sem_post(&mutex);
    sem_post(&empty);
    return 0;
}

void initialize()                                             //to initialize the semiphores and global flags 
{
    begin=0;
    end=0;
    amount=0;
    sem_init(&mutex, 0, 1);
    sem_init(&full, 0, 0);
    sem_init(&empty, 0, BUFFER_SIZE);
    stop=0;
}
