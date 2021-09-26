#include "list.h"
#include <stdlib.h>
#include <stdio.h>

list_t* listCreate() {
    list_t* res=malloc(sizeof(list_t));
    if(res) {
        res->pHead=NULL;
        res->size=0;
    } else {
        perror("Malloc failed");
    }
    return res;
}

void listDestroy(list_t* list) {
    if(list) {
        node_t *pNext=list->pHead, *pNode;
        for(int i=0;i<list->size;i++) {
            pNode=pNext;
            pNext=pNext->pNext;
            nodeFree(pNode);
        }
        free(list);
    }
}


void insertHead(list_t* list, node_t* node) {
    if(list&&node) {
        node->pNext=list->pHead;
        list->pHead=node;
        list->size++;
    }
}

node_t* nodeCreate(void* data) {
    node_t* res=malloc(sizeof(node_t));
    if(res) {
        res->pNext=NULL;
        res->data=data;
    } else {
        perror("Node creation failed");
    }
    return res;
}

void nodeFree(node_t* node) {
    if(node) {
        free(node);
    }
}

void insertAfter(node_t* prev, node_t *node) {
    if(prev&&node) {
        node->pNext=prev->pNext;
        prev->pNext=node;
    }
}

void sortList(list_t* list, int (sortfunc)(const void*,const void*)) {
    if(!list) {
        fprintf(stderr,"Trying to sort nullptr");
        return;
    }
    node_t* node=list->pHead;
    int size=list->size;
    // cut off list after head ifit exists.
    if(node) {
        list->size=1;
        node_t* next=node->pNext;
        node->pNext=NULL;
        node=next;
    } else {
        list->size=0;
    }
    // re-insert all nodes in sorted way
    for(int i=1;i<size;i++) {
        node_t* next=node->pNext;
        sortedInsert(list,node,sortfunc);
        node=next;
    }
}

void sortedInsert(list_t* list, node_t* node, int (sortfunc)(const void*,const void*)) {
    node_t *cur=list->pHead;
    if(sortfunc(node->data,cur->data)<0) {
        insertHead(list,node);
        return;
    }
    for(int i=1;i<list->size;i++) {
        node_t* next=cur->pNext;
        // if current node is less than next node
        if(sortfunc(node->data,next->data)<0) {
            insertAfter(cur,node);
            list->size++;
            return;
        }
        cur=next;
    }
    insertAfter(cur,node);
    list->size++;
}
