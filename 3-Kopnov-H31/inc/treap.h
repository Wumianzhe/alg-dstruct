#ifndef TREAP_H_
#define TREAP_H_

typedef struct node {
    int key;
    int p;
    struct node* pLeft;
    struct node* pRight;
} node_t;
typedef node_t treap_t;

node_t* createNode(int key);
void deleteTreap(treap_t* T);
treap_t* treapMerge(treap_t* L, treap_t* R);
void treapSplit(treap_t* T, int key, treap_t** L, treap_t** R);
treap_t* treapInsert(treap_t* T, int key);
treap_t* treapRemove(treap_t* T, int key);
node_t* treapFind(treap_t* T, int key);

#endif // TREAP_H_
