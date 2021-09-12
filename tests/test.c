#include<exotic/cester.h>
// libcester test framework
// TODO find out how to remove language server error

// test verifying that everything built correctly
CESTER_TEST(test_zero, inst,
			cester_assert_equal((void*)0, NULL);
			)
