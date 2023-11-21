#include <bits/stdint-uintn.h>
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

	uint64_t nodes = perft_bench(board, 5);
	printf("nodes: %lu\n", nodes);

	return EXIT_SUCCESS;
}
