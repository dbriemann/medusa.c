#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "board.h"
#include "fen.h"
#include "plist.h"
#include "errors.h"

void Board__clear(Board *b) {
	// TODO: manually unroll loops for performance?
	// NOTE: this is probably only needed for testing.. no need to optimize.

	for(size_t i = 0; i < 2 * 64; i++) {
		b->squares[i] = EMPTY;
	}
	b->castle_short[BLACK] = false;
	b->castle_short[WHITE] = false;
	b->castle_long[BLACK]  = false;
	b->castle_long[WHITE]  = false;
	b->move_number		   = 1;
	b->draw_counter		   = 0;
	b->ep_square		   = OTB;
	b->player			   = WHITE;

	for(Color c = BLACK; c <= WHITE; c++) {
		b->kings[c]		   = OTB;
		b->sliders_size[c] = 0;
		for(size_t i = 0; i < 13; i++) {
			b->sliders[c][i] = OTB;
		}
		b->queens_size[c] = 0;
		for(size_t i = 0; i < 9; i++) {
			b->queens[c][i] = OTB;
		}
		b->rooks_size[c] = 0;
		for(size_t i = 0; i < 10; i++) {
			b->rooks[c][i] = OTB;
		}
		b->bishops_size[c] = 0;
		for(size_t i = 0; i < 10; i++) {
			b->bishops[c][i] = OTB;
		}
		b->knights_size[c] = 0;
		for(size_t i = 0; i < 10; i++) {
			b->knights[c][i] = OTB;
		}
		b->pawns_size[c] = 0;
		for(size_t i = 0; i < 8; i++) {
			b->pawns[c][i] = OTB;
		}
	}
}

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
		return err;
	}

	for(size_t i = 0; i < 64 * 2; i++) {
		b->squares[i] = EMPTY;
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
		case EMPTY: break;
		case BKING: b->kings[BLACK] = sq; break;
		case WKING: b->kings[WHITE] = sq; break;
		case BQUEEN: PieceList__add(b->queens[BLACK], &(b->queens_size[BLACK]), sq); break;
		case WQUEEN: PieceList__add(b->queens[WHITE], &(b->queens_size[WHITE]), sq); break;
		case BROOK: PieceList__add(b->rooks[BLACK], &(b->rooks_size[BLACK]), sq); break;
		case WROOK: PieceList__add(b->rooks[WHITE], &(b->rooks_size[WHITE]), sq); break;
		case BBISHOP: PieceList__add(b->bishops[BLACK], &(b->bishops_size[BLACK]), sq); break;
		case WBISHOP: PieceList__add(b->bishops[WHITE], &(b->bishops_size[WHITE]), sq); break;
		case BKNIGHT: PieceList__add(b->knights[BLACK], &(b->knights_size[BLACK]), sq); break;
		case WKNIGHT: PieceList__add(b->knights[WHITE], &(b->knights_size[WHITE]), sq); break;
		case BPAWN: PieceList__add(b->pawns[BLACK], &(b->pawns_size[BLACK]), sq); break;
		case WPAWN: PieceList__add(b->pawns[WHITE], &(b->pawns_size[WHITE]), sq); break;
		default: return ERR_INVALID_INPUT;
		}
	}

	// TODO: Detect checks (and pins?) here for init.
	b->check_info = CHECK_NONE;

	return OK;
}

// TODO: NULL check?
void Board__add_piece(Board *b, Square sq, Piece p) {
	if(!on_board(sq)) {
		return;
	}
	b->squares[sq] = p;

	const Piece ptype = p & PIECE_MASK;
	const Piece pcol  = p & COLOR_ONLY_MASK;

	switch(ptype) {
	case PAWN: PieceList__add(b->pawns[pcol], &(b->pawns_size[pcol]), sq); break;
	case KNIGHT: PieceList__add(b->knights[pcol], &(b->knights_size[pcol]), sq); break;
	case BISHOP: PieceList__add(b->bishops[pcol], &(b->bishops_size[pcol]), sq); break;
	case ROOK: PieceList__add(b->rooks[pcol], &(b->rooks_size[pcol]), sq); break;
	case QUEEN: PieceList__add(b->queens[pcol], &(b->queens_size[pcol]), sq); break;
	case KING: b->kings[pcol] = sq; break;
	default:
		// This should never happen. Ignore all other values.
		break;
	}
}

// TODO: NULL check?
void Board__del_piece(Board *b, Square sq) {
	if(!on_board(sq)) {
		return;
	}
	const Piece p	  = b->squares[sq];
	const Piece ptype = p & PIECE_MASK;
	const Piece pcol  = p & COLOR_ONLY_MASK;

	b->squares[sq] = EMPTY;
	switch(ptype) {
		// TODO: plists
	}
}
