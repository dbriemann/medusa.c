#ifndef BITMOVE_H
#define BITMOVE_H

#include <stdint.h>

#include "base.h"

// BitMove is structured as follows:
//
// MSB                                             LSB
// ... |21 (8 bits) 14||13 (7 bits) 7||6 (7 bits) 0|
// ... |-----prom-----||-----to------||----from----|
// A few bits are 'wasted' here. Squares could be encoded with 6 bits and
// the promotion piece type could be encoded by two bits. Doing it this way
// however allows for easier usage and better performance in this engine.
// The from and to squares are actual 0x88 indexes (0-127) and the promotion
// field is a Piece.
//
typedef uint32_t BitMove;

static const BitMove MOVE_FROM_MASK	 = 0x0000007F;
static const BitMove MOVE_TO_SHIFT	 = 7;
static const BitMove MOVE_TO_MASK	 = 0x00003F80;
static const BitMove MOVE_PROMOTION_SHIFT = 7 + 7;
static const BitMove MOVE_PROMOTION_MASK  = 0x003FC000;

BitMove BitMove__new(const Square from, const Square to, const Piece promo);
Square BitMove__from(const BitMove bm);
Square BitMove__to(const BitMove bm);
Piece BitMove__promoted_piece(const BitMove bm);
BitMove BitMove__add_feature(BitMove move, BitMove mask, BitMove shift, Square bits);

// TODO: to mini-notation string : e2e4, f7f8Q

#endif