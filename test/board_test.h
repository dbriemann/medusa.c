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

MunitResult test_board__set_fen(const MunitParameter params[], void *data);

extern MunitTest test_board_suite[];

#endif
