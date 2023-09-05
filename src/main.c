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

	const char *fen = "1k6/4R3/8/4n3/8/1r3K1P/QB6/8 w - - 0 1";

	Error error = Board__set_fen(&board, fen);
	if (error != OK) {
		printf("OUCH: %d\n", error);
		return 1;
	}

	MoveList__clear(&moves);

	Board__detect_checks_and_pins(&board, board.player);
	Board__generate_all_legal_moves(&board, &moves, board.player);

	return EXIT_SUCCESS;
}
