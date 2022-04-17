#ifndef BTREE_H_
#define BTREE_H_

#define BAD_MALLOC 0
extern int badmallocsAllowed;

#include <stdbool.h>

typedef struct node {
    // for leafs first two are pointers to value (cast as needed) and third is for in-order traversal
    struct node* ptrs[3];
    int keys[2];
    struct node* parent;
    int height; // leafs have height of 0
    bool full;
} tree_t;

tree_t* nodeCreate(tree_t* sibling, int key, void* pValue);
void treeDelete(tree_t* tree);
tree_t* treeInsert(tree_t* tree, int key, void* pValue);
tree_t* treeRemove(tree_t* tree, int key);
// should return value pointer, but it's not a hard rewrite and
// value pointers are going to be NULL in tests, so i need to differentiate
bool treeFind(tree_t* tree, int key);
void treePrint(tree_t* tree, int offset);
void sequentialPrint(tree_t* tree);

// if key is in tree, leaf containing it (and only it) is returned separately. Can be merged back, but is useful for
// removal
tree_t* treeSplit(tree_t* tree, int key, tree_t** left, tree_t** right);
tree_t* treeMerge(tree_t* left, tree_t* right);

#endif // BTREE_H_
