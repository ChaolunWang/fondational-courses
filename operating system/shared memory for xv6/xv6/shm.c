#include "types.h"
#include "spinlock.h"
#include "defs.h"


struct{
  struct spinlock lock;
  struct shm_page{
    uint id;
    char *frame;
    int refcnt;
  }shm_pages[64];
}shm_table;



