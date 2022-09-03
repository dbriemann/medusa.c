#ifndef BASE_H
#define BASE_H

#include <stdint.h>

/**
 * Piece, Color, Square and Info are all just uint8_t and are often
 * arithmetically mixed. However they all exist to make the code more readable.
 */
// TODO: find out if narrow integer types make sense because of arithmetic
// promotion. 
// https://wiki.sei.cmu.edu/confluence/display/c/INT02-C.+Understand+integer+conversion+rules
typedef uint8_t Piece;
typedef uint8_t Color;
typedef uint8_t Square;
typedef uint8_t Info;
typedef int8_t Direction;

const Piece NONE = 0;
const Square OTB = 0x7F;

// Colors
const Color BLACK = 0;
const Color WHITE = 1;
const Color COLOR_ONLY_MASK = 1;
const Color COLOR_TEST_MASK = 129;

// Pieces:
const Piece EMPTY = 128;       // 10000000
const Piece PAWN = 2;          // 00000010
const Piece KNIGHT = 4;        // 00000100
const Piece BISHOP = 8;        // 00001000
const Piece ROOK = 16;         // 00010000
const Piece QUEEN = 32;        // 00100000
const Piece KING = 64;         // 01000000
const Piece PIECE_MASK = 126;  // 01111110
const Piece PINNERS_MASK = 56; // 00111000

// Colored pieces:
const Piece BPAWN = PAWN | BLACK;
const Piece BKNIGHT = KNIGHT | BLACK;
const Piece BBISHOP = BISHOP | BLACK;
const Piece BROOK = ROOK | BLACK;
const Piece BQUEEN = QUEEN | BLACK;
const Piece BKING = KING | BLACK;
const Piece WPAWN = PAWN | WHITE;
const Piece WKNIGHT = KNIGHT | WHITE;
const Piece WBISHOP = BISHOP | WHITE;
const Piece WROOK = ROOK | WHITE;
const Piece WQUEEN = QUEEN | WHITE;
const Piece WKING = KING | WHITE;

// Directions:
const Direction UP = 16;
const Direction DOWN = -16;
const Direction LEFT = -1;
const Direction RIGHT = 1;
const Direction UP_LEFT = UP + LEFT;
const Direction UP_RIGHT = UP + RIGHT;
const Direction DOWN_LEFT = DOWN + LEFT;
const Direction DOWN_RIGHT = DOWN + RIGHT;

const Direction PAWN_PUSH_DIRS[2] = {DOWN, UP};
const Direction PAWN_CAPTURE_DIRS[2][2] = {{DOWN_LEFT, DOWN_RIGHT}, {UP_LEFT, UP_RIGHT}};
const Square PAWN_BASE_RANK[2] = {6, 1};
const Square PAWN_PROMOTE_RANK[2] = {0, 7};
const Direction KNIGHT_DIRS[8] = {
	UP + UP_LEFT, UP + UP_RIGHT, DOWN + DOWN_LEFT, DOWN + DOWN_RIGHT, 
	LEFT + UP_LEFT, LEFT + DOWN_LEFT, RIGHT + UP_RIGHT, RIGHT + DOWN_RIGHT
};
const Direction DIAGONAL_DIRS[4] = {UP_LEFT, UP_RIGHT, DOWN_LEFT, DOWN_RIGHT};
const Direction ORTHOGONAL_DIRS[4]={LEFT, UP, RIGHT, DOWN};
const Direction KING_DIRS[8]={RIGHT, LEFT, UP, DOWN, UP_LEFT, UP_RIGHT, DOWN_LEFT, DOWN_RIGHT};
const Square CASTLING_PATH_SHORT[2][2]={{0x75, 0x76}, {0x5, 0x6}};
const Square CASTLING_PATH_LONG[2][3]={{0x73, 0x72, 0x71}, {0x3, 0x2, 0x1}};
const Square CASTLING_ROOK_SHORT[2]={0x77, 0x07};
const Square CASTLING_ROOK_LONG[2]={0x70, 0x00};
const Square CASTLING_DETECT_SHORT[2][2]={{0x74, 0x76}, {0x04, 0x06}}; // FROM->TO squares
const Square CASTLING_DETECT_LONG[2][2]={{0x74, 0x72}, {0x04, 0x02}}; // FROM->TO squares

const Square META_BOARD_INDEXES[64] = {
	0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
	0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,
	0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f,
	0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f,
	0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f,
	0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f,
	0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f,
	0x78, 0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f,
};

#endif
