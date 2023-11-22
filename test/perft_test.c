#include <stdint.h>

#include "../src/base.h"
#include "../src/board.h"
#include "../src/mlist.h"
#include "perft_test.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"

MunitResult test_perft_validate(const MunitParameter params[], void * data) {
	const PerftTestCase testcases[] = {
		{
         .name  = "starting position: depth 1",
         .fen   = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
         .depth = 1,
         .nodes = 20,
		 },
		{
         .name  = "starting position: depth 2",
         .fen   = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
         .depth = 2,
         .nodes = 400,
		 },
		{
         .name  = "starting position: depth 3",
         .fen   = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
         .depth = 3,
         .nodes = 8902,
		 },
		{
         .name  = "starting position: depth 4",
         .fen   = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
         .depth = 4,
         .nodes = 197281,
		 },
		{
         .name  = "starting position: depth 5",
         .fen   = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
         .depth = 5,
         .nodes = 4865609,
		 },
 // TODO: add more perfts
	};

	const size_t len = sizeof(testcases) / sizeof(PerftTestCase);

	Board board;

	for (size_t tc = 0; tc < len; tc++) {
		munit_logf(MUNIT_LOG_INFO, "testcase %zu: %s", tc, testcases[tc].name);

		Error error = Board__set_fen(&board, testcases[tc].fen);
		munit_assert_int(OK, ==, error);

		uint64_t nodes = perft(board, testcases[tc].depth);

		munit_assert_uint64(testcases[tc].nodes, ==, nodes);
	}

	return MUNIT_OK;
}

MunitTest test_perft_suite[] = {
	{"perft_validate", test_perft_validate, 0, 0, MUNIT_TEST_OPTION_NONE, 0},

	{			   0,				   0, 0, 0, MUNIT_TEST_OPTION_NONE, 0},
};
