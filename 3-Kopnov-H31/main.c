#include "treap.h"
#include <stdio.h>
#include <stdlib.h>

void mainLoop(treap_t** T);

int main(int argc, char* argv[]) {
    treap_t* T = NULL;
    mainLoop(&T);
    deleteTreap(T);
    return 0;
}

void mainLoop(treap_t** pT) {
    treap_t* T = *pT;
    char action;
    int key;
    while (scanf("%c %d\n", &action, &key) > 0) {
        switch (action) {
        case 'a': {
            T = treapInsert(T, key);
            break;
        }
        case 'r': {
            T = treapRemove(T, key);
            break;
        }
        case 'f': {
            printf("%s\n", treapFind(T, key) ? "yes" : "no");
            break;
        }
        default: {
            *pT = T;
            return;
        }
        }
    }
}
