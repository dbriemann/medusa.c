#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "base.h"
#include "board.h"
#include "errors.h"
#include "mlist.h"

int main(void) {
	Board board;

	printf("board size including padding/packing: %lu\n", sizeof(Board));

	const char *fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

	Error error = Board__set_fen(&board, fen);
	if (error != OK) {
		printf("OUCH: %d\n", error);
		return 1;
	}

	Board__make_legal_move(&board, BitMove__new(WPAWN, 0x14, 0x34, PROMO_NONE, EMPTY, CASTLE_NONE, false));


	return EXIT_SUCCESS;
}
