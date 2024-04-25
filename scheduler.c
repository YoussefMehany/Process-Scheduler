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

struct msgbuf {
    long mtype;
    char mtext[30];
};
struct finish_message_pg {
    long mtype;     
    int finish;      
};

void *ready_queue;
Process* running_proc;
int is_finish_pg = 0;
int currentTime;
bool prio_flag = false;
int algo;

void receiveProcess();
void createProcess(Process* p);
void HPF();
void SRTN();
void runPeak();
void rec_finish_pg();
void send_finish_scheduler();
void handler2();
Process* stringtoProcess(char* str);



int main(int argc, char * argv[])
{
    signal(SIGUSR1, receiveProcess);
    signal(SIGUSR2, handler2);
    initClk();
    algo = atoi(argv[1]);
    prio_flag = (algo == 3);
    switch(algo) {
        case 1:
            break;
        case 2:
            SRTN();
            break;
        case 3:
            HPF();
            break;
    }
   //TODO implement the scheduler :)
   //upon termination release the clock resources.
   send_finish_scheduler();
   destroyClk(true);
}

void HPF()
{
    ready_queue = (Heap*) createHeap(); 
    running_proc = NULL;
    while(1) {
        if(!is_finish_pg) rec_finish_pg();
        if(is_finish_pg && isEmpty(ready_queue) && running_proc == NULL) break;
        if(!isEmpty(ready_queue)) {
            if(running_proc == NULL) {
                running_proc = peak(ready_queue);
                createProcess(running_proc);
                strcpy(running_proc->state, "Running");
            }
        }

    }
}
void runPeak() {
    running_proc = peak(ready_queue);
    if(running_proc->runtime != running_proc->remainingTime)
        kill(running_proc->pid, SIGCONT);
    else createProcess(running_proc);
    currentTime = getClk();
    strcpy(running_proc->state, "Running");
}
void SRTN() {
    running_proc = NULL;
    ready_queue = (Heap*)createHeap(); 
    while(1) {
        if(!is_finish_pg) rec_finish_pg();
        if(is_finish_pg && isEmpty(ready_queue) && running_proc == NULL) break;
        if(!isEmpty(ready_queue)) {
            if(running_proc != peak(ready_queue)) {
                if(running_proc) {
                    kill(running_proc->pid, SIGSTOP);
                    strcpy(running_proc->state, "Ready");
                }
                runPeak();
            }
        }
        if(getClk() > currentTime) {
            currentTime = getClk();
            if(running_proc)
                running_proc->remainingTime--;
        }
    }
}
void createProcess(Process* p) {
    int r = p->remainingTime;
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
    p->pid = pid;
    running_proc->startTime = getClk();
}

void handler2() {
    strcpy(running_proc->state, "finished");
    running_proc->remainingTime = 0;
    displayProcess(running_proc);
    pop(ready_queue, prio_flag);
    running_proc = NULL;
}

Process* stringtoProcess(char* str) {
    Process* p = (Process*)malloc(sizeof(Process));
    sscanf(str, "%d %d %d %d %d %d %d %s", &p->id, &p->arrival, &p->startTime, &p->runtime, &p->priority, &p->WaitingTime, &p->remainingTime, p->state);
    return p;
}

void receiveProcess() {
    key_t key;
    int msgid;
    struct msgbuf buffer;
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
    memset(&buffer, 0, sizeof(buffer));
    if (msgrcv(msgid, &buffer, sizeof(buffer.mtext), 5, !IPC_NOWAIT) == -1) {
        perror("msgrcv");
        exit(EXIT_FAILURE);
    }
    Process *p = stringtoProcess(buffer.mtext);
    // INSTEAD OF DISPLAYING THE PROCESS PUT IN THE QUEUE ACCORDING TO THE CHOSEN ALGO
    //displayProcess(p);
    push(ready_queue, p, prio_flag);
}

void rec_finish_pg() {
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
    if (msgrcv(msgid, &fm, sizeof(struct finish_message_pg) - sizeof(long), 1, !IPC_NOWAIT) == -1) {
        perror("msgrcv");
        exit(EXIT_FAILURE);
    }
    is_finish_pg = fm.finish;
}

void send_finish_scheduler(){
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
    
    fm.mtype = 1;
    fm.finish = 0;
    if (msgsnd(msgid, &fm, sizeof(struct finish_message_pg) - sizeof(long), 0) == -1) {
        perror("msgsnd");
        exit(EXIT_FAILURE);
    }
}
