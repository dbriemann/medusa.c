#include <stdbool.h>

#define MUNIT_ENABLE_ASSERT_ALIASES
#include "../munit/munit.h"

#include "fen_test.h"

const MunitSuite fen_tests = {
	"tests: ",				/* name */
	test_fen_suite,			/* tests */
	NULL,					/* suites */
	1,						/* iterations */
	MUNIT_SUITE_OPTION_NONE /* options */
};

int main(int argc, char *argv[MUNIT_ARRAY_PARAM(argc + 1)]) {
	return munit_suite_main(&fen_tests, 0, argc, argv);
}
