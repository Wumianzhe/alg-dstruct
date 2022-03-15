#include "treap.h"
#include <stdlib.h>

// all treaps are in UpperCamelCase, other objects in lowerCamelCase
treap_t* treapMerge(treap_t* L, treap_t* R) {
    if (!L)
        return R;
    if (!R)
        return L;
    if (L->p > R->p) {
        L->pRight = treapMerge(L->pRight, R);
        return L;
    } else {
        R->pLeft = treapMerge(L, R->pLeft);
        return R;
    }
}

void treapSplit(treap_t* T, int key, treap_t** L, treap_t** R) {
    if (!T) {
        *L = NULL;
        *R = NULL;
        return;
    }
    treap_t* Tmp = NULL;
    if (T->key <= key) {
        treapSplit(T->pRight, key, &Tmp, R);
        T->pRight = Tmp;
        *L = T;
    } else {
        treapSplit(T->pLeft, key, L, &Tmp);
        T->pLeft = Tmp;
        *R = T;
    }
}

node_t* createNode(int key) {
    node_t* node = malloc(sizeof(node_t));
    if (node) {
        node->key = key;
        node->p = rand();
        node->pLeft = NULL;
        node->pRight = NULL;
    }
    return node;
}

treap_t* treapInsert(treap_t* T, int key) {
    treap_t *L = NULL, *R = NULL;
    if (T) {
        treapSplit(T, key, &L, &R);
    }
    treap_t* M = createNode(key);
    return treapMerge(treapMerge(L, M), R);
}

treap_t* treapRemove(treap_t* T, int key) {
    treap_t *L, *M, *R;
    treapSplit(T, key - 1, &L, &R);
    treapSplit(R, key, &M, &T);
    deleteTreap(M);
    return treapMerge(L, T);
}

void deleteTreap(treap_t* T) {
    if (T) {
        deleteTreap(T->pLeft);
        deleteTreap(T->pRight);
        free(T);
    }
}

node_t* treapFind(treap_t* T, int key) {
    treap_t* res = NULL;
    while (T) {
        if (T->key == key) {
            res = T;
            break;
        }
        if (T->key > key) {
            T = T->pLeft;
        } else {
            T = T->pRight;
        }
    }
    return res;
}
