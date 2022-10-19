#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "base.h"
#include "board.h"
#include "errors.h"

int main() {
	Board board;

	const char *name = "real position: sevian vs. caruana (usc 2022)";
	const char *fen = "r1bqr1k1/3n1pp1/3b1n1p/1ppP4/8/P1NNB3/1P2B1PP/R2Q1RK1 w - - 2 17";
	const bool attacked_by_black[64] = {
		false, false, false, false, false, false, false, false,
		false, false, false, false, false, false, false, true,
		true, false, false, false, true, false, true, false,
		true, true, true, true, true, true, true, false,
		true, false, true, true, true, false, true, true,
		true, true, false, false, true, true, true, true,
		true, true, true, true, true, true, true, true,
		false, true, true, true, true, true, true, true,
	};

	Error error = Board__set_fen(&board, fen);

	for(size_t i = 0; i < 64; i++) {
		const Square sq = LOOKUP_0x88[i];
		bool attack_check = Board__is_sq_attacked(&board, sq, OTB, WHITE);
		if(attacked_by_black[i] != attack_check) {
			printf("Wrong result for i: %zu\n", i);
		}
	}

	return EXIT_SUCCESS;
}
