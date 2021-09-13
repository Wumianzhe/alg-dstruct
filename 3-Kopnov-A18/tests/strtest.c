#include <criterion/criterion.h>
#include <criterion/internal/assert.h>
#include <strlist.h>

//test validity of strcmp results
Test(stringtests, comparison) {
	cr_assert(strcmp("less","more") < 0);
	cr_assert(strcmp("more","less") > 0);
	cr_assert(strcmp("equal","equal") == 0);
	cr_assert(strcmp("","") == 0);
	cr_assert(strcmp("","0") < 0);
}

//test whether strnodeCreate nodes are valid (not null and data is safe)
Test(stringtests, strnodes) {
	node_t* node=strnodeCreate("test");
	cr_assert_not_null(node,"Node creation failed");
	cr_assert_str_eq(node->data, "test", "Data corrupted");
	nodeFree(node);
}
