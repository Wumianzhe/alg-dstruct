#include "memallocator.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define ARR_SIZE 1024
#define BLOCK_MAX 800
#define RUN_COUNT 25

// huge static buffer so memory adress stays consistent between runs with same seed
// helps greatly with watchpoints
char buf[1024 * 1024];

int main(int argc, char* argv[]) {
    int seed;
    if (argc > 1) {
        sscanf(argv[1], "%d", &seed);
    } else {
        srand(time(0));
        seed = rand() % 1024;
        printf("seed: %d\n", seed);
    }
    srand(seed);
    void* arr[ARR_SIZE];
    meminit(buf, 1024 * 1024);
    for (int r = 0; r < RUN_COUNT; r++) {
        for (int i = 0; i < ARR_SIZE; i++) {
            arr[i] = memalloc(1 + rand() % BLOCK_MAX);
        }
        for (int i = 0; i < ARR_SIZE; i++) {
            int index = rand() % (ARR_SIZE - i);
            memfree(arr[index]);
            if (errno) {
                errno = 0;
                printf("run: %d, iteration: %d\n", r, i);
            }
            arr[index] = arr[ARR_SIZE - 1 - i];
        }
    }
    memdone();
    return 0;
}
