#include "base.h"
#include "minboard.h"

bool parse_fen(char [static 100], MinBoard *);
bool fen_parse_pieces(char [static 15], Piece [64]);
bool fen_parse_color_to_move(const char [static 1], Color *);
bool fen_parse_castling_rights(const char [static 1], bool [2], bool [2]);
bool fen_square_to_index(const char [static 2], Square *);
bool fen_parse_ep_square(const char [static 1], Square *);
