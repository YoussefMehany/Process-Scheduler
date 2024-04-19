#include "headers.h"
#include "process_funcs.h"

void clearResources(int);
void initiateScheduler();
void initiateClkProcess();

int main(int argc, char * argv[])
{
    signal(SIGINT, clearResources);
    // TODO Initialization
    // 1. Read the input files.
    // 2. Ask the user for the chosen scheduling algorithm and its parameters, if there are any.
    // 3. Initiate and create the scheduler and clock processes.
    // 4. Use this function after creating the clock process to initialize clock
    Process** processes;
    int numProcesses = 0;
    readProcessesFromFile("processes.txt", processes, &numProcesses);
    initiateScheduler();
    initiateClkProcess();
    initClk();
    // To get time use getClk()
    // TODO Generation Main Loop
    // 5. Create a data structure for processes and provide it with its parameters.
    // 6. Send the information to the scheduler at the appropriate time.
    while(true) {
        int currentTime = getClk();
        for (int i = 0; i < numProcesses; i++) {
            if (processes[i]->arrival == currentTime) {
                // Send the process to the scheduler
            }
        }
    }
    // 7. Clear clock resources
    destroyClk(true);
    return 0;
}
void initiateScheduler()
{
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        char *args[] = {"./scheduler.out", NULL};
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
void clearResources(int signum)
{
    //TODO Clears all resources in case of interruption
}
