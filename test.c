#include "memory.h"
#include <stdio.h>

void main() {
    Memory* root = createMemory(1024, 0, 1023, NULL);
    Memory* x = InsertMemory(root, 128, 1);
    printf("%d %d\n", x->start, x->end);
    DisplayMemory(root);
    printf("\n");
    printf("\n");
    x = InsertMemory(root, 128, 2);
    printf("%d %d\n", x->start, x->end);
    DisplayMemory(root);
    printf("\n");
    printf("\n");
    x = InsertMemory(root, 128, 3);
    printf("%d %d\n", x->start, x->end);
    DisplayMemory(root);
    printf("\n");
    printf("\n");
    x = InsertMemory(root, 128, 4);
    printf("%d %d\n", x->start, x->end);
    DisplayMemory(root);
    printf("\n");
    printf("\n");
    x = InsertMemory(root, 128, 5);
    printf("%d %d\n", x->start, x->end);
    DisplayMemory(root);
    printf("\n");
    printf("\n");

    DeleteMemory(root, 5);
    DisplayMemory(root);
    printf("\n");
    printf("\n");
    DeleteMemory(root, 1);
    DisplayMemory(root);
    printf("\n");
    printf("\n");
    DeleteMemory(root, 2);
    DisplayMemory(root);
    printf("\n");
    printf("\n");
    DeleteMemory(root, 3);
    DisplayMemory(root);
    printf("\n");
    printf("\n");
    DeleteMemory(root, 4);
    DisplayMemory(root);
    printf("\n");
    printf("\n");
    x = InsertMemory(root, 1024, 1);
    DisplayMemory(root);
    DestroyMemory(&root);
}