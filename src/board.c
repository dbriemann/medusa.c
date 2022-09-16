#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "board.h"
#include "fen.h"
#include "plist.h"
#include "errors.h"

void Board__set_starting_position(Board *b) {
	assert(b != NULL);

	Error err = Board__set_fen(b, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	assert(err == OK);
}

Error Board__set_fen(Board *b, const char *fen) {
	if(b == NULL || fen == NULL) {
		return ERR_NULL_PTR;
	}

	MinBoard mb;
	Error	 err = parse_fen(fen, &mb);
	if(err != OK) {
		return false;
	}

	// Clear piece lists by zeroing their sizes.
	for(Color c = BLACK; c <= WHITE; c++) {
		b->sliders_size[c] = 0;
		b->queens_size[c]  = 0;
		b->rooks_size[c]   = 0;
		b->bishops_size[c] = 0;
		b->knights_size[c] = 0;
		b->pawns_size[c]   = 0;
	}

	// Copy over data from minboard.
	b->move_number	= mb.move_num;
	b->draw_counter = mb.half_moves;

	if(mb.ep_square != OTB) {
		b->ep_square = LOOKUP_0x88[mb.ep_square];
	} else {
		b->ep_square = OTB;
	}

	b->castle_short[BLACK] = mb.castle_short[BLACK];
	b->castle_short[WHITE] = mb.castle_short[WHITE];
	b->castle_long[BLACK]  = mb.castle_long[BLACK];
	b->castle_long[WHITE]  = mb.castle_long[WHITE];

	b->player = mb.color;

	for(size_t i = 0; i < 64; i++) {
		Square sq	 = LOOKUP_0x88[i];
		Piece  piece = mb.squares[i];

		b->squares[sq] = piece;

		switch(piece) {
		case BKING: b->kings[BLACK] = sq; break;
		case WKING: b->kings[WHITE] = sq; break;
		case BQUEEN:
			PieceList__add(sq, b->queens[BLACK], &(b->queens_size[BLACK]));
			break;
		case WQUEEN:
			PieceList__add(sq, b->queens[WHITE], &(b->queens_size[WHITE]));
			break;
		case BROOK:
			PieceList__add(sq, b->rooks[BLACK], &(b->rooks_size[BLACK]));
			break;
		case WROOK:
			PieceList__add(sq, b->rooks[WHITE], &(b->rooks_size[WHITE]));
			break;
		case BBISHOP:
			PieceList__add(sq, b->bishops[BLACK], &(b->bishops_size[BLACK]));
			break;
		case WBISHOP:
			PieceList__add(sq, b->bishops[WHITE], &(b->bishops_size[WHITE]));
			break;
		case BKNIGHT:
			PieceList__add(sq, b->knights[BLACK], &(b->knights_size[BLACK]));
			break;
		case WKNIGHT:
			PieceList__add(sq, b->knights[WHITE], &(b->knights_size[WHITE]));
			break;
		case BPAWN:
			PieceList__add(sq, b->pawns[BLACK], &(b->pawns_size[BLACK]));
			break;
		case WPAWN:
			PieceList__add(sq, b->pawns[WHITE], &(b->pawns_size[WHITE]));
			break;
		default:
			return ERR_INVALID_INPUT;
		}
	}

	// TODO: Detect checks (and pins?) here for init.

	return OK;
}
