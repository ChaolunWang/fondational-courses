#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return proc->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = proc->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;
  
  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(proc->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;
  
  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

extern struct proc* getproc();

int
sys_getprocs(void)
{
    int max, i, j;
    char *table;

    if(argint(0, &max) < 0 || argptr(1, &table, max)<0)
        return -1;

    struct proc *ptable=getproc();

    for(i=0; i<max; ++i){
        if(ptable->state==UNUSED)
            break;
        *(int*)(table+i*28)=ptable->pid;
        *(int*)(table+i*28+4)=ptable->state;
        *(uint*)(table+i*28+8)=ptable->sz;
        for(j=0; j<16; ++j)
            *(table+i*28+12+j)=(ptable->name)[j];
        ++ptable;
    }
    return 0;    
}


