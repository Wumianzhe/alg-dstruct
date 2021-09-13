#ifndef _LIST_H
#define _LIST_H 1

typedef struct node {
	struct node* pNext;
	void* data;
} node_t;

typedef struct list {
	node_t* pHead;
	int size;
} list_t;

list_t* listCreate();
void listDestroy(list_t* list);
node_t* nodeCreate(void* data);
void nodeFree(node_t* node);
void insertAfter(node_t* prev, node_t* node);
void insertHead(list_t* list, node_t* node);
void sortedInsert(list_t* list, node_t* node, int (sortfunc)(const void*,const void*));
void sortList(list_t* list, int (sortfunc)(const void*,const void*));

#endif
