#include "base.h"
#include "minboard.h"

bool parse_fen(char [static 100], MinBoard *);
bool fen_parse_pieces(char [static 15], Piece [64]);
bool fen_parse_color_to_move(const char [static 1], Color *);
