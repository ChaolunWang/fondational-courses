#include "types.h"
#include "stat.h"
#include "user.h"
#include "uspinlock.h"

struct shm_cnt{
  struct uspinlock lock;
  int cnt;
};

int
main(int argc, char *argv[])
{  
  int pid;
  int m, i;
  struct shm_cnt *n;

  pid=fork();
  if(pid == 0) 
  {  
    m=shm_open(1024);
    sleep(1);
    if(m ==-1){
        printf(1, "open memory failure");
        exit();
    }
    n=(struct shm_cnt*)m;

    for(i=0; i<10000; ++i){
      uacquire(&(n->lock));
      n->cnt++;
      urelease(&(n->lock));
    }
    printf(1,"%d\n", n->cnt);
    if(shm_close(1024)==-1){
        printf(1, "close memory failure");
        exit();
    }
  }
  else
  {
    m=shm_open(1024);
    sleep(1);
    if(m ==-1){
        printf(1, "open memory failure");
        exit();
    }
    n=(struct shm_cnt*)m;
    for(i=0; i<10000; ++i){
      uacquire(&(n->lock));
      n->cnt++;
      urelease(&(n->lock));
    }
    printf(1,"%d\n",  n->cnt);   
    wait();
    if(shm_close(1024)==-1){
        printf(1, "close memory failure");
        exit();
    }
  } 
  exit();
}

