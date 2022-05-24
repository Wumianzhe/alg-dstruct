#ifndef HASH_H_
#define HASH_H_

#include <stdbool.h>
#include <stdlib.h>

typedef enum { HASH_FREE, HASH_FULL, HASH_DELETED } status;
#define HASH_SIZE_DEFAULT 500113 // greater of first pair of twin primes after 500000

typedef struct record {
    char* value;
    status state;
} record_t;

typedef struct {
    record_t* data;
    int size; // somewhat useful stat
    int capacity;
} hashmap_t;

// used by pointer in most functions, so creation as pointer
hashmap_t* hashMapInit(int maxSize);

bool hashMapInsert(hashmap_t* map, const char* str);
bool hashMapFind(const hashmap_t* map, const char* str);
void hashMapRemove(hashmap_t* map, const char* str);
void hashMapDelete(hashmap_t* map);

#endif // HASH_H_
