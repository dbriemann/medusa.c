#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "base.h"
#include "bitmove.h"

BitMove BitMove__new(const Piece piece, const Square from, const Square to, const Piece promo, const Piece cap_piece,
                     const CastleType castle_type, const bool ep) {
	BitMove move = 0;
	move |= (BitMove)ep << MOVE_EP_SHIFT;
	move |= (BitMove)castle_type << MOVE_CASTLE_TYPE_SHIFT;
	move |= (BitMove)cap_piece << MOVE_CAPTURE_SHIFT;
	move |= (BitMove)piece << MOVE_PIECE_SHIFT;
	move |= (BitMove)promo << MOVE_PROMOTION_SHIFT;
	move |= (BitMove)to << MOVE_TO_SHIFT;
	move |= (BitMove)from;

	return move;
}

// BitMove BitMove__new_fast(const Square from, const Square to, const Piece promo) {
// 	BitMove move = (BitMove)from | (BitMove)to << MOVE_TO_SHIFT | (BitMove)promo << MOVE_PROMOTION_SHIFT;

// 	return move;
// }

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

Piece BitMove__captured_piece(const BitMove bm) {
	return (Piece)((bm & MOVE_CAPTURE_MASK) >> MOVE_CAPTURE_SHIFT);
}

CastleType BitMove__castle_type(const BitMove bm) {
	return (CastleType)((bm & MOVE_CASTLE_TYPE_MASK) >> MOVE_CASTLE_TYPE_SHIFT);
}

bool BitMove__en_passent(const BitMove bm) {
	return (CastleType)((bm & MOVE_EP_MASK) >> MOVE_EP_SHIFT);
}

BitMove BitMove__add_feature(BitMove move, BitMove mask, BitMove shift, Square bits) {
	move &= ~mask;                  // Erase existing bits in the mask's region.
	bits <<= shift;                 // Move bits to the right place
	move |= ((BitMove)bits) & mask; // Add them to the move,
	return move;
}

char *BitMove__to_notation(const BitMove move) {
	// e2-e4, Nb1-c3, Rd3xd7, 0-0, 0-0-0, c7-c8=Q
	Square from = BitMove__from(move);
	Square to = BitMove__to(move);
	Piece promo = BitMove__promoted_piece(move);
	Piece piece = BitMove__piece(move);
	Piece cap_piece = BitMove__captured_piece(move);
	CastleType castle_type = BitMove__castle_type(move);
	bool ep = BitMove__en_passent(move);

	size_t len = 0;

	char *notation = NULL;
	if (castle_type == CASTLE_OO) {
		// Castles short.
		notation = calloc(3 + 1, sizeof(char));
		if (!notation) {
			return NULL;
		}
		len = 4;
		memcpy(notation, "0-0", 3);
	} else if (castle_type == CASTLE_OOO) {
		// Castles long.
		notation = calloc(5 + 1, sizeof(char));
		if (!notation) {
			return NULL;
		}
		len = 6;
		memcpy(notation, "0-0-0", 5);
	} else {
		const char *from_s = LOOKUP_SQUARE_NAMES[from];
		const char *to_s = LOOKUP_SQUARE_NAMES[to];
		const char piece_s = Piece_to_notation(piece);

		notation = calloc(6 + 1, sizeof(char));
		if (!notation) {
			return NULL;
		}
		len = 7;
		notation[0] = piece_s;
		notation[1] = from_s[0];
		notation[2] = from_s[1];
		if (cap_piece != EMPTY) {
			notation[3] = 'x';
		} else {
			notation[3] = '-';
		}
		notation[4] = to_s[0];
		notation[5] = to_s[1];
		if (promo) {
			char *extend = realloc(notation, 8 + 1);
			if (!extend) {
				return NULL;
			}
			len = 9;
			notation = extend;
			const char promo_s = Piece_to_notation(promo);
			notation[6] = '=';
			notation[7] = promo_s;
			notation[8] = '\0';
		}
	}
	if (ep) {
		char *extend = realloc(notation, len + 2);
		if (!extend) {
			return NULL;
		}
		len = len + 2;
		notation = extend;
		notation[len - 3] = 'e';
		notation[len - 2] = 'p';
		notation[len - 1] = '\0';
	}

	return notation;
}

char Piece_to_notation(const Piece piece) {
	Color c = piece & COLOR_ONLY_MASK;
	Piece p = piece & PIECE_MASK;
	char ps = ' ';

	if (p & KNIGHT) {
		ps = 'n';
	} else if (p & BISHOP) {
		ps = 'b';
	} else if (p & ROOK) {
		ps = 'r';
	} else if (p & QUEEN) {
		ps = 'q';
	} else if (p & KING) {
		ps = 'k';
	}

	if (c == WHITE) {
		ps = toupper(ps);
	}

	return ps;
}
