
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
			.name			  = "White: 0-0, 0-0-0, Black: 0-0. 0-0-0 (valid)",
			.input_fen		  = "KQkq",
			.expected_success = true,
			.expected_oo	  = {true, true},
			.expected_ooo	  = {true, true},
		},
		{
			.name			  = "White: 0-0, 0-0-0 (valid)",
			.input_fen		  = "KQ",
			.expected_success = true,
			.expected_oo	  = {false, true},
			.expected_ooo	  = {false, true},
		},
		{
			.name			  = "White: 0-0, 0-0-0 (valid)",
			.input_fen		  = "kq",
			.expected_success = true,
			.expected_oo	  = {true, false},
			.expected_ooo	  = {true, false},
		},
		{
			.name			  = "White: 0-0-0, Black: 0-0 (valid)",
			.input_fen		  = "Qk", // reversed notation is accepted in medusa.
			.expected_success = true,
			.expected_oo	  = {true, false},
			.expected_ooo	  = {false, true},
		},
		{
			.name			  = "Empty string (invalid)",
			.input_fen		  = "",
			.expected_success = false,
			.expected_oo	  = {},
			.expected_ooo	  = {},
		},
		{
			.name			  = "White: 0-0, unknown char (invalid)",
			.input_fen		  = "Qx",
			.expected_success = false,
			.expected_oo	  = {},
			.expected_ooo	  = {},
		},
		{
			.name			  = "Too many chars (invalid)",
			.input_fen		  = "KQkqK",
			.expected_success = false,
			.expected_oo	  = {},
			.expected_ooo	  = {},
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

MunitResult test_fen_square_to_index(const MunitParameter params[], void *data) {
	const char *valid_squares[64] = {
		// clang-format off
		"a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
		"a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
		"a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
		"a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
		"a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
		"a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
		"a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
		"a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
		// clang-format on
	};
	const Square valid_squares_index[64] = {
		// clang-format off
		56, 57, 58, 59, 60, 61, 62, 63,
		48, 49, 50, 51, 52, 53, 54, 55,
		40, 41, 42, 43, 44, 45, 46, 47,
		32, 33, 34, 35, 36, 37, 38, 39,
		24, 25, 26, 27, 28, 29, 30, 31,
		16, 17, 18, 19, 20, 21, 22, 23,
		 8,  9, 10, 11, 12, 13, 14, 15,
		 0,  1,  2,  3,  4,  5,  6,  7,
		// clang-format on
	};

	munit_log(MUNIT_LOG_INFO, "testcase: all squares on board (valid)");
	Square sq;
	for(int i = 0; i < 64; i++) {
		bool ok = fen_square_to_index(valid_squares[i], &sq);
		munit_assert_true(ok);

		if(ok) {
			munit_assert_int(valid_squares_index[i], ==, sq);
		}
	}

	munit_log(MUNIT_LOG_INFO, "testcase: fantasy squares (invalid)");
	size_t		len				  = 6;
	const char *fantasy_squares[] = {"a0", "l8", "c9", "i2", "zz", "33"};
	for(int i = 0; i < len; i++) {
		bool ok = fen_square_to_index(fantasy_squares[i], &sq);
		munit_assert_false(ok);
	}

	munit_log(MUNIT_LOG_INFO, "testcase: malformed input (invalid)");
// Disable bounds check for this test.. should be handled by function.
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Warray-bounds"
	bool ok = fen_square_to_index("", &sq);
#pragma clang diagnostic pop
	munit_assert_false(ok);
	ok = fen_square_to_index("a88", &sq);
	munit_assert_false(ok);

	return MUNIT_OK;
}

MunitResult test_fen_parse_ep_square(const MunitParameter params[], void *data) {
	const char *valid_squares[16] = {
		// clang-format off
		"a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
		"a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
		// clang-format on
	};

	const Square valid_squares_index[16] = {
		// clang-format off
		40, 41, 42, 43, 44, 45, 46, 47,
		16, 17, 18, 19, 20, 21, 22, 23,
		// clang-format on
	};

	munit_log(MUNIT_LOG_INFO, "testcase: all squares on rank 3 and 6 (valid)");
	Square sq;
	for(int i = 0; i < 16; i++) {
		bool ok = fen_parse_ep_square(valid_squares[i], &sq);
		munit_assert_true(ok);

		if(ok) {
			munit_assert_int(valid_squares_index[i], ==, sq);
		}
	}

	const char *invalid_squares[48] = {
		// clang-format off
		"a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
		"a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
		"a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
		"a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
		"a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
		"a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
		// clang-format on
	};

	munit_log(MUNIT_LOG_INFO, "testcase: no ep square (valid)");
	bool ok = fen_parse_ep_square("-", &sq);
	munit_assert_true(ok);
	munit_assert_int(OTB, ==, sq);

	munit_log(MUNIT_LOG_INFO, "testcase: all squares on rank 1,2,4,5,7,8 (invalid)");
	for(int i = 0; i < 48; i++) {
		bool ok = fen_parse_ep_square(invalid_squares[i], &sq);
		munit_assert_false(ok);
	}

	munit_log(MUNIT_LOG_INFO, "testcase: malformed input (invalid)");
// Disable bounds check for this test.. should be handled by function.
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Warray-bounds"
	ok = fen_parse_ep_square("", &sq);
#pragma clang diagnostic pop
	munit_assert_false(ok);
	ok = fen_parse_ep_square("xx8", &sq);
	munit_assert_false(ok);

	return MUNIT_OK;
}

MunitResult test_fen_parse_move_number(const MunitParameter params[], void *data) {
	size_t num;

	munit_log(MUNIT_LOG_INFO, "testcase: move number 1 (valid)");
	bool ok = fen_parse_move_number("1", &num);
	munit_assert_true(ok);
	munit_assert_int(1, ==, num);

	munit_log(MUNIT_LOG_INFO, "testcase: move number 666 (valid)");
	ok = fen_parse_move_number("666", &num);
	munit_assert_true(ok);
	munit_assert_int(666, ==, num);

	munit_log(MUNIT_LOG_INFO, "testcase: move number 0 (invalid)");
	ok = fen_parse_move_number("0", &num);
	munit_assert_false(ok);

	munit_log(MUNIT_LOG_INFO, "testcase: string is not a number: \"22c\" (invalid)");
	ok = fen_parse_move_number("22c", &num);
	munit_assert_false(ok);

	munit_log(MUNIT_LOG_INFO, "testcase: string is not a number: \"x44\" (invalid)");
	ok = fen_parse_move_number("x44", &num);
	munit_assert_false(ok);

	munit_log(MUNIT_LOG_INFO, "testcase: string is hex number: \"0xEE\" (invalid)");
	ok = fen_parse_move_number("0xEE", &num);
	munit_assert_false(ok);

	return MUNIT_OK;
}

MunitResult test_fen_parse_half_move_clock(const MunitParameter params[], void *data) {
	size_t num;

	munit_log(MUNIT_LOG_INFO, "testcase: move number 1 (valid)");
	bool ok = fen_parse_half_move_clock("1", &num);
	munit_assert_true(ok);
	munit_assert_int(1, ==, num);

	munit_log(MUNIT_LOG_INFO, "testcase: move number 666 (valid)");
	ok = fen_parse_half_move_clock("666", &num);
	munit_assert_true(ok);
	munit_assert_int(666, ==, num);

	munit_log(MUNIT_LOG_INFO, "testcase: move number 0 (valid)");
	ok = fen_parse_half_move_clock("0", &num);
	munit_assert_true(ok);
	munit_assert_int(0, ==, num);

	munit_log(MUNIT_LOG_INFO, "testcase: string is not a number: \"22c\" (invalid)");
	ok = fen_parse_half_move_clock("22c", &num);
	munit_assert_false(ok);

	munit_log(MUNIT_LOG_INFO, "testcase: string is not a number: \"x44\" (invalid)");
	ok = fen_parse_half_move_clock("x44", &num);
	munit_assert_false(ok);

	munit_log(MUNIT_LOG_INFO, "testcase: string is hex number: \"0xEE\" (invalid)");
	ok = fen_parse_half_move_clock("0xEE", &num);
	munit_assert_false(ok);

	return MUNIT_OK;
}

MunitTest test_fen_suite[] = {
	{"parse_pieces", test_fen_parse_pieces, 0, 0, MUNIT_TEST_OPTION_NONE, 0},
	{"parse_color_to_move", test_fen_parse_color_to_move, 0, 0, MUNIT_TEST_OPTION_NONE, 0},
	{"parse_castling_rights", test_fen_parse_castling_rights, 0, 0, MUNIT_TEST_OPTION_NONE, 0},
	{"fen_square_to_index", test_fen_square_to_index, 0, 0, MUNIT_TEST_OPTION_NONE, 0},
	{"fen_ep_squares", test_fen_parse_ep_square, 0, 0, MUNIT_TEST_OPTION_NONE, 0},
	{"fen_move_number", test_fen_parse_move_number, 0, 0, MUNIT_TEST_OPTION_NONE, 0},
	{"fen_half_move_clock", test_fen_parse_half_move_clock, 0, 0, MUNIT_TEST_OPTION_NONE, 0},

	{0, 0, 0, 0, MUNIT_TEST_OPTION_NONE, 0},
};