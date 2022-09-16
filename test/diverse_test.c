#include "diverse_test.h"
#include "../src/plist.h"

MunitResult test_piecelist__add(const MunitParameter params[], void *data) {
	Square sq = 0x34; //e4
	Piece bqueens[9] = {0};
	size_t len = 0;

	PieceList__add(sq, bqueens, &len);

	munit_assert_int(sq, ==, bqueens[0]);
	munit_assert_int(len, ==, 1);

	return MUNIT_OK;
}

MunitTest test_diverse_suite[] = {
	{"piecelist__add", test_piecelist__add, 0, 0, MUNIT_TEST_OPTION_NONE, 0},

	{0, 0, 0, 0, MUNIT_TEST_OPTION_NONE, 0},
};
