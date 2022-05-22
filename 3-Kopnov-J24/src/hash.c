#include "hash.h"
#include <string.h>

int h1(const char* str, int maxSize);
int h2(const char* str, int maxSize);

int findIndex(const hashmap_t* map, const char* str);
bool insertInR(hashmap_t* map, const char* str, int r);

hashmap_t* hashMapInit(int maxSize) {
    hashmap_t* res = malloc(sizeof(hashmap_t));
    if (res) {
        res->data = malloc(sizeof(record_t) * maxSize);
        if (!res->data) {
            free(res);
            return NULL;
        }
        for (int i = 0; i < maxSize; i++) {
            res->data[i].value = NULL;
            res->data[i].state = HASH_FREE;
        }
        res->maxSize = maxSize;
        res->size = 0;
    }
    return res;
}

void hashMapDelete(hashmap_t* map) {
    for (int i = 0; i < map->maxSize; i++) {
        if (map->data[i].value) {
            free(map->data[i].value);
        }
    }
    free(map->data);
    free(map);
}

bool hashMapFind(const hashmap_t* map, const char* str) { return findIndex(map, str) != map->maxSize; }

void hashMapRemove(hashmap_t* map, const char* str) {
    int index = findIndex(map, str);
    if (index != map->maxSize) {
        map->data[index].state = HASH_DELETED;
        free(map->data[index].value);
        map->data[index].value = NULL;
    }
}

bool hashMapInsert(hashmap_t* map, const char* str) {
    if (map->size == map->maxSize) {
        return false;
    }
    int pos = h1(str, map->maxSize);
    int step = h2(str, map->maxSize);
    int r = 0;
    while (map->data[pos].state == HASH_FULL) {
        r++;
        pos = (pos + step) % map->maxSize;
        if (r > 2) {
            if (insertInR(map, str, r)) {
                return true;
            }
        }
    }
    char* copy = malloc((strlen(str) + 1) * sizeof(char));
    if (!copy) {
        return false;
    }
    strcpy(copy, str);
    map->data[pos].value = copy;
    map->data[pos].state = HASH_FULL;
    map->size++;
    return true;
}

bool insertInR(hashmap_t* map, const char* str, int r) {
    int pos = h1(str, map->maxSize);
    int step = h2(str, map->maxSize);
    for (int i = 0; i < r; i++) {
        int posI = (pos + i * step) % map->maxSize;
        int c = h2(map->data[posI].value, map->maxSize);
        int k = r - i;
        int posIK = (posI + k * c) % map->maxSize;
        if (map->data[posIK].state != HASH_FULL) {
            map->data[posIK] = map->data[posI];
            char* copy = malloc((strlen(str) + 1) * sizeof(char));
            if (!copy) {
                return false;
            }
            strcpy(copy, str);
            map->data[posI].value = copy;
            map->size++;
            return true;
        }
    }
    return false;
}

int h1(const char* str, int maxSize) {
    const int c = 13; // magic number
    int hash = 0;
    int len = strlen(str);
    for (int i = 0; i < len; i++) {
        hash = hash * c + str[i];
    }
    return hash % maxSize;
}

int h2(const char* str, int maxSize) {
    const int c = 13; // magic number
    int hash = 0;
    int len = strlen(str);
    for (int i = 0; i < len; i++) {
        hash = hash * c + str[i];
    }
    // -2 as if maxSize is prime, -1 will be even
    return 1 + hash % (maxSize - 2);
}

int findIndex(const hashmap_t* map, const char* str) {
    int pos = h1(str, map->maxSize);
    int step = h2(str, map->maxSize);
    const int initPos = pos;
    if (map->data[pos].state == HASH_FREE) {
        return map->maxSize;
    }
    // while
    while (map->data[pos].state != HASH_FREE && (!map->data[pos].value || strcmp(map->data[pos].value, str))) {
        pos = (pos + step) % map->maxSize;
        if (pos == initPos || map->data[pos].state == HASH_FREE) {
            return map->maxSize;
        }
    }
    return pos;
}
