
#include "perft.h"
#include "base.h"
#include "bitmove.h"
#include <stdio.h>

void perft__validate(Board b, unsigned int depth, PerftData *pdata) {
	if (depth == 0) {
		return;
	}

	MoveList mlist;
	MoveList__clear(&mlist);

	Board__detect_checks_and_pins(&b, b.player);
	Board__generate_all_legal_moves(&b, &mlist, b.player);

	Board depth_board = b;

	for (uint64_t i = 0; i < mlist.size; i++) {
		Board__make_legal_move(&b, mlist.moves[i]);

		if (depth == 1) {
			Board__detect_checks_and_pins(&b, b.player);
			pdata->nodes++;
			if (BitMove__castle_type(mlist.moves[i]) != CASTLE_NONE) {
				pdata->castles++;
			} else if (BitMove__en_passent(mlist.moves[i])) {
				pdata->eps++;
			} else if (BitMove__captured_piece(mlist.moves[i]) != EMPTY) {
				pdata->captures++;
			} else if (b.check_info != CHECK_NONE) {
				pdata->checks++;
			}
		} else {
			perft__validate(b, depth - 1, pdata);
		}
		b = depth_board;
	}
}
