#ifndef PROCESS_H
#define PROCESS_H

typedef struct Process {
    int id;
    int pid;
    int arrival;
    int startTime;
    int runtime;
    int priority;
    int WaitingTime;
    int remainingTime;
    char state[20];
    int Sent;
} Process;

// Function prototypes
void initProcess(struct Process* p, int id, int arrival, int starttime, int runtime, int priority, int WaitingTime, int remainingTime, const char *state);
void displayProcess(const struct Process* p);
void readProcessesFromFile(char *filename, struct Process** processes);
int GetNumProcesses(char* filename);
FILE* openFile(char filename[]);
void WriteToFile(char text[], FILE* file);
void closeFile(FILE* file);

#endif