
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
void RecieveProcesses();
void handler(int signum);
void SendId();
struct message2 {
    long mtype;     
    int mvalue;      
};
int main(int argc, char * argv[])
{
    signal(SIGUSR1, handler);
    initClk();
    //printf("from it self is %d \n",getpid());
    SendId();
while(1){}
   //TODO implement the scheduler :)
   //upon termination release the clock resources.
   destroyClk(true);
}

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

    msgid = msgget(key, 0666);
    if (msgid == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    if (msgrcv(msgid, &process, sizeof(struct Process) - sizeof(long), 1,!IPC_NOWAIT ) == -1) {
        perror("msgrcv");
        exit(EXIT_FAILURE);
    }
    // INSTEAD OF DISPLAYING THE PROCESS PUT IN THE QUEUE ACCORDING TO THE CHOSEN ALGO
   displayProcess(&process);


}
void handler(int signum)
{
	RecieveProcesses();
  //printf("signal received \n");
}
void SendId()
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
    msg.mtype=1;
    msg.mvalue=getpid();
    
     if (msgsnd(msgid, &msg, sizeof(struct message2) - sizeof(long), 0) == -1) {
        perror("msgsnd");
        exit(EXIT_FAILURE);
    }
    //printf("id was sent \n");
}