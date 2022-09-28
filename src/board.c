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

void Board__clear_meta(Board *b) {
	// This function is called often and thus the "loop" is manually unrolled.
	// It is a lot faster than any looping construct.
	b->check_info = OTB;

	b->squares[0x8] = INFO_NONE;
	b->squares[0x9] = INFO_NONE;
	b->squares[0xa] = INFO_NONE;
	b->squares[0xb] = INFO_NONE;
	b->squares[0xc] = INFO_NONE;
	b->squares[0xd] = INFO_NONE;
	b->squares[0xe] = INFO_NONE;
	b->squares[0xf] = INFO_NONE;
	b->squares[0x18] = INFO_NONE;
	b->squares[0x19] = INFO_NONE;
	b->squares[0x1a] = INFO_NONE;
	b->squares[0x1b] = INFO_NONE;
	b->squares[0x1c] = INFO_NONE;
	b->squares[0x1d] = INFO_NONE;
	b->squares[0x1e] = INFO_NONE;
	b->squares[0x1f] = INFO_NONE;
	b->squares[0x28] = INFO_NONE;
	b->squares[0x29] = INFO_NONE;
	b->squares[0x2a] = INFO_NONE;
	b->squares[0x2b] = INFO_NONE;
	b->squares[0x2c] = INFO_NONE;
	b->squares[0x2d] = INFO_NONE;
	b->squares[0x2e] = INFO_NONE;
	b->squares[0x2f] = INFO_NONE;
	b->squares[0x38] = INFO_NONE;
	b->squares[0x39] = INFO_NONE;
	b->squares[0x3a] = INFO_NONE;
	b->squares[0x3b] = INFO_NONE;
	b->squares[0x3c] = INFO_NONE;
	b->squares[0x3d] = INFO_NONE;
	b->squares[0x3e] = INFO_NONE;
	b->squares[0x3f] = INFO_NONE;
	b->squares[0x48] = INFO_NONE;
	b->squares[0x49] = INFO_NONE;
	b->squares[0x4a] = INFO_NONE;
	b->squares[0x4b] = INFO_NONE;
	b->squares[0x4c] = INFO_NONE;
	b->squares[0x4d] = INFO_NONE;
	b->squares[0x4e] = INFO_NONE;
	b->squares[0x4f] = INFO_NONE;
	b->squares[0x58] = INFO_NONE;
	b->squares[0x59] = INFO_NONE;
	b->squares[0x5a] = INFO_NONE;
	b->squares[0x5b] = INFO_NONE;
	b->squares[0x5c] = INFO_NONE;
	b->squares[0x5d] = INFO_NONE;
	b->squares[0x5e] = INFO_NONE;
	b->squares[0x5f] = INFO_NONE;
	b->squares[0x68] = INFO_NONE;
	b->squares[0x69] = INFO_NONE;
	b->squares[0x6a] = INFO_NONE;
	b->squares[0x6b] = INFO_NONE;
	b->squares[0x6c] = INFO_NONE;
	b->squares[0x6d] = INFO_NONE;
	b->squares[0x6e] = INFO_NONE;
	b->squares[0x6f] = INFO_NONE;
	b->squares[0x78] = INFO_NONE;
	b->squares[0x79] = INFO_NONE;
	b->squares[0x7a] = INFO_NONE;
	b->squares[0x7b] = INFO_NONE;
	b->squares[0x7c] = INFO_NONE;
	b->squares[0x7d] = INFO_NONE;
	b->squares[0x7e] = INFO_NONE;
	b->squares[0x7f] = INFO_NONE;
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
		const Square sq	   = LOOKUP_0x88[i];
		const Piece	 piece = mb.squares[i];
		const Piece	 ptype = piece & PIECE_MASK;
		const Piece	 pcol  = piece & COLOR_ONLY_MASK;

		if(piece == EMPTY) {
			b->squares[sq] = EMPTY;
			continue;
		}

		switch(ptype) {
		case PAWN: PieceList__add(b->pawns[pcol], &(b->pawns_size[pcol]), sq); break;
		case KNIGHT: PieceList__add(b->knights[pcol], &(b->knights_size[pcol]), sq); break;
		case BISHOP:
			PieceList__add(b->bishops[pcol], &(b->bishops_size[pcol]), sq);
			PieceList__add(b->sliders[pcol], &(b->sliders_size[pcol]), sq);
			break;
		case ROOK:
			PieceList__add(b->rooks[pcol], &(b->rooks_size[pcol]), sq);
			PieceList__add(b->sliders[pcol], &(b->sliders_size[pcol]), sq);
			break;
		case QUEEN:
			PieceList__add(b->queens[pcol], &(b->queens_size[pcol]), sq);
			PieceList__add(b->sliders[pcol], &(b->sliders_size[pcol]), sq);
			break;
		case KING:
			b->squares[sq] = piece;
			b->kings[pcol] = sq;
			break;
		default: printf("ptype: %d\n", ptype); return ERR_INVALID_INPUT; // Skips squares assignment.
		}
		b->squares[sq] = piece;
	}

	// TODO: Detect checks (and pins?) here for init.
	b->check_info = CHECK_NONE;

	return OK;
}

// TODO: NULL check?
// TODO: what if square has a piece already?
// TODO: check if plists are full? (should never happen [maybe in chess variants])
void Board__add_piece(Board *b, Square sq, Piece p) {
	if(!on_board(sq)) {
		return;
	}

	const Piece ptype = p & PIECE_MASK;
	const Piece pcol  = p & COLOR_ONLY_MASK;

	switch(ptype) {
	case PAWN: PieceList__add(b->pawns[pcol], &(b->pawns_size[pcol]), sq); break;
	case KNIGHT: PieceList__add(b->knights[pcol], &(b->knights_size[pcol]), sq); break;
	case BISHOP:
		PieceList__add(b->bishops[pcol], &(b->bishops_size[pcol]), sq);
		PieceList__add(b->sliders[pcol], &(b->sliders_size[pcol]), sq);
		break;
	case ROOK:
		PieceList__add(b->rooks[pcol], &(b->rooks_size[pcol]), sq);
		PieceList__add(b->sliders[pcol], &(b->sliders_size[pcol]), sq);
		break;
	case QUEEN:
		PieceList__add(b->queens[pcol], &(b->queens_size[pcol]), sq);
		PieceList__add(b->sliders[pcol], &(b->sliders_size[pcol]), sq);
		break;
	case KING: b->kings[pcol] = sq; break;
	default:
		// This should never happen. Ignore all other values.
		return; // Skips squares assignment.
	}
	b->squares[sq] = p;
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
	case PAWN: PieceList__del(b->pawns[pcol], &(b->pawns_size[pcol]), sq); break;
	case KNIGHT: PieceList__del(b->knights[pcol], &(b->knights_size[pcol]), sq); break;
	case BISHOP:
		PieceList__del(b->bishops[pcol], &(b->bishops_size[pcol]), sq);
		PieceList__del(b->sliders[pcol], &(b->sliders_size[pcol]), sq);
		break;
	case ROOK:
		PieceList__del(b->rooks[pcol], &(b->rooks_size[pcol]), sq);
		PieceList__del(b->sliders[pcol], &(b->sliders_size[pcol]), sq);
		break;
	case QUEEN:
		PieceList__del(b->queens[pcol], &(b->queens_size[pcol]), sq);
		PieceList__del(b->sliders[pcol], &(b->sliders_size[pcol]), sq);
		break;
	case KING: b->kings[pcol] = sq; break;
	default:
		// This should never happen. Ignore all other values.
		break;
	}
}

bool Board__is_sq_attacked(Board *b, const Square sq, const Square ignore_sq, Color color) {
	Color opp_color = flip_color(color);
	
	// 1. Detect attacks by knights.
	for(size_t i = 0; i < b->knights_size[opp_color]; i++) {
		Square ksq = b->knights[opp_color][i];
		if(ksq == ignore_sq) {
			continue;
		}
		Square diff = square_diff(ksq, sq);
		if(contains_piece(DIFF_ATTACK_MAP[diff], KNIGHT)) {
			return true;
		}
	}


	// TODO: continue here

}

// IsSquareAttacked tests if a specific square is attacked by any enemy.
// func (b *Board) IsSquareAttacked(sq, ignoreSq Square, color Color) bool {
// 	oppColor := color.Flip()
//
// 	// 1. Detect attacks	by knights.
// 	for i := uint8(0); i < b.Knights[oppColor].Size; i++ {
// 		knightSq := b.Knights[oppColor].Pieces[i]
// 		if knightSq == ignoreSq {
// 			continue
// 		}
// 		diff := knightSq.Diff(sq)
// 		if SQUARE_DIFFS[diff].Contains(KNIGHT) {
// 			return true
// 		}
// 	}
//
// 	// 3. Detect attacks by sliders.
// 	if b.IsSqAttackedBySlider(color, sq, ignoreSq) {
// 		return true
// 	}
// 	//	if b.IsSqAttackedBySlider(color, sq, ignoreSq, &b.Queens[oppColor], QUEEN) {
// 	//		return true
// 	//	}
// 	//	if b.IsSqAttackedBySlider(color, sq, ignoreSq, &b.Bishops[oppColor], BISHOP) {
// 	//		return true
// 	//	}
// 	//	if b.IsSqAttackedBySlider(color, sq, ignoreSq, &b.Rooks[oppColor], ROOK) {
// 	//		return true
// 	//	}
//
// 	// 2. Detect attacks by pawns.
// 	oppPawn := PAWN | oppColor
//
// 	maybePawnSq := Square(int8(sq) + PAWN_CAPTURE_DIRS[color][0])
// 	if maybePawnSq.OnBoard() && b.Squares[maybePawnSq] == oppPawn {
// 		// Found an attacking pawn by inspecting in reverse direction.
// 		return true
// 	}
// 	maybePawnSq = Square(int8(sq) + PAWN_CAPTURE_DIRS[color][1])
// 	if maybePawnSq.OnBoard() && b.Squares[maybePawnSq] == oppPawn {
// 		// Found an attacking pawn by inspecting in reverse direction.
// 		return true
// 	}
//
// 	// 0. Detect attacks by kings.
// 	oppKingSq := b.Kings[oppColor]
// 	diff := oppKingSq.Diff(sq)
// 	if SQUARE_DIFFS[diff].Contains(KING) {
// 		return true
// 	}
//
// 	return false
// }
//

Error Board__to_string(Board *b, char *str) {
	if(b == NULL || str == NULL) {
		return ERR_NULL_PTR;
	}

	// str := "  +-----------------+\n"
	// for r := 7; r >= 0; r-- {
	// 	str += strconv.Itoa(r+1) + " | "
	// 	for f := 0; f < 8; f++ {
	// 		idx := 16*r + f
	// 		if Piece(idx) == b.EpSquare {
	// 			str += ", "
	// 		} else {
	// 			str += PrintMap[b.Squares[idx]] + " "
	// 		}
	//
	// 		if f == 7 {
	// 			str += "|\n"
	// 		}
	// 	}
	// }
	// str += "  +-----------------+\n"
	// str += "    a b c d e f g h\n"

	return OK;
}
