#include "headers.h"
#include "priority_queue.h"
#define INITIAL_SIZE 10



//int main(int argc, char * argv[])
//{
//    initClk();
//
//    //TODO implement the scheduler :)
//    //upon termination release the clock resources.
//
//    destroyClk(true);
//}

int main(int argc, char * argv[])
{
    Heap heap;
    heap.N = 0;
    heap.capacity = INITIAL_SIZE;
    heap.tree = malloc(heap.capacity * sizeof(int));
    //Testing
//    push(&heap, 110);
//    push(&heap, 120);
//    push(&heap, 54);
//    push(&heap, 57);
//    push(&heap, 50);
//    push(&heap, 35);
//    push(&heap, 75);
//    printf("Peak element: %d\n", peak(&heap));
//    pop(&heap);
//    printf("Peak element: %d\n", peak(&heap));
//    pop(&heap);
//    printf("Peak element: %d\n", peak(&heap));
//    pop(&heap);
//    printf("Peak element: %d\n", peak(&heap));
//    pop(&heap);
//    printf("Peak element: %d\n", peak(&heap));
//    pop(&heap);
//    printf("Peak element: %d\n", peak(&heap));
//    pop(&heap);
//    printf("Peak element: %d\n", peak(&heap));
//    pop(&heap);
    return 0;
}
