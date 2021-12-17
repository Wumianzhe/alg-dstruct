#include "tree.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

int insert(tree_t* tree, int key) {
    if (!tree) {
        return 0;
    }
    int flag = 0;
    // do not add duplicates
    if (tree->key == key) {
        errno = ENOTSUP;
        return 0;
    }
    // if key is less, try inserting into left subtree
    if (tree->key > key) {
        if (tree->left) {
            flag = insert(tree->left, key);
            if (flag) {
                tree->size++;
            }
        } else {
            tree->left = treeCreate(key);
            if (tree->left) {
                tree->size++;
                flag = 1;
            }
        }
    }
    // if key is greater, try inserting into right subtree
    if (tree->key < key) {
        if (tree->right) {
            flag = insert(tree->right, key);
            if (flag) {
                tree->size++;
            }
        } else {
            tree->right = treeCreate(key);
            if (tree->right) {
                tree->size++;
                flag = 1;
            }
        }
    }
    return flag;
}

void treeDelete(tree_t* tree) {
    if (tree) {
        treeDelete(tree->left);
        treeDelete(tree->right);
        free(tree);
    }
}

tree_t* treeCreate(int key) {
    tree_t* tree = malloc(sizeof(tree_t));
    if (tree) {
        tree->key = key;
        tree->right = tree->left = NULL;
        tree->size = 1;
    }
    return tree;
}

tree_t* treeRead(FILE* in) {
    int key;
    // space matches any whitespace including newline (all of it, so it needs non-whitespace character to return)
    // when reading stdin use either duplicate or EOF keybind (C-d) on newline to stop reading
    fscanf(in, "%d ", &key);
    tree_t* tree = treeCreate(key);
    if (tree) {
        while (fscanf(in, "%d ", &key) > 0) {
            // does stop reading on duplicates
            if (!insert(tree, key)) {
                perror("Insertion failed");
                return tree;
            }
        }
    }
    return tree;
}

tree_t* kMin(tree_t* tree, int k) {
    int n = 1;
    if (!tree) {
        return NULL;
    }
    if (tree->left) {
        n += tree->left->size;
    }
    if (n != k) {
        // no fear of mangling as only one of paths is possible
        if (k < n) {
            tree = kMin(tree->left, k);
        } else {
            tree = kMin(tree->right, k - n);
        }
    }
    return tree;
}

void treePrint(tree_t* tree) {
    if (tree) {
        treePrint(tree->left);
        printf("%d ", tree->key);
        treePrint(tree->right);
    }
}

void treePrintLesser(tree_t* tree, int key, FILE* out) {
    if (tree) {
        treePrintLesser(tree->left, key, out);
        if (key >= tree->key) {
            fprintf(out, "%d ", tree->key);
            treePrintLesser(tree->right, key, out);
        }
    }
}

void treePrintLesserFancy(tree_t* tree, int key, int depth) {
    if (tree) {
        treePrintLesserFancy(tree->left, key, depth + 1);
        if (key >= tree->key) {
            for (int i = 0; i < 3 * depth; i++) {
                putchar(' ');
            }
            printf("%d\n", tree->key);
            treePrintLesserFancy(tree->right, key, depth + 1);
        }
    }
}

int wrapMain(FILE* in, FILE* out, int k) {
    tree_t* tree = treeRead(in);
    tree_t* sub = kMin(tree, k);
    if (sub) {
        if (sub->key % 2 == 0) {
            if (out == stdout) {
                treePrintLesserFancy(tree, sub->key, 0);
            } else {
                treePrintLesser(tree, sub->key, out);
            }
        }
    } else {
        fprintf(out, "Tree is too small\n");
    }
    treeDelete(tree);
    return 0;
}
