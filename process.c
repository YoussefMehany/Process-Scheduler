#include "headers.h"
#include <stdbool.h>
#include <signal.h>
#include "process_funcs.c"
#include "priority_queue.h"

int currentTime;

void updateCurrent() {
    currentTime = getClk();
}
int main(int agrc, char * argv[])
{
    signal(SIGUSR1, updateCurrent);
    initClk();
    int shmid = shmget(399, 5, IPC_CREAT | 0666);
    int* shared_memory = (int *) shmat(shmid, (void *)0, 0);

    int remainingtime = atoi(argv[1]);
    currentTime = getClk();
    *shared_memory = remainingtime;

    while (remainingtime > 0) {
        int time = getClk();
        if(time > currentTime) {
            remainingtime--;
            *shared_memory = remainingtime;
            currentTime = time;
        }
    }
    kill(getppid(), SIGPIPE);
    destroyClk(false);
    return 0;
}

