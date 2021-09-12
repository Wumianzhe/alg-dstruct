typedef struct node {
	struct node* pNext;
	void* data;
} node_t;

typedef struct list {
	node_t* pHead;
	int size;
} list_t;

list_t* listCreate();
void listDelete(list_t* list);
node_t* nodeCreate(void* data);
void nodeFree(node_t* node);
