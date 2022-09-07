#include <stdbool.h>

#define MUNIT_ENABLE_ASSERT_ALIASES
#include "../munit/munit.h"

#include "../fen.h"

static MunitResult test_fen_parse_pieces_start_pos(const MunitParameter params[], void *data) {
	Piece result[64];
	Piece target[64] = {
		// clang-format off
		WROOK, WKNIGHT, WBISHOP, WQUEEN, WKING, WBISHOP, WKNIGHT, WROOK,
		WPAWN, WPAWN, WPAWN, WPAWN, WPAWN, WPAWN, WPAWN, WPAWN,
		EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, 
		EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, 
		EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, 
		EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, 
		BPAWN, BPAWN, BPAWN, BPAWN, BPAWN, BPAWN, BPAWN, BPAWN,
		BROOK, BKNIGHT, BBISHOP, BQUEEN, BKING, BBISHOP, BKNIGHT, BROOK,
		// clang-format on
	};

	bool ok = fen_parse_pieces("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR", result);
	munit_assert_int(true, ==, ok);

	for(size_t i = 0; i < 64; i++) {
		// munit_logf(MUNIT_LOG_WARNING, "%zu: %d / %d", i, target[i], result[i]);
		munit_assert_int(target[i], ==, result[i]);
	}
	return MUNIT_OK;
}

// Test suite is array of tests.
static MunitTest test_fen_suite[] = {
	{"parse_pieces: valid: start_pos", test_fen_parse_pieces_start_pos, 0, 0, MUNIT_TEST_OPTION_NONE, 0},

 // End of tests.
	{							   0,							   0, 0, 0, MUNIT_TEST_OPTION_NONE, 0}
};

static const MunitSuite fen_tests = {
	"fen_tests: ",			/* name */
	test_fen_suite,			/* tests */
	NULL,					/* suites */
	1,						/* iterations */
	MUNIT_SUITE_OPTION_NONE /* options */
};

int main(int argc, char *argv[MUNIT_ARRAY_PARAM(argc + 1)]) {
	return munit_suite_main(&fen_tests, 0, argc, argv);
}
