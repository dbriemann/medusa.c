#include <stdbool.h>

#define MUNIT_ENABLE_ASSERT_ALIASES
#include "../munit/munit.h"

#include "../fen.h"

static MunitResult test_parse_fen_pieces_start_pos(const MunitParameter params[], void *data) {
	Piece result[64];
	Piece target[64] = {
		// clang-format off
		BROOK, BKNIGHT, BBISHOP, BQUEEN, BKING, BBISHOP, BKNIGHT, BROOK,
		BPAWN, BPAWN, BPAWN, BPAWN, BPAWN, BPAWN, BPAWN, BPAWN,
		EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, 
		EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, 
		EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, 
		EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, 
		WPAWN, WPAWN, WPAWN, WPAWN, WPAWN, WPAWN, WPAWN, WPAWN,
		WROOK, WKNIGHT, WBISHOP, WQUEEN, WKING, WBISHOP, WKNIGHT, WROOK,
		// clang-format on
	};

	bool ok = parse_fen_pieces("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR", result);
	munit_assert_int(true, ==, ok);

	for(int i = 0; i < 64; i++) {
		munit_assert_int(target[i], ==, result[i]);
	}
	return MUNIT_OK;
}

// Test suite is array of tests.
static MunitTest test_fen_suite[] = {
	{"parse_fen_pieces_start_pos", test_parse_fen_pieces_start_pos, 0, 0, MUNIT_TEST_OPTION_NONE, 0},

 // End of tests.
	{						   0,							   0, 0, 0, MUNIT_TEST_OPTION_NONE, 0}
};

static const MunitSuite fen_tests = {
	"fen tests: ",			/* name */
	test_fen_suite,			/* tests */
	NULL,					/* suites */
	1,						/* iterations */
	MUNIT_SUITE_OPTION_NONE /* options */
};

int main(int argc, char *argv[MUNIT_ARRAY_PARAM(argc + 1)]) {
	return munit_suite_main(&fen_tests, 0, argc, argv);
}
