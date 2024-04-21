#ifndef PROCESS_H
#define PROCESS_H

typedef struct Process {
    long mtype;
    int id;
    int arrival;
    int runtime;
    int priority;
    int WaitingTime;
    int remainingTime;
    char state[20];  // Assuming the state string will not exceed 20 characters
    int Sent;
} Process;

// Function prototypes
void initProcess(struct Process *p, int id, int arrival, int runtime, int priority, int WaitingTime, int remainingTime, const char *state);
void displayProcess(const struct Process *p);
void readProcessesFromFile(char *filename, struct Process **processes);
int GetNumProcesses(char* filename);

#endif