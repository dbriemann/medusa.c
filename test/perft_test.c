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
		{
         .name  = "kiwipete position: depth 1",
         .fen   = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1",
         .depth = 1,
         .nodes = 48,
		 },
		{
         .name  = "kiwipete position: depth 2",
         .fen   = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1",
         .depth = 2,
         .nodes = 2039,
		 },
		{
         .name  = "kiwipete position: depth 3",
         .fen   = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1",
         .depth = 3,
         .nodes = 97862,
		 },
		{
         .name  = "kiwipete position: depth 4",
         .fen   = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1",
         .depth = 4,
         .nodes = 4085603,
		 },
		{
         .name  = "rook endgame position: depth 1",
         .fen   = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1",
         .depth = 1,
         .nodes = 14,
		 },
		{
         .name  = "rook endgame position: depth 2",
         .fen   = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1",
         .depth = 2,
         .nodes = 191,
		 },
		{
         .name  = "rook endgame position: depth 3",
         .fen   = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1",
         .depth = 3,
         .nodes = 2812,
		 },
		{
         .name  = "rook endgame position: depth 4",
         .fen   = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1",
         .depth = 4,
         .nodes = 43238,
		 },
		{
         .name  = "rook endgame position: depth 5",
         .fen   = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1",
         .depth = 5,
         .nodes = 674624,
		 },
		{
         .name  = "rook endgame position: depth 6",
         .fen   = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1",
         .depth = 6,
         .nodes = 11030083,
		 },
		{
         .name  = "talkchess position: depth 1",
         .fen   = "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8",
         .depth = 1,
         .nodes = 44,
		 },
		{
         .name  = "talkchess position: depth 2",
         .fen   = "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8",
         .depth = 2,
         .nodes = 1486,
		 },
		{
         .name  = "talkchess position: depth 3",
         .fen   = "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8",
         .depth = 3,
         .nodes = 62379,
		 },
		{
         .name  = "talkchess position: depth 4",
         .fen   = "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8",
         .depth = 4,
         .nodes = 2103487,
		 },
		{
         .name  = "talkchess position: depth 5",
         .fen   = "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8",
         .depth = 5,
         .nodes = 89941194,
		 },
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
