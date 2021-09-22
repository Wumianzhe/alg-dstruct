#include <criterion/criterion.h>
#include <criterion/internal/assert.h>
#include "list.h"

/* list.h functions
 * [+] listCreate
 * [?] listDestroy
 * [+] nodeCreate
 * [?] nodeFree
 * [+] insertAfter
 * [+] insertHead
 * [+] sortedInsert
 * [+] sortList
 */

list_t list;
node_t nodes[3];
int data[3];

void setup(void) {
    list.pHead=NULL;
    list.size=0;
    for(int i=0;i<3;i++) {
        nodes[i].pNext=NULL;
        nodes[i].data=NULL;
    }
}
void sortSetup(void) {
    setup();

    for(int i=0;i<3;i++) {
        data[i]=i+1;
        nodes[i].data=&data[i];
    }
}

Test(list, creation) {
    list_t* list = listCreate();
    cr_assert_not_null(list);
    cr_expect_eq(list->size,0);
    cr_expect_null(list->pHead);
    listDestroy(list);
    // don't know how to test if memory is freed
}

Test(list, node_creation) {
    void* data=malloc(4);
    node_t* node=nodeCreate(data);
    cr_assert_eq(node->data,data);
    cr_assert_null(node->pNext);
    nodeFree(node);
}

Test(list, head_change, .init=setup) {
    list.pHead=&nodes[1];
    list.size=1;
    insertHead(&list, &nodes[0]);
    cr_assert_eq(list.pHead,&nodes[0]);
    cr_assert_eq(list.size,2);
    cr_assert_eq(list.pHead->pNext,&nodes[1]);
}

Test(list, appending, .init=setup) {
    nodes[0].pNext=&nodes[2];
    insertAfter(&nodes[0],&nodes[1]);
    cr_expect_eq(nodes[0].pNext,&nodes[1]);
    cr_expect_eq(nodes[0].pNext->pNext,&nodes[2]);
}

// sorting functionality will be tested based on integer lists for simplicity
int intcmp(const void* a,const void* b) {
    return *(int*)a-*(int*)b;
}

Test(list, sorting_insert, .init=sortSetup) {
    insertHead(&list, &nodes[2]);
    sortedInsert(&list,&nodes[1],intcmp);
    sortedInsert(&list,&nodes[0],intcmp);
    cr_assert_eq(list.size,3);
    cr_expect(*(int*)list.pHead->data <= *(int*)list.pHead->pNext->data);
    cr_expect(*(int*)list.pHead->pNext->data <= *(int*)list.pHead->pNext->pNext->data);
}

Test(list, sort_empty, .init=sortSetup) {
    // empty list
    sortList(&list,intcmp);
    cr_expect(list.size==0);
    cr_expect(list.pHead==NULL);
}

Test(list, sort_nonempty, .init=sortSetup) {
    //unsorted list
    list.pHead=&nodes[2];
    list.pHead->pNext=&nodes[0];
    nodes[0].pNext=&nodes[1];
    list.size=3;
    sortList(&list,intcmp);
    cr_assert_eq(list.size,3);
    cr_expect(*(int*)list.pHead->data <= *(int*)list.pHead->pNext->data);
    cr_expect(*(int*)list.pHead->pNext->data <= *(int*)list.pHead->pNext->pNext->data);

    //sorted list
    list.pHead=&nodes[0];
    nodes[0].pNext=&nodes[1];
    nodes[1].pNext=&nodes[2];
    nodes[2].pNext=NULL;
    list.size=3;
    sortList(&list,intcmp);
    cr_assert_eq(list.size,3);
    cr_expect(*(int*)list.pHead->data <= *(int*)list.pHead->pNext->data);
    cr_expect(*(int*)list.pHead->pNext->data <= *(int*)list.pHead->pNext->pNext->data);
}
