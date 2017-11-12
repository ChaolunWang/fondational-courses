#include "types.h"
#include "stat.h"
#include "user.h"


#define MAX 64


void
uproc_copy(struct uproc * lhs, struct uproc * rhs){
    int i;
    lhs->pid=rhs->pid;
    lhs->state=rhs->state;
    lhs->sz=rhs->sz;
    for(i=0; i<16; ++i)
        lhs->name[i]=rhs->name[i];
}

void
swap(struct uproc *lhs, struct uproc *rhs){
    struct uproc temp;
    uproc_copy(&temp, rhs);
    uproc_copy(rhs, lhs);
    uproc_copy(lhs, &temp);
}

int 
is_smaller(struct uproc *lhs, struct uproc *rhs){
    if( lhs->sz != rhs->sz){
        return lhs->sz < rhs->sz;
        }else{
            return strcmp(lhs->name, rhs->name)<0;
    }
}

int
main(int argc, char *argv[])
{  
    struct uproc table[MAX];
    int i, j;  

    int return_value = getprocs(MAX, table);
    if(return_value == -1)
        printf(1,"Error to execute 'top'");
    
    for(i=1; i<MAX; ++i){                         
        if(table[i-1].state==UNUSED || table[i].state==UNUSED)
            break;
        for(j=i; j>=1; --j){
            if(is_smaller(&table[j-1],&table[j]))
                swap(&table[j-1], &table[j]); 
            else
                break;
        }   
    }

    for(i=0; i<MAX; ++i){
        if(table[i].state!=UNUSED){
  	    printf(1," %d ", table[i].pid);
  	    switch(table[i].state){
  	    case UNUSED:
  		printf(1," %s ", "UNUSED");
  		break;
  	    case EMBRYO:
  		printf(1," %s ", "EMBRYO");
  		break;
  	    case SLEEPING:
  		printf(1," %s ", "SLEEPING");
  		break;
  	    case RUNNABLE:
  		printf(1," %s ", "RUNNABLE");
  		break;
  	    case RUNNING:
  		printf(1," %s ", "RUNNING");
  		break;
  	    case ZOMBIE:
  		printf(1," %s ", "ZOMBIE");
                break;
  	    } 
  	    printf(1," %d %s \n", table[i].sz, table[i].name);
        }else break;
    }
    exit();
}


