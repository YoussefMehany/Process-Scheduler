
#include "headers.h"
#include "process_funcs.c"
#include "priority_queue.h"


/* Modify this file as needed*/
int remainingtime;


int main(int agrc, char * argv[])
{
    initClk();
    int currenttime = getClk();

    remainingtime = atoi(argv[1]);
    //TODO it needs to get the remaining time from somewhere
    //remainingtime = ??;
    while (remainingtime > 0)
    {
        if(getClk()!=currenttime) {
            remainingtime--;
            currenttime = getClk();
        }
        // remainingtime = ??;
    }
    kill(getppid(),SIGUSR2);
    destroyClk(false);
    
    return 0;
}

