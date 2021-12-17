#ifndef TREE_H_
#define TREE_H_

#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    struct node* left;
    struct node* right;
    size_t size;
    int key;
} tree_t;

int wrapMain(FILE* in, FILE* out, int k);
int insert(tree_t* treePtr, int key);
tree_t* treeCreate(int key);
tree_t* treeRead(FILE* in);
void treeDelete(tree_t* tree);
tree_t* kMin(tree_t* tree, int k);
void treePrint(tree_t* tree);
void treePrintLesser(tree_t* tree, int key, FILE* out);
void treePrintLesserFancy(tree_t* tree, int key, int depth);

#endif // TREE_H_
