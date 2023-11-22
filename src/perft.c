#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "base.h"
#include "bitmove.h"
#include "board.h"
#include "errors.h"
#include "perft.h"

uint64_t perft(Board b, unsigned int depth) {
	MoveList mlist;
	MoveList__clear(&mlist);

	Board__detect_checks_and_pins(&b, b.player);
	Board__generate_all_legal_moves(&b, &mlist, b.player);

	if (depth == 1) {
		return mlist.size;
	}

	Board depth_board = b;

	uint64_t nodes = 0;

	for (uint64_t i = 0; i < mlist.size; i++) {
		Board__make_legal_move(&b, mlist.moves[i]);
		nodes += perft(b, depth - 1);
		b = depth_board;
	}

	return nodes;
}
