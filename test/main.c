#include <stdbool.h>

#define MUNIT_ENABLE_ASSERT_ALIASES
#include "../munit/munit.h"

#include "../src/fen.h"

typedef struct FenParsePiecesTestCase FenParsePiecesTestCase;
struct FenParsePiecesTestCase {
	const char *name;
	char		 *input_fen;
	const bool	expected_result;
	const Piece expected_pieces[64];
};

static MunitResult test_fen_parse_pieces(const MunitParameter params[], void *data) {
	const FenParsePiecesTestCase testcases[] = {
	// clang-format off
		{
			.name 			 = "Starting position (valid)",
			.input_fen		 = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR",
			.expected_result = true,
			.expected_pieces = {
				WROOK, WKNIGHT, WBISHOP, WQUEEN, WKING, WBISHOP, WKNIGHT, WROOK,
				WPAWN, WPAWN, WPAWN, WPAWN, WPAWN, WPAWN, WPAWN, WPAWN,
				EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, 
				EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, 
				EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, 
				EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, 
				BPAWN, BPAWN, BPAWN, BPAWN, BPAWN, BPAWN, BPAWN, BPAWN,
				BROOK, BKNIGHT, BBISHOP, BQUEEN, BKING, BBISHOP, BKNIGHT, BROOK,
			},
		},
		{
			.name 			 = "Sicilian after 1.e4 e5 (valid)",
			.input_fen		 = "rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR",
			.expected_result = true,
			.expected_pieces = {
				WROOK, WKNIGHT, WBISHOP, WQUEEN, WKING, WBISHOP, WKNIGHT, WROOK,
				WPAWN, WPAWN, WPAWN, WPAWN, EMPTY, WPAWN, WPAWN, WPAWN,
				EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, 
				EMPTY, EMPTY, EMPTY, EMPTY, WPAWN, EMPTY, EMPTY, EMPTY, 
				EMPTY, EMPTY, BPAWN, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, 
				EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, 
				BPAWN, BPAWN, EMPTY, BPAWN, BPAWN, BPAWN, BPAWN, BPAWN,
				BROOK, BKNIGHT, BBISHOP, BQUEEN, BKING, BBISHOP, BKNIGHT, BROOK,
			},
		},
		{
			.name 			 = "Sicilian after 1.e4 e5 2. Nf3 (valid)",
			.input_fen		 = "rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R",
			.expected_result = true,
			.expected_pieces = {
				WROOK, WKNIGHT, WBISHOP, WQUEEN, WKING, WBISHOP, EMPTY, WROOK,
				WPAWN, WPAWN, WPAWN, WPAWN, EMPTY, WPAWN, WPAWN, WPAWN,
				EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, WKNIGHT, EMPTY, EMPTY, 
				EMPTY, EMPTY, EMPTY, EMPTY, WPAWN, EMPTY, EMPTY, EMPTY, 
				EMPTY, EMPTY, BPAWN, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, 
				EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, 
				BPAWN, BPAWN, EMPTY, BPAWN, BPAWN, BPAWN, BPAWN, BPAWN,
				BROOK, BKNIGHT, BBISHOP, BQUEEN, BKING, BBISHOP, BKNIGHT, BROOK,
			},
		},
		{
			.name 			 = "Too many ranks (invalid)",
			.input_fen		 = "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR/pppPPppP",
			.expected_result = false,
			.expected_pieces = {},
		},
		{
			.name 			 = "Wrong number of empty fields (invalid)",
			.input_fen		 = "rnbqkbnr/pp1ppppp/7/2p5/4P3/8/PPPP1PPP/RNBQKBNR",
			.expected_result = false,
			.expected_pieces = {},
		},
		{
			.name 			 = "Unknown piece character (invalid)",
			.input_fen		 = "rnbqkbnr/pp1pppop/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R",
			.expected_result = false,
			.expected_pieces = {},
		},
	// clang-format on
	};
	Piece output[64];

	const size_t len = sizeof(testcases) / sizeof(FenParsePiecesTestCase);

	for(size_t tc = 0; tc < len; tc++) {
		munit_logf(MUNIT_LOG_INFO, "testcase %zu: %s", tc, testcases[tc].name);

		bool ok = fen_parse_pieces(testcases[tc].input_fen, output);
		munit_assert_int(testcases[tc].expected_result, ==, ok);

		if(ok) {
			for(size_t i = 0; i < 64; i++) {
				// munit_logf(MUNIT_LOG_WARNING, "%zu: %d / %d", i, target[i], result[i]);
				munit_assert_int(testcases[tc].expected_pieces[i], ==, output[i]);
			}
		}
	}

	return MUNIT_OK;
}

// Test suite is array of tests.
static MunitTest test_fen_suite[] = {
	{"parse_pieces", test_fen_parse_pieces, 0, 0, MUNIT_TEST_OPTION_NONE, 0},

 // End of tests.
	{			 0,					 0, 0, 0, MUNIT_TEST_OPTION_NONE, 0}
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
