#include "btree.h"
#include <stdio.h>

int main(int argc, char* argv[]) {
    tree_t* tree = NULL;
    for (int i = 1; i < 101; i++) {
        tree = treeInsert(tree, i, NULL);
    }
    treePrint(tree, 0);
    return 0;
}
