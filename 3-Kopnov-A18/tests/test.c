#include <criterion/criterion.h>
#include <criterion/internal/assert.h>
#include <list.h>

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

list_t* list=NULL;
node_t* nodes[3]={NULL,NULL,NULL};
int* data[3] = {NULL,NULL,NULL};

void setup(void) {
	list = malloc(sizeof(list_t));
	list->pHead=NULL;
	list->size=0;
	for (int i=0;i<3;i++) {
		nodes[i]=malloc(sizeof(list_t));
		nodes[i]->pNext=NULL;
		nodes[i]->data=NULL;
	}
}
void sortSetup(void) {
	setup();

	for(int i=0;i<3;i++) {
		data[i]=malloc(sizeof(int));
		*data[i]=i+1;
		nodes[i]->data=data[i];
	}
}

void teardown(void) {
	free(list);
	for (int i=0;i<3;i++) {
		free(nodes[i]);
	}
}
void sortTeardown(void) {
	for(int i=0;i<3;i++) {
		free(data[i]);
	}
	teardown();
}

Test(list, creation) {
	list = listCreate();
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

Test(list, head_change, .init=setup, .fini = teardown) {
	list->pHead=nodes[1];
	list->size=1;
	insertHead(list, nodes[0]);
	cr_assert_eq(list->pHead,nodes[0]);
	cr_assert_eq(list->size,2);
	cr_assert_eq(list->pHead->pNext,nodes[1]);
}

Test(list, appending, .init=setup, .fini = teardown) {
	nodes[0]->pNext=nodes[2];
	insertAfter(nodes[0],nodes[1]);
	cr_expect_eq(nodes[0]->pNext,nodes[1]);
	cr_expect_eq(nodes[0]->pNext->pNext,nodes[2]);
}

// sorting functionality will be tested based on integer lists for simplicity
int intcmp(const void* a,const void* b) {
	return *(int*)a-*(int*)b;
}

Test(list, sorting_insert, .init=sortSetup, .fini = sortTeardown) {
	insertHead(list, nodes[2]);
	sortedInsert(list,nodes[1],intcmp);
	sortedInsert(list,nodes[0],intcmp);
	cr_assert_eq(list->size,3);
	cr_expect(*(int*)list->pHead->data <= *(int*)list->pHead->pNext->data);
	cr_expect(*(int*)list->pHead->pNext->data <= *(int*)list->pHead->pNext->pNext->data);
}

Test(list, sort, .init=sortSetup, .fini = sortTeardown) {
	// empty list
	sortList(list,intcmp);
	cr_expect(list->size==0);
	cr_expect(list->pHead==NULL);

	//unsorted list
	list->pHead=nodes[2];
	list->pHead->pNext=nodes[0];
	nodes[0]->pNext=nodes[1];
	list->size=3;
	sortList(list,intcmp);
	cr_assert_eq(list->size,3);
	cr_expect(*(int*)list->pHead->data <= *(int*)list->pHead->pNext->data);
	cr_expect(*(int*)list->pHead->pNext->data <= *(int*)list->pHead->pNext->pNext->data);

	//sorted list
	list->pHead=nodes[0];
	nodes[0]->pNext=nodes[1];
	nodes[1]->pNext=nodes[2];
	nodes[2]->pNext=NULL;
	list->size=3;
	sortList(list,intcmp);
	cr_assert_eq(list->size,3);
	cr_expect(*(int*)list->pHead->data <= *(int*)list->pHead->pNext->data);
	cr_expect(*(int*)list->pHead->pNext->data <= *(int*)list->pHead->pNext->pNext->data);
}
