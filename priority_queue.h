#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#define swap(x, y) {int temp = y; y = x; x = temp;}

typedef struct {
    int* tree;
    int N;
    int capacity;
} Heap;

void resize(Heap* heap) {
    heap->capacity *= 2;
    heap->tree = realloc(heap->tree, (heap->capacity) * sizeof(int));
    if (heap->tree == NULL) {
        printf("Memory reallocation failed\n");
        exit(1);
    }
}

void heapify(Heap* heap, int idx) {
    if (idx > heap->N) return;
    int left = 2 * idx;
    int right = 2 * idx + 1;
    int largest = idx;
    if (left <= heap->N && heap->tree[left] > heap->tree[largest]) {
        largest = left;
    }
    if (right <= heap->N && heap->tree[right] > heap->tree[largest]) {
        largest = right;
    }
    if (largest != idx) {
        int temp = heap->tree[largest];
        heap->tree[largest] = heap->tree[idx];
        heap->tree[idx] = temp;
        heapify(heap, largest);
    }
}

void push(Heap* heap, int value) {
    if (heap->N + 1 >= heap->capacity) {
        resize(heap);
    }
    (heap->N)++;
    heap->tree[heap->N] = value;
    int current = heap->N;
    while (current > 1 && heap->tree[current] > heap->tree[current / 2]) {
        int temp = heap->tree[current];
        heap->tree[current] = heap->tree[current / 2];
        heap->tree[current / 2] = temp;
        current /= 2;
    }
}

int peak(Heap* heap) {
    return heap->tree[1];
}

void pop(Heap* heap) {
    heap->tree[1] = heap->tree[(heap->N)--];
    heapify(heap, 1);
}

#endif
