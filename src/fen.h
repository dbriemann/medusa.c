#include <stddef.h>

#include "base.h"
#include "minboard.h"

bool parse_fen(char fen[], MinBoard *mb);
bool fen_parse_pieces(const char fen[], Piece squares[64]);
bool fen_parse_color_to_move(const char fen[], Color *color);
bool fen_parse_castling_rights(const char fen[], bool oo[2], bool ooo[2]);
bool fen_square_to_index(const char fen[], Square *sq);
bool fen_parse_ep_square(const char fen[], Square *sq);
bool fen_parse_half_move_clock(const char fen[], uint16_t *num);
bool fen_parse_move_number(const char fen[], uint16_t *num);
