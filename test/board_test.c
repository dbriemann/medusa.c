#include "board_test.h"
#include "../src/base.h"
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
					{0x50, 0x77, 0, 0, 0, 0, 0, 0, 0, 0},
					{0x00, 0x27, 0, 0, 0, 0, 0, 0, 0, 0},
				},
				.rooks_size = {2, 2},
				.bishops = {
					{0x72, 0x75, 0, 0, 0, 0, 0, 0, 0, 0},
					{0x02, 0x05, 0, 0, 0, 0, 0, 0, 0, 0},
				},
				.bishops_size = {2, 2},
				.sliders = {
					{0x50, 0x72, 0x73, 0x75, 0x77, 0, 0, 0, 0, 0, 0, 0, 0},
					{0x00, 0x02, 0x03, 0x05, 0x27, 0, 0, 0, 0, 0, 0, 0, 0},
				},
				.sliders_size = {5, 5},
				.knights = {
					{0x55, 0x71, 0, 0, 0, 0, 0, 0, 0, 0},
					{0x06, 0x22, 0, 0, 0, 0, 0, 0, 0, 0},
				},
				.knights_size = {2, 2},
				.pawns = {
					{0x40, 0x46, 0x61, 0x62, 0x63, 0x64, 0x65, 0x67},
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
			for(size_t i = 0; i < 2 * 64; i++) {
				munit_assert_int(expb->squares[i], ==, out_board.squares[i]);
			}

			munit_assert_int(expb->move_number, ==, out_board.move_number);
			munit_assert_int(expb->draw_counter, ==, out_board.draw_counter);
			munit_assert_int(expb->check_info, ==, out_board.check_info);
			munit_assert_int(expb->ep_square, ==, out_board.ep_square);
			munit_assert_int(expb->player, ==, out_board.player);

			for(Color c = BLACK; c <= WHITE; c++) {
				// Castling rights
				munit_assert_int(expb->castle_short[c], ==, out_board.castle_short[c]);
				munit_assert_int(expb->castle_short[c], ==, out_board.castle_short[c]);

				// Test piece lists.
				munit_assert_int(expb->kings[c], ==, out_board.kings[c]);
				munit_assert_int(expb->sliders_size[c], ==, out_board.sliders_size[c]);
				for(size_t i = 0; i < out_board.sliders_size[c]; i++) {
					munit_assert_int(expb->sliders[c][i], ==, out_board.sliders[c][i]);
				}
				munit_assert_int(expb->queens_size[c], ==, out_board.queens_size[c]);
				for(size_t i = 0; i < out_board.queens_size[c]; i++) {
					munit_assert_int(expb->queens[c][i], ==, out_board.queens[c][i]);
				}
				munit_assert_int(expb->rooks_size[c], ==, out_board.rooks_size[c]);
				for(size_t i = 0; i < out_board.rooks_size[c]; i++) {
					munit_assert_int(expb->rooks[c][i], ==, out_board.rooks[c][i]);
				}
				munit_assert_int(expb->bishops_size[c], ==, out_board.bishops_size[c]);
				for(size_t i = 0; i < out_board.bishops_size[c]; i++) {
					munit_assert_int(expb->bishops[c][i], ==, out_board.bishops[c][i]);
				}
				munit_assert_int(expb->knights_size[c], ==, out_board.knights_size[c]);
				for(size_t i = 0; i < out_board.knights_size[c]; i++) {
					munit_assert_int(expb->knights[c][i], ==, out_board.knights[c][i]);
				}
				munit_assert_int(expb->pawns_size[c], ==, out_board.pawns_size[c]);
				for(size_t i = 0; i < out_board.pawns_size[c]; i++) {
					munit_assert_int(expb->pawns[c][i], ==, out_board.pawns[c][i]);
				}
			}
		}
	}

	// TODO: test rest of board info.. plist etc

	munit_log(MUNIT_LOG_INFO, "testcase 1: NULL args");
	Error error = Board__set_fen(NULL, "arg does not matter");
	munit_assert_int(ERR_NULL_PTR, ==, error);
	error = Board__set_fen(&out_board, NULL);
	munit_assert_int(ERR_NULL_PTR, ==, error);

	return MUNIT_OK;
}

MunitResult test_board__add_del_piece(const MunitParameter params[], void *data) {
	// Set up a starting position and validate the correctness including plists.
	const BoardAddDelPieceTestCase testcases[] = {
		{.piece = WPAWN, .sq = 0x10},
		{.piece = WPAWN, .sq = 0x11},
		{.piece = WPAWN, .sq = 0x12},
		{.piece = WPAWN, .sq = 0x13},
		{.piece = WPAWN, .sq = 0x14},
		{.piece = WPAWN, .sq = 0x15},
		{.piece = WPAWN, .sq = 0x16},
		{.piece = WPAWN, .sq = 0x17},
		{.piece = WROOK, .sq = 0x00},
		{.piece = WKNIGHT, .sq = 0x01},
		{.piece = WBISHOP, .sq = 0x02},
		{.piece = WQUEEN, .sq = 0x03},
		{.piece = WKING, .sq = 0x04},
		{.piece = WBISHOP, .sq = 0x05},
		{.piece = WKNIGHT, .sq = 0x06},
		{.piece = WROOK, .sq = 0x07},

		{.piece = BPAWN, .sq = 0x60},
		{.piece = BPAWN, .sq = 0x61},
		{.piece = BPAWN, .sq = 0x62},
		{.piece = BPAWN, .sq = 0x63},
		{.piece = BPAWN, .sq = 0x64},
		{.piece = BPAWN, .sq = 0x65},
		{.piece = BPAWN, .sq = 0x66},
		{.piece = BPAWN, .sq = 0x67},
		{.piece = BROOK, .sq = 0x70},
		{.piece = BKNIGHT, .sq = 0x71},
		{.piece = BBISHOP, .sq = 0x72},
		{.piece = BQUEEN, .sq = 0x73},
		{.piece = BKING, .sq = 0x74},
		{.piece = BBISHOP, .sq = 0x75},
		{.piece = BKNIGHT, .sq = 0x76},
		{.piece = BROOK, .sq = 0x77},

		// Plus one nonsense add operation outside of board. Should be ignored.
		{.piece = BQUEEN, .sq = OTB},
	};

	// Manually set up a starting board for comparison.
	Board starting_board = {
		// clang-format off
		.squares = {
			WROOK, WKNIGHT, WBISHOP, WQUEEN, WKING, WBISHOP, WKNIGHT, WROOK,		EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,	
			WPAWN, WPAWN, WPAWN, WPAWN, WPAWN, WPAWN, WPAWN, WPAWN,				EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
			EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,			EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
			EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,				EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
			EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,				EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
			EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,			EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
			BPAWN, BPAWN, BPAWN, BPAWN, BPAWN, BPAWN, BPAWN, BPAWN,				EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
			BROOK, BKNIGHT, BBISHOP, BQUEEN, BKING, BBISHOP, BKNIGHT, BROOK,		EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
		},
		.kings = {0x74, 0x04},
		.queens = {
			{0x73, 0, 0, 0, 0, 0, 0, 0, 0},
			{0x03, 0, 0, 0, 0, 0, 0, 0, 0},
		},
		.queens_size = {1, 1},
		.rooks = {
			{0x70, 0x77, 0, 0, 0, 0, 0, 0, 0, 0},
			{0x00, 0x07, 0, 0, 0, 0, 0, 0, 0, 0},
		},
		.rooks_size = {2, 2},
		.bishops = {
			{0x72, 0x75, 0, 0, 0, 0, 0, 0, 0, 0},
			{0x02, 0x05, 0, 0, 0, 0, 0, 0, 0, 0},
		},
		.bishops_size = {2, 2},
		.sliders = {
			{0x70, 0x72, 0x73, 0x75, 0x77, 0, 0, 0, 0, 0, 0, 0, 0},
			{0x00, 0x02, 0x03, 0x05, 0x07, 0, 0, 0, 0, 0, 0, 0, 0},
		},
		.sliders_size = {5, 5},
		.knights = {
			{0x71, 0x76, 0, 0, 0, 0, 0, 0, 0, 0},
			{0x01, 0x06, 0, 0, 0, 0, 0, 0, 0, 0},
		},
		.knights_size = {2, 2},
		.pawns = {
			{0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67},
			{0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17},
		},
		.pawns_size = {8, 8},
		// clang-format on
	};

	Board out_board;
	Board__clear(&out_board);

	const size_t len = sizeof(testcases) / sizeof(BoardAddDelPieceTestCase);
	for(size_t i = 0; i < len; i++) {
		BoardAddDelPieceTestCase tc = testcases[i];
		Board__add_piece(&out_board, tc.sq, tc.piece);
	}

	// Test board squares.
	for(size_t i = 0; i < 64 * 2; i++) {
		munit_assert_int(starting_board.squares[i], ==, out_board.squares[i]);
	}

	// Test piece lists.
	for(Color c = BLACK; c <= WHITE; c++) {
		munit_assert_int(starting_board.kings[c], ==, out_board.kings[c]);
		munit_assert_int(starting_board.sliders_size[c], ==, out_board.sliders_size[c]);
		for(size_t i = 0; i < out_board.sliders_size[c]; i++) {
			munit_assert_int(starting_board.sliders[c][i], ==, out_board.sliders[c][i]);
		}
		munit_assert_int(starting_board.queens_size[c], ==, out_board.queens_size[c]);
		for(size_t i = 0; i < out_board.queens_size[c]; i++) {
			munit_assert_int(starting_board.queens[c][i], ==, out_board.queens[c][i]);
		}
		munit_assert_int(starting_board.rooks_size[c], ==, out_board.rooks_size[c]);
		for(size_t i = 0; i < out_board.rooks_size[c]; i++) {
			munit_assert_int(starting_board.rooks[c][i], ==, out_board.rooks[c][i]);
		}
		munit_assert_int(starting_board.bishops_size[c], ==, out_board.bishops_size[c]);
		for(size_t i = 0; i < out_board.bishops_size[c]; i++) {
			munit_assert_int(starting_board.bishops[c][i], ==, out_board.bishops[c][i]);
		}
		munit_assert_int(starting_board.knights_size[c], ==, out_board.knights_size[c]);
		for(size_t i = 0; i < out_board.knights_size[c]; i++) {
			munit_assert_int(starting_board.knights[c][i], ==, out_board.knights[c][i]);
		}
		munit_assert_int(starting_board.pawns_size[c], ==, out_board.pawns_size[c]);
		for(size_t i = 0; i < out_board.pawns_size[c]; i++) {
			munit_assert_int(starting_board.pawns[c][i], ==, out_board.pawns[c][i]);
		}
	}

	for(size_t i = 0; i < len; i++) {
		BoardAddDelPieceTestCase tc = testcases[i];
		Board__del_piece(&out_board, tc.sq);
	}

	// Test board squares.
	for(size_t i = 0; i < 64 * 2; i++) {
		munit_assert_int(EMPTY, ==, out_board.squares[i]);
	}

	// Test piece lists.
	for(Color c = BLACK; c <= WHITE; c++) {
		munit_assert_int(0, ==, out_board.sliders_size[c]);
		munit_assert_int(0, ==, out_board.queens_size[c]);
		munit_assert_int(0, ==, out_board.rooks_size[c]);
		munit_assert_int(0, ==, out_board.bishops_size[c]);
		munit_assert_int(0, ==, out_board.knights_size[c]);
		munit_assert_int(0, ==, out_board.pawns_size[c]);
	}

	return MUNIT_OK;
}

MunitResult test_board__clear_funcs(const MunitParameter params[], void *data) {
	Board board;

	munit_log(MUNIT_LOG_INFO, "Board__clear");
	for(size_t i = 0; i < 2 * 64; i++) {
		board.squares[i] = OTB;
	}
	Board__clear(&board);
	for(size_t i = 0; i < 2 * 64; i++) {
		munit_assert_int(EMPTY, ==, board.squares[i]);
	}

	munit_log(MUNIT_LOG_INFO, "Board__clear_meta");
	for(size_t i = 0; i < 2 * 64; i++) {
		board.squares[i] = OTB;
	}
	Board__clear_meta(&board);
	for(size_t i = 0; i < 2 * 64; i++) {
		if(on_board(i)) {
			munit_assert_int(OTB, ==, board.squares[i]);
		} else {
			munit_assert_int(INFO_NONE, ==, board.squares[i]);
		}
	}

	return MUNIT_OK;
}

MunitResult test_board__is_sq_attacked(const MunitParameter params[], void *data) {
	const IsSqAttackedTestCase testcases[] = {
		// clang-format off
		{
			.name = "constructed, only pawns",
			.fen = "k7/7p/5ppP/3ppPP1/1ppPP3/pPP5/P7/7K w - - 0 1",
			.attacked_by_black = {
				false, false, false, false, false, false, false, false,
				false, true, false, false, false, false, false, false,
				true, true, true, true, false, false, false, false,
				false, false, true, true, true, true, false, false,
				false, false, false, false, true, true, true, true,
				false, false, false, false, false, false, true, false,
				true, true, false, false, false, false, false, false,
				false, true, false, false, false, false, false, false,
			},
			.attacked_by_white = {
				false, false, false, false, false, false, true, false,
				false, false, false, false, false, false, true, true,
				false, true, false, false, false, false, false, false,
				true, true, true, true, false, false, false, false,
				false, false, true, true, true, true, false, false,
				false, false, false, false, true, true, true, true,
				false, false, false, false, false, false, true, false,
				false, false, false, false, false, false, false, false,
			},
		},
		{
			.name = "real position: sevian vs. caruana (usc 2022)",
			.fen = "r1bqr1k1/3n1pp1/3b1n1p/1ppP4/8/P1NNB3/1P2B1PP/R2Q1RK1 w - - 2 17",
			.attacked_by_black = {
				false, false, false, false, false, false, false, false,
				false, false, false, false, false, false, false, true,
				true, false, false, false, true, false, true, false,
				true, true, true, true, true, true, true, false,
				true, false, true, true, true, false, true, true,
				true, true, false, false, true, true, true, true,
				true, true, true, true, true, true, true, true,
				false, true, true, true, true, true, true, true,
			},
			.attacked_by_white = {
				true, true, true, true, true, true, true, true,
				true, true, true, true, true, true, true, true,
				true, true, true, true, false, true, true, true,
				true, true, false, true, true, true, true, false,
				false, true, true, true, true, true, true, true,
				false, false, true, false, true, true, false, true,
				false, false, false, false, false, false, false, false,
				false, false, false, false, false, false, false, false,
			},
		},
		// clang-format on
	};

	const size_t len = sizeof(testcases) / sizeof(IsSqAttackedTestCase);

	for(size_t tc = 0; tc < len; tc++) {
		munit_logf(MUNIT_LOG_INFO, "testcase %zu: %s", tc, testcases[tc].name);

		Board board;
		Error error = Board__set_fen(&board, testcases[tc].fen);
		munit_assert_int(OK, ==, error);

		for(size_t i = 0; i < 64; i++) {
			const Square sq = LOOKUP_0x88[i];
			// munit_logf(MUNIT_LOG_INFO, "i: %zu, sq:%d\n", i, sq);
			// Attacked by black.
			bool attack_check = Board__is_sq_attacked(&board, sq, OTB, WHITE);
			munit_assert_int(testcases[tc].attacked_by_black[i], ==, attack_check);
			// Attacked by white.
			attack_check = Board__is_sq_attacked(&board, sq, OTB, BLACK);
			munit_assert_int(testcases[tc].attacked_by_white[i], ==, attack_check);
		}
	}

	return MUNIT_OK;
}

MunitResult test_board__detect_checks_and_pins(const MunitParameter params[], void *data) {
	const DetectChecksAndPinsTestCase testcases[] = {
		// clang-format off
		{
			.name = "white pawn checks black king",
			.fen  = "8/8/8/2k1p3/3P4/4K3/8/8 b - - 0 1",
			.expected_board = {
				.check_info = 0x33,
				.ep_square	= OTB,
				.player		= BLACK,
				.squares = {
					// We only care for the info board .. thus the regular squares are just set to EMPTY.
					EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,		INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE,
					EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,		INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE,
					EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, 	INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE,
					EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, 	INFO_NONE, INFO_NONE, INFO_NONE, INFO_MASK_CHECK, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE,
					EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,		INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE,
					EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, 	INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE,
					EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, 	INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE,
					EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, 	INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE,
				}
			}
		},
		{
			.name = "black knight checks white king",
			.fen  = "8/8/8/8/5nk1/8/6K1/8 w - - 0 1",
			.expected_board = {
				.check_info = 0x35,
				.ep_square	= OTB,
				.player		= WHITE,
				.squares = {
					// We only care for the info board .. thus the regular squares are just set to EMPTY.
					EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,		INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE,
					EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,		INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE,
					EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, 	INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE,
					EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, 	INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_MASK_CHECK, INFO_NONE, INFO_NONE,
					EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,		INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE,
					EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, 	INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE,
					EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, 	INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE,
					EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, 	INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE,
				}
			}
		},
		{
			.name = "white bishop checks black king",
			.fen  = "2B5/8/8/8/6k1/8/6K1/8 b - - 0 1",
			.expected_board = {
				.check_info = 0x72,
				.ep_square	= OTB,
				.player		= WHITE,
				.squares = {
					// We only care for the info board .. thus the regular squares are just set to EMPTY.
					EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,		INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE,
					EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,		INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE,
					EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, 	INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_MASK_FORBIDDEN_ESCAPE,
					EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, 	INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE,
					EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,		INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_MASK_CHECK, INFO_NONE, INFO_NONE,
					EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, 	INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_MASK_CHECK, INFO_NONE, INFO_NONE, INFO_NONE,
					EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, 	INFO_NONE, INFO_NONE, INFO_NONE, INFO_MASK_CHECK, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE,
					EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, 	INFO_NONE, INFO_NONE, INFO_MASK_CHECK, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE,
				}
			}
		},
		{
			.name = "white rook checks black king",
			.fen  = "8/8/8/8/1R3k2/8/5K2/8 b - - 0 1",
			.expected_board = {
				.check_info = 0x31,
				.ep_square	= OTB,
				.player		= WHITE,
				.squares = {
					// We only care for the info board .. thus the regular squares are just set to EMPTY.
					EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,		INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE,
					EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,		INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE,
					EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, 	INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE,
					EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, 	INFO_NONE, INFO_MASK_CHECK, INFO_MASK_CHECK, INFO_MASK_CHECK, INFO_MASK_CHECK, INFO_NONE, INFO_MASK_FORBIDDEN_ESCAPE, INFO_NONE,
					EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,		INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE,
					EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, 	INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE,
					EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, 	INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE,
					EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, 	INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE,
				}
			}
		},
		{
			.name = "white queen checks black king (mate)",
			.fen  = "k7/1Q6/1K6/8/8/8/8/8 b - - 0 1",
			.expected_board = {
				.check_info = 0x61,
				.ep_square	= OTB,
				.player		= WHITE,
				.squares = {
					// We only care for the info board .. thus the regular squares are just set to EMPTY.
					EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,		INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE,
					EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,		INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE,
					EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, 	INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE,
					EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, 	INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE,
					EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,		INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE,
					EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, 	INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE,
					EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, 	INFO_NONE, INFO_MASK_CHECK, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE,
					EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, 	INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE, INFO_NONE,
				}
			}
		},
	};
	// clang-format on

	const size_t len = sizeof(testcases) / sizeof(DetectChecksAndPinsTestCase);

	Board board;

	for(size_t tc = 0; tc < len; tc++) {
		munit_logf(MUNIT_LOG_INFO, "testcase %zu: %s", tc, testcases[tc].name);

		Error error = Board__set_fen(&board, testcases[tc].fen);
		munit_assert_int(OK, ==, error);

		Board__detect_checks_and_pins(&board, board.player);

		for(size_t i = 0; i < 2 * 64; i++) {
			// Only check info board squares.. this test does not care about the pieces.
			if(!on_board(i)) {
				// munit_logf(MUNIT_LOG_INFO, "i: %zu\n", i);
				munit_assert_int(testcases[tc].expected_board.squares[i], ==, board.squares[i]);
			}
		}

		// Test check_info
		munit_assert_int(testcases[tc].expected_board.check_info, ==, board.check_info);
	}

	return MUNIT_OK;
}

MunitTest test_board_suite[] = {
	{"board__set_fen", test_board__set_fen, 0, 0, MUNIT_TEST_OPTION_NONE, 0},
	{"board__add_del_piece", test_board__add_del_piece, 0, 0, MUNIT_TEST_OPTION_NONE, 0},
	{"board__clear_funcs", test_board__clear_funcs, 0, 0, MUNIT_TEST_OPTION_NONE, 0},
	{"board__is_sq_attacked", test_board__is_sq_attacked, 0, 0, MUNIT_TEST_OPTION_NONE, 0},
	{"board__detect_checks_and_pins", test_board__detect_checks_and_pins, 0, 0, MUNIT_TEST_OPTION_NONE, 0},

	{0, 0, 0, 0, MUNIT_TEST_OPTION_NONE, 0},
};