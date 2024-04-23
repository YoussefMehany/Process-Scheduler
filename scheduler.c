#include <sys/sem.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include "headers.h"
#include "process_funcs.c"
#include "priority_queue.h"


struct finish_message_pg {
    long mtype;     
    int finish;      
};


Heap *pr_que;

int is_finish_pg = 0; //0 -> not finish  / 1->finish

void RecieveProcesses();
void rec_handle(int signum);
void HPF(int r);
// void send_process_torun(Process* process);
void rec_finish_pg();
void send_finish_shceduler();
void handler2();


int main(int argc, char * argv[])
{
    signal(SIGUSR1, rec_handle);
    signal(SIGUSR2, handler2);
    initClk();
    //printf("from it self is %d \n",getpid());
    pr_que = createHeap();  // to change when user choice of algo 
    Process* running_proc=NULL;
    while(1){
        // if(!is_finish_pg) rec_finish_pg();
        // if(is_finish_pg )break;
        if(peak(pr_que)!=NULL){
            running_proc = peak(pr_que);
            displayProcess(running_proc);
            pop(pr_que,1);
            // if(running_proc != peak(pr_que)) 
            // {
            //     running_proc = peak(pr_que);
            //     displayProcess(running_proc);
            //     pop(pr_que,1);
            //     // strcpy(running_proc->state,"Running");
            //     // running_proc->startTime = getClk();
            //     // HPF(running_proc->remainingTime);
            // }
        }

    }
   //TODO implement the scheduler :)
   //upon termination release the clock resources.
   //send_finish_shceduler();
   destroyClk(true);
}

void HPF(int r)
{
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        char* args[3];
        args[0] = "./process.out";
        args[1] = (char *)malloc(12);
        sprintf(args[1], "%d", r);
        args[2] = NULL;
        execvp(args[0], args);
        perror("execvp");
        exit(EXIT_FAILURE);
    }
}

void handler2()
{
    Process* p = peak(pr_que);
    strcpy(p->state,"finished");
    displayProcess(p);
    pop(pr_que,1);

}

// void send_process_torun(Process* process){
//     key_t key;
//     int msgid;
//     key = ftok("keyfile", 'D');
//     if (key == -1) {
//         perror("ftok");
//         exit(EXIT_FAILURE);
//     }
//     msgid = msgget(key, 0666 | IPC_CREAT);
//     if (msgid == -1) {
//         perror("msgget");
//         exit(EXIT_FAILURE);
//     }

//     process->mtype=1;
//     if (msgsnd(msgid, process, sizeof(struct Process) - sizeof(long), 0) == -1) {
//         perror("msgsnd");
//         exit(EXIT_FAILURE);
//     }
// }

void RecieveProcesses()
{
    
//printf("Receive Process is entered \n");

    key_t key;
    int msgid;
    struct Process process;
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

    if (msgrcv(msgid, &process, sizeof(struct Process) - sizeof(long), 1,!IPC_NOWAIT ) == -1) {
        perror("msgrcv");
        exit(EXIT_FAILURE);
    }
    // INSTEAD OF DISPLAYING THE PROCESS PUT IN THE QUEUE ACCORDING TO THE CHOSEN ALGO
   Process *p = &process;
   //displayProcess(p);
   push(pr_que,p,1);

}

void rec_handle(int signum)
{
	RecieveProcesses();
  //printf("signal received \n");
}


void rec_finish_pg(){
    key_t key;
    int msgid;
    struct finish_message_pg fm;
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

    if (msgrcv(msgid, &fm, sizeof(struct finish_message_pg) - sizeof(long), 1,!IPC_NOWAIT ) == -1) {
        perror("msgrcv");
        exit(EXIT_FAILURE);
    }
    is_finish_pg=fm.finish;
}

void send_finish_shceduler(){
    key_t key;
    int msgid;
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
    struct finish_message_pg fm;
    
    fm.mtype=1;
    fm.finish=0;
    if (msgsnd(msgid, &fm, sizeof(struct finish_message_pg) - sizeof(long), 0) == -1) {
        perror("msgsnd");
        exit(EXIT_FAILURE);
    }
}
