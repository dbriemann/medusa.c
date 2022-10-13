#ifndef BOARD_TEST_H
#define BOARD_TEST_H

#include "../src/board.h"
#include "../src/errors.h"

#define MUNIT_ENABLE_ASSERT_ALIASES
#include "../munit/munit.h"

typedef struct BoardSetFenTestCase BoardSetFenTestCase;
struct BoardSetFenTestCase {
	const char *name;
	const char *input_fen;
	const Error expected_result;
	const Board expected_board;
};

typedef struct BoardAddDelPieceTestCase BoardAddDelPieceTestCase;
struct BoardAddDelPieceTestCase {
	const Piece piece;
	const Square sq;
};

MunitResult test_board__set_fen(const MunitParameter params[], void *data);
MunitResult test_board__add_del_piece(const MunitParameter params[], void *data);
MunitResult test_board__clear_funcs(const MunitParameter params[], void *data);

extern MunitTest test_board_suite[];

#endif
