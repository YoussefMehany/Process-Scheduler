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


int pid_Scheduler;

struct finish_message_pg {
    long mtype;    
    int finish;   
};


struct msgbuf {
    long mtype;
    char mtext[30];
};

void clearResources();
void sendProcess(Process* process);
void initiateScheduler(int algo);
void initiateClkProcess();
void send_finish_pg(int f);
void rec_finish_scheduler();
char* processToString(Process *process);



int main(int argc, char * argv[])
{
    int algo;
    printf("choose your alogrithm: (1)RR (2)SRTN (3)HPF\n");
    scanf("%d", &algo);
    initiateScheduler(algo);
    initiateClkProcess();
    initClk();
    int numProcesses = GetNumProcesses("processes.txt");
    int waitSend = 1;
    Process** processes = malloc(numProcesses * sizeof(struct Process));
    readProcessesFromFile("processes.txt", processes);
    while(waitSend) {
        int currentTime = getClk();
        for (int i = 0; i < numProcesses; i++) {
            if (processes[i]->arrival <= currentTime && processes[i]->Sent == 0) {
                sendProcess(processes[i]);
                processes[i]->Sent = 1;
            }
        }
        for (int i = 0; i < numProcesses; i++) {
            if (processes[i]->Sent == 0) {
                send_finish_pg(0);
                break;
            }
            if(i == numProcesses - 1) {
                send_finish_pg(1);
                waitSend = 0;
            }
        }
    }
    rec_finish_scheduler();
    clearResources();
    return 0;
}



void initiateScheduler(int algo)
{
    pid_t pid = fork();
    pid_Scheduler = pid;

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        char* args[3];
        args[0] = "./scheduler.out";
        args[1] = (char *)malloc(12);
        sprintf(args[1], "%d", algo);
        args[2] = NULL;
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



char *processToString(Process *process) {
    char *str = malloc(30 * sizeof(char));
    sprintf(str, "%d %d %d %d %d %d %d %s", process->id, process->arrival, process->startTime, process->runtime, process->priority, process->WaitingTime, process->remainingTime, process->state);
    return str;
}

void sendProcess(Process *process) {
    kill(pid_Scheduler, SIGUSR1);
    key_t key_up, key_down;
    int msgid_up, msgid_down;
    struct msgbuf buffer_up, buffer_down;
    char *processStr = processToString(process);

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
    printf("Sending process: %s\n", processStr);
    memset(&buffer_up, 0, sizeof(buffer_up));
    memset(&buffer_down, 0, sizeof(buffer_down));

    strncpy(buffer_up.mtext, processStr, sizeof(buffer_up.mtext) - 1);
    buffer_up.mtype = 5;
    if (msgsnd(msgid_up, &buffer_up, sizeof(buffer_up.mtext), 0) == -1) {
        perror("msgsnd");
        exit(EXIT_FAILURE);
    }

    if(msgrcv(msgid_down, &buffer_down, sizeof(buffer_down.mtext), 5, !IPC_NOWAIT) == -1) {
        perror("msgrec");
        exit(EXIT_FAILURE);
    }
    free(processStr);
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
    
    fm.mtype = 1;
    fm.finish = f;
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

    if (msgrcv(msgid, &fm, sizeof(struct finish_message_pg) - sizeof(long), 1, !IPC_NOWAIT) == -1) {
        perror("msgrcv");
        exit(EXIT_FAILURE);
    }
}
void clearResources() {
    key_t key_up, key_down, key_s, key_f;
    int msgid_up, msgid_down, msgid_s, msgid_f;
    key_up = ftok("keyfile", 'A');
    key_down = ftok("keyfile", 'Z');
    key_s = ftok("keyfile", 'B');
    key_f = ftok("keyfile", 'C');
    if (key_up == -1 || key_down == -1 || key_s == -1 || key_f == -1) {
        perror("ftok");
        exit(EXIT_FAILURE);
    }
    msgid_up = msgget(key_up, 0666 | IPC_CREAT);
    msgid_down = msgget(key_down, 0666 | IPC_CREAT);
    msgid_s = msgget(key_s, 0666 | IPC_CREAT);
    msgid_f = msgget(key_f, 0666 | IPC_CREAT);
    if (msgid_up == -1 || msgid_down == -1 || msgid_s == -1 || msgid_f == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }
    if (msgctl(msgid_up, IPC_RMID, NULL) == -1 || msgctl(msgid_down, IPC_RMID, NULL) == -1 || msgctl(msgid_s, IPC_RMID, NULL) == -1 || msgctl(msgid_f, IPC_RMID, NULL) == -1) {
        perror("msgctl");
        exit(EXIT_FAILURE);
    }   
    destroyClk(true);
}

