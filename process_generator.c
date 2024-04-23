
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
#include "priority_queue.h"


int pid_Scheduler;

struct finish_message_pg {
    long mtype;    
    int finish;   
};

void clearResources(int);
void initiateScheduler();
void initiateClkProcess();
void SendProcesses(Process* process);
void send_finish_pg(int f);
void rec_finish_scheduler();
void handler(int signum);


int main(int argc, char * argv[])
{
    //signal(SIGINT, clearResources);
    //initiateScheduler();
    initiateClkProcess();
    initClk();
    Heap* que = createHeap();
    int numProcesses = GetNumProcesses("processes.txt");
    Process** processes = malloc(numProcesses * sizeof(struct Process));
    readProcessesFromFile("processes.txt", processes);
    while(true) {
        int currentTime = getClk();
        for (int i=0; i < numProcesses; i++) {
            if (processes[i]->arrival == currentTime&&processes[i]->Sent==0) {
                //SendProcesses(processes[i]);
                displayProcess(processes[i]);
                push(que,processes[i],1);
                processes[i]->Sent=1;
            }
        }
        // for (int i=0; i < numProcesses; i++) {
        //     // if (processes[i]->Sent==0) {
        //     //     send_finish_pg(0);
        //     //     break;
        //     // }
        //     if(i==numProcesses-1){
        //         //send_finish_pg(1);
        //         goto check_finish_scheduler;
        //     }
        // }
    }
    //  check_finish_scheduler:
    //  printf("a7aaaaaaaaaaaaaaaaa\n");
    // //     rec_finish_scheduler();
    // while(peak(que)!=NULL){
    //     displayProcess(peak(que));
    //     pop(que,1);
    // }

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

void handler(int signum)
{
	
}

void initiateScheduler()
{
    pid_t pid = fork();
    pid_Scheduler=pid;

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
    kill(pid_Scheduler,SIGUSR1);

}

void send_finish_pg(int f){
    key_t key;
    int msgid;
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
    struct finish_message_pg fm;
    
    fm.mtype=1;
    fm.finish=f;
    if (msgsnd(msgid, &fm, sizeof(struct finish_message_pg) - sizeof(long), 0) == -1) {
        perror("msgsnd");
        exit(EXIT_FAILURE);
    }
}

void rec_finish_scheduler()
{
    key_t key;
    int msgid;
    struct finish_message_pg fm;
    key = ftok("keyfile", 'C');
    if (key == -1) {
        perror("ftok");
        exit(EXIT_FAILURE);
    }

    msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    if (msgrcv(msgid, &fm, sizeof(struct finish_message_pg) - sizeof(long), 1,!IPC_NOWAIT ) == -1) {
        perror("msgrcv");
        exit(EXIT_FAILURE);
    }
}


