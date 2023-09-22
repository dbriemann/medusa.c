#include <stdbool.h>

#define MUNIT_ENABLE_ASSERT_ALIASES
#include "../munit/munit.h"

#include "fen_test.h"
#include "diverse_test.h"
#include "board_test.h"
#include "perft_test.h"

MunitSuite other_suites[] = {
	{ "diverse/", test_diverse_suite, NULL, 1, MUNIT_SUITE_OPTION_NONE },
	{ "board/", test_board_suite, NULL, 1, MUNIT_SUITE_OPTION_NONE },
	{ "perft/", test_perft_suite, NULL, 1, MUNIT_SUITE_OPTION_NONE },

	{ NULL, NULL, NULL, 0, MUNIT_SUITE_OPTION_NONE },
};

const MunitSuite fen_tests = {
	"tests: ",              /* name */
	test_fen_suite,         /* tests */
	other_suites,           /* suites */
	1,                      /* iterations */
	MUNIT_SUITE_OPTION_NONE /* options */
};

int main(int argc, char *argv[MUNIT_ARRAY_PARAM(argc + 1)]) {
	return munit_suite_main(&fen_tests, 0, argc, argv);
}
