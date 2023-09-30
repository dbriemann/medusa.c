#include <stdio.h>
#include <stdlib.h>

#include "board.h"
#include "errors.h"
#include "perft.h"
#include "base.h"
#include "bitmove.h"

void perft_new(Board b, unsigned int depth, PerftData *pdata, bool debug) {

}

void perft__validate(Board b, unsigned int depth, PerftData *pdata, bool debug) {
	// TODO: collect perft data for all first moves.
	MoveList mlist;
	MoveList__clear(&mlist);

	Board__detect_checks_and_pins(&b, b.player);
	Board__generate_all_legal_moves(&b, &mlist, b.player);

	if (b.check_info != CHECK_NONE) {
		if (mlist.size == 0) {
			pdata->mates++;
		} else {
			pdata->checks++;
			// if (b.squares[0x37] != BQUEEN
			// 	&& b.squares[0x40] != BQUEEN
			// 	&& b.squares[0x31] != BBISHOP) {
			printf("----------------------------------\n");
			char *print_board = NULL;
			Board__to_string(&b, &print_board);
			printf("board:\n%s\n", print_board);
			free(print_board);
			printf("----------------------------------\n");
			// }
		}
	} else {
		// TODO: STALEMATE
	}

	if (debug) {
		printf("----------------------------------\n");
		char *print_board = NULL;
		Board__to_string(&b, &print_board);
		printf("board:\n%s\n", print_board);
		free(print_board);
	}

	if (depth == 0) {
		if (debug) {
			printf("----------------------------------\n");
			printf("----------------------------------\n");
		}
		return;
	}

	Board depth_board = b;

	for (uint64_t i = 0; i < mlist.size; i++) {
		if (debug) {
			char *notation = BitMove__to_notation(mlist.moves[i]);
			printf("move: %s\n\n", notation);
			free(notation);
		}

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
			}
		}
		perft__validate(b, depth - 1, pdata, debug);

		b = depth_board;
	}
}
