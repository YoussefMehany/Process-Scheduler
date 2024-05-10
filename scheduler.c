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
#include <math.h>
#include "semaphore.h"
#include "headers.h"
#include "circularQueue.h"
#include "process_funcs.c"
#include "priority_queue.h"

struct msgbuf {
    long mtype;
    char mtext[100];
};
struct finish_message_pg {
    long mtype;     
    int finish;      
};
void* ready_queue;
Heap* memory_wait;
int* shared_memory;
Process* running_proc;
int is_finish_pg = 0;
bool prio_flag = false;
int algo;
int TotalRunTime = 0;
int numProcesses;
int TotalWaiting = 0;
int FirstArrival =- 1;
double* WTAs;
int WTAsIndex = 0;
void receiveProcess();
void createProcess(Process* p);
void HPF();
void RR(int quantum);
void SRTN();
void runPeak();
void clearIpcs();
void handler2();
void pg_finish();
void finishStat();
void finishProcess();
void writetoOutput(Process* p, char* str);
Process* stringtoProcess(char* str);
FILE* stat_file;
FILE* schedLog_file;
FILE* memLog_file;
Memory* memory;
int sem;


int main(int argc, char * argv[])
{
    initClk();
    signal(SIGUSR1, receiveProcess);
    signal(SIGUSR2, pg_finish);
    signal(SIGPIPE, handler2);
    signal(SIGINT, clearIpcs);
    
    memory = createMemory(1024, 0, 1023, NULL);
    memory_wait = createHeap();

    schedLog_file = openFile("scheduler.log");
    memLog_file = openFile("memory.log");
    algo = atoi(argv[1]);
    numProcesses = atoi(argv[2]);
    int quantum = atoi(argv[3]);
    WTAs = (double *)malloc(numProcesses * sizeof(double));
    prio_flag = (algo == 3);
    
    sem = init_sema('M');
    set_val(sem, 1);

    switch(algo) {
        case 1:
            RR(quantum);
            break;
        case 2:
            SRTN();
            break;
        case 3:
            HPF();
            break;
    }
   finishStat();
   destroyClk(true);
}

void RR(int quantum)
{
    int shmid = shmget(399, 5, IPC_CREAT | 0666);
    int *shared_memory = (int *)shmat(shmid, (void *)0, 0);
    int q;
    ready_queue = (CircularQueue *)createQueue();
    running_proc = NULL;
    int currentTime;
    while (1) {
        if (is_finish_pg && is_empty(ready_queue) && running_proc == NULL)
            break;
        if (running_proc == NULL) {
            if (!is_empty(ready_queue)) {
                running_proc = getCurrent(ready_queue);
                q = quantum;
                currentTime = getClk();
                if (running_proc->runtime != running_proc->remainingTime) {
                    kill(running_proc->pid, SIGUSR1);
                    kill(running_proc->pid, SIGCONT);
                    writetoOutput(running_proc, "Resumed");
                    strcpy(running_proc->state, "Resumed");
                    displayProcess(running_proc);
                }
                else {
                    createProcess(running_proc);
                    writetoOutput(running_proc, "Started");
                    strcpy(running_proc->state, "Running");
                    displayProcess(running_proc);
                }
            }
        }
        else {
            if (*shared_memory > 1) {
                if (getClk() > currentTime) {
                    currentTime = getClk();
                    running_proc->remainingTime--;
                    q--;
                    if (q == 0) {
                        kill(running_proc->pid, SIGSTOP);
                        strcpy(running_proc->state, "Stopped");
                        writetoOutput(running_proc, "Stopped");
                        displayProcess(running_proc);
                        running_proc = NULL;
                        moveToNext(ready_queue);
                    }
                }
            }
        }
    }
}

void HPF()
{
    ready_queue = (Heap*) createHeap(); 
    running_proc = NULL;
    while(1) {
        if(is_finish_pg && isEmpty(ready_queue) && running_proc == NULL) break;
        if(!isEmpty(ready_queue)) {
            if(running_proc == NULL) {
                running_proc = peak(ready_queue);
                pop(ready_queue, prio_flag);
                createProcess(running_proc);
                writetoOutput(running_proc, "Started");
                strcpy(running_proc->state, "Running");
                displayProcess(running_proc);
            }
        }
    }
}

void runPeak() {
    running_proc = peak(ready_queue);
    if(running_proc->runtime != running_proc->remainingTime) {
        kill(running_proc->pid, SIGUSR1);
        kill(running_proc->pid, SIGCONT);
        writetoOutput(running_proc, "Resumed");
        strcpy(running_proc->state, "Resumed");
        displayProcess(running_proc);
    }
    else {
        *shared_memory = running_proc->runtime;
        createProcess(running_proc);
        writetoOutput(running_proc, "Started");
        strcpy(running_proc->state, "Running");
        displayProcess(running_proc);
    }
}

void SRTN() {
    running_proc = NULL;
    int shmid = shmget(399, 5, IPC_CREAT | 0666);
    shared_memory = (int *) shmat(shmid, (void *)0, 0);
    ready_queue = (Heap*)createHeap(); 
    while(1) {
        if(is_finish_pg && isEmpty(ready_queue) && running_proc == NULL) break;
        if(!isEmpty(ready_queue)) {
            if(running_proc) {
                running_proc->remainingTime = *shared_memory;
            }
            if(running_proc != peak(ready_queue)) {
                if(!peak(ready_queue)) continue;
                if(running_proc) {
                    kill(running_proc->pid, SIGSTOP);
                    writetoOutput(running_proc, "Stopped");
                    strcpy(running_proc->state, "Stopped");
                    displayProcess(running_proc);
                }
                runPeak();
            }
        }
    }
}

void writetoOutput(Process* p, char* str) {
    int clk = getClk();
    int curWaiting = clk - (p->arrival) - (p->runtime) + (p->remainingTime);
    p->WaitingTime = curWaiting;
    int pid = fork();
    if(pid == 0) {
        down(sem);
        char Text[150];
        sprintf(Text, "At Time %d process %d %s arr %d total %d remaining %d wait %d  \n", clk, p->id, str, running_proc->arrival, running_proc->runtime,
        p->remainingTime, p->WaitingTime);
        WriteToFile(Text, schedLog_file);
        up(sem);
        exit(0);
    }
}

void memoryOutput(Process* p, char* str) {
    int clk = getClk();
    int curWaiting = clk - (p->arrival) - (p->runtime) + (p->remainingTime);
    p->WaitingTime = curWaiting;
    char Text[150];
    sprintf(Text, "At Time %d %s %d bytes for process %d from %d to %d\n", clk, str, p->memorySize, p->id, p->memory->start, p->memory->end);
    WriteToFile(Text, memLog_file);
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

void finishProcess() {
    char Text[150];
    int cur_waiting = getClk() - (running_proc->arrival) - (running_proc->runtime) + (running_proc->remainingTime);
    double cur_WTA = (double)(getClk() - running_proc->arrival) / running_proc->runtime;
    WTAs[WTAsIndex++] = cur_WTA;
    TotalWaiting += cur_waiting;
    sprintf(Text, "At Time %d process %d finished arr %d total %d remaining %d wait %d TA %d  WTA %.3f \n", getClk(),  running_proc->id, running_proc->arrival,running_proc-> runtime,
    running_proc-> remainingTime, cur_waiting, getClk() - running_proc->arrival, cur_WTA);
    running_proc->WaitingTime = cur_waiting;
    WriteToFile(Text, schedLog_file);
}

void handler2() {
    strcpy(running_proc->state, "finished");
    running_proc->remainingTime = 0;
    finishProcess();
    displayProcess(running_proc);

    memoryOutput(running_proc, "freed");
    DeleteMemory(memory, running_proc->id);

    if(algo == 1) deleteCurrent(ready_queue);
    if(algo == 2) pop(ready_queue, prio_flag);
    while(!isEmpty(memory_wait)) {
        Process* p = peak(memory_wait);
        Memory* processMemory = InsertMemory(memory, p->memorySize, p->id);
        if(processMemory) {
            p->memory = processMemory;
            memoryOutput(p, "allocated");
            if(algo == 1)
                enqueue(ready_queue, p);
            else push(ready_queue, p, prio_flag);
            pop(memory_wait, 2);
        }else break;
    }
    running_proc = NULL;
}

Process* stringtoProcess(char* str) {
    Process* p = malloc(sizeof(Process));
    sscanf(str, "%d %d %d %d %d %d %d %d %s", &p->id, &p->arrival, &p->startTime, &p->runtime, &p->priority, &p->WaitingTime, &p->remainingTime, &p->memorySize, p->state);
    return p;
}

void receiveProcess() {
    key_t key_up, key_down;
    int msgid_up, msgid_down;
    struct msgbuf buffer_up, buffer_down;
    key_up = ftok("keyfile", 'A');
    key_down = ftok("keyfile", 'Z');
    if (key_up == -1 || key_down == -1) {
        perror("ftok");
        exit(EXIT_FAILURE);
    }

    msgid_up = msgget(key_up, 0666 | IPC_CREAT);
    msgid_down = msgget(key_down, 0666 | IPC_CREAT);
    if (msgid_up == -1 || msgid_down == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }
    memset(&buffer_up, 0, sizeof(buffer_up));
    memset(&buffer_down, 0, sizeof(buffer_down));
    if (msgrcv(msgid_up, &buffer_up, sizeof(buffer_up.mtext), 5, !IPC_NOWAIT) == -1) {
        perror("msgrcv");
        exit(EXIT_FAILURE);
    }
    Process *p = stringtoProcess(buffer_up.mtext);
    TotalRunTime += p->runtime;
    if(FirstArrival == -1)
    FirstArrival = p->arrival;

    Memory* processMemory = InsertMemory(memory, p->memorySize, p->id);
    if(processMemory) {
        p->memory = processMemory;
        memoryOutput(p, "allocated");
        if(algo == 1)
            enqueue(ready_queue, p);
        else push(ready_queue, p, prio_flag);
    }else 
        push(memory_wait, p, 2);
    
    buffer_down.mtype = 5;
    if(msgsnd(msgid_down, &buffer_down, sizeof(buffer_down.mtext), 5) == -1) {
        perror("msgsend");
        exit(EXIT_FAILURE);
    }
}

void pg_finish() {
    is_finish_pg = 1;
}

void clearIpcs() {
    wait(NULL);
    int shmid = shmget(399, 5, IPC_CREAT | 0666);
    shmdt(shared_memory);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("shmctl");
        exit(EXIT_FAILURE);
    }
    DestroyMemory(&memory);
    dis_sem(sem);
    exit(0);
}

void finishStat()
{
    int currentTime = getClk();
    double TotalWTA = 0;
    double StdWTA = 0;

    for(int i = 0; i < numProcesses; i++)
        TotalWTA += WTAs[i];

    double AvgWTA = TotalWTA / numProcesses;

    for(int i = 0; i < numProcesses; i++)
        StdWTA += (WTAs[i] - AvgWTA) *  (WTAs[i] - AvgWTA);

    StdWTA = sqrt(StdWTA / numProcesses);

    char Text[150];
    sprintf(Text, "CPU utilization = %.2f %% \nAvg WTA = %.2f \nAvg Waiting = %.2f \nStd WTA = %.2f",
     (TotalRunTime / (double)(currentTime - FirstArrival)) * 100, AvgWTA, (double)TotalWaiting / numProcesses, StdWTA);
    
    stat_file = openFile("scheduler.perf");
    WriteToFile(Text, stat_file);
    free(WTAs);
}