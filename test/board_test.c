#include "board_test.h"
#include "../src/board.h"

MunitResult test_board__set_fen(const MunitParameter params[], void *data) {
	// clang-format off
	const BoardSetFenTestCase testcases[] = {
		{
			.name 			 = "Strange position (valid)",
			.input_fen		 = "1nbqkb1r/1ppppp1p/r4n2/p5pP/8/2N4R/PPPPPPP1/R1BQKBN1 w Qk g6 0 5",
			.expected_result = OK,
			.expected_board = {
				.castle_short = {true, false},
				.castle_long = {false, true},
				.move_number = 5,
				.draw_counter = 0,
				.check_info = OTB,
				.ep_square = 0x56, // g6
				.player = WHITE,
				.squares = {
					WROOK, EMPTY, WBISHOP, WQUEEN, WKING, WBISHOP, WKNIGHT, EMPTY,		EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,	
					WPAWN, WPAWN, WPAWN, WPAWN, WPAWN, WPAWN, WPAWN, EMPTY,				EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
					EMPTY, EMPTY, WKNIGHT, EMPTY, EMPTY, EMPTY, EMPTY, WROOK,			EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
					EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,				EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
					BPAWN, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, BPAWN, WPAWN,				EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
					BROOK, EMPTY, EMPTY, EMPTY, EMPTY, BKNIGHT, EMPTY, EMPTY,			EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
					EMPTY, BPAWN, BPAWN, BPAWN, BPAWN, BPAWN, EMPTY, BPAWN,				EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
					EMPTY, BKNIGHT, BBISHOP, BQUEEN, BKING, BBISHOP, EMPTY, BROOK,		EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
				},
				.kings = {0x74, 0x04},
				.queens = {
					{0x73, 0, 0, 0, 0, 0, 0, 0, 0},
					{0x03, 0, 0, 0, 0, 0, 0, 0, 0},
				},
				.queens_size = {1, 1},
				.rooks = {
					{0x77, 0x50, 0, 0, 0, 0, 0, 0, 0, 0},
					{0x00, 0x27, 0, 0, 0, 0, 0, 0, 0, 0},
				},
				.rooks_size = {2, 2},
				.bishops = {
					{0x72, 0x75, 0, 0, 0, 0, 0, 0, 0, 0},
					{0x02, 0x07, 0, 0, 0, 0, 0, 0, 0, 0},
				},
				.bishops_size = {2, 2},
				.sliders = {
					{0x72, 0x73, 0x75, 0x77, 0x50, 0, 0, 0, 0, 0, 0, 0, 0},
					{0x00, 0x02, 0x03, 0x05, 0x27, 0, 0, 0, 0, 0, 0, 0, 0},
				},
				.sliders_size = {5, 5},
				.knights = {
					{0x71, 0x55, 0, 0, 0, 0, 0, 0, 0, 0},
					{0x06, 0x22, 0, 0, 0, 0, 0, 0, 0, 0},
				},
				.knights_size = {2, 2},
				.pawns = {
					{0x61, 0x62, 0x63, 0x64, 0x65, 0x67, 0x40, 0x46},
					{0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x47},
				},
				.pawns_size = {8, 8},
			},
		},
	};
	// clang-format on

	const size_t len = sizeof(testcases) / sizeof(BoardSetFenTestCase);

	Board out_board;

	for(size_t tc = 0; tc < len; tc++) {
		munit_logf(MUNIT_LOG_INFO, "testcase %zu: %s", tc, testcases[tc].name);

		Error error = Board__set_fen(&out_board, testcases[tc].input_fen);
		munit_assert_int(testcases[tc].expected_result, ==, error);

		if(error == OK) {
			const Board *expb = &testcases[tc].expected_board;
			for(size_t i = 0; i < 2*64; i++) {
				munit_assert_int(expb->squares[i], ==, out_board.squares[i]);
			}
		}
	}

	// munit_log(MUNIT_LOG_INFO, "testcase: NULL args");
	// Error error = fen_parse_pieces(NULL, output);
	// munit_assert_int(ERR_NULL_PTR, ==, error);
	// error = fen_parse_pieces("does not matter", NULL);

	return MUNIT_OK;
}

MunitTest test_board_suite[] = {
	{"board__set_fen", test_board__set_fen, 0, 0, MUNIT_TEST_OPTION_NONE, 0},

	{0, 0, 0, 0, MUNIT_TEST_OPTION_NONE, 0},
};
