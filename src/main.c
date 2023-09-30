#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "../src/board.h"
#include "../src/errors.h"
#include "../src/perft.h"

int main(void) {
	Board board;

	const char *fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	// const char *fen = "rnbqkbnr/ppp1pppp/3p4/1B6/4P3/8/PPPP1PPP/RNBQK1NR b KQkq - 1 2";
	// const char *fen = "rnbqkbnr/ppp1pppp/3p4/8/Q7/2P5/PP1PPPPP/RNB1KBNR b KQkq - 1 2";

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
	perft__validate(board, 4, &pdata, false);
	printf("nodes: %lu\n", pdata.nodes);
	printf("captures: %lu\n", pdata.captures);
	printf("eps: %lu\n", pdata.eps);
	printf("castles: %lu\n", pdata.castles);
	printf("checks: %lu\n", pdata.checks);
	printf("mates: %lu\n", pdata.mates);

	// TODO:debug

	// char *str = NULL;
	// Board__to_string(&board, &str);
	// printf("BOARD:\n%s\n", str);
	// free(str);

	return EXIT_SUCCESS;
}
