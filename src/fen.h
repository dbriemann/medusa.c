#include <stddef.h>

#include "base.h"
#include "minboard.h"
#include "errors.h"

Error parse_fen(const char fen[], MinBoard* mb);
Error fen_parse_pieces(const char fen[], Piece squares[64]);
Error fen_parse_color_to_move(const char fen[], Color* color);
Error fen_parse_castling_rights(const char fen[], bool oo[2], bool ooo[2]);
Error fen_square_to_index(const char fen[static 2], Square* sq);
Error fen_parse_ep_square(const char fen[], Square* sq);
Error fen_parse_half_move_clock(const char fen[], uint16_t* num);
Error fen_parse_move_number(const char fen[], uint16_t* num);
