#include "headers.h"
#include <stdbool.h>
#include "process_funcs.c"
#include "priority_queue.h"



int main(int agrc, char * argv[])
{
    initClk();
    int currentTime = getClk();
    int shmid = shmget(399, 4, IPC_CREAT | 0666);
    int* shared_memory = (int *) shmat(shmid, (void *)0, 0);

    int remainingtime = atoi(argv[1]);

    *shared_memory = remainingtime;
    while (remainingtime > 0) {
        if(getClk() > currentTime) {
            remainingtime--;
            *shared_memory = remainingtime;
            currentTime = getClk();
        }
    }
    kill(getppid(), SIGPIPE);
    destroyClk(false);
    return 0;
}

