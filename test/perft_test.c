#include "perft_test.h"
#include "../src/base.h"
#include "../src/mlist.h"
#include "../src/board.h"

#pragma clang diagnostic ignored "-Wunused-parameter"

MunitResult test_perft_validate(const MunitParameter params[], void *data) {
	const PerftTestCase testcases[] = {
		{
			.name         = "starting position: depth 1",
			.fen          = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
			.depth        = 1,
			.target_pdata = {
				.nodes = 20,
			},
		},
		{
			.name         = "starting position: depth 2",
			.fen          = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
			.depth        = 2,
			.target_pdata = {
				.nodes = 400,
			},
		},
		{
			.name         = "starting position: depth 3",
			.fen          = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
			.depth        = 3,
			.target_pdata = {
				.nodes    = 8902,
				.checks   = 12,
				.captures = 34,
			},
		},
		{
			.name         = "starting position: depth 4",
			.fen          = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
			.depth        = 4,
			.target_pdata = {
				.nodes    = 197281,
				.checks   = 469,
				.captures = 1576,
				.mates    = 8,
			},
		},
	};

	const size_t len = sizeof(testcases) / sizeof(PerftTestCase);

	Board board;

	for(size_t tc = 0; tc < len; tc++) {
		PerftData pdata = {
			.nodes         = 0,
			.castles       = 0,
			.eps           = 0,
			.mates         = 0,
			.checks        = 0,
			.promos        = 0,
			.double_checks = 0,
			.captures      = 0,
		};

		munit_logf(MUNIT_LOG_INFO, "testcase %zu: %s", tc, testcases[tc].name);

		Error error = Board__set_fen(&board, testcases[tc].fen);
		munit_assert_int(OK, ==, error);

		perft__validate(board, testcases[tc].depth, &pdata, false);

		munit_assert_uint64(testcases[tc].target_pdata.nodes, ==, pdata.nodes);
		if (testcases[tc].target_pdata.checks >= 0) {
			munit_assert_uint64(testcases[tc].target_pdata.checks, ==, pdata.checks);
		}
		if (testcases[tc].target_pdata.captures >= 0) {
			munit_assert_uint64(testcases[tc].target_pdata.captures, ==, pdata.captures);
		}
		if (testcases[tc].target_pdata.eps >= 0) {
			munit_assert_uint64(testcases[tc].target_pdata.eps, ==, pdata.eps);
		}
	}

	return MUNIT_OK;
}

MunitTest test_perft_suite[] = {
	{ "perft_validate", test_perft_validate, 0, 0, MUNIT_TEST_OPTION_NONE, 0 },

	{ 0, 0, 0, 0, MUNIT_TEST_OPTION_NONE, 0 },
};
