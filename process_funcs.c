#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "process_funcs.h"

void displayProcess(const struct Process *p) {
    if(!p) return;
    printf("ID: %d\n", p->id);
    printf("Arrival Time: %d\n", p->arrival);
    printf("Start Time: %d\n", p->startTime);
    printf("Runtime: %d\n", p->runtime);
    printf("Priority: %d\n", p->priority);
    printf("Waiting Time: %d\n", p->WaitingTime);
    printf("Remaining Time: %d\n", p->remainingTime);
    printf("State: %s\n", p->state);
    fflush(stdout);
}
int GetNumProcesses(char *filename)
{
    FILE* file = fopen(filename,"r");;
    char line[100];
    int numProcesses = -1;
    while (fgets(line, sizeof(line), file)) {
        numProcesses++;
    }
    return numProcesses;
}
void initProcess(struct Process *p, int id, int arrival, int starttime, int runtime, int priority, int WaitingTime, int remainingTime, const char *state, int memorySize) {
    if(!p) return;
    p->id = id;
    p->arrival = arrival;
    p->startTime= starttime;
    p->runtime = runtime;
    p->priority = priority;
    p->WaitingTime = WaitingTime;
    p->remainingTime = remainingTime;
    p->Sent = 0;
    p->memorySize = memorySize;
    strcpy(p->state, state);
}

void readProcessesFromFile(char *filename, struct Process **processes) {
    FILE* file = fopen(filename,"r");
    char line[100];
    int index = 0;
    fseek(file, 0, SEEK_SET);
    fgets(line, sizeof(line), file);
    while (fgets(line, sizeof(line), file)) {
        int id, arrival, runtime, priority, memorySize;
        sscanf(line, "%d %d %d %d %d", &id, &arrival, &runtime, &priority, &memorySize);
        processes[index] = malloc(sizeof(struct Process));
        initProcess(processes[index], id, arrival, 0, runtime, priority, 0, runtime, "waiting", memorySize);
        index++;
    }

    fclose(file);
}

FILE* openFile(char filename[]) {
    FILE* outfile;
    outfile = fopen(filename, "w");
    return outfile;
}

void WriteToFile(char text[], FILE* file)
{
    if (file != NULL) {
        fprintf(file, "%s\n", text);
        fflush(file);
    }
}

void closeFile(FILE* file) {
    fclose(file);
}
