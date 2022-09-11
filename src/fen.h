#include <stddef.h>

#include "base.h"
#include "minboard.h"

bool parse_fen(char fen[static 1], MinBoard *mb);
bool fen_parse_pieces(const char fen[static 15], Piece squares[64]);
bool fen_parse_color_to_move(const char fen[static 1], Color *color);
bool fen_parse_castling_rights(const char fen[static 1], bool oo[2], bool ooo[2]);
bool fen_square_to_index(const char fen[static 2], Square *sq);
bool fen_parse_ep_square(const char fen[static 1], Square *sq);
bool fen_parse_half_move_clock(const char fen[static 1], uint16_t *num);
bool fen_parse_move_number(const char fen[static 1], uint16_t *num);
