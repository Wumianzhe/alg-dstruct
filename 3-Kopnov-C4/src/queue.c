#include "queue.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

void enqueue(queue_t* queue, int n) {
    node_t* node = nodeCreate(n);
    if (!node) {
        perror("Enqueue malloc error");
        return;
    }
    if (queue->size > 0) {
        queue->pTail->pNext = node;
        queue->pTail = node;
    } else {
        queue->pHead = node;
        queue->pTail = node;
    }
    queue->size++;
}
int dequeue(queue_t* queue) {
    if (queue->size > 0) {
        node_t* node = queue->pHead;
        int val = node->data;
        queue->pHead = node->pNext;
        queue->size--;
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
queue_t* queueCreate() {
    queue_t* queue = malloc(sizeof(queue_t));
    if (queue) {
        queue->pHead = NULL;
        queue->pTail = NULL;
        queue->size = 0;
    }
    return queue;
}
void queueDelete(queue_t* queue) {
    node_t* node = queue->pHead;
    for (int i = 0; i < queue->size; i++) {
        node_t* tmp = node;
        node = node->pNext;
        free(tmp);
    }
    free(queue);
}
// for use within GDB to see contents
void queuePrint(const queue_t* queue) {
    node_t* node = queue->pHead;
    for (int i = 0; i < queue->size; i++) {
        printf("%d ", node->data);
        node = node->pNext;
    }
    printf("\n");
    fflush(stdout);
}
