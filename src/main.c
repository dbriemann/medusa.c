#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "../src/board.h"
#include "../src/errors.h"
#include "../src/perft.h"

int main(void) {
	Board board;

	const char *fen = "rnbqkbnr/ppppp1pp/8/5p2/4P3/8/PPPP1PPP/RNBQKBNR w KQkq - 0 2";

	Error error = Board__set_fen(&board, fen);
	if (error != OK) {
		printf("OUCH: %d\n", error);
		return 1;
	}

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
	perft__validate(board, 1, &pdata);
	printf("nodes: %lu\n", pdata.nodes);
	printf("captures: %lu\n", pdata.captures);
	printf("eps: %lu\n", pdata.eps);
	printf("castles: %lu\n", pdata.castles);
	printf("checks: %lu\n", pdata.checks);

	return EXIT_SUCCESS;
}
