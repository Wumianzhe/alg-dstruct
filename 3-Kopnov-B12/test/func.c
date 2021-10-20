#include "memallocator.h"
#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <stdio.h>

char buf[1024];
struct descriptor {
    int prevSize;
    int size;
    void* pNext;
};

Test(basic, allocate_and_free) {
    int poolSize = memgetminimumsize() + 1;
    meminit(&buf, poolSize);
    void* ptr = memalloc(1);
    struct descriptor* descr;
    cr_expect_not_null(ptr);
    descr = ptr - 16;
    // pNext of allocated node is NULL
    cr_expect_eq(descr->pNext, NULL);
    // size at beginning is size
    cr_expect_eq(descr->size, memgetblocksize() + 1);
    // size at end is -size
    cr_expect_eq(*(int*)(ptr + 1), -memgetblocksize() - 1);
    // can't allocate anything else
    cr_redirect_stderr();
    cr_expect_null(memalloc(0));
    fflush(stderr);
    cr_expect_stderr_eq_str("No space remaining\n");
    // freed block tests
    memfree(ptr);
    descr = (void*)&buf - 4;
    cr_expect_eq(descr->pNext, descr);
    // size at beginning is size
    cr_expect_eq(descr->size, poolSize);
    // size at end is size
    cr_expect_eq(*(int*)(ptr + 1), poolSize);
    memdone();
}

Test(basic, allocate_and_free_2) {
    int poolSize = memgetminimumsize() + memgetblocksize() + 2;
    meminit(&buf, poolSize);
    void* ptr1 = memalloc(1);
    void* ptr2 = memalloc(1);
    struct descriptor* descr;
    cr_expect_not_null(ptr1);
    cr_expect_not_null(ptr2);
    descr = ptr1 - 16;
    // pNext of allocated node is NULL
    cr_expect_eq(descr->pNext, NULL);
    // size at beginning is size
    cr_expect_eq(descr->size, memgetblocksize() + 1);
    // size at end is -size
    cr_expect_eq(*(int*)(ptr1 + 1), -memgetblocksize() - 1);
    /* cr_expect_null(memalloc(0)); */
    memfree(ptr1);
    descr = (void*)&buf - 4;
    struct descriptor* descr2 = ptr1 - 16;
    // descriptor added to list
    cr_expect_eq(descr->pNext, descr2);
    // descriptor size is accurate
    cr_expect_eq(descr2->size, memgetblocksize() + 1);
    // size at end is size
    /* cr_expect_eq(*(int *)(ptr1 + 1), m); */
    memfree(ptr2);
    memdone();
}

Test(basic, merge_right_and_leak) {
    cr_redirect_stderr();
    int poolSize = sizeof(buf);
    meminit(&buf, poolSize);
    void* ptr1 = memalloc(1);
    void* ptr2 = memalloc(1);
    void* ptr3 = memalloc(1);
    cr_expect(ptr1 && ptr2 && ptr3);
    memfree(ptr1);
    memfree(ptr2);
    struct descriptor* descr = ptr2 - 16;
    struct descriptor* head = (void*)&buf - 4;
    cr_expect_eq(head->pNext, descr);
    cr_expect_eq(descr->size, 2 * memgetblocksize() + 2);
    memdone();
    fflush(stderr);
    char str[100];
    sprintf(str, "MEMORY LEAKED: 1 bytes at %p\n", ptr3);
    cr_expect_stderr_eq_str(str);
}
