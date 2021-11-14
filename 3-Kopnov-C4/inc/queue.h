#ifndef QUEUE_H
#define QUEUE_H

typedef struct node {
    struct node* pNext;
    int data;
} node_t;

typedef struct {
    node_t* pHead;
    node_t* pTail;
    int size;
} queue_t;

void enqueue(queue_t* queue, int n);
int dequeue(queue_t* queue);
node_t* nodeCreate(int n);
queue_t* queueCreate();
void queueDelete(queue_t* queue);
void queuePrint(const queue_t* queue);

#endif
