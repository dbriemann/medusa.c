#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "../src/board.h"
#include "../src/errors.h"
#include "../src/perft.h"

int main(void) {
	Board board;

	const char *fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

	// const char *fen = "rnbqkbnr/pppppppp/8/8/5P2/8/PPPPP1PP/RNBQKBNR b KQkq - 0 1";
	// const char *fen = "rnbqkbnr/pppp1ppp/4p3/8/5P2/8/PPPPP1PP/RNBQKBNR w KQkq - 0 2";
	// const char *fen = "rnbqkbnr/pppppppp/8/8/8/7N/PPPPPPPP/RNBQKB1R b KQkq - 1 1";
	// const char *fen = "rnbqkbnr/pppp1ppp/8/4p3/8/7N/PPPPPPPP/RNBQKB1R w KQkq - 0 2";
	// const char *fen = "rnbqkbnr/pppp1ppp/8/4p3/8/5P1N/PPPPP1PP/RNBQKB1R b KQkq - 0 2";
	// const char *fen = "rnb1kbnr/pppp1ppp/8/4p3/7q/5P1N/PPPPP1PP/RNBQKB1R w KQkq - 1 3";

	// const char *fen = "rnbqkbnr/pppppppp/8/8/1P6/8/P1PPPPPP/RNBQKBNR b KQkq - 0 1";
	// const char *fen = "rnbqkbnr/pp1ppppp/2p5/8/1P6/8/P1PPPPPP/RNBQKBNR w KQkq - 0 2";
	// const char *fen = "rnbqkbnr/pp1ppppp/2p5/8/1P6/3P4/P1P1PPPP/RNBQKBNR b KQkq - 0 2";
	// const char *fen = "rnb1kbnr/pp1ppppp/2p5/q7/1P6/3P4/P1P1PPPP/RNBQKBNR w KQkq - 1 3";

	Error error = Board__set_fen(&board, fen);
	if (error != OK) {
		printf("OUCH: %d\n", error);
		return 1;
	}

	PerftData pdata = {
		.nodes = 0,
		.castles = 0,
		.eps = 0,
		.mates = 0,
		.checks = 0,
		.promos = 0,
		.double_checks = 0,
		.captures = 0,
	};
	perft_analyze(board, 4, &pdata);
	printf("nodes: %lu\n", pdata.nodes);
	printf("captures: %lu\n", pdata.captures);
	printf("eps: %lu\n", pdata.eps);
	printf("castles: %lu\n", pdata.castles);
	printf("checks: %lu\n", pdata.checks);
	printf("mates: %lu\n", pdata.mates);

	// TODO:debug
	// INVALID ROOK MOVE (CAPTURES THROUGH PAWNS)

	// char *str = NULL;
	// Board__to_string(&board, &str);
	// printf("BOARD:\n%s\n", str);
	// free(str);

	return EXIT_SUCCESS;
}
