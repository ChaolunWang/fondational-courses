#include "types.h"
#include "x86.h"
#include "uspinlock.h"

void uacquire(struct uspinlock *lock){
    while(xchg(&(lock->locked), 1) != 0)
    ;
}

void urelease(struct uspinlock *lock){
    xchg(&(lock->locked), 0);
}

void uinitlock(struct uspinlock *lock){
    lock->locked=0;
}
