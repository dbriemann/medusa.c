#ifndef FEN_TEST_H
#define FEN_TEST_H

#define MUNIT_ENABLE_ASSERT_ALIASES
#include "../munit/munit.h"

#include "../src/fen.h"
#include "../src/errors.h"

typedef struct FenParsePiecesTestCase FenParsePiecesTestCase;
struct FenParsePiecesTestCase {
	const char* name;
	const char* input_fen;
	const Error expected_result;
	const Piece expected_pieces[64];
};

typedef struct FenParseCastlingRightsTestCase FenParseCastlingRightsTestCase;
struct FenParseCastlingRightsTestCase {
	const char* name;
	const char* input_fen;
	bool expected_oo[2];
	bool expected_ooo[2];
	Error expected_result;
};

typedef struct ParseFenTestCase ParseFenTestCase;
struct ParseFenTestCase {
	const char* name;
	const char* input_fen;
	MinBoard expected_mb;
	Error expected_result;
};

MunitResult test_fen_parse_pieces(const MunitParameter[], void*);
MunitResult test_fen_parse_color_to_move(const MunitParameter[], void*);
MunitResult test_fen_parse_castling_rights(const MunitParameter[], void*);
MunitResult test_fen_square_to_index(const MunitParameter[], void*);
MunitResult test_fen_parse_ep_square(const MunitParameter[], void*);
MunitResult test_fen_parse_move_number(const MunitParameter[], void*);
MunitResult test_fen_parse_half_move_clock(const MunitParameter[], void*);
MunitResult test_parse_fen(const MunitParameter[], void*);

extern MunitTest test_fen_suite[];

#endif
