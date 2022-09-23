#ifndef BITMOVE_H
#define BITMOVE_H

#include <stdint.h>

#include "base.h"

// BitMove is structured as follows:
//
// MSB                                             LSB
// ... |17 (4 bits) 14||13 (7 bits) 7||6 (7 bits) 0|
// ... |-----prom-----||-----to------||----from----|
// A few bits are 'wasted' here. Squares could be encoded with 6 bits and
// the promotion piece type could be encoded by two bits. Doing it this way
// however allows for easier usage and better performance in this engine.
// The from and to squares are actual 0x88 indexes (0-127) and the promotion
// field contains 1 bit for each possible type: knight, bishop, rook, queen.
//
// TODO this type should be enhanced by an additional value field in the future
// which allows better move ordering in the move search.
typedef uint32_t BitMove;

static const BitMove MOVE_FROM_MASK	 = 0x0000007F;
static const BitMove MOVE_TO_SHIFT	 = 7;
static const BitMove MOVE_TO_MASK	 = 0x00003F80;
// TODO(the promotion shift seems wrong.. what about bits from "to" part that remain.)
// NOTE: KING would be highest 7-bit. PAWN would be 2-bit.
// We skip 2 bits in the shift because the 2 lowest Piece bits are 0 and ignored.
static const BitMove MOVE_PROMOTION_SHIFT = 7 + 7 - 2;
static const BitMove MOVE_PROMOTION_MASK  = 0x0003C000;

BitMove BitMove__new(const Square from, const Square to, const Piece promo);
Square BitMove__from(const BitMove bm);
Square BitMove__to(const BitMove bm);
Piece BitMove__promoted_piece(const BitMove bm);

// TODO: to mini-notation string : e2e4, f7f8Q

#endif
