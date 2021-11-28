#ifndef LIST_H
#define LIST_H 1

typedef struct node {
    int data;
    struct node* pNext;
} node_t;

typedef struct {
    node_t* pHead;
    node_t* pTail;
    int size;
} list_t;
// internals are the same, but types are different to differentiate between usage
typedef list_t stack_t;

node_t* nodeCreate(int n);
list_t* listCreate();
void listDelete(list_t* list);
void listPrint(const list_t* list);
void insertLast(list_t* list, int n);

void push(stack_t* stack, int n);
int pop(stack_t* stack);

#endif
