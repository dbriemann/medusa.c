#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "base.h"
#include "board.h"
#include "errors.h"
#include "mlist.h"

int main(void) {
	Board    board;
	MoveList moves;

	const char *fen = "rnbqkbnr/ppp1p1pp/8/3pPp2/8/8/PPPP1PPP/RNBQKBNR w KQkq f6 0 3";

	Error error = Board__set_fen(&board, fen);
	if (error != OK) {
		printf("OUCH: %d\n", error);
		return 1;
	}

	MoveList__clear(&moves);

	Board__detect_checks_and_pins(&board, board.player);
	Board__generate_pawn_moves(&board, &moves, board.player);

	return EXIT_SUCCESS;
}
