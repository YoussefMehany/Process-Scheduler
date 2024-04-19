#ifndef PROCESS_H
#define PROCESS_H

// Define the Process structure
typedef struct Process {
    int id;
    int arrival;
    int runtime;
    int priority;
    int WaitingTime;
    int remainingTime;
    char state[20];  // Assuming the state string will not exceed 20 characters
} Process;

// Function prototypes
void initProcess(struct Process *p, int id, int arrival, int runtime, int priority, int WaitingTime, int remainingTime, const char *state);
void displayProcess(const struct Process *p);
void readProcessesFromFile(char *filename, struct Process **processes, int* numProcesses);

#endif