#ifndef BOARD_TEST_H
#define BOARD_TEST_H

#include "../src/board.h"
#include "../src/mlist.h"
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

typedef struct IsSqAttackedTestCase IsSqAttackedTestCase;
struct IsSqAttackedTestCase {
	const char *name;
	const char *fen;
	const bool attacked_by_black[64];
	const bool attacked_by_white[64];
};

typedef struct DetectChecksAndPinsTestCase DetectChecksAndPinsTestCase;
struct DetectChecksAndPinsTestCase {
	const char *name;
	const char *fen;
	const Board expected_board;
};

typedef struct GenerateMovesTestCase GenerateMovesTestCase;
struct GenerateMovesTestCase {
	const char *name;
	const char *fen;
	MoveList expected_moves;
};

MunitResult test_board__set_fen(const MunitParameter params[], void *data);
MunitResult test_board__add_del_piece(const MunitParameter params[], void *data);
MunitResult test_board__clear_funcs(const MunitParameter params[], void *data);
MunitResult test_board__is_sq_attacked(const MunitParameter params[], void *data);
MunitResult test_board__detect_checks_and_pins(const MunitParameter params[], void *data);
MunitResult test_board__generate_knight_moves(const MunitParameter params[], void *data);
MunitResult test_board__generate_king_moves(const MunitParameter params[], void *data);
MunitResult test_board__generate_sliding_moves(const MunitParameter params[], void *data);

extern MunitTest test_board_suite[];

#endif
