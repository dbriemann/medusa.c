#ifndef FEN_TEST_H
#define FEN_TEST_H

#define MUNIT_ENABLE_ASSERT_ALIASES
#include "../munit/munit.h"

#include "../src/fen.h"

typedef struct FenParsePiecesTestCase FenParsePiecesTestCase;
struct FenParsePiecesTestCase {
	const char *name;
	char *		input_fen;
	const bool	expected_success;
	const Piece expected_pieces[64];
};

typedef struct FenParseCastlingRightsTestCase FenParseCastlingRightsTestCase;
struct FenParseCastlingRightsTestCase {
	const char *name;
	char *		input_fen;
	bool		expected_oo[2];
	bool		expected_ooo[2];
	bool		expected_success;
};

MunitResult test_fen_parse_pieces(const MunitParameter[], void *);
MunitResult test_fen_parse_color_to_move(const MunitParameter[], void *);
MunitResult test_fen_parse_castling_rights(const MunitParameter[], void *);
MunitResult test_fen_square_to_index(const MunitParameter[], void *);
MunitResult test_fen_parse_ep_square(const MunitParameter[], void *);

extern MunitTest test_fen_suite[];

#endif
