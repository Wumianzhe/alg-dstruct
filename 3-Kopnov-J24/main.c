#include <hash.h>
#include <stdio.h>

void mainLoop(hashmap_t** pMap);

int main(int argc, char* argv[]) {
    hashmap_t* map = hashMapInit(HASH_SIZE_DEFAULT);
    mainLoop(&map);
    hashMapDelete(map);
    return 0;
}

void mainLoop(hashmap_t** pMap) {
    hashmap_t* map = *pMap;
    char action;
    char key[20];
    while (scanf("%c %s\n", &action, key) > 0) {
        switch (action) {
        case 'a': {
            hashMapInsert(map, key);
            break;
        }
        case 'r': {
            hashMapRemove(map, key);
            break;
        }
        case 'f': {
            printf("%s\n", hashMapFind(map, key) ? "yes" : "no");
            break;
        }
        default: {
            *pMap = map;
            return;
        }
        }
    }
    *pMap = map;
}
