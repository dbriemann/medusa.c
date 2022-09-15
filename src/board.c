#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "board.h"
#include "fen.h"

void Board__set_starting_position(Board *b) {
	assert(b != NULL);

	bool ok = Board__set_fen(b, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	assert(ok);
}

bool Board__set_fen(Board *b, const char *fen) {
	if(b == NULL || fen == NULL) {
		return false;
	}
	// parse_fen
	MinBoard mb;
	bool ok = parse_fen(fen, &mb);
	if(!ok) {
		return false;
	}

}
