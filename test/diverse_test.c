#include "diverse_test.h"
#include "../src/plist.h"
#include "../src/bitmove.h"
#include "../src/base.h"
#include "../src/mlist.h"

#pragma clang diagnostic ignored "-Wunused-parameter"

MunitResult test_piecelist__add(const MunitParameter params[], void *data) {
	Square sq         = 0x34; // e4
	Piece  bqueens[9] = { 0 };
	size_t len        = 0;

	PieceList__add(bqueens, &len, sq);

	munit_assert_int(sq, ==, bqueens[0]);
	munit_assert_int(len, ==, 1);

	return MUNIT_OK;
}

MunitResult test_piecelist__del(const MunitParameter params[], void *data) {
	Piece  pieces[8] = { 0x11, 0x56, 0x22, 0, 0, 0, 0, 0 };
	size_t len       = 3;

	// Not in list, no changes.
	Piece  want_pieces1[8] = { 0x11, 0x56, 0x22, 0, 0, 0, 0, 0 };
	size_t want_len1       = 3;
	PieceList__del(pieces, &len, 0x88);
	munit_assert_int(want_len1, ==, len);
	for(size_t i = 0; i < 8; i++) {
		// We check all elements in test, not only up to len.
		munit_assert_int(want_pieces1[i], ==, pieces[i]);
	}

	// First element is replaced by last. which then remains in the list twice
	// and len is reduced by 1, rendering the duplicate "dead".
	Piece  want_pieces2[8] = { 0x22, 0x56, 0x22, 0, 0, 0, 0, 0 };
	size_t want_len2       = 2;
	PieceList__del(pieces, &len, 0x11);
	munit_assert_int(want_len2, ==, len);
	for(size_t i = 0; i < 8; i++) {
		// We check all elements in test, not only up to len.
		munit_assert_int(want_pieces2[i], ==, pieces[i]);
	}

	return MUNIT_OK;
}

MunitResult test_piecelist__del_index(const MunitParameter params[], void *data) {
	Piece  pieces[8] = { 0x11, 0x56, 0x22, 0, 0, 0, 0, 0 };
	size_t len       = 3;

	// Delete element at index 0 until len == 0:
	// 1. {0x22, 0x56, 0x22, 0, 0, 0, 0, 0} --> len 2
	// 2. {0x56, 0x56, 0x22, 0, 0, 0, 0, 0} --> len 1
	// 3. {0x56, 0x56, 0x22, 0, 0, 0, 0, 0} --> len 0
	Piece want_pieces1[8] = { 0x56, 0x56, 0x22, 0, 0, 0, 0, 0 };
	while(len > 0) {
		PieceList__del_index(pieces, &len, 0);
	}
	for(size_t i = 0; i < 8; i++) {
		// We check all elements in test, not only up to len.
		munit_assert_int(want_pieces1[i], ==, pieces[i]);
	}

	return MUNIT_OK;
}

MunitResult test_bitmove_all(const MunitParameter params[], void *data) {
	// We don't care about the underlying representation in the test,
	// just that what is put into a BitMove also comes back out of it
	// using the extraction functions.
	{
		const Square  e2   = 0x14;
		const Square  e4   = 0x34;
		const BitMove e2e4 = BitMove__new(WPAWN, e2, e4, PROMO_NONE, EMPTY, CASTLE_NONE, false);
		munit_assert_int(e2, ==, BitMove__from(e2e4));
		munit_assert_int(e4, ==, BitMove__to(e2e4));
		munit_assert_int(PROMO_NONE, ==, BitMove__promoted_piece(e2e4));
		munit_assert_int(WPAWN, ==, BitMove__piece(e2e4));
		munit_assert_int(EMPTY, ==, BitMove__captured_piece(e2e4));
		munit_assert_int(CASTLE_NONE, ==, BitMove__castle_type(e2e4));
		munit_assert_int(false, ==, BitMove__en_passent(e2e4));
		char *e2e4str = BitMove__to_notation(e2e4);
		munit_assert_string_equal(" e2-e4", e2e4str);
		free(e2e4str);
	}

	{
		const Square  g7    = 0x66;
		const Square  g8    = 0x76;
		const BitMove g7g8Q = BitMove__new(WPAWN, g7, g8, WQUEEN, EMPTY, CASTLE_NONE, false);
		munit_assert_int(g7, ==, BitMove__from(g7g8Q));
		munit_assert_int(g8, ==, BitMove__to(g7g8Q));
		munit_assert_int(WQUEEN, ==, BitMove__promoted_piece(g7g8Q));
		munit_assert_int(WPAWN, ==, BitMove__piece(g7g8Q));
		munit_assert_int(EMPTY, ==, BitMove__captured_piece(g7g8Q));
		munit_assert_int(CASTLE_NONE, ==, BitMove__castle_type(g7g8Q));
		munit_assert_int(false, ==, BitMove__en_passent(g7g8Q));
		munit_log(MUNIT_LOG_INFO, BitMove__to_notation(g7g8Q));
		char *g7g8Qstr = BitMove__to_notation(g7g8Q);
		munit_assert_string_equal(" g7-g8=Q", g7g8Qstr);
		free(g7g8Qstr);
	}

	{
		const Square  a1    = 0x0;
		const Square  h8    = 0x77;
		const BitMove ba1h8 = BitMove__new(BBISHOP, a1, h8, PROMO_NONE, WQUEEN, CASTLE_NONE, false);
		munit_assert_int(a1, ==, BitMove__from(ba1h8));
		munit_assert_int(h8, ==, BitMove__to(ba1h8));
		munit_assert_int(PROMO_NONE, ==, BitMove__promoted_piece(ba1h8));
		munit_assert_int(BBISHOP, ==, BitMove__piece(ba1h8));
		munit_assert_int(WQUEEN, ==, BitMove__captured_piece(ba1h8));
		munit_assert_int(CASTLE_NONE, ==, BitMove__castle_type(ba1h8));
		munit_assert_int(false, ==, BitMove__en_passent(ba1h8));
		munit_log(MUNIT_LOG_INFO, BitMove__to_notation(ba1h8));
		char *ba1h8str = BitMove__to_notation(ba1h8);
		munit_assert_string_equal("ba1xh8", ba1h8str);
		free(ba1h8str);
	}

	{
		const Square  d4     = 0x33;
		const Square  c3     = 0x22;
		const BitMove d4c3ep = BitMove__new(BPAWN, d4, c3, PROMO_NONE, WPAWN, CASTLE_NONE, true);
		munit_assert_int(d4, ==, BitMove__from(d4c3ep));
		munit_assert_int(c3, ==, BitMove__to(d4c3ep));
		munit_assert_int(PROMO_NONE, ==, BitMove__promoted_piece(d4c3ep));
		munit_assert_int(BPAWN, ==, BitMove__piece(d4c3ep));
		munit_assert_int(WPAWN, ==, BitMove__captured_piece(d4c3ep));
		munit_assert_int(CASTLE_NONE, ==, BitMove__castle_type(d4c3ep));
		munit_assert_int(true, ==, BitMove__en_passent(d4c3ep));
		munit_log(MUNIT_LOG_INFO, BitMove__to_notation(d4c3ep));
		char *d4c3epstr = BitMove__to_notation(d4c3ep);
		munit_assert_string_equal(" d4xc3ep", d4c3epstr);
		free(d4c3epstr);
	}

	{
		const Square  e1 = 0x04;
		const Square  g1 = 0x06;
		const BitMove oo = BitMove__new(WKING, e1, g1, PROMO_NONE, EMPTY, CASTLE_OO, false);
		munit_assert_int(e1, ==, BitMove__from(oo));
		munit_assert_int(g1, ==, BitMove__to(oo));
		munit_assert_int(PROMO_NONE, ==, BitMove__promoted_piece(oo));
		munit_assert_int(WKING, ==, BitMove__piece(oo));
		munit_assert_int(EMPTY, ==, BitMove__captured_piece(oo));
		munit_assert_int(CASTLE_OO, ==, BitMove__castle_type(oo));
		munit_assert_int(false, ==, BitMove__en_passent(oo));
		munit_log(MUNIT_LOG_INFO, BitMove__to_notation(oo));
		char *oostr = BitMove__to_notation(oo);
		munit_assert_string_equal("0-0", oostr);
		free(oostr);
	}

	{
		const Square  e1 = 0x04;
		const Square  c1 = 0x02;
		const BitMove oo = BitMove__new(WKING, e1, c1, PROMO_NONE, EMPTY, CASTLE_OOO, false);
		munit_assert_int(e1, ==, BitMove__from(oo));
		munit_assert_int(c1, ==, BitMove__to(oo));
		munit_assert_int(PROMO_NONE, ==, BitMove__promoted_piece(oo));
		munit_assert_int(WKING, ==, BitMove__piece(oo));
		munit_assert_int(EMPTY, ==, BitMove__captured_piece(oo));
		munit_assert_int(CASTLE_OOO, ==, BitMove__castle_type(oo));
		munit_assert_int(false, ==, BitMove__en_passent(oo));
		munit_log(MUNIT_LOG_INFO, BitMove__to_notation(oo));
		char *oostr = BitMove__to_notation(oo);
		munit_assert_string_equal("0-0-0", oostr);
		free(oostr);
	}

	return MUNIT_OK;
}

MunitResult test_utility_functions(const MunitParameter params[], void *data) {
	munit_log(MUNIT_LOG_INFO, "func on_board");
	for(Square sq = 0; sq < 2 * 64; sq++) {
		bool is_on_board = on_board(sq);
		bool result      = false;
		for(size_t i = 0; i < 64; i++) {
			if(LOOKUP_0x88[i] == sq) {
				result = true;
				break;
			}
		}
		munit_assert_int(result, ==, is_on_board);
	}

	munit_log(MUNIT_LOG_INFO, "func square_diff");
	Square diff = square_diff(0x7, 0x17);
	munit_assert_int(0x10 + 0x77, ==, diff);
	diff = square_diff(0x17, 0x7);
	munit_assert_int(-0x10 + 0x77, ==, diff);

	diff = square_diff(0x77, 0x0);
	munit_assert_int(-0x77 + 0x77, ==, diff);
	diff = square_diff(0x0, 0x77);
	munit_assert_int(0x77 + 0x77, ==, diff);

	diff = square_diff(0x40, 0x47);
	munit_assert_int(0x7 + 0x77, ==, diff);
	diff = square_diff(0x47, 0x40);
	munit_assert_int(-0x7 + 0x77, ==, diff);

	munit_log(MUNIT_LOG_INFO, "func flip_color");
	Color flipped = flip_color(BLACK);
	munit_assert_int(WHITE, ==, flipped);

	flipped = flip_color(WHITE);
	munit_assert_int(BLACK, ==, flipped);

	flipped = flip_color(WKNIGHT);
	munit_assert_int(BKNIGHT, ==, flipped);

	flipped = flip_color(BQUEEN);
	munit_assert_int(WQUEEN, ==, flipped);

	munit_log(MUNIT_LOG_INFO, "func contains_piece");
	Piece pieces = PAWN | KNIGHT | BISHOP | ROOK | QUEEN | KING;
	munit_assert_true(contains_piece_type(pieces, PAWN));
	munit_assert_true(contains_piece_type(pieces, KNIGHT));
	munit_assert_true(contains_piece_type(pieces, BISHOP));
	munit_assert_true(contains_piece_type(pieces, ROOK));
	munit_assert_true(contains_piece_type(pieces, QUEEN));
	munit_assert_true(contains_piece_type(pieces, KING));

	return MUNIT_OK;
}

MunitResult test_movelist_all(const MunitParameter params[], void *data) {
	MoveList mlist;
	mlist.size = 0;

	munit_log(MUNIT_LOG_INFO, "testcase: put & get");
	for(size_t i = 0; i < 512; i++) {
		MoveList__put(&mlist, (BitMove)i);
	}
	for(size_t i = 0; i < 512; i++) {
		BitMove move = MoveList__get(&mlist, i);
		munit_assert_int((BitMove)i, ==, move);
	}

	munit_log(MUNIT_LOG_INFO, "testcase: clear");
	MoveList__clear(&mlist);
	munit_assert_int(mlist.size, ==, 0);
	// Does not change contents of list.
	for(size_t i = 0; i < 512; i++) {
		BitMove move = MoveList__get(&mlist, i);
		munit_assert_int((BitMove)i, ==, move);
	}

	return MUNIT_OK;
}

MunitTest test_diverse_suite[] = {
	{ "piecelist__add", test_piecelist__add, 0, 0, MUNIT_TEST_OPTION_NONE, 0 },
	{ "piecelist__del", test_piecelist__del, 0, 0, MUNIT_TEST_OPTION_NONE, 0 },
	{ "piecelist__del_index", test_piecelist__del_index, 0, 0, MUNIT_TEST_OPTION_NONE, 0 },
	{ "bitmove__all", test_bitmove_all, 0, 0, MUNIT_TEST_OPTION_NONE, 0 },
	{ "utility_functions", test_utility_functions, 0, 0, MUNIT_TEST_OPTION_NONE, 0 },
	{ "movelist_all", test_movelist_all, 0, 0, MUNIT_TEST_OPTION_NONE, 0 },

	{ 0, 0, 0, 0, MUNIT_TEST_OPTION_NONE, 0 },
};
