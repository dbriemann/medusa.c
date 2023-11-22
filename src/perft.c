#include <bits/stdint-uintn.h>
#include <stdio.h>
#include <stdlib.h>

#include "base.h"
#include "bitmove.h"
#include "board.h"
#include "errors.h"
#include "perft.h"

// void perft_analyze(Board b, unsigned int max_depth, PerftData * pdata) {
// 	if (max_depth == 0) {
// 		return;
// 	}

// 	if (max_depth == 1) {
// 		perft_validate(b, max_depth, pdata, false);
// 		return;
// 	}

// 	MoveList mlist;
// 	MoveList__clear(&mlist);

// 	Board__detect_checks_and_pins(&b, b.player);
// 	Board__generate_all_legal_moves(&b, &mlist, b.player);

// 	Board root_board = b;

// 	for (uint64_t i = 0; i < mlist.size; i++) {
// 		PerftData root_data = {
// 			.nodes         = 0,
// 			.captures      = 0,
// 			.eps           = 0,
// 			.promos        = 0,
// 			.checks        = 0,
// 			.double_checks = 0,
// 			.mates         = 0,
// 			.castles       = 0,
// 		};

// 		Board__make_legal_move(&b, mlist.moves[i]);

// 		perft_validate(b, max_depth - 1, &root_data, false);

// 		b = root_board;

// 		char * move = BitMove__to_notation(mlist.moves[i]);
// 		printf("%s: nodes=%lu, checks=%lu\n", move, root_data.nodes, root_data.checks);
// 		free(move);

// 		pdata->nodes += root_data.nodes;
// 		pdata->captures += root_data.captures;
// 		pdata->eps += root_data.eps;
// 		pdata->promos += root_data.promos;
// 		pdata->checks += root_data.checks;
// 		pdata->double_checks += root_data.double_checks;
// 		pdata->mates += root_data.mates;
// 		pdata->castles += root_data.castles;
// 	}
// }

// void perft_validate(Board b, unsigned int depth, PerftData * pdata, bool debug) {
// 	MoveList mlist;
// 	MoveList__clear(&mlist);

// 	Board__detect_checks_and_pins(&b, b.player);
// 	Board__generate_all_legal_moves(&b, &mlist, b.player);

// 	if (depth == 0) {
// 		if (b.check_info != CHECK_NONE) {
// 			if (mlist.size == 0) {
// 				pdata->checks++;
// 				pdata->mates++;
// 			} else {
// 				pdata->checks++;
// 			}
// 		} else {
// 			// TODO: STALEMATE
// 		}
// 		return;
// 	}

// 	Board depth_board = b;

// 	for (uint64_t i = 0; i < mlist.size; i++) {
// 		Board__make_legal_move(&b, mlist.moves[i]);

// 		if (depth == 2) {
// 			Board__detect_checks_and_pins(&b, b.player);
// 			pdata->nodes++;

// 			char * move = BitMove__to_notation(mlist.moves[i]);
// 			printf("    %s\n", move);
// 			free(move);
// 			// Square from = BitMove__from(mlist.moves[i]);
// 			// Square to   = BitMove__to(mlist.moves[i]);

// 			// if ((from == CASTLING_FROM[b.player] && to == CASTLING_OO_TO[b.player]) ||
// 			//     (from == CASTLING_FROM[b.player] && to == CASTLING_OOO_TO[b.player])) {
// 			// 	pdata->castles++;
// 			// } else if (to == b.ep_square) {
// 			// 	pdata->eps++;
// 			// 	pdata->captures++;
// 			// } else if (b.squares[to] != EMPTY) {
// 			// 	// TODO:  square is checked after move is executed...
// 			// 	pdata->captures++;
// 			// }
// 		}
// 		if (depth == 1) {
// 			char * move = BitMove__to_notation(mlist.moves[i]);
// 			printf("        %s\n", move);
// 			free(move);
// 		}
// 		perft_validate(b, depth - 1, pdata, debug);

// 		b = depth_board;
// 	}
// }

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
