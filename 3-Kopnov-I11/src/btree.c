#include "btree.h"
#ifndef NDEBUG
#if (BAD_MALLOC == 1)
#define _GNU_SOURCE
#include <dlfcn.h>
#endif
#endif
#include <stdio.h>
#include <stdlib.h>

#ifndef NDEBUG
#if (BAD_MALLOC == 1)
void* malloc(size_t size) {
    static void* (*sysmalloc)(size_t) = NULL;
    if (!sysmalloc) {
        sysmalloc = dlsym(RTLD_NEXT, "malloc");
    }
    if (!(rand() % 10)) {
        return NULL;
    }
    return sysmalloc(size);
}
#endif
#endif

int badmallocsAllowed = 2;

tree_t* insertAbove(tree_t* caller, int key);
tree_t* findLeaf(tree_t* tree, int key);
tree_t* root(tree_t* node);

tree_t* nodeCreate(tree_t* sibling, int key, void* pValue) {
    tree_t* res = malloc(sizeof(tree_t));
    if (res) {
        res->full = false;
        res->keys[0] = key;
        res->ptrs[0] = pValue;
        if (sibling) {
            if ((res->height = sibling->height)) {
                res->ptrs[0]->parent = res;
            }
            res->parent = sibling->parent;
            res->ptrs[2] = sibling->ptrs[2];
            sibling->ptrs[2] = res;
        } else {
            res->height = 0;
            res->parent = NULL;
            res->ptrs[2] = NULL;
        }
    }
    return res;
}

void treeDelete(tree_t* node) {
    if (node) {
        if (node->height) {
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
                treeDelete(tree);
                return NULL;
            }
            inOrderInsert(leaf, key, pValue);
            // propagate
            return insertAbove(leaf, leaf->keys[0]);
        } else if (key < leaf->keys[1]) {
            tree_t* next = nodeCreate(leaf, leaf->keys[1], leaf->ptrs[1]);
            if (!next) {
                treeDelete(tree);
                return NULL;
            }
            inOrderInsert(leaf, key, pValue);
            // propagate
            return insertAbove(leaf, key);
        } else {
            tree_t* next = nodeCreate(leaf, key, pValue);
            if (!next) {
                treeDelete(tree);
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
            treeDelete(caller->ptrs[2]);
            treeDelete(caller);
            // if malloc failed on the way up, tree is pretty mangled. It's hard to revert, so just null it all
            return NULL;
        }
        node->height = caller->height + 1;
        node->ptrs[1] = caller->ptrs[2];
        caller->ptrs[2] = NULL;

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
                treeDelete(caller->ptrs[2]);
                treeDelete(root(node));
                return NULL;
            }
            // this node is now "halved"
            node->full = false;
            node->keys[1] = 0;

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
            node->full = false;
            if (!next) {
                treeDelete(caller->ptrs[2]);
                treeDelete(root(node));
                return NULL;
            }
            // move ptrs[2] where it belongs and reparent
            next->ptrs[1] = next->ptrs[2];
            next->ptrs[2] = NULL;
            next->ptrs[1]->parent = next;

            node->keys[1] = 0;
            // propagate
            return insertAbove(node, key);
        } else {
            tree_t* next = nodeCreate(node, key, node->ptrs[2]);
            if (!next) {
                treeDelete(caller->ptrs[2]);
                treeDelete(root(node));
                return NULL;
            }
            node->full = false;
            next->ptrs[1] = caller->ptrs[2];
            next->ptrs[1]->parent = next;
            return insertAbove(node, node->keys[1]);
        }
    }
    // return the root
    // this is still logarithmic, and there's at most one insertion in chain that's going to get here anyway
    return root(node);
}

tree_t* root(tree_t* node) {
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
    while (cur->height) {
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

bool treeFind(tree_t* tree, int key) {
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
    if (!tree->height) {
        if (offset > 0) {
            printf("\033[%dC->", offset - 2);
        }
        printf("[%2d", tree->keys[0]);
        if (tree->full) {
            printf(", %2d", tree->keys[1]);
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

void sequentialPrint(tree_t* tree) {
    if (!tree) {
        return;
    }
    tree_t* cur = tree;
    while (cur->height) {
        cur = cur->ptrs[0];
    }
    while (cur) {
        printf("[%d", cur->keys[0]);
        if (cur->full) {
            printf(", %d", cur->keys[1]);
        }
        printf("] ");
        cur = cur->ptrs[2];
    }
    printf("\n");
    fflush(stdout);
}

tree_t* treeMerge(tree_t* left, tree_t* right) {
    if (!left) {
        return right;
    }
    if (!right) {
        return left;
    }
    if (left->height == right->height) {
        tree_t* lParent = left->parent;
        tree_t* rParent = right->parent;
        int lmax;
        tree_t* cur = left;
        while (cur->height) {
            cur = !cur->full ? cur->ptrs[1] : cur->ptrs[2];
        }
        lmax = !cur->full ? cur->keys[0] : cur->keys[1];
        // if one of pointers is part of bigger tree
        if (lParent) {
            right->parent = lParent;
            if (!lParent->full) {
                lParent->keys[1] = lmax;
                lParent->ptrs[2] = right;
            } else {
                tree_t* next = nodeCreate(lParent, lmax, left);
                next->ptrs[1] = right;
                lParent->full = false;
                return insertAbove(lParent, lParent->keys[1]);
            }
        } else if (rParent) {
            left->parent = rParent;
            if (!rParent->full) {
                rParent->keys[1] = rParent->keys[0];
                rParent->ptrs[2] = rParent->ptrs[1];
                rParent->ptrs[1] = rParent->ptrs[0];

                rParent->keys[0] = lmax;
                rParent->ptrs[0] = left;
            } else {
                tree_t* next = nodeCreate(rParent, rParent->keys[1], rParent->ptrs[1]);
                rParent->full = false;
                next->ptrs[1] = next->ptrs[2];
                next->ptrs[2] = NULL;
                int old_key = rParent->keys[0];
                rParent->ptrs[1] = rParent->ptrs[0];
                rParent->ptrs[0] = left;
                rParent->keys[0] = lmax;
                return insertAbove(rParent, old_key);
            }
        } else {
            tree_t* tree = nodeCreate(NULL, lmax, left);
            if (!tree) {
                return NULL;
            }
            tree->height = left->height + 1;
            tree->ptrs[1] = right;
            left->parent = tree;
            right->parent = tree;
            return tree;
        }
    }
    if (left->height < right->height) {
        return treeMerge(left, right->ptrs[0]);
    } else {
        return treeMerge(left->full ? left->ptrs[1] : left->ptrs[2], right);
    }
}

tree_t* extract(tree_t** ptr) {
    tree_t* res = *ptr;
    *ptr = NULL;
    res->parent = NULL;
    return res;
}

tree_t* treeSplit(tree_t* tree, int key, tree_t** pLeft, tree_t** pRight) {
    if (tree) {
        // internal node
        if (tree->height) {
            tree_t* left = extract(&tree->ptrs[0]);
            tree_t* middle = extract(&tree->ptrs[1]);
            tree_t* right = NULL;
            if (tree->full) {
                right = extract(&tree->ptrs[2]);
            }
            if (key < tree->keys[0]) {
                if (right) {
                    *pRight = treeMerge(right, *pRight);
                }
                *pRight = treeMerge(middle, *pRight);
                return treeSplit(left, key, pLeft, pRight);
            }
            *pLeft = treeMerge(*pLeft, left);
            if (tree->full && key > tree->keys[1]) {
                *pLeft = treeMerge(*pLeft, middle);
                return treeSplit(right, key, pLeft, pRight);
            }
            if (right) {
                *pRight = treeMerge(right, *pRight);
            }
            return treeSplit(middle, key, pLeft, pRight);
        }
        // leaf
        if (key < tree->keys[0]) {
            *pRight = treeInsert(*pRight, tree->keys[0], tree->ptrs[0]);
            if (tree->full) {
                *pRight = treeInsert(*pRight, tree->keys[1], tree->ptrs[1]);
            }
            // not used anymore, and probably leaked
            free(tree);
            return NULL;
        }
        if (key == tree->keys[0]) {
            if (tree->full) {
                *pRight = treeInsert(*pRight, tree->keys[1], tree->ptrs[1]);
            }
            tree->parent = NULL;
            tree->keys[1] = 0;
            tree->ptrs[1] = NULL;
            return tree;
        }
        *pLeft = treeInsert(*pLeft, tree->keys[0], tree->ptrs[0]);
        if (tree->full && key < tree->keys[1]) {
            *pRight = treeInsert(*pRight, tree->keys[1], tree->ptrs[1]);
            free(tree);
            return NULL;
        }
        if (tree->full && key == tree->keys[1]) {
            tree->parent = NULL;
            tree->keys[0] = key;
            tree->ptrs[0] = tree->ptrs[1];
            tree->keys[1] = 0;
            tree->ptrs[1] = NULL;
            tree->full = false;
            return tree;
        }
        if (tree->full) {
            *pLeft = treeInsert(*pLeft, tree->keys[1], tree->ptrs[1]);
        }
        free(tree);
        return NULL;
    }
    return NULL;
}

tree_t* treeRemove(tree_t* tree, int key) {
    tree_t *left = NULL, *right = NULL;
    if (!treeFind(tree, key)) {
        return tree;
    }
    tree_t* m = treeSplit(tree, key, &left, &right);
    free(m);
    return treeMerge(left, right);
}
