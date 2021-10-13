#include "memallocator.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct memPool {
    int size;
    void* pMemory;
} pool_t;

typedef struct header {
    struct header* pNext;
    int size;
} header_t;

pool_t memPool;
header_t head;

bool isMergeable(header_t* this) {
    // if next hole is right after this hole
    // works because allocated spaces are not in list
    return (char*)this->pNext == (char*)this + this->size + sizeof(header_t);
}

header_t* findPosition(int size) {
    static header_t* cur = &head;
    // remember old value of cur
    header_t* begin=cur;
    //go for one cycle
    while (cur->pNext!= begin) {
        if(cur->size > size) {
            // pointer to empty space that can fit header and requested size at end of pointed location
            return cur;
        }
        cur=cur->pNext;
    }
    // one unchecked node at tail
    if(cur->size > size) {
        return cur;
    }
    //reset pointer location
    cur=begin;
    // unable to find suitable position
    return NULL;
}

int memgetminimumsize() {
    return sizeof(header_t);
}

int memgetblocksize() {
    return sizeof(header_t);
}

void merge(header_t* this) {
    header_t* next = this->pNext;
    //increase hole size
    this->size += next->size+sizeof(header_t);
    this->pNext = next->pNext;
    //header cleanup
    *next = (header_t){0,0};
}

void* memalloc(int size) {
    header_t* header=findPosition(size);
    if(!header) {
        fprintf(stderr, "Unable to find memory block of sufficient size\n");
        return NULL;
    }
    header_t* tmp=header;
    // head check
    if (tmp == &head) {
        header=memPool.pMemory;
    }
    //move header towards end of empty memory
    //type conversion so pointer arythmetics does not mess things up
    header = (void*)header + tmp->size - size - sizeof(header_t);
    header->size=size;
    header->pNext=NULL;
    //for debug purposes
    memset((void*)(header+1),1,header->size);
    //offset returned value
    return (void*)(header+1);
}

void memfree(void* p) {
    //offset back
    header_t* header=(header_t*)p-1;
    //find previous node for merge check
    header_t* prev=&head;
    //while behind freed region and not at tail
    while (prev->pNext < header && prev->pNext != &head) {
        prev = prev->pNext;
    }
    //insert into list
    header->pNext=prev->pNext;
    prev->pNext=header;
    //try merging with previous
    if (isMergeable(prev)) {
        merge(prev);
        //try merging with next
        if(isMergeable(prev)) {
            merge(prev);
        }
    } else {
        //try merging with next
        if(isMergeable(header)) {
            merge(header);
        }
    }
}

int meminit(void *pMemory, int size) {
    memPool.pMemory=pMemory;
    memPool.size=size;
    //cyclic list
    head.pNext = &head;
    head.size = size;
    return true;
}

void memdone() {

}
