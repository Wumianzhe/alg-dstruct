#include "btree.h"
#include <stdio.h>
#include <stdlib.h>

tree_t* nodeCreate(tree_t* sibling, int key, void* pValue) {
    tree_t* res = malloc(sizeof(tree_t));
    if (res) {
        res->full = false;
        res->keys[0] = key;
        res->ptrs[0] = pValue;
        if (sibling) {
            if (!(res->leaf = sibling->leaf)) {
                res->ptrs[0]->parent = res;
            }
            res->parent = sibling->parent;
            res->ptrs[2] = sibling->ptrs[2];
            sibling->ptrs[2] = res;
        } else {
            res->leaf = true;
            res->parent = NULL;
            res->ptrs[2] = NULL;
        }
    }
    return res;
}

void treeDelete(tree_t* node) {
    if (node) {
        if (!node->leaf) {
            treeDelete(node->ptrs[0]);
            treeDelete(node->ptrs[1]);
            if (node->full) {
                treeDelete(node->ptrs[2]);
            }
        }
        free(node);
    }
}

void inOrderInsert(tree_t* node, int key, void* pValue) {
    if (key > node->keys[0]) {
        node->keys[1] = key;
        node->ptrs[1] = pValue;
    } else {
        node->keys[1] = node->keys[0];
        node->ptrs[1] = node->ptrs[0];
        node->keys[0] = key;
        node->ptrs[0] = pValue;
    }
}

tree_t* treeInsert(tree_t* tree, int key, void* pValue) {
    if (!tree) {
        return nodeCreate(NULL, key, pValue);
    }
    tree_t* leaf = findLeaf(tree, key);
    // duplicate detection
    if (key == leaf->keys[0] || (leaf->full && key == leaf->keys[1])) {
        // do nothing
        return tree;
    }

    if (!leaf->full) {
        inOrderInsert(leaf, key, pValue);
        leaf->full = true;
        return tree;
    } else {
        if (key < leaf->keys[0]) {
            tree_t* next = nodeCreate(leaf, leaf->keys[1], leaf->ptrs[1]);
            if (!next) {
                return NULL;
            }
            inOrderInsert(leaf, key, pValue);
            // propagate
            return insertAbove(leaf, leaf->keys[0]);
        } else if (key < leaf->keys[1]) {
            tree_t* next = nodeCreate(leaf, leaf->keys[1], leaf->ptrs[1]);
            if (!next) {
                return NULL;
            }
            inOrderInsert(leaf, key, pValue);
            // propagate
            return insertAbove(leaf, key);
        } else {
            tree_t* next = nodeCreate(leaf, key, pValue);
            if (!next) {
                return NULL;
            }
            // propagate
            return insertAbove(leaf, leaf->keys[1]);
        }
    }
}

tree_t* insertAbove(tree_t* caller, int key) {
    tree_t* node = caller->parent;
    if (!node) {
        node = nodeCreate(NULL, key, caller);
        if (!node) {
            // if malloc failed on the way up, tree is pretty mangled. It's hard to revert, so just null it all
            return NULL;
        }
        node->leaf = false;
        node->ptrs[1] = caller->ptrs[2];

        caller->parent = node;
        node->ptrs[1]->parent = node;
        return node;
    }
    if (!node->full) {
        if (key > node->keys[0]) {
            node->keys[1] = key;
            node->ptrs[2] = caller->ptrs[2];
            node->ptrs[2]->parent = node;
        } else {
            node->keys[1] = node->keys[0];
            node->ptrs[2] = node->ptrs[0];

            node->keys[0] = key;
            node->ptrs[1] = caller->ptrs[2];
            node->ptrs[1]->parent = node;
        }
        node->full = true;
    } else {
        // HACK I'm abusing now-unused second pointer in internal node to point to new child
        // better way is another argument, but by the time i'm writing this, fixing is not worth it
        if (key < node->keys[0]) {
            tree_t* next = nodeCreate(node, node->keys[1], node->ptrs[1]);
            if (!next) {
                return NULL;
            }
            // this node is now "halved"
            node->full = false;
            node->keys[1] = 0;
            node->ptrs[2] = NULL;

            // move ptrs[2] where it belongs and reparent
            next->ptrs[1] = next->ptrs[2];
            next->ptrs[2] = NULL;
            next->ptrs[1]->parent = next;

            int old_key = node->keys[0];
            node->keys[0] = key;
            node->ptrs[1] = caller->ptrs[2];
            // propagate
            return insertAbove(node, old_key);
        } else if (key < node->keys[1]) {
            tree_t* next = nodeCreate(node, node->keys[1], caller->ptrs[2]);
            if (!next) {
                return NULL;
            }
            // move ptrs[2] where it belongs and reparent
            next->ptrs[1] = next->ptrs[2];
            next->ptrs[2] = NULL;
            next->ptrs[1]->parent = next;

            node->full = false;
            node->keys[1] = 0;
            node->ptrs[2] = NULL;
            // propagate
            return insertAbove(node, key);
        } else {
            tree_t* next = nodeCreate(node, key, node->ptrs[2]);
            if (!next) {
                return NULL;
            }
            next->ptrs[1] = caller->ptrs[2];
            next->ptrs[1]->parent = next;
            node->full = false;
            return insertAbove(node, node->keys[1]);
        }
    }
    // return the root
    // this is still logarithmic, and there's at most one insertion in chain that's going to get here anyway
    tree_t* cur = node;
    while (cur->parent) {
        cur = cur->parent;
    }
    return cur;
}

tree_t* findLeaf(tree_t* tree, int key) {
    // don't know if this path is possible, but just in case
    if (!tree) {
        return NULL;
    }
    tree_t* cur = tree;
    while (!cur->leaf) {
        // i'm using max keys, so equality is checked here
        if (key <= cur->keys[0]) {
            cur = cur->ptrs[0];
        } else if (cur->full && key > cur->keys[1]) {
            cur = cur->ptrs[2];
        } else {
            cur = cur->ptrs[1];
        }
    }
    return cur;
}

bool find(tree_t* tree, int key) {
    if (!tree) {
        return false;
    }
    tree_t* leaf = findLeaf(tree, key);
    if (key == leaf->keys[0] || (leaf->full && key == leaf->keys[1])) {
        return true;
    }
    return false;
}

void treePrint(tree_t* tree, int offset) {
    if (!tree) {
        return;
    }
    if (tree->leaf) {
        if (offset > 0) {
            printf("\033[%dC->", offset - 2);
        }
        printf("[%d", tree->keys[0]);
        if (tree->full) {
            printf(", %d", tree->keys[1]);
        }
        printf("]\n");
    } else {
        treePrint(tree->ptrs[0], offset + 4);
        if (!tree->full) {
            if (offset > 0) {
                printf("\033[%dC->", offset - 2);
            }
            printf("%2d\n", tree->keys[0]);
            treePrint(tree->ptrs[1], offset + 4);
        } else {
            if (offset > 0) {
                printf("\033[%dC", offset);
            }
            printf("%2d\n", tree->keys[0]);
            if (offset > 0) {
                printf("\033[%dC->\r", offset - 2);
            }
            treePrint(tree->ptrs[1], offset + 4);
            if (offset > 0) {
                printf("\033[%dC", offset);
            }
            printf("%2d\n", tree->keys[1]);
            treePrint(tree->ptrs[2], offset + 4);
        }
    }
    if (offset == 0) {
        fflush(stdout);
    }
}
