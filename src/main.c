#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "base.h"
#include "board.h"
#include "errors.h"

int main() {
	Board board;

	const char* name = "DEBUG POS";
	const char* fen  = "b3r1k1/5pp1/p6p/3RB3/P3KN1q/8/6PP/8 w - - 0 1";

	Error error = Board__set_fen(&board, fen);

	Board__detect_checks_and_pins(&board, board.player);

	// for(size_t i = 0; i < 64; i++) {
	// 	const Square sq = LOOKUP_0x88[i];
	// 	bool attack_check = Board__is_sq_attacked(&board, sq, OTB, BLACK);
	// 	if(attacked_by[i] != attack_check) {
	// 		printf("Wrong result for i: %zu\n", i);
	// 	}
	// }

	return EXIT_SUCCESS;
}
