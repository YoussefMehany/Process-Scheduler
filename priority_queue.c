#include "priority_queue.h"

void resize(Heap* heap) {
    heap->capacity *= 2;
    heap->tree = realloc(heap->tree, heap->capacity * sizeof(Process*));
    if (heap->tree == NULL) {
        printf("Memory reallocation failed\n");
        exit(1);
    }
}
int getRequested(Process* process, int key) {
    if(key == 0) {
        return process->remainingTime;
    }else if(key == 1) {
        return process->priority;
    }else {
        return process->memorySize;
    }
}

void heapify(Heap* heap, int idx, int key) {
    if (idx > heap->N) return;
    int left = 2 * idx;
    int right = 2 * idx + 1;
    int smallest = idx;

    if (left <= heap->N && (getRequested(heap->tree[left], key) < getRequested(heap->tree[smallest], key) || 
    getRequested(heap->tree[left], key) == getRequested(heap->tree[smallest], key) && heap->tree[left]->id < heap->tree[smallest]->id)) smallest = left;
    if (right <= heap->N && (getRequested(heap->tree[right], key) < getRequested(heap->tree[smallest], key) || 
    getRequested(heap->tree[right], key) == getRequested(heap->tree[smallest], key) && heap->tree[right]->id < heap->tree[smallest]->id)) smallest = right;
    if (smallest != idx) {
        Process* temp = heap->tree[smallest];
        heap->tree[smallest] = heap->tree[idx];
        heap->tree[idx] = temp;
        heapify(heap, smallest, key);
    }
}


void push(Heap* heap, Process* newProcess, int key) {
    if (heap->N + 1 >= heap->capacity) {
        resize(heap);
    }
    (heap->N)++;
    heap->tree[heap->N] = newProcess;
    heap->count++;
    int current = heap->N;

    while (current > 1 && (getRequested(heap->tree[current], key) < getRequested(heap->tree[current / 2], key) ||
     getRequested(heap->tree[current], key) == getRequested(heap->tree[current / 2], key) && heap->tree[current]->id < heap->tree[current / 2]->id)) {
        Process* temp = heap->tree[current];
        heap->tree[current] = heap->tree[current / 2];
        heap->tree[current / 2] = temp;
        current /= 2;
    }
}

Process* peak(Heap* heap) {
    return (isEmpty(heap) ? NULL : heap->tree[1]);
}

bool isEmpty(Heap* heap) {
    return !heap->count;
}

void pop(Heap* heap, int key) {
    heap->tree[1] = heap->tree[heap->N--];
    heap->count--;
    heapify(heap, 1, key);
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

void destroyHeap(Heap** heap) {
    if (heap == NULL || *heap == NULL) {
        return;
    }

    free((*heap)->tree);
    free(*heap);
    *heap = NULL; 
}
