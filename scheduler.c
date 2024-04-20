#include "headers.h"
#include "priority_queue.h"


int main(int argc, char * argv[])
{
   initClk();
   printf("Scheduler initialized successfully\n");
   //TODO implement the scheduler :)
   //upon termination release the clock resources.
   destroyClk(true);
}
