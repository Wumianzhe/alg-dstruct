#include "list.h"
#include <stdlib.h>
#include <stdio.h>

list_t* listCreate() {
	list_t* res = malloc(sizeof(list_t));
	if (res) {
		res->pHead = NULL;
		res->size = 0;
	} else {
		perror("Malloc failed");
	}
	return res;
}

void listDelete(list_t* list) {
	if(list) {
		node_t *pNext = list->pHead, *pNode;
		for (int i=0; i < list->size; i++) {
			pNode = pNext;
			pNext = pNext->pNext;
			nodeFree(pNode);
		}
		free(list);
	}
}

node_t* nodeCreate(void* data) {
	node_t* res = malloc(sizeof(node_t));
	if (res) {
		res->pNext = NULL;
		res->data = data;
	} else {
		perror("malloc failed");
	}
	return res;
}

void nodeFree(node_t* node) {
	if (node) {
		free(node->data);
		free(node);
	}
}
