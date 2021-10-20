#include "memallocator.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct memPool {
    int size;
    void *pMemory;
} pool_t;

// keeping struct here so i can do pointer conversion instead of functions
typedef struct header {
    int prevSize; // also works as offset to make size and pNext adjancent
    int size;
    struct header *pNext;
} header_t;

#define BLOCK_SIZE 16

pool_t memPool;
header_t *cur;

bool isMergeable(header_t *this) {
    // if next hole is right after this hole
    return (void *)this->pNext == (void *)this + this->size;
}

header_t *findPosition(int size) {
    // remember old value of cur
    header_t *begin = cur;
    // go for one cycle
    while (cur->pNext != begin) {
        if (cur->pNext->size >= size) {
            // pointer to empty space that can fit requested size at end of pointed location
            return cur->pNext;
        }
        cur = cur->pNext;
    }
    // check old cur
    if (cur->pNext->size >= size) {
        return cur->pNext;
    }
    // unable to find suitable position
    return NULL;
}

int memgetminimumsize() { return 2 * BLOCK_SIZE; }

int memgetblocksize() { return BLOCK_SIZE; }

void merge(header_t *this) {
    header_t *next = this->pNext;
    // increase hole size
    this->size += next->size;
    this->pNext = next->pNext;
    // header cleanup
    *next = (header_t){0, 0, 0};
    // change memory on right side
    *(int *)((void *)this + this->size) = this->size;
}

void *memalloc(int size) {
    size += BLOCK_SIZE;
    header_t *header = findPosition(size);
    if (!header) {
        fprintf(stderr, "Unable to find memory block of sufficient size\n");
        return NULL;
    }
    header_t *tmp = header;
    // take care of small leftovers
    if (tmp->size - size < BLOCK_SIZE) {
        size = tmp->size;
    }
    // move header towards end of empty memory
    // type conversion for correct pointer arythmetics
    header = (void *)header + tmp->size - size;
    // If header is reserved header
    if (header == memPool.pMemory - 4) {
        fprintf(stderr, "No space remaining\n");
        return NULL;
    }
    // reduce remaining memory
    tmp->size = tmp->size - size;
    if (header != tmp) {
        header->prevSize = tmp->size;
    } else {
        cur->pNext = tmp->pNext;
    }
    header->size = size;
    header->pNext = NULL;
    // prevSize at the end
    *(int *)((void *)header + size) = -size;
    // for debug purposes
    memset((void *)(header + 1), '0', size - BLOCK_SIZE);
    // offset returned value
    return (void *)(header + 1);
}

void memfree(void *p) {
    // offset back
    header_t *header = (header_t *)(p - BLOCK_SIZE);
    // find previous node for merge check
    header_t *prev;
    // if previous block is free
    if (header->prevSize > 0) {
        prev = (void *)header - header->prevSize;
        // merge
        prev->size += header->size;
        // compiler says assignment to casts is illeagal and one-liner is horrible anyway
        header_t *next = (void *)header + header->size;
        next->prevSize = prev->size;
        // try merging with next
        if (isMergeable(prev)) {
            merge(prev);
        }
    } else {
        prev = memPool.pMemory - 4;
        // while behind freed region and not at end of list
        while ((prev->pNext < header) && (prev->pNext != memPool.pMemory - 4)) {
            prev = prev->pNext;
        }
        // insert into list
        header->pNext = prev->pNext;
        prev->pNext = header;
        // try merging with next
        if (isMergeable(header)) {
            merge(header);
        }
    }
}

int meminit(void *pMemory, int size) {
    if (size < memgetminimumsize()) {
        return false;
    }
    memPool.pMemory = pMemory;
    memPool.size = size;
    cur = pMemory - 4;
    // cyclic list
    cur->pNext = cur;
    cur->size = size;
    return true;
}

void memdone() {
    void *iter = memPool.pMemory;
    while (iter < memPool.pMemory + memPool.size) {
        if (*(int *)(iter + *(int *)iter - sizeof(int)) < 0) {
            fprintf(stderr, "MEMORY LEAKED: %d bytes at %p\n", *(int *)iter - 16, iter + 12);
        }
        iter += *(int *)iter;
    }
}
