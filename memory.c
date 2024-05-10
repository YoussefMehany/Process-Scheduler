#include "memory.h"

Memory* createMemory(int size, int start, int end, Memory* parent) {
    Memory* memory = (Memory*)malloc(sizeof(Memory));
    memory->size = size;
    memory->start = start;
    memory->end = end;
    memory->isAllocated = false;
    memory->parent = parent;
    memory->left = NULL;
    memory->right = NULL;
    memory->processId = -1;
    return memory;
}

bool isLeaf(Memory* memory) {
    return memory->left == NULL && memory->right == NULL;
}

void FindMemory(Memory* root, int size, Memory** node) {
    if(root->size >= size && !root->isAllocated && isLeaf(root) && (!(*node) || *node && root->size < (*node)->size)) {
        *node = root;
    }   
    if(root->left && !root->left->isAllocated) {
        FindMemory(root->left, size, node);
    }
    if(root->right && !root->right->isAllocated) {
        FindMemory(root->right, size, node);
    }
}

Memory* InsertMemory(Memory* root, int size, int processId) {
    Memory* memory = NULL;
    FindMemory(root, size, &memory);
    if(!memory) {
        return NULL;
    }
    while(memory->size / 2 >= size) {
        memory->left = createMemory(memory->size / 2, memory->start, memory->start + memory->size / 2 - 1, memory);
        memory->right = createMemory(memory->size / 2, memory->start + memory->size / 2, memory->end, memory);
        memory = memory->left;
    }
    memory->isAllocated = true;
    memory->processId = processId;
    return memory;
}

void GetMemory(Memory* root, int pid, Memory** node) {
    if (root == NULL || *node != NULL) {
        return;
    }
    if (root->processId == pid) {
        *node = root;
        return;
    }
    GetMemory(root->left, pid, node);
    GetMemory(root->right, pid, node);
}

void DeleteMemory(Memory* root, int pid) {
    Memory* memory = NULL;
    GetMemory(root, pid, &memory);
    if (memory == NULL) {
        printf("Memory not found\n");
        return;
    }
    memory->isAllocated = false;
    memory->processId = -1;
    while (memory->parent) {
        Memory* parent = memory->parent;
        if(!parent->left->isAllocated && !parent->right->isAllocated && isLeaf(parent->left) && isLeaf(parent->right)) {
            parent->isAllocated = false;
            parent->processId = -1;
            memory = parent;
            free(parent->left);
            free(parent->right);
            parent->left = NULL;
            parent->right = NULL;
        } else {
            break;
        }
    }
}

void DisplayMemory(Memory* root) {
    if (root == NULL) {
        return;
    }
    printf("Size: %d, Start: %d, End: %d, isAllocated: %d, ProcessId: %d\n", root->size, root->start, root->end, root->isAllocated, root->processId);
    DisplayMemory(root->left);
    DisplayMemory(root->right);

}

void DestroyMemory(Memory** root) {
    if (root == NULL || *root == NULL) {
        return;
    }
    free(*root);
    *root = NULL;
}