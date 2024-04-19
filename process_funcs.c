#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process_funcs.h"

void displayProcess(const struct Process *p) {
    printf("ID: %d\n", p->id);
    printf("Arrival Time: %d\n", p->arrival);
    printf("Runtime: %d\n", p->runtime);
    printf("Priority: %d\n", p->priority);
    printf("Waiting Time: %d\n", p->WaitingTime);
    printf("Remaining Time: %d\n", p->remainingTime);
    printf("State: %s\n", p->state);
}
void initProcess(struct Process *p, int id, int arrival, int runtime, int priority, int WaitingTime, int remainingTime, const char *state) {
    p = malloc(sizeof(struct Process));
    p->id = id;
    p->arrival = arrival;
    p->runtime = runtime;
    p->priority = priority;
    p->WaitingTime = WaitingTime;
    p->remainingTime = remainingTime;
    strcpy(p->state, state);
}

void readProcessesFromFile(char *filename, struct Process **processes, int* numProcesses) {
    FILE* file = fopen(filename,"r");;
    char line[100];
    *numProcesses = 0;
    while (fgets(line, sizeof(line), file)) {
        (*numProcesses)++;
    }
    processes = malloc(*numProcesses * sizeof(struct Process));
    int index = 0;
    fseek(file, 0, SEEK_SET);
    while (fgets(line, sizeof(line), file)) {
        int id, arrival, runtime, priority;
        sscanf(line, "%d %d %d %d", &id, &arrival, &runtime, &priority);
        initProcess(processes[index], id, arrival, runtime, priority, 0, runtime, "started");
        index++;
    }

    fclose(file);
}
