#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "base.h"
#include "board.h"
#include "errors.h"

int main() {
	Board board;

	const char *name = "DEBUG POS";
	const char *fen = "k7/7p/5ppP/3ppPP1/1ppPP3/pPP5/P7/7K w - - 0 1";
	const bool attacked_by_white[64] = {
		true, true, true, true, true, true, true, true,
		true, true, true, true, true, true, true, true,
		true, true, true, true, false, true, true, true,
		true, true, false, true, true, true, true, false,
		false, true, true, true, true, true, true, true,
		false, false, true, false, true, true, false, true,
		false, false, false, false, false, false, false, false,
		false, false, false, false, false, false, false, false,
	};

	Error error = Board__set_fen(&board, fen);

	for(size_t i = 0; i < 64; i++) {
		const Square sq = LOOKUP_0x88[i];
		bool attack_check = Board__is_sq_attacked(&board, sq, OTB, BLACK);
		if(attacked_by_white[i] != attack_check) {
			printf("Wrong result for i: %zu\n", i);
		}
	}

	return EXIT_SUCCESS;
}
