#ifndef BITMOVE_H
#define BITMOVE_H

#include <stdbool.h>
#include <stdint.h>

#include "base.h"

// BitMove is structured as follows:
//
// MSB                                           LSB
// |...|21 (8 bits) 14||13 (7 bits) 7||6 (7 bits) 0|
// |...|--promo piece-||-----to------||----from----|
// A few bits are 'wasted' here. Squares could be encoded with 6 bits and
// the piece types could be encoded by two bits. Doing it this way
// however allows for easier usage and better performance in this engine.
// The from and to squares are actual 0x88 indexes (0-127) and the promotion
// field is a Piece.
typedef uint64_t BitMove;
// TODO: uint32?

static const BitMove MOVE_FROM_MASK       = 0x000000007F;
static const BitMove MOVE_TO_SHIFT        = 7;
static const BitMove MOVE_TO_MASK         = 0x0000003F80;
static const BitMove MOVE_PROMOTION_SHIFT = 7 + 7;
static const BitMove MOVE_PROMOTION_MASK  = 0x00003FC000;

// TODO: we may need BitMove BitMove__new_lean() when not the complete move information is needed.
BitMove BitMove__new(const Piece piece, const Square from, const Piece promo);
Square  BitMove__from(const BitMove bm);
Square  BitMove__to(const BitMove bm);
Piece   BitMove__promoted_piece(const BitMove bm);
char *  BitMove__to_notation(const BitMove move);

char Piece_to_notation(const Piece p);

#endif
