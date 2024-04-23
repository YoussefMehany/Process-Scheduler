#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "process_funcs.h"

#define INITIAL_CAPACITY 10

typedef struct Heap {
    Process** tree;
    int N;
    int count;
    int capacity;
} Heap;

Heap* createHeap();
void destroyHeap(Heap* heap);
void resize(Heap* heap);
bool isEmpty(Heap* heap);
void heapify(Heap* heap, int idx,int ispiriority);
void push(Heap* heap, Process* newProcess,int ispiriority);
Process* peak(Heap* heap);
void pop(Heap* heap,int ispiriority);

#endif
