#include "memallocator.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLOCK_SIZE (2 * sizeof(int) + sizeof(void*))

char* poolMemory;
int poolSize;
void* cur;

inline static int* prevSize(void* ptr) { return (int*)((char*)ptr - BLOCK_SIZE); }
inline static int* blockSize(void* ptr) { return (int*)((char*)ptr - BLOCK_SIZE + 4); }
inline static void** blockNext(void* ptr) { return (void**)((char*)ptr - BLOCK_SIZE + 8); }
inline static int* rightSize(void* ptr) { return (int*)((char*)ptr + *(blockSize(ptr)) - BLOCK_SIZE); }

void* findPosition(int size) {
    // check if cur is big enouth to not need previous node
    if (*blockSize(cur) >= size + BLOCK_SIZE) {
        return cur;
    }
    // remember old value of cur
    void* begin = cur;
    void* next = *blockNext(cur);
    // go for one cycle
    while (next != begin) {
        if (*blockSize(next) >= size) {
            return next;
        }
        next = *blockNext(next);
        cur = next;
    }
    // check old cur
    if (*blockSize(begin) >= size) {
        return begin;
    }
    // unable to find suitable position
    return NULL;
}

int memgetminimumsize() { return 2 * BLOCK_SIZE; }

int memgetblocksize() { return BLOCK_SIZE; }

void merge(void* ptr) {
    void* next = *blockNext(ptr);
    // increase hole size
    *blockSize(ptr) += *blockSize(next);
    *blockNext(ptr) = *blockNext(next);
    // change memory on right side
    *rightSize(ptr) = *blockSize(ptr);
}

void* memalloc(int size) {
    if (size < 1 || size > poolSize - memgetblocksize()) {
        return NULL;
    }
    size += BLOCK_SIZE;
    void* ptr = findPosition(size);
    if (!ptr) {
        fprintf(stderr, "Unable to find memory block of sufficient size\n");
        return NULL;
    }
    // take care of small leftovers
    if (*blockSize(ptr) - size < BLOCK_SIZE) {
        size = *blockSize(ptr);
    }
    void* tmp = ptr;
    // move header towards end of empty memory
    ptr = (char*)ptr + *blockSize(ptr) - size;
    // If header is reserved header
    if ((char*)ptr == poolMemory + BLOCK_SIZE - sizeof(int)) {
        fprintf(stderr, "No space remaining\n");
        return NULL;
    }
    // reduce remaining memory
    *blockSize(tmp) = *blockSize(tmp) - size;
    if (ptr != tmp) {
        *prevSize(ptr) = *blockSize(tmp);
    } else {
        *blockNext(cur) = *blockNext(ptr);
    }
    *blockSize(ptr) = size;
    *blockNext(ptr) = NULL;
    // used for previous block allocation check
    *rightSize(ptr) = -size;
    return ptr;
}

void memfree(void* ptr) {
    void* node;
    // if previous block is free
    if (*prevSize(ptr) > 0) {
        node = (char*)ptr - *prevSize(ptr);
        // merge
        *blockSize(node) += *blockSize(ptr);
        *rightSize(node) = *blockSize(node);
        // try merging with next
        if (*blockNext(node) == (char*)node + *blockSize(node)) {
            merge(node);
        }
    } else {
        // next node
        node = (char*)ptr + *blockSize(ptr);
        // allocated nodes have NULL as pNext
        if ((node < poolMemory + poolSize) && *blockNext(node)) {
            *blockNext(ptr) = node;
            merge(ptr);
        }
        // insert into list (somewhere)
        *blockNext(ptr) = *blockNext(cur);
        *blockNext(cur) = ptr;
    }
}

int meminit(void* pMemory, int size) {
    if (size < memgetminimumsize()) {
        return false;
    }
    poolMemory = (char*)pMemory;
    poolSize = size;

    cur = poolMemory + BLOCK_SIZE - sizeof(int);
    // cyclic list
    *blockNext(cur) = cur;
    *blockSize(cur) = size;
    return true;
}

void memdone() {
    char* iter = poolMemory;
    if (*(int*)iter == poolSize)
        return;
    while (iter < poolMemory + poolSize) {
        if (*(void**)(iter + sizeof(int)) == 0) {
            fprintf(stderr, "MEMORY LEAKED: %d bytes at %p\n", *(int*)iter - 16, iter + 12);
        }
        iter += *(int*)iter;
    }
}
