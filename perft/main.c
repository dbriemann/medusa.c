#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "../src/board.h"
#include "../src/errors.h"
#include "../src/perft.h"

int main(int argc, char * argv[]) {
	if (argc < 3) {
		printf("usage: perft depth \"fen\"\n");
	}

	// arg 1 must be uint
	int depth;
	if (sscanf(argv[1], "%d", &depth) != 1) {
		fprintf(stderr, "error - depth not an unsigned integer: %s\n", argv[1]);
		return EXIT_FAILURE;
	}
	if (depth <= 0) {
		fprintf(stderr, "error - depth must be >= 1\n");
		return EXIT_FAILURE;
	}

	// arg 2 must be valid FEN.
	char * fen = argv[2];

	Board board;

	Error error = Board__set_fen(&board, fen);
	if (error != OK) {
		fprintf(stderr, "error - invalid FEN provided, err code: %d\n", error);
		return EXIT_FAILURE;
	}

	uint64_t nodes = perft(board, depth);
	printf("nodes: %lu\n", nodes);

	return EXIT_SUCCESS;
}
