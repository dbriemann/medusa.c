#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "bitmove.h"
#include "base.h"

BitMove BitMove__new(const Piece piece, const Square from, const Square to, const Piece promo, bool is_capture) {
	BitMove move = (BitMove)is_capture << MOVE_CAPTURE_SHIFT;
	move |= (BitMove)piece << MOVE_PIECE_SHIFT;
	move |= (BitMove)promo << MOVE_PROMOTION_SHIFT;
	move |= (BitMove)to << MOVE_TO_SHIFT;
	move |= (BitMove)from;

	return move;
}

Square BitMove__from(const BitMove bm) {
	return (Square)(bm & MOVE_FROM_MASK);
}

Square BitMove__to(const BitMove bm) {
	return (Square)((bm & MOVE_TO_MASK) >> MOVE_TO_SHIFT);
}

Piece BitMove__promoted_piece(const BitMove bm) {
	return (Square)((bm & MOVE_PROMOTION_MASK) >> MOVE_PROMOTION_SHIFT);
}

Piece BitMove__piece(const BitMove bm) {
	return (Square)((bm & MOVE_PIECE_MASK) >> MOVE_PIECE_SHIFT);
}

bool BitMove__is_capture(const BitMove bm) {
	return (bool)((bm & MOVE_CAPTURE_MASK) >> MOVE_CAPTURE_SHIFT);
}

BitMove BitMove__add_feature(BitMove move, BitMove mask, BitMove shift, Square bits) {
	move &= ~mask;                  // Erase existing bits in the mask's region.
	bits <<= shift;                 // Move bits to the right place
	move |= ((BitMove)bits) & mask; // Add them to the move,
	return move;
}

char* BitMove__to_notation(const BitMove move) {
	// TODO
	// e2-e4, Nb1-c3, Rd3xd7, 0-0, 0-0-0, c7-c8=Q
	Square from       = BitMove__from(move);
	Square to         = BitMove__to(move);
	Piece  promo      = BitMove__promoted_piece(move);
	Piece  piece      = BitMove__piece(move);
	bool   is_capture = BitMove__is_capture(move);

	char* notation = NULL;
	if(piece == KING) {
		if((from == 0x04 && to == 0x06) || (from == 0x74 && to == 0x76)) {
			// Castles short.
			notation = calloc(3 + 1, sizeof(char));
			if(!notation) {
				return NULL;
			}
			memcpy(notation, "0-0", 3);
		} else if((from == 0x04 && to == 0x02) || (from == 0x74 && to == 0x72)) {
			// Castles long.
			notation = calloc(5 + 1, sizeof(char));
			if(!notation) {
				return NULL;
			}
			memcpy(notation, "0-0-0", 5);
		}
	} else {
		const char* from_s  = LOOKUP_SQUARE_NAMES[from];
		const char* to_s    = LOOKUP_SQUARE_NAMES[to];
		const char  piece_s = Piece_to_notation(piece);

		notation            = calloc(6 + 1, sizeof(char));
		if(!notation) {
			return NULL;
		}
		notation[0] = piece_s;
		notation[1] = from_s[0];
		notation[2] = from_s[1];
		if(is_capture) {
			notation[3] = 'x';
		} else {
			notation[3] = '-';
		}
		notation[4] = to_s[0];
		notation[5] = to_s[1];
		if(promo) {
			char* extend = realloc(notation, 8 + 1);
			if(!extend) {
				return NULL;
			}
			notation = extend;
			const char promo_s = Piece_to_notation(promo);
			notation[6] = '=';
			notation[7] = promo_s;
			notation[8] = '\0';
		}
	}

	return notation;
}

char Piece_to_notation(const Piece piece) {
	Color c  = piece & COLOR_ONLY_MASK;
	Piece p  = piece & PIECE_MASK;
	char  ps = ' ';

	if(p & KNIGHT) {
		ps = 'n';
	} else if(p & BISHOP) {
		ps = 'b';
	} else if(p & ROOK) {
		ps = 'r';
	} else if(p & QUEEN) {
		ps = 'q';
	} else if(p & KING) {
		ps = 'k';
	}

	if(c == WHITE) {
		ps = toupper(ps);
	}

	return ps;
}
