#include "priority_queue.h"

void resize(Heap* heap) {
    heap->capacity *= 2;
    heap->tree = realloc(heap->tree, heap->capacity * sizeof(Process*));
    if (heap->tree == NULL) {
        printf("Memory reallocation failed\n");
        exit(1);
    }
}

void heapify(Heap* heap, int idx, int ispriority) {
    if (idx > heap->N) return;
    int left = 2 * idx;
    int right = 2 * idx + 1;
    int smallest = idx;
    if(ispriority == 0)
    {
        if (left <= heap->N && heap->tree[left]->remainingTime < heap->tree[smallest]->remainingTime) {
            smallest = left;
        }
        if (right <= heap->N && heap->tree[right]->remainingTime < heap->tree[smallest]->remainingTime) {
            smallest = right;
        }
        if (smallest != idx) {
            Process* temp = heap->tree[smallest];
            heap->tree[smallest] = heap->tree[idx];
            heap->tree[idx] = temp;
            heapify(heap, smallest, ispriority);
        }
    }
    else{
        if (left <= heap->N && heap->tree[left]->priority < heap->tree[smallest]->priority) {
            smallest = left;
        }
        if (right <= heap->N && heap->tree[right]->priority < heap->tree[smallest]->priority) {
            smallest = right;
        }
        if (smallest != idx) {
            Process* temp = heap->tree[smallest];
            heap->tree[smallest] = heap->tree[idx];
            heap->tree[idx] = temp;
            heapify(heap, smallest, ispriority);
        }
    }
}

void push(Heap* heap, Process* newProcess, int ispriority) {
    if (heap->N + 1 >= heap->capacity) {
        resize(heap);
    }
    (heap->N)++;
    heap->tree[heap->N] = newProcess;
    heap->count++;
    int current = heap->N;
    if(ispriority == 0){
        while (current > 1 && heap->tree[current]->remainingTime < heap->tree[current / 2]->remainingTime) {
            Process* temp = heap->tree[current];
            heap->tree[current] = heap->tree[current / 2];
            heap->tree[current / 2] = temp;
            current /= 2;
        }
    }
    else{
        while (current > 1 && heap->tree[current]->priority < heap->tree[current / 2]->priority) {
            Process* temp = heap->tree[current];
            heap->tree[current] = heap->tree[current / 2];
            heap->tree[current / 2] = temp;
            current /= 2;
        }
    }
}

Process* peak(Heap* heap) {
    return (isEmpty(heap) ? NULL : heap->tree[1]);
}

bool isEmpty(Heap* heap) {
    return !heap->count;
}

void pop(Heap* heap, int ispiriority) {
    heap->tree[1] = heap->tree[heap->N--];
    heap->count--;
    heapify(heap, 1, ispiriority);
}

Heap* createHeap() {
    Heap* heap = malloc(sizeof(Heap));
    if (heap == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    heap->tree = malloc(INITIAL_CAPACITY * sizeof(Process*));
    if (heap->tree == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    heap->N = 0;
    heap->capacity = INITIAL_CAPACITY;
    heap->count = 0;
    
    return heap;
}

void destroyHeap(Heap* heap) {
    free(heap->tree);
    free(heap);
}
