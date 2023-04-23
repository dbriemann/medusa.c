#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "base.h"
#include "board.h"
#include "errors.h"

int main() {
	Board    board;
	MoveList moves;

	const char* fen = "6k1/5ppp/4p3/8/8/8/NP6/QK6 w - - 0 1";

	Error error = Board__set_fen(&board, fen);
	if(error != OK) {
		printf("OUCH: %d\n", error);
		return 1;
	}

	printf("YAY\n");


	Board__detect_checks_and_pins(&board, board.player);
	Board__generate_sliding_moves(&board, &moves, board.player, QUEEN, ORTHOGONAL_DIRS, board.queens[board.player], board.queens_size[board.player]);


	// for(size_t i = 0; i < 64; i++) {
	// 	const Square sq = LOOKUP_0x88[i];
	// 	bool attack_check = Board__is_sq_attacked(&board, sq, OTB, BLACK);
	// 	if(attacked_by[i] != attack_check) {
	// 		printf("Wrong result for i: %zu\n", i);
	// 	}
	// }

	return EXIT_SUCCESS;
}
