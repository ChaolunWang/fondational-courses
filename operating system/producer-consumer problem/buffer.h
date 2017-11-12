/* buffer.h */
/* Programmed by: Chaolun Wang
              at: 03/24/2016
*/

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

     
typedef int buffer_item;
#define BUFFER_SIZE 5

int begin;
int end;
int amount;
sem_t mutex;
sem_t full;
sem_t empty;
int insert_item(buffer_item item);
int remove_item(buffer_item *item);
void initialize();
int stop;                                                      //the flag indicate whether the stop have happened
