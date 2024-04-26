#include "circularQueue.h"

Process** queue = NULL;
int front = -1, rear = -1;
int capacity = 0;

int isFull() {
    return (rear + 1) % capacity == front;
}

int isEmpty() {
    return front == -1;
}

void resize() {
    int newCapacity = capacity == 0 ? INITIAL_CAPACITY : capacity * 2;
    Process** newQueue = (Process**)malloc(newCapacity * sizeof(Process*));
    if (newQueue == NULL) {
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    int i = front;
    int j = 0;
    while (i != rear) {
        newQueue[j++] = queue[i];
        i = (i + 1) % capacity;
    }
    newQueue[j] = queue[rear];
    front = 0;
    rear = j;
    capacity = newCapacity;
    free(queue);
    queue = newQueue;
}

void enqueue(Process* data) {
    if (isFull()) {
        resize();
    }
    if (front == -1) {
        front = 0;
    }
    rear = (rear + 1) % capacity;
    queue[rear] = data;
    printf("Inserted element: %d\n", data->pid); // Assuming Process struct has a pid field
}

Process* dequeue() {
    if (isEmpty()) {
        printf("Queue underflow\n");
        return NULL;
    }
    Process* data = queue[front];
    if (front == rear) {
        front = rear = -1;
    } else {
        front = (front + 1) % capacity;
    }
    return data;
}

void print() {
    if (isEmpty()) {
        printf("Queue is empty\n");
        return;
    }
    printf("Queue elements: ");
    int i = front;
    while (i != rear) {
        printf("%d ", queue[i]->pid); // Assuming Process struct has a pid field
        i = (i + 1) % capacity;
    }
    printf("%d\n", queue[rear]->pid); // Assuming Process struct has a pid field
}
