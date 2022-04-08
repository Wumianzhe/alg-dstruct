#include "btree.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    tree_t* tree = NULL;
    for (int i = 0; i < 32; i++) {
        tree = treeInsert(tree, i, NULL);
    }
    treePrint(tree, 0);
    tree = treeRemove(tree, 20);
    printf("\n\n");
    treePrint(tree, 0);
    treeDelete(tree);
    return 0;
}
