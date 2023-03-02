#include <string.h>

#define MUNIT_ENABLE_ASSERT_ALIASES
#include "../munit/munit.h"

#include "../src/errors.h"
#include "../src/fen.h"
#include "fen_test.h"

MunitResult test_fen_parse_pieces(const MunitParameter params[], void* data) {
	const FenParsePiecesTestCase testcases[] = {
		{
			.name            = "Starting position (valid)",
			.input_fen       = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR",
			.expected_result = OK,
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
			.name            = "Sicilian after 1.e4 e5 (valid)",
			.input_fen       = "rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR",
			.expected_result = OK,
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
			.name            = "Sicilian after 1.e4 e5 2. Nf3 (valid)",
			.input_fen       = "rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R",
			.expected_result = OK,
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
			.name            = "Strange position (valid)",
			.input_fen       = "1nbqkb1r/1ppppp1p/r4n2/p5pP/8/2N4R/PPPPPPP1/R1BQKBN1",
			.expected_result = OK,
			.expected_pieces = {
				WROOK, EMPTY, WBISHOP, WQUEEN, WKING, WBISHOP, WKNIGHT, EMPTY,
				WPAWN, WPAWN, WPAWN, WPAWN, WPAWN, WPAWN, WPAWN, EMPTY,
				EMPTY, EMPTY, WKNIGHT, EMPTY, EMPTY, EMPTY, EMPTY, WROOK,
				EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
				BPAWN, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, BPAWN, WPAWN,
				BROOK, EMPTY, EMPTY, EMPTY, EMPTY, BKNIGHT, EMPTY, EMPTY,
				EMPTY, BPAWN, BPAWN, BPAWN, BPAWN, BPAWN, EMPTY, BPAWN,
				EMPTY, BKNIGHT, BBISHOP, BQUEEN, BKING, BBISHOP, EMPTY, BROOK,
			},
		},
		{
			.name            = "Too many ranks (invalid)",
			.input_fen       = "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR/pppPPppP",
			.expected_result = ERR_INVALID_INPUT,
			.expected_pieces = {},
		},
		{
			.name            = "Wrong number of empty fields (invalid)",
			.input_fen       = "rnbqkbnr/pp1ppppp/7/2p5/4P3/8/PPPP1PPP/RNBQKBNR",
			.expected_result = ERR_INVALID_INPUT,
			.expected_pieces = {},
		},
		{
			.name            = "Unknown piece character (invalid)",
			.input_fen       = "rnbqkbnr/pp1pppop/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R",
			.expected_result = ERR_INVALID_INPUT,
			.expected_pieces = {},
		},
	};
	Piece                        output[64];

	const size_t len = sizeof(testcases) / sizeof(FenParsePiecesTestCase);

	for(size_t tc = 0; tc < len; tc++) {
		munit_logf(MUNIT_LOG_INFO, "testcase %zu: %s", tc, testcases[tc].name);

		Error error = fen_parse_pieces(testcases[tc].input_fen, output);
		munit_assert_int(testcases[tc].expected_result, ==, error);

		if(error == OK) {
			for(size_t i = 0; i < 64; i++) {
				munit_assert_int(testcases[tc].expected_pieces[i], ==, output[i]);
			}
		}
	}

	munit_log(MUNIT_LOG_INFO, "testcase: NULL args");
	Error error = fen_parse_pieces(NULL, output);
	munit_assert_int(ERR_NULL_PTR, ==, error);
	error = fen_parse_pieces("does not matter", NULL);
	munit_assert_int(ERR_NULL_PTR, ==, error);

	return MUNIT_OK;
}

MunitResult test_fen_parse_color_to_move(const MunitParameter params[], void* data) {
	Color c;

	munit_log(MUNIT_LOG_INFO, "testcase: color string is black: \"b\" (valid)");
	Error error = fen_parse_color_to_move("b", &c);
	munit_assert_int(OK, ==, error);
	munit_assert_int(BLACK, ==, c);

	munit_log(MUNIT_LOG_INFO, "testcase: color string is white: \"w\" (valid)");
	error = fen_parse_color_to_move("w", &c);
	munit_assert_int(OK, ==, error);
	munit_assert_int(WHITE, ==, c);

	munit_log(MUNIT_LOG_INFO, "testcase: color string is empty: \"\" (invalid)");
	error = fen_parse_color_to_move("", &c);
	munit_assert_int(ERR_INVALID_INPUT, ==, error);

	munit_log(MUNIT_LOG_INFO, "testcase: color string is too long: \"bw\" (invalid)");
	error = fen_parse_color_to_move("bw", &c);
	munit_assert_int(ERR_INVALID_INPUT, ==, error);

	munit_log(MUNIT_LOG_INFO, "testcase: NULL args");
	error = fen_parse_color_to_move(NULL, &c);
	munit_assert_int(ERR_NULL_PTR, ==, error);
	error = fen_parse_color_to_move("does not matter", NULL);
	munit_assert_int(ERR_NULL_PTR, ==, error);

	return MUNIT_OK;
}

MunitResult test_fen_parse_castling_rights(const MunitParameter params[], void* data) {
	const FenParseCastlingRightsTestCase testcases[] = {
		{
			.name            = "White: 0-0, 0-0-0, Black: 0-0. 0-0-0 (valid)",
			.input_fen       = "KQkq",
			.expected_result = OK,
			.expected_oo     = { true, true },
			.expected_ooo    = { true, true },
		},
		{
			.name            = "White: 0-0, 0-0-0 (valid)",
			.input_fen       = "KQ",
			.expected_result = OK,
			.expected_oo     = { false, true },
			.expected_ooo    = { false, true },
		},
		{
			.name            = "White: 0-0, 0-0-0 (valid)",
			.input_fen       = "kq",
			.expected_result = OK,
			.expected_oo     = { true, false },
			.expected_ooo    = { true, false },
		},
		{
			.name            = "White: 0-0-0, Black: 0-0 (valid)",
			.input_fen       = "Qk", // reversed notation is accepted in medusa.
			.expected_result = OK,
			.expected_oo     = { true, false },
			.expected_ooo    = { false, true },
		},
		{
			.name            = "Empty string (invalid)",
			.input_fen       = "",
			.expected_result = ERR_INVALID_INPUT,
			.expected_oo     = {},
			.expected_ooo    = {},
		},
		{
			.name            = "White: 0-0, unknown char (invalid)",
			.input_fen       = "Qx",
			.expected_result = ERR_INVALID_INPUT,
			.expected_oo     = {},
			.expected_ooo    = {},
		},
		{
			.name            = "Too many chars (invalid)",
			.input_fen       = "KQkqK",
			.expected_result = ERR_INVALID_INPUT,
			.expected_oo     = {},
			.expected_ooo    = {},
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

		Error error = fen_parse_castling_rights(testcases[tc].input_fen, target_oo, target_ooo);
		munit_assert_int(testcases[tc].expected_result, ==, error);

		if(error == OK) {
			munit_assert_int(testcases[tc].expected_oo[BLACK], ==, target_oo[BLACK]);
			munit_assert_int(testcases[tc].expected_oo[WHITE], ==, target_oo[WHITE]);
			munit_assert_int(testcases[tc].expected_ooo[BLACK], ==, target_ooo[BLACK]);
			munit_assert_int(testcases[tc].expected_ooo[WHITE], ==, target_ooo[WHITE]);
		}
	}

	munit_log(MUNIT_LOG_INFO, "testcase: NULL args");
	Error error = fen_parse_castling_rights(NULL, target_oo, target_ooo);
	munit_assert_int(ERR_NULL_PTR, ==, error);
	error = fen_parse_castling_rights("does not matter", NULL, target_ooo);
	munit_assert_int(ERR_NULL_PTR, ==, error);
	error = fen_parse_castling_rights("does not matter", target_oo, NULL);
	munit_assert_int(ERR_NULL_PTR, ==, error);

	return MUNIT_OK;
}

MunitResult test_fen_square_to_index(const MunitParameter params[], void* data) {
	const char*  valid_squares[64] = {
		"a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
		"a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
		"a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
		"a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
		"a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
		"a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
		"a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
		"a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
	};
	const Square valid_squares_index[64] = {
		56, 57, 58, 59, 60, 61, 62, 63,
		48, 49, 50, 51, 52, 53, 54, 55,
		40, 41, 42, 43, 44, 45, 46, 47,
		32, 33, 34, 35, 36, 37, 38, 39,
		24, 25, 26, 27, 28, 29, 30, 31,
		16, 17, 18, 19, 20, 21, 22, 23,
		8,  9, 10, 11, 12, 13, 14, 15,
		0,  1,  2,  3,  4,  5,  6,  7,
	};

	munit_log(MUNIT_LOG_INFO, "testcase: all squares on board (valid)");
	Square sq;
	for(size_t i = 0; i < 64; i++) {
		Error error = fen_square_to_index(valid_squares[i], &sq);
		munit_assert_int(OK, ==, error);

		if(error == OK) {
			munit_assert_int(valid_squares_index[i], ==, sq);
		}
	}

	munit_log(MUNIT_LOG_INFO, "testcase: fantasy squares (invalid)");
	size_t      len               = 6;
	const char* fantasy_squares[] = { "a0", "l8", "c9", "i2", "zz", "33" };
	for(size_t i = 0; i < len; i++) {
		Error error = fen_square_to_index(fantasy_squares[i], &sq);
		munit_assert_int(ERR_INVALID_INPUT, ==, error);
	}

	munit_log(MUNIT_LOG_INFO, "testcase: malformed input (invalid)");
	Error error = fen_square_to_index("", &sq);
	munit_assert_int(ERR_INVALID_INPUT, ==, error);
	error = fen_square_to_index("a88", &sq);
	munit_assert_int(ERR_INVALID_INPUT, ==, error);

	munit_log(MUNIT_LOG_INFO, "testcase: NULL args");
	error = fen_square_to_index(NULL, &sq);
	munit_assert_int(ERR_NULL_PTR, ==, error);
	error = fen_square_to_index("does not matter", NULL);
	munit_assert_int(ERR_NULL_PTR, ==, error);

	return MUNIT_OK;
}

MunitResult test_fen_parse_ep_square(const MunitParameter params[], void* data) {
	const char* valid_squares[16] = {
		"a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
		"a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
	};

	const Square valid_squares_index[16] = {
		40, 41, 42, 43, 44, 45, 46, 47,
		16, 17, 18, 19, 20, 21, 22, 23,
	};

	munit_log(MUNIT_LOG_INFO, "testcase: all squares on rank 3 and 6 (valid)");
	Square sq;
	for(size_t i = 0; i < 16; i++) {
		Error error = fen_parse_ep_square(valid_squares[i], &sq);
		munit_assert_int(OK, ==, error);

		if(error == OK) {
			munit_assert_int(valid_squares_index[i], ==, sq);
		}
	}

	const char* invalid_squares[48] = {
		"a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
		"a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
		"a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
		"a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
		"a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
		"a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
	};

	munit_log(MUNIT_LOG_INFO, "testcase: no ep square (valid)");
	Error error = fen_parse_ep_square("-", &sq);
	munit_assert_int(OK, ==, error);
	munit_assert_int(OTB, ==, sq);

	munit_log(MUNIT_LOG_INFO, "testcase: all squares on rank 1,2,4,5,7,8 (invalid)");
	for(size_t i = 0; i < 48; i++) {
		Error error = fen_parse_ep_square(invalid_squares[i], &sq);
		munit_assert_int(ERR_INVALID_INPUT, ==, error);
	}

	munit_log(MUNIT_LOG_INFO, "testcase: malformed input (invalid)");
	error = fen_parse_ep_square("", &sq);
	munit_assert_int(ERR_INVALID_INPUT, ==, error);
	error = fen_parse_ep_square("xx8", &sq);
	munit_assert_int(ERR_INVALID_INPUT, ==, error);

	munit_log(MUNIT_LOG_INFO, "testcase: NULL args");
	error = fen_parse_ep_square(NULL, &sq);
	munit_assert_int(ERR_NULL_PTR, ==, error);
	error = fen_parse_ep_square("does not matter", NULL);
	munit_assert_int(ERR_NULL_PTR, ==, error);

	return MUNIT_OK;
}

MunitResult test_fen_parse_move_number(const MunitParameter params[], void* data) {
	uint16_t num;

	munit_log(MUNIT_LOG_INFO, "testcase: move number 1 (valid)");
	Error error = fen_parse_move_number("1", &num);
	munit_assert_int(OK, ==, error);
	munit_assert_int(1, ==, num);

	munit_log(MUNIT_LOG_INFO, "testcase: move number 666 (valid)");
	error = fen_parse_move_number("666", &num);
	munit_assert_int(OK, ==, error);
	munit_assert_int(666, ==, num);

	munit_log(MUNIT_LOG_INFO, "testcase: move number 0 (invalid)");
	error = fen_parse_move_number("0", &num);
	munit_assert_int(ERR_INVALID_INPUT, ==, error);

	munit_log(MUNIT_LOG_INFO, "testcase: string is not a number: \"22c\" (invalid)");
	error = fen_parse_move_number("22c", &num);
	munit_assert_int(ERR_INVALID_INPUT, ==, error);

	munit_log(MUNIT_LOG_INFO, "testcase: string is not a number: \"x44\" (invalid)");
	error = fen_parse_move_number("x44", &num);
	munit_assert_int(ERR_INVALID_INPUT, ==, error);

	munit_log(MUNIT_LOG_INFO, "testcase: string is hex number: \"0xEE\" (invalid)");
	error = fen_parse_move_number("0xEE", &num);
	munit_assert_int(ERR_INVALID_INPUT, ==, error);

	munit_log(MUNIT_LOG_INFO, "testcase: NULL args");
	error = fen_parse_move_number(NULL, &num);
	munit_assert_int(ERR_NULL_PTR, ==, error);
	error = fen_parse_move_number("does not matter", NULL);
	munit_assert_int(ERR_NULL_PTR, ==, error);

	return MUNIT_OK;
}

MunitResult test_fen_parse_half_move_clock(const MunitParameter params[], void* data) {
	uint16_t num;

	munit_log(MUNIT_LOG_INFO, "testcase: move number 1 (valid)");
	Error error = fen_parse_half_move_clock("1", &num);
	munit_assert_int(OK, ==, error);
	munit_assert_int(1, ==, num);

	munit_log(MUNIT_LOG_INFO, "testcase: move number 666 (valid)");
	error = fen_parse_half_move_clock("666", &num);
	munit_assert_int(OK, ==, error);
	munit_assert_int(666, ==, num);

	munit_log(MUNIT_LOG_INFO, "testcase: move number 0 (valid)");
	error = fen_parse_half_move_clock("0", &num);
	munit_assert_int(OK, ==, error);
	munit_assert_int(0, ==, num);

	munit_log(MUNIT_LOG_INFO, "testcase: string is not a number: \"22c\" (invalid)");
	error = fen_parse_half_move_clock("22c", &num);
	munit_assert_int(ERR_INVALID_INPUT, ==, error);

	munit_log(MUNIT_LOG_INFO, "testcase: string is not a number: \"x44\" (invalid)");
	error = fen_parse_half_move_clock("x44", &num);
	munit_assert_int(ERR_INVALID_INPUT, ==, error);

	munit_log(MUNIT_LOG_INFO, "testcase: string is hex number: \"0xEE\" (invalid)");
	error = fen_parse_half_move_clock("0xEE", &num);
	munit_assert_int(ERR_INVALID_INPUT, ==, error);

	munit_log(MUNIT_LOG_INFO, "testcase: NULL args");
	error = fen_parse_half_move_clock(NULL, &num);
	munit_assert_int(ERR_NULL_PTR, ==, error);
	error = fen_parse_half_move_clock("does not matter", NULL);
	munit_assert_int(ERR_NULL_PTR, ==, error);

	return MUNIT_OK;
}

MunitResult test_parse_fen(const MunitParameter params[], void* data) {
	ParseFenTestCase
		testcases[] =
	{
		{
			.name            = "Start position",
			.input_fen       = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
			.expected_result = OK,
			.expected_mb     =
			{
				.squares =
				{
					WROOK, WKNIGHT, WBISHOP, WQUEEN, WKING, WBISHOP, WKNIGHT, WROOK,
					WPAWN, WPAWN, WPAWN, WPAWN, WPAWN, WPAWN, WPAWN, WPAWN,
					EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
					EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
					EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
					EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
					BPAWN, BPAWN, BPAWN, BPAWN, BPAWN, BPAWN, BPAWN, BPAWN,
					BROOK, BKNIGHT, BBISHOP, BQUEEN, BKING, BBISHOP, BKNIGHT, BROOK,
				},
				.color        = WHITE,
				.castle_short = { true, true },
				.castle_long  = { true, true },
				.half_moves   = 0,
				.move_num     = 1,
				.ep_square    = OTB,
			},
		},
		{
			.name            = "Ep passent & no castles",
			.input_fen       = "rnbq1bnr/pp2kppp/4p3/2ppP3/8/2N5/PPPPKPPP/R1BQ1BNR w - d6 0 5",
			.expected_result = OK,
			.expected_mb     =
			{
				.squares =
				{
					WROOK, EMPTY, WBISHOP, WQUEEN, EMPTY, WBISHOP, WKNIGHT, WROOK,
					WPAWN, WPAWN, WPAWN, WPAWN, WKING, WPAWN, WPAWN, WPAWN,
					EMPTY, EMPTY, WKNIGHT, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
					EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
					EMPTY, EMPTY, BPAWN, BPAWN, WPAWN, EMPTY, EMPTY, EMPTY,
					EMPTY, EMPTY, EMPTY, EMPTY, BPAWN, EMPTY, EMPTY, EMPTY,
					BPAWN, BPAWN, EMPTY, EMPTY, BKING, BPAWN, BPAWN, BPAWN,
					BROOK, BKNIGHT, BBISHOP, BQUEEN, EMPTY, BBISHOP, BKNIGHT, BROOK,
				},
				.color        = WHITE,
				.castle_short = { false, false },
				.castle_long  = { false, false },
				.half_moves   = 0,
				.move_num     = 5,
				.ep_square    = 43,
			},
		},
	};

	MinBoard out_board;

	const size_t len = sizeof(testcases) / sizeof(ParseFenTestCase);

	for(size_t tc = 0; tc < len; tc++) {
		munit_logf(MUNIT_LOG_INFO, "testcase %zu: %s", tc, testcases[tc].name);

		Error error = parse_fen(testcases[tc].input_fen, &out_board);
		munit_assert_int(testcases[tc].expected_result, ==, error);

		if(error == OK) {
			const MinBoard* emb = &testcases[tc].expected_mb;
			munit_assert_int(emb->color, ==, out_board.color);
			for(size_t i = BLACK; i <= WHITE; i++) {
				munit_assert_int(emb->castle_short[i], ==, out_board.castle_short[i]);
				munit_assert_int(emb->castle_long[i], ==, out_board.castle_long[i]);
			}
			munit_assert_int(emb->ep_square, ==, out_board.ep_square);
			munit_assert_int(emb->half_moves, ==, out_board.half_moves);
			munit_assert_int(emb->move_num, ==, out_board.move_num);
			for(size_t i = 0; i < 64; i++) {
				munit_assert_int(emb->squares[i], ==, out_board.squares[i]);
			}
		}
	}

	munit_log(MUNIT_LOG_INFO, "testcase: NULL args");
	Error error = parse_fen(NULL, &out_board);
	munit_assert_int(ERR_NULL_PTR, ==, error);
	error = fen_parse_pieces("does not matter", NULL);
	munit_assert_int(ERR_NULL_PTR, ==, error);

	return MUNIT_OK;
}

MunitTest test_fen_suite[] = {
	{ "fen/parse_pieces", test_fen_parse_pieces, 0, 0, MUNIT_TEST_OPTION_NONE, 0 },
	{ "fen/parse_color_to_move", test_fen_parse_color_to_move, 0, 0, MUNIT_TEST_OPTION_NONE, 0 },
	{ "fen/parse_castling_rights", test_fen_parse_castling_rights, 0, 0, MUNIT_TEST_OPTION_NONE, 0 },
	{ "fen/parse_square_to_index", test_fen_square_to_index, 0, 0, MUNIT_TEST_OPTION_NONE, 0 },
	{ "fen/parse_ep_squares", test_fen_parse_ep_square, 0, 0, MUNIT_TEST_OPTION_NONE, 0 },
	{ "fen/parse_move_number", test_fen_parse_move_number, 0, 0, MUNIT_TEST_OPTION_NONE, 0 },
	{ "fen/parse_half_move_clock", test_fen_parse_half_move_clock, 0, 0, MUNIT_TEST_OPTION_NONE, 0 },
	{ "fen/parse_fen", test_parse_fen, 0, 0, MUNIT_TEST_OPTION_NONE, 0 },

	{ 0, 0, 0, 0, MUNIT_TEST_OPTION_NONE, 0 },
};
