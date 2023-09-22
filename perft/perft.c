#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "../src/board.h"
#include "../src/errors.h"

int main(void) {
	Board board;

	const char *fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

	Error error = Board__set_fen(&board, fen);
	if (error != OK) {
		printf("OUCH: %d\n", error);
		return 1;
	}

	uint64_t nodes = Board__perft(board, 3);
	printf("nodes reached: %lu\n", nodes);

	return EXIT_SUCCESS;
}
