#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "../src/board.h"
#include "../src/errors.h"
#include "../src/perft.h"

int main(void) {
	Board board;

	const char *fen = "4k3/ppppppp1/7p/8/7P/8/PPPPPPP1/4K3 w - - 0 1";

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

	// char *str = NULL;
	// Board__to_string(&board, &str);
	// printf("BOARD:\n%s\n", str);
	// free(str);

	return EXIT_SUCCESS;
}
