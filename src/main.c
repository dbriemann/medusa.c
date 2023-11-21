#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "../src/board.h"
#include "../src/errors.h"
#include "../src/perft.h"

int main(void) {
	Board board;

	const char *fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

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
	perft_analyze(board, 6, &pdata);
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
