#ifndef CIRCULAR_QUEUE_H
#define CIRCULAR_QUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include "process_funcs.h"


typedef struct Node {
    Process* data;
    struct Node* next;
} Node;


typedef struct CircularQueue {
    Node* head;
    Node* tail;
    Node* current;
} CircularQueue;

CircularQueue* createQueue();
void deleteQueue(CircularQueue* queue);
int isEmpty(CircularQueue* queue);
void enqueue(CircularQueue* queue, Process* data);
Process* dequeue(CircularQueue* queue);
void print(CircularQueue* queue);
void moveToNext(CircularQueue* queue);
void deleteCurrent(CircularQueue* queue);
#endif /* CIRCULAR_QUEUE_H */
