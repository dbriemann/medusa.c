#ifndef BOARD_H
#define BOARD_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "base.h"
#include "mlist.h"
#include "errors.h"

// Board contains all information for a chess board state,
// including the board itself as 0x88 board.
//
//   +------------------------+
// 8 |70 71 72 73 74 75 76 77 | 78 79 7a 7b 7c 7d 7e 7f
// 7 |60 61 62 63 64 65 66 67 | 68 69 6a 6b 6c 6d 6e 6f
// 6 |50 51 52 53 54 55 56 57 | 58 59 5a 5b 5c 5d 5e 5f
// 5 |40 41 42 43 44 45 46 47 | 48 49 4a 4b 4c 4d 4e 4f
// 4 |30 31 32 33 34 35 36 37 | 38 39 3a 3b 3c 3d 3e 3f
// 3 |20 21 22 23 24 25 26 27 | 28 29 2a 2b 2c 2d 2e 2f
// 2 |10 11 12 13 14 15 16 17 | 18 19 1a 1b 1c 1d 1e 1f
// 1 | 0  1  2  3  4  5  6  7 |  8  9  a  b  c  d  e  f
//   +------------------------+
//     a  b  c  d  e  f  g  h
//
// All indexes shown are in HEX. The left board represents the actual playing board,
// whereas there right board is used as meta information board aiding move generation.
typedef struct Board Board;
struct Board {
	Piece squares[64 * 2];
	bool castle_short[2];
	bool castle_long[2];
	uint16_t move_number;
	uint16_t draw_counter;
	Square check_info;
	Square ep_square;
	Color player;
	// Piece lists.
	Square kings[2];
	Square sliders[2][13];
	size_t sliders_size[2];
	Square queens[2][9];
	size_t queens_size[2];
	Square rooks[2][10];
	size_t rooks_size[2];
	Square bishops[2][10];
	size_t bishops_size[2];
	Square knights[2][10];
	size_t knights_size[2];
	Square pawns[2][8];
	size_t pawns_size[2];
};

// TODO convert all Board * to Board * const ?
void Board__set_starting_position(Board* b);
Error Board__set_fen(Board* b, const char* fen);
void Board__add_piece(Board* b, Square sq, Piece p);
void Board__del_piece(Board* b, Square sq);
void Board__clear(Board* b);
void Board__clear_meta(Board* b);
bool Board__is_sq_attacked(Board* b, const Square sq, const Square ignore_sq, Color color);
bool Board__is_sq_attacked_by_slider(Board* b, const Square sq, const Square ignore_sq, Color color);
void Board__detect_checks_and_pins(Board* b, Color color);
int Board__detect_slider_checks_and_pins(Board* b, Color color, Info* pmarker, const int ccount, size_t plist_len, Square const* const plist, Piece ptype);
void Board__generate_knight_moves(Board* board, MoveList* mlist, Color color);
Error Board__to_string(Board* b, char* str);

#endif
