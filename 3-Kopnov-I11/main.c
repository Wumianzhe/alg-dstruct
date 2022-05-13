#include "btree.h"
#include <stdio.h>
#include <stdlib.h>

void mainLoop(tree_t** pTree);

int main(int argc, char* argv[]) {
    tree_t* tree = NULL;
    mainLoop(&tree);
    treeDelete(tree);
    return 0;
}

void mainLoop(tree_t** pTree) {
    tree_t* tree = *pTree;
    char action;
    int key;
    while (scanf("%c %d\n", &action, &key) > 0) {
        switch (action) {
        case 'a': {
            tree = treeInsert(tree, key, NULL);
            break;
        }
        case 'r': {
            tree = treeRemove(tree, key);
            break;
        }
        case 'f': {
            printf("%s\n", treeFind(tree, key) ? "yes" : "no");
            break;
        }
        case 'p': {
            treePrint(tree, 0);
            break;
        }
        case 'l': {
            sequentialPrint(tree);
            break;
        }
        default: {
            *pTree = tree;
            return;
        }
        }
    }
    *pTree = tree;
}
