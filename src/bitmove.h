#ifndef BITMOVE_H
#define BITMOVE_H

#include <stdint.h>
#include <stdbool.h>

#include "base.h"

// BitMove is structured as follows:
//
// MSB                                                       LSB
// |40 (1 bits) 40||39 (2 bits) 38||37 (8 bits) 30||29 (8 bits) 22||21 (8 bits) 14||13 (7 bits) 7||6 (7 bits) 0|
// |--en passent--||--castle type-||-capt. piece--||----piece-----||--promo piece-||-----to------||----from----|
// A few bits are 'wasted' here. Squares could be encoded with 6 bits and
// the piece types could be encoded by two bits. Doing it this way
// however allows for easier usage and better performance in this engine.
// The from and to squares are actual 0x88 indexes (0-127) and the promotion
// field is a Piece.
typedef uint64_t BitMove;

static const BitMove MOVE_FROM_MASK         = 0x000000007F;
static const BitMove MOVE_TO_SHIFT          = 7;
static const BitMove MOVE_TO_MASK           = 0x0000003F80;
static const BitMove MOVE_PROMOTION_SHIFT   = 7 + 7;
static const BitMove MOVE_PROMOTION_MASK    = 0x00003FC000;
static const BitMove MOVE_PIECE_SHIFT       = 7 + 7 + 8;
static const BitMove MOVE_PIECE_MASK        = 0x003FC00000;
static const BitMove MOVE_CAPTURE_SHIFT     = 7 + 7 + 8 + 8;
static const BitMove MOVE_CAPTURE_MASK      = 0x3FC0000000;
static const BitMove MOVE_CASTLE_TYPE_SHIFT = 7 + 7 + 8 + 8 + 8;
static const BitMove MOVE_CASTLE_TYPE_MASK  = 0xC000000000;
static const BitMove MOVE_EP_SHIFT          = 7 + 7 + 8 + 8 + 8 + 2;
static const BitMove MOVE_EP_MASK           = 0x10000000000;

// TODO: we may need BitMove BitMove__new_lean() when not the complete move information is needed.
BitMove    BitMove__new(const Piece piece, const Square from, const Square to, const Piece promo, Piece cap_piece, CastleType castle_type, bool ep);
Square     BitMove__from(const BitMove bm);
Square     BitMove__to(const BitMove bm);
Piece      BitMove__promoted_piece(const BitMove bm);
Piece      BitMove__piece(const BitMove bm);
Piece      BitMove__captured_piece(const BitMove bm);
CastleType BitMove__castle_type(const BitMove bm);
bool       BitMove__en_passent(const BitMove bm);
BitMove    BitMove__add_feature(BitMove move, BitMove mask, BitMove shift, Square bits);
char*      BitMove__to_notation(const BitMove move);

char Piece_to_notation(const Piece p);

#endif
