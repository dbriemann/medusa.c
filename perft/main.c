#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

	printf("perft(\"%s\")\n", fen);

	Board board;

	Error error = Board__set_fen(&board, fen);
	if (error != OK) {
		fprintf(stderr, "error - invalid FEN provided, err code: %d\n", error);
		return EXIT_FAILURE;
	}

	for (size_t d = 1; d <= (size_t)depth; d++) {
		clock_t  start_time = clock();
		uint64_t nodes      = perft(board, d);
		clock_t  end_time   = clock();

		double used_cycles = (double)(end_time - start_time);
		double used_time   = (double)(used_cycles) / CLOCKS_PER_SEC;

		printf("depth %lu => nodes: %lu, time: %f sec, n/sec: %f\n", d, nodes, used_time, (double)nodes / used_time);
	}

	return EXIT_SUCCESS;
}
