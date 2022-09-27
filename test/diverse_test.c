#include "diverse_test.h"
#include "../src/plist.h"
#include "../src/bitmove.h"
#include "../src/base.h"

MunitResult test_piecelist__add(const MunitParameter params[], void *data) {
	Square sq = 0x34; //e4
	Piece bqueens[9] = {0};
	size_t len = 0;

	PieceList__add(bqueens, &len, sq);

	munit_assert_int(sq, ==, bqueens[0]);
	munit_assert_int(len, ==, 1);

	return MUNIT_OK;
}

MunitResult test_piecelist__del(const MunitParameter params[], void *data) {
	Piece pieces[8] = {0x11, 0x56, 0x22, 0, 0, 0, 0, 0};
	size_t len = 3;

	// Not in list, no changes.
	Piece want_pieces1[8] = {0x11, 0x56, 0x22, 0, 0, 0, 0, 0};
	size_t want_len1 = 3;
	PieceList__del(pieces, &len, 0x88);
	munit_assert_int(want_len1, ==, len);
	for(size_t i = 0; i < 8; i++) {
		// We check all elements in test, not only up to len.
		munit_assert_int(want_pieces1[i], ==, pieces[i]);
	}

	// First element is replaced by last. which then remains in the list twice
	// and len is reduced by 1, rendering the duplicate "dead".
	Piece want_pieces2[8] = {0x22, 0x56, 0x22, 0, 0, 0, 0, 0};
	size_t want_len2 = 2;
	PieceList__del(pieces, &len, 0x11);
	munit_assert_int(want_len2, ==, len);
	for(size_t i = 0; i < 8; i++) {
		// We check all elements in test, not only up to len.
		munit_assert_int(want_pieces2[i], ==, pieces[i]);
	}

	return MUNIT_OK;
}

MunitResult test_piecelist__del_index(const MunitParameter params[], void *data) {
	Piece pieces[8] = {0x11, 0x56, 0x22, 0, 0, 0, 0, 0};
	size_t len = 3;

	// Delete element at index 0 until len == 0:
	// 1. {0x22, 0x56, 0x22, 0, 0, 0, 0, 0} --> len 2
	// 2. {0x56, 0x56, 0x22, 0, 0, 0, 0, 0} --> len 1
	// 3. {0x56, 0x56, 0x22, 0, 0, 0, 0, 0} --> len 0
	Piece want_pieces1[8] = {0x56, 0x56, 0x22, 0, 0, 0, 0, 0};
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
	// using the extraction function.
	const Square e2 = 0x14;
	const Square e4 = 0x34;
	const BitMove e2e4 = BitMove__new(e2, e4, PROMO_NONE);
	munit_assert_int(e2, ==, BitMove__from(e2e4));
	munit_assert_int(e4, ==, BitMove__to(e2e4));
	munit_assert_int(PROMO_NONE, ==, BitMove__promoted_piece(e2e4));

	const Square g7 = 0x66;
	const Square g8 = 0x76;
	const BitMove g7g8Q = BitMove__new(g7, g8, WQUEEN);
	munit_assert_int(g7, ==, BitMove__from(g7g8Q));
	munit_assert_int(g8, ==, BitMove__to(g7g8Q));
	munit_assert_int(WQUEEN, ==, BitMove__promoted_piece(g7g8Q));

	return MUNIT_OK;
}

MunitTest test_diverse_suite[] = {
	{"piecelist__add", test_piecelist__add, 0, 0, MUNIT_TEST_OPTION_NONE, 0},
	{"piecelist__del", test_piecelist__del, 0, 0, MUNIT_TEST_OPTION_NONE, 0},
	{"piecelist__del_index", test_piecelist__del_index, 0, 0, MUNIT_TEST_OPTION_NONE, 0},
	{"bitmove__all", test_bitmove_all, 0, 0, MUNIT_TEST_OPTION_NONE, 0},

	{0, 0, 0, 0, MUNIT_TEST_OPTION_NONE, 0},
};
