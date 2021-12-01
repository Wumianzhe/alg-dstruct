#include "list.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

void insertLast(list_t* list, int n) {
    node_t* node = nodeCreate(n);
    if (!node) {
        perror("Insertion malloc error");
        return;
    }
    if (list->size > 0) {
        list->pTail->pNext = node;
        list->pTail = node;
    } else {
        list->pHead = node;
        list->pTail = node;
    }
    list->size++;
}
void push(myStack_t* stack, int n) {
    node_t* node = nodeCreate(n);
    if (!node) {
        perror("Insertion malloc error");
        return;
    }
    node->pNext = stack->pHead;
    stack->pHead = node;
    if (stack->size == 0) {
        stack->pTail = node;
    }
    stack->size++;
}
int pop(myStack_t* stack) {
    if (stack->size > 0) {
        node_t* node = stack->pHead;
        int val = node->data;
        stack->pHead = node->pNext;
        stack->size--;
        free(node);
        return val;
    } else {
        errno = EINVAL;
        return 0;
    }
}
node_t* nodeCreate(int n) {
    node_t* node = malloc(sizeof(node_t));
    if (node) {
        node->data = n;
        node->pNext = NULL;
    }
    return node;
}
list_t* listCreate() {
    list_t* list = malloc(sizeof(list_t));
    if (list) {
        list->pHead = NULL;
        list->pTail = NULL;
        list->size = 0;
    }
    return list;
}
void listDelete(list_t* list) {
    node_t* node = list->pHead;
    for (int i = 0; i < list->size; i++) {
        node_t* tmp = node;
        node = node->pNext;
        free(tmp);
    }
    free(list);
}

// for use within GDB to see contents
void listPrint(const list_t* list) {
    node_t* node = list->pHead;
    for (int i = 0; i < list->size; i++) {
        printf("%d ", node->data);
        node = node->pNext;
    }
    printf("\n");
    fflush(stdout);
}
