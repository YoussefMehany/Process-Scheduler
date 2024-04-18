#include "circularQueue.h"

int main() {
    enqueue(10);
    enqueue(20);
    enqueue(30);
    print();

    printf("Dequeued element: %d\n", dequeue());
    print();

    return 0;
}