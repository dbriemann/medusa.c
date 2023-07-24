#ifndef BASE_H
#define BASE_H

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

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
typedef uint8_t CastleType;

const static CastleType CASTLE_NONE = 0x0;
const static CastleType CASTLE_OO   = 0x1;
const static CastleType CASTLE_OOO  = 0x2;

const static Square OTB = 0x7F;

// All special check values are off-board.
const static Square CHECK_NONE         = OTB;
const static Square CHECK_DOUBLE_CHECK = 0x0F;
const static Square CHECK_CHECKMATE    = 0x1F;

// Other special constants.
const static Piece PROMO_NONE = 0;

const static Info INFO_NONE              = 0x0;
const static Info INFO_PIN_COUNTER_START = 1;
const static Info INFO_MASK_PINNED       = 0x0F; // Bits 1-4 == space for possible pinner enumeration.
// const static Info INFO_MASK_ATTACKED		 = 16;	 // Bit 5 == attacked?
const static Info INFO_MASK_FORBIDDEN_ESCAPE = 32;  // Bit 6 == forbidden square for king (line of check).
const static Info INFO_MASK_MAYBE_PINNED     = 64;  // Bit 7 == mark for pinning.
const static Info INFO_MASK_CHECK            = 128; // Bit 8 == check.

// Colors
const static Color BLACK           = 0;
const static Color WHITE           = 1;
const static Color COLOR_ONLY_MASK = 1;
const static Color COLOR_TEST_MASK = 129;

// Pieces:
const static Piece EMPTY        = 128; // 10000000
const static Piece PAWN         = 2;   // 00000010
const static Piece KNIGHT       = 4;   // 00000100
const static Piece BISHOP       = 8;   // 00001000
const static Piece ROOK         = 16;  // 00010000
const static Piece QUEEN        = 32;  // 00100000
const static Piece KING         = 64;  // 01000000
const static Piece PIECE_MASK   = 126; // 01111110
const static Piece PINNERS_MASK = 56;  // 00111000

// Colored pieces:
const static Piece BPAWN   = PAWN | BLACK;
const static Piece BKNIGHT = KNIGHT | BLACK;
const static Piece BBISHOP = BISHOP | BLACK;
const static Piece BROOK   = ROOK | BLACK;
const static Piece BQUEEN  = QUEEN | BLACK;
const static Piece BKING   = KING | BLACK;
const static Piece WPAWN   = PAWN | WHITE;
const static Piece WKNIGHT = KNIGHT | WHITE;
const static Piece WBISHOP = BISHOP | WHITE;
const static Piece WROOK   = ROOK | WHITE;
const static Piece WQUEEN  = QUEEN | WHITE;
const static Piece WKING   = KING | WHITE;

// Directions:
const static Direction UP         = 16;
const static Direction DOWN       = -16;
const static Direction LEFT       = -1;
const static Direction RIGHT      = 1;
const static Direction UP_LEFT    = UP + LEFT;
const static Direction UP_RIGHT   = UP + RIGHT;
const static Direction DOWN_LEFT  = DOWN + LEFT;
const static Direction DOWN_RIGHT = DOWN + RIGHT;

const static Direction PAWN_PUSH_DIRS[2]       = { DOWN, UP };
const static size_t    PAWN_PUSH_DIRS_LEN      = 2;
const static Direction PAWN_CAPTURE_DIRS[2][2] = { { DOWN_LEFT, DOWN_RIGHT }, { UP_LEFT, UP_RIGHT } };
const static size_t    PAWN_PUSH_CAPTURE_LEN   = 2;
const static Square    PAWN_BASE_RANK[2]       = { 6, 1 };
const static size_t    PAWN_BASE_RANK_LEN      = 2;
const static Square    PAWN_PROMOTE_RANK[2]    = { 0, 7 };
const static size_t    PAWN_PROMOTE_RANK_LEN   = 2;

const static Direction KNIGHT_DIRS[8] = {
	UP + UP_LEFT, UP + UP_RIGHT, DOWN + DOWN_LEFT, DOWN + DOWN_RIGHT,
	LEFT + UP_LEFT, LEFT + DOWN_LEFT, RIGHT + UP_RIGHT, RIGHT + DOWN_RIGHT
};
const static size_t    KNIGHT_DIRS_LEN             = 8;
const static Direction SLIDER_DIRS                 = 4;
const static Direction DIAGONAL_DIRS[4]            = { UP_LEFT, UP_RIGHT, DOWN_LEFT, DOWN_RIGHT };
const static size_t    DIAGONAL_DIRS_LEN           = 4;
const static Direction ORTHOGONAL_DIRS[4]          = { LEFT, UP, RIGHT, DOWN };
const static size_t    ORTHOGONAL_DIRS_LEN         = 4;
const static Direction ALL_DIRS[8]                 = { RIGHT, LEFT, UP, DOWN, UP_LEFT, UP_RIGHT, DOWN_LEFT, DOWN_RIGHT };
const static size_t    ALL_DIRS_LEN                = 8;
const static Square    CASTLING_PATH_SHORT[2][2]   = { { 0x75, 0x76 }, { 0x5, 0x6 } };
const static Square    CASTLING_PATH_LONG[2][3]    = { { 0x73, 0x72, 0x71 }, { 0x3, 0x2, 0x1 } };
const static Square    CASTLING_ROOK_SHORT[2]      = { 0x77, 0x07 };
const static Square    CASTLING_ROOK_LONG[2]       = { 0x70, 0x00 };
const static Square    CASTLING_DETECT_SHORT[2][2] = { { 0x74, 0x76 }, { 0x04, 0x06 } }; // FROM->TO squares
const static Square    CASTLING_DETECT_LONG[2][2]  = { { 0x74, 0x72 }, { 0x04, 0x02 } };// FROM->TO squares
const static size_t    CASTLING_LEN                = 2;

// const static Square META_BOARD_INDEXES[64] = {
// 	0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
// 	0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,
// 	0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f,
// 	0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f,
// 	0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f,
// 	0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f,
// 	0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f,
// 	0x78, 0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f,
// };

const static char LOOKUP_SQUARE_NAMES[128][2] = {
	"a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1", "i1", "j1", "k1", "l1", "m1", "n1", "o1", "p1",
	"a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2", "i2", "j2", "k2", "l2", "m2", "n2", "o2", "p2",
	"a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3", "i3", "j3", "k3", "l3", "m3", "n3", "o3", "p3",
	"a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4", "i4", "j4", "k4", "l4", "m4", "n4", "o4", "p4",
	"a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5", "i5", "j5", "k5", "l5", "m5", "n5", "o5", "p5",
	"a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6", "i6", "j6", "k6", "l6", "m6", "n6", "o6", "p6",
	"a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7", "i7", "j7", "k7", "l7", "m7", "n7", "o7", "p7",
	"a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8", "i8", "j8", "k8", "l8", "m8", "n8", "o8", "p8",
};

// 0x88 board:
const static Square LOOKUP_0x88[64] = {
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
	0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
	0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
	0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
	0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,
	0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57,
	0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,
	0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77
};

const static Square DIFF_ATTACK_MAP[240] = {
	40, 0, 0, 0, 0, 0, 0, 48, 0, 0, 0, 0, 0, 0, 40, 0, 0, 40, 0, 0,
	0, 0, 0, 48, 0, 0, 0, 0, 0, 40, 0, 0, 0, 0, 40, 0, 0, 0, 0, 48,
	0, 0, 0, 0, 40, 0, 0, 0, 0, 0, 0, 40, 0, 0, 0, 48, 0, 0, 0, 40,
	0, 0, 0, 0, 0, 0, 0, 0, 40, 0, 0, 48, 0, 0, 40, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 40, 4, 48, 4, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 4, 104, 112, 104, 4, 0, 0, 0, 0, 0, 0, 48, 48, 48, 48, 48, 48, 112, 0,
	112, 48, 48, 48, 48, 48, 48, 0, 0, 0, 0, 0, 0, 4, 104, 112, 104, 4, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 40, 4, 48, 4, 40, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 40, 0, 0, 48, 0, 0, 40, 0, 0, 0, 0, 0, 0, 0, 0, 40,
	0, 0, 0, 48, 0, 0, 0, 40, 0, 0, 0, 0, 0, 0, 40, 0, 0, 0, 0, 48,
	0, 0, 0, 0, 40, 0, 0, 0, 0, 40, 0, 0, 0, 0, 0, 48, 0, 0, 0, 0,
	0, 40, 0, 0, 40, 0, 0, 0, 0, 0, 0, 48, 0, 0, 0, 0, 0, 0, 40, 0,
};

const static Direction DIFF_DIR_MAP[240] = {
	-17, 0, 0, 0, 0, 0, 0, -16, 0, 0, 0, 0, 0, 0, -15, 0, 0, -17, 0, 0,
	0, 0, 0, -16, 0, 0, 0, 0, 0, -15, 0, 0, 0, 0, -17, 0, 0, 0, 0, -16,
	0, 0, 0, 0, -15, 0, 0, 0, 0, 0, 0, -17, 0, 0, 0, -16, 0, 0, 0, -15,
	0, 0, 0, 0, 0, 0, 0, 0, -17, 0, 0, -16, 0, 0, -15, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, -17, -33, -16, -31, -15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, -18, -17, -16, -15, -14, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, 0,
	1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 14, 15, 16, 17, 18, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 15, 31, 16, 33, 17, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 15, 0, 0, 16, 0, 0, 17, 0, 0, 0, 0, 0, 0, 0, 0, 15,
	0, 0, 0, 16, 0, 0, 0, 17, 0, 0, 0, 0, 0, 0, 15, 0, 0, 0, 0, 16,
	0, 0, 0, 0, 17, 0, 0, 0, 0, 15, 0, 0, 0, 0, 0, 16, 0, 0, 0, 0,
	0, 17, 0, 0, 15, 0, 0, 0, 0, 0, 0, 16, 0, 0, 0, 0, 0, 0, 17, 0,
};

// NOLINTNEXTLINE clangd incorrectly warns of unused function.
static inline bool has_color(const Piece p, const Color c) {
	return (COLOR_TEST_MASK & p) == c;
}

// NOLINTNEXTLINE clangd incorrectly warns of unused function.
static inline bool on_board(Square sq) {
	return !(sq & 0x88);
}

// NOLINTNEXTLINE clangd incorrectly warns of unused function.
static inline Color flip_color(const Color c) {
	return c ^ 1;
}

// NOLINTNEXTLINE clangd incorrectly warns of unused function.
static inline Square square_diff(const Square from, const Square to) {
	return (Square)(0x77 + ((Direction)to - (Direction)from));
}

// NOLINTNEXTLINE clangd incorrectly warns of unused function.
static inline bool contains_piece_type(const Piece pieces, const Piece p) {
	return p & pieces;
}

// NOLINTNEXTLINE clangd incorrectly warns of unused function.
static inline Square to_info_index(const Square sq) {
	// NOTE: does not check overflow. Must receive on-board square.
	return sq + 8;
}

// NOLINTNEXTLINE clangd incorrectly warns of unused function.
static inline Square pinval(Info i) {
	return i & INFO_MASK_PINNED;
}

// NOLINTNEXTLINE clangd incorrectly warns of unused function.
static inline bool is_mask_set(Info i, Info mask) {
	return i & mask;
}

#endif
