#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "base.h"
#include "fen.h"
#include "../test/fen_test.h"

int main() {
	ParseFenTestCase testcases[] = {
		{
			.name = "Start position",
			.input_fen		 = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
			.expected_success = true,
			.expected_mb = {
				.color = WHITE,
				.castle_short = {true, true},
				.castle_long = {true, true},
				.ep_square = OTB,
			},
		},
	};

	MinBoard out_board;

	const size_t len = sizeof(testcases) / sizeof(ParseFenTestCase);

	for(size_t tc = 0; tc < len; tc++) {
		char *cpy = strdup(testcases[tc].input_fen);
		bool ok = parse_fen(cpy, &out_board);
		free(cpy);
	}

	return EXIT_SUCCESS;
}
