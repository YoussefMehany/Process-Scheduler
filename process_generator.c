
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>
#include "headers.h"
#include "process_funcs.h"
struct message2 {
    long mtype;    
    int mvalue;   
};
void clearResources(int);
void initiateScheduler();
void initiateClkProcess();
void SendProcesses(Process* process);
void ReceiveChildId();
void handler(int signum);
int schedulerId=5;
int main(int argc, char * argv[])
{
    initiateScheduler();
    initiateClkProcess();
    initClk();
    int numProcesses = GetNumProcesses("processes.txt");
    Process** processes = malloc(numProcesses * sizeof(struct Process));
    readProcessesFromFile("processes.txt", processes);
    ReceiveChildId();
    while(true) {
        int currentTime = getClk();
        for (int i=0; i < numProcesses; i++) {
            if (processes[i]->arrival == currentTime&&processes[i]->Sent==0) {
                SendProcesses(processes[i]);
                processes[i]->Sent=1;
            }
        }
    }
   
   
    signal(SIGINT, clearResources);

    // TODO Initialization
    // 1. Read the input files.
    // 2. Ask the user for the chosen scheduling algorithm and its parameters, if there are any.
    // 3. Initiate and create the scheduler and clock processes.
    // 4. Use this function after creating the clock process to initialize clock
   
    
    // To get time use getClk()
    // TODO Generation Main Loop
    // 5. Create a data structure for processes and provide it with its parameters.
    // 6. Send the information to the scheduler at the appropriate time.
    
    // 7. Clear clock resources
    destroyClk(true);
    return 0;
}
void initiateScheduler()
{
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        char *args[] = {"./scheduler.out", NULL};
        execvp(args[0], args);
        perror("execvp");
        exit(EXIT_FAILURE);
    }
}
void initiateClkProcess()
{
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        char *args[] = {"./clk.out", NULL};
        execvp(args[0], args);
        perror("execvp");
        exit(EXIT_FAILURE);
    }
}
void clearResources(int signum)
{
    //TODO Clears all resources in case of interruption
}
void SendProcesses(Process* process)
{
    //printf("sending signal to %d \n",schedulerId);
    key_t key;
    int msgid;
    key = ftok("keyfile", 'A');
    if (key == -1) {
        perror("ftok");
        exit(EXIT_FAILURE);
    }
    msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    process->mtype=1;
    if (msgsnd(msgid, process, sizeof(struct Process) - sizeof(long), 0) == -1) {
        perror("msgsnd");
        exit(EXIT_FAILURE);
    }
    kill(schedulerId,SIGUSR1);

}
void ReceiveChildId()
{
    key_t key;
    int msgid;
    struct message2 msg;

    key = ftok("keyfile", 'B');
    if (key == -1) {
        perror("ftok");
        exit(EXIT_FAILURE);
    }

    msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    if (msgrcv(msgid, &msg, sizeof(struct message2) - sizeof(long), 1, !IPC_NOWAIT) == -1) {
        perror("msgrcv");
        exit(EXIT_FAILURE);
    }

    //printf("child id received is : %d \n", msg.mvalue);
    schedulerId=msg.mvalue;

}
void handler(int signum)
{
	
}
