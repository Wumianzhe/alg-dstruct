#include <criterion/criterion.h>
#include <criterion/internal/assert.h>
#include "strlist.h"

//test whether strnodeCreate nodes are valid (not null and data is safe)
Test(stringtests, strnodes) {
	node_t* node=strnodeCreate("test");
	cr_assert_not_null(node,"Node creation failed");
	cr_assert_str_eq(node->data, "test", "Data corrupted");
	nodeFree(node);
}

list_t list;
node_t nodes[3];

void sortSetup(void) {
    list.pHead=NULL;
    list.size=0;
    for(int i=0;i<3;i++) {
        nodes[i].pNext=NULL;
    }
	nodes[0].data="less";
	nodes[1].data="more";
	nodes[2].data="data";
}

Test(strlist, sort_empty, .init=sortSetup) {
    // empty list
    sortstrList(&list);
    cr_expect(list.size==0);
    cr_expect(list.pHead==NULL);
}

Test(strlist, sort_nonempty, .init=sortSetup) {
    //unsorted list
    list.pHead=&nodes[0];
    list.pHead->pNext=&nodes[1];
    nodes[1].pNext=&nodes[2];
    list.size=3;
    sortstrList(&list);
    cr_assert_eq(list.size,3);
    cr_expect_str_leq(list.pHead->data,list.pHead->pNext->data);
    cr_expect_str_leq(list.pHead->pNext->data,list.pHead->pNext->pNext->data);

    //sorted list
    list.pHead=&nodes[2];
    nodes[2].pNext=&nodes[0];
    nodes[0].pNext=&nodes[1];
    nodes[1].pNext=NULL;
    list.size=3;
    sortstrList(&list);
    cr_assert_eq(list.size,3);
    cr_expect_str_leq(list.pHead->data,list.pHead->pNext->data);
    cr_expect_str_leq(list.pHead->pNext->data,list.pHead->pNext->pNext->data);
}
