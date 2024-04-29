#include "circularQueue.h"

CircularQueue* createQueue() {
    CircularQueue* queue = (CircularQueue*)malloc(sizeof(CircularQueue));
    if (queue == NULL) {
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    queue->head = queue->tail = queue->current = NULL;

    return queue;
}

void deleteQueue(CircularQueue* queue) {
    while (!isEmpty(queue)) {
        dequeue(queue);
    }
    free(queue);
}

int isEmpty(CircularQueue* queue) {
    return queue->head == NULL;
}

void enqueue(CircularQueue* queue, Process* data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    newNode->data = data;
    newNode->next = NULL;

    if (isEmpty(queue)) {
        queue->head = queue->tail = newNode;
    } else {
        queue->tail->next = newNode;
        queue->tail = newNode;
    }
    queue->tail->next = queue->head;
}

Process* dequeue(CircularQueue* queue) {
    if (isEmpty(queue)) {
        printf("Queue underflow\n");
        return NULL;
    }

    Process* data = queue->head->data;

    if (queue->head == queue->tail) {
        free(queue->head);
        queue->head = queue->tail = NULL;
    } else {
        Node* temp = queue->head;
        queue->head = queue->head->next;
        free(temp);
        queue->tail->next = queue->head;
    }
    return data;
}

void print(CircularQueue* queue) {
    if (isEmpty(queue)) {
        printf("Queue is empty\n");
        return;
    }
    printf("Queue elements: ");
    Node* current = queue->head;
    do {
        printf("%d ", current->data->pid);
        current = current->next;
    } while (current != queue->head);
    printf("\n");
}
void moveToNext(CircularQueue* queue) {
    if (isEmpty(queue)) {
        printf("Queue is empty\n");
        return;
    }
    queue->current = queue->current->next;
}

void deleteCurrent(CircularQueue* queue) {
    if (isEmpty(queue)) {
        printf("Queue is empty\n");
        return;
    }

    if (queue->current == queue->head) {
        // If the current pointer is at the head
        dequeue(queue);
        return;
    }

    Node* prev = NULL;
    Node* current = queue->head;
    while (current != queue->current) {
        prev = current;
        current = current->next;
    }
    prev->next = current->next;

    if (current == queue->tail) {
        // If the current pointer is at the tail
        queue->tail = prev;
    }
    free(current);
    queue->current = prev->next;
}
