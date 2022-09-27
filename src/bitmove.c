#include "bitmove.h"

BitMove BitMove__new(const Square from, const Square to, const Piece promo) {
	BitMove move = ((BitMove)promo << MOVE_PROMOTION_SHIFT) & MOVE_PROMOTION_MASK;
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

BitMove BitMove__add_feature(BitMove move, BitMove mask, BitMove shift, Square bits) {
	move &= ~mask;					// Erase existing bits in the mask's region.
	bits <<= shift;					// Move bits to the right place
	move |= ((BitMove)bits) & mask; // Add them to the move,
	return move;
}
