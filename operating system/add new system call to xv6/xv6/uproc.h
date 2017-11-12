//-------------------uproc.h-------------------------------
/* This is the declearation file of the uproc struct,
   which is needed by the function getprocs in user.h file
*/

enum procstate { UNUSED, EMBRYO, SLEEPING, RUNNABLE, RUNNING, ZOMBIE };

struct uproc{
    int pid;
    int state;
    uint sz;
    char name[16];
};


