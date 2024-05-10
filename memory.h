#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#ifndef MEMORY_H
#define MEMORY_H


typedef struct Memory {
    int size;
    int start;
    int end;
    bool isAllocated;
    int processId;
    struct Memory* parent;
    struct Memory* left;
    struct Memory* right;
} Memory;

Memory* createMemory(int size, int start, int end, Memory* parent);
void FindMemory(Memory* root, int size, Memory** node);
Memory* InsertMemory(Memory* root, int size, int processId);
void GetMemory(Memory* root, int pid, Memory** node);
void DeleteMemory(Memory* root, int pid);
void DisplayMemory(Memory* root);
void DestroyMemory(Memory** root);
bool isLeaf(Memory* memory);



#endif