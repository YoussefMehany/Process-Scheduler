#ifndef CIRCULARQUEUE_H
#define CIRCULARQUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include "process_funcs.h"

#define INITIAL_CAPACITY 10

extern Process** queue;
extern int front, rear, capacity;

int isFull();
int isEmpty();
void resize();
void enqueue(Process* data);
Process* dequeue();
void print();

#endif // CIRCULARQUEUE_H
