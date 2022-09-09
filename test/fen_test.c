
#define MUNIT_ENABLE_ASSERT_ALIASES
#include "../munit/munit.h"

#include "../src/fen.h"
#include "fen_test.h"

MunitResult test_fen_parse_pieces(const MunitParameter params[], void *data) {
	const FenParsePiecesTestCase testcases[] = {
	// clang-format off
		{
			.name 			 = "Starting position (valid)",
			.input_fen		 = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR",
			.expected_success = true,
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
			.expected_success = true,
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
			.expected_success = true,
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
			.expected_success = false,
			.expected_pieces = {},
		},
		{
			.name 			 = "Wrong number of empty fields (invalid)",
			.input_fen		 = "rnbqkbnr/pp1ppppp/7/2p5/4P3/8/PPPP1PPP/RNBQKBNR",
			.expected_success = false,
			.expected_pieces = {},
		},
		{
			.name 			 = "Unknown piece character (invalid)",
			.input_fen		 = "rnbqkbnr/pp1pppop/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R",
			.expected_success = false,
			.expected_pieces = {},
		},
	// clang-format on
	};
	Piece output[64];

	const size_t len = sizeof(testcases) / sizeof(FenParsePiecesTestCase);

	for(size_t tc = 0; tc < len; tc++) {
		munit_logf(MUNIT_LOG_INFO, "testcase %zu: %s", tc, testcases[tc].name);

		bool ok = fen_parse_pieces(testcases[tc].input_fen, output);
		munit_assert_int(testcases[tc].expected_success, ==, ok);

		if(ok) {
			for(size_t i = 0; i < 64; i++) {
				// munit_logf(MUNIT_LOG_WARNING, "%zu: %d / %d", i, target[i], result[i]);
				munit_assert_int(testcases[tc].expected_pieces[i], ==, output[i]);
			}
		}
	}

	return MUNIT_OK;
}

MunitResult test_fen_parse_color_to_move(const MunitParameter params[], void *data) {
	Color c;

	munit_log(MUNIT_LOG_INFO, "testcase: color string is black: \"b\" (valid)");
	bool ok = fen_parse_color_to_move("b", &c);
	munit_assert_int(true, ==, ok);
	munit_assert_int(BLACK, ==, c);

	munit_log(MUNIT_LOG_INFO, "testcase: color string is white: \"w\" (valid)");
	ok = fen_parse_color_to_move("w", &c);
	munit_assert_int(true, ==, ok);
	munit_assert_int(WHITE, ==, c);

	munit_log(MUNIT_LOG_INFO, "testcase: color string is empty: \"\" (invalid)");
	ok = fen_parse_color_to_move("", &c);
	munit_assert_int(false, ==, ok);

	munit_log(MUNIT_LOG_INFO, "testcase: color string is too long: \"bw\" (invalid)");
	ok = fen_parse_color_to_move("bw", &c);
	munit_assert_int(false, ==, ok);

	return MUNIT_OK;
}

MunitResult test_fen_parse_castling_rights(const MunitParameter params[], void *data) {
	const FenParseCastlingRightsTestCase testcases[] = {
		{
			.name			  = "KQkq (valid)",
			.input_fen		  = "KQkq",
			.expected_success = true,
			.expected_oo	  = {true, true},
			.expected_ooo	  = {true, true},
		 },
	};

	const size_t len = sizeof(testcases) / sizeof(FenParseCastlingRightsTestCase);

	bool target_oo[2];
	bool target_ooo[2];

	for(size_t tc = 0; tc < len; tc++) {
		munit_logf(MUNIT_LOG_INFO, "testcase %zu: %s", tc, testcases[tc].name);

		// Prepare by setting the targets to the opposite of the expected result.
		target_oo[BLACK]  = !testcases[tc].expected_oo[BLACK];
		target_oo[WHITE]  = !testcases[tc].expected_oo[WHITE];
		target_ooo[BLACK] = !testcases[tc].expected_ooo[BLACK];
		target_ooo[WHITE] = !testcases[tc].expected_ooo[WHITE];

		bool ok = fen_parse_castling_rights(testcases[tc].input_fen, target_oo, target_ooo);
		munit_assert_int(testcases[tc].expected_success, ==, ok);

		if(ok) {
			munit_assert_int(testcases[tc].expected_oo[BLACK], ==, target_oo[BLACK]);
			munit_assert_int(testcases[tc].expected_oo[WHITE], ==, target_oo[WHITE]);
			munit_assert_int(testcases[tc].expected_ooo[BLACK], ==, target_ooo[BLACK]);
			munit_assert_int(testcases[tc].expected_ooo[WHITE], ==, target_ooo[WHITE]);
		}
	}
	return MUNIT_OK;
}

MunitTest test_fen_suite[] = {
	{		 "parse_pieces",			 test_fen_parse_pieces, 0, 0, MUNIT_TEST_OPTION_NONE, 0},
	{	 "parse_color_to_move",	test_fen_parse_color_to_move, 0, 0, MUNIT_TEST_OPTION_NONE, 0},
	{"parse_castling_rights", test_fen_parse_castling_rights, 0, 0, MUNIT_TEST_OPTION_NONE, 0},

	{					  0,							  0, 0, 0, MUNIT_TEST_OPTION_NONE, 0}
};
