#ifndef BTREE_H_
#define BTREE_H_
#include <stdbool.h>

typedef struct node {
    // for leafs first two are pointers to value (cast as needed) and third is for in-order traversal
    struct node* ptrs[3];
    int keys[2];
    struct node* parent;
    bool full;
    bool leaf;
} tree_t;

tree_t* nodeCreate(tree_t* sibling, int key, void* pValue);
void treeDelete(tree_t* tree);
tree_t* treeInsert(tree_t* tree, int key, void* pValue);
tree_t* findLeaf(tree_t* tree, int key);
tree_t* insertAbove(tree_t* caller, int key);
tree_t* treeRemove(tree_t* tree, int key);
// should return value pointer, but it's not a hard rewrite and
// value pointers are going to be NULL in tests, so i need to differentiate
bool find(tree_t* tree, int key);
void treePrint(tree_t* tree, int offset);

void split(tree_t* tree, int key, tree_t** left, tree_t** right);
tree_t* merge(tree_t* left, tree_t* right);

#endif // BTREE_H_
