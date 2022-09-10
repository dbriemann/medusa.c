#include <string.h>
#include <stdlib.h>

#include "base.h"
#include "fen.h"
#include "minboard.h"

bool parse_fen(char fen[static 100], MinBoard *mb) {
	char group_delim[] = " "; // Spaces splits fen into 6 groups.

	// Group 1 : pieces
	char *strpos = strtok(fen, group_delim);
	if(strpos == 0) {
		return false;
	}
	bool ok = fen_parse_pieces(strpos, mb->squares);
	if(!ok) {
		return false;
	}

	// Group 2 : color to move
	strpos = strtok(fen, group_delim);
	if(strpos == 0) {
		return false;
	}

	// strpos = strtok(0, group_delim);
	return false;
}

/**
	Parses the first FEN group which represents the pieces on the board.
	Stores the result in 'squares' which has the following layout:

	   a b c d e f g h
	 +----------------
	8|56,..         63] <- array index 63 is black rook @ H8
	7|48,..
	6|40,..
	5|32,..
	4|24,..
	3|16,..
	2| 8,..
	1|[0,1,2,3,4,5,6,7, <- array index 0 is white rook @ A1
	 +-----------------
*/
bool fen_parse_pieces(char fen[static 15], Piece squares[64]) {
	size_t pos		= 0;
	size_t boardpos = 0;

	while(fen[pos] != 0) {
		char token = fen[pos];
		// Check if strpos hold 1-8 (number of continues empty squares).
		if(token >= '1' && token <= '8') {
			int empty_squares = token - '0';
			for(size_t i = 0; i < empty_squares; i++) {
				squares[boardpos] = EMPTY;
				boardpos++;
			}
			pos++;
			continue;
		} else {
			// Else detect the piece type.
			switch(token) {
			// White pieces:
			case 'P': squares[boardpos] = WPAWN; break;
			case 'N': squares[boardpos] = WKNIGHT; break;
			case 'B': squares[boardpos] = WBISHOP; break;
			case 'R': squares[boardpos] = WROOK; break;
			case 'Q': squares[boardpos] = WQUEEN; break;
			case 'K': squares[boardpos] = WKING; break;
			// Black pieces:
			case 'p': squares[boardpos] = BPAWN; break;
			case 'n': squares[boardpos] = BKNIGHT; break;
			case 'b': squares[boardpos] = BBISHOP; break;
			case 'r': squares[boardpos] = BROOK; break;
			case 'q': squares[boardpos] = BQUEEN; break;
			case 'k': squares[boardpos] = BKING; break;
			case '/': // Indicates next rank. Handled after switch-case.
				break;
			default: // Unknown character (invalid FEN).
				// Fail: return false.
				return false;
			}
		}

		pos++;
		if(token != '/') {
			boardpos++;
		} else {
			// If boardpos is not on file 1 after rank break, the fen is malformed.
			if(boardpos % 8) {
				return false;
			}
		}

		// Check if FEN completed the board.
		if(boardpos == 64) {
			if(pos < strlen(fen)) {
				return false;
			}
			break;
		}
	}

	/*
	We now mirror the ranks in the array to be compatible with the internal engine board type:

	a1 b1 c1 .. h1             a8 b8 c8 .. h8             56 57 58 .. 63
	a2 b2 c2 .. h2   ======>   .            .   where     .            .
	.            .   becomes   .            .   indexes   .            .
	.            .             a2 b2 c2 .. h2   are       08 09 10 .. 15
	a8 b8 c8 .. h8             a1 b1 c1 .. h1             01 02 03 .. 07

	*/

	for(size_t lo = 0, hi = 56; lo < 32; lo++, hi++) {
		Piece tmp	= squares[lo];
		squares[lo] = squares[hi];
		squares[hi] = tmp;
		if((hi + 1) % 8 == 0) {
			// Wrap back to next rank.
			hi -= 16;
		}
	}

	return true;
}

bool fen_parse_color_to_move(const char fen[static 1], Color *color) {
	if(strlen(fen) != 1) {
		return false;
	}
	switch(fen[0]) {
	case 'b': *color = BLACK; return true;
	case 'w': *color = WHITE; return true;
	default: return false;
	}
}

bool fen_parse_castling_rights(const char fen[static 1], bool oo[2], bool ooo[2]) {
	if(strlen(fen) < 1 || strlen(fen) > 4) {
		return false;
	}
	size_t pos = 0;
	// Init all with false.
	oo[WHITE]  = false;
	oo[BLACK]  = false;
	ooo[WHITE] = false;
	ooo[BLACK] = false;
	if(strlen(fen) == 1 && fen[pos] == '-') {
		// No more castling permitted.
		// Just use default from above.
		return true;
	}
	while(fen[pos] != 0) {
		switch(fen[pos]) {
		case 'K': oo[WHITE] = true; break;
		case 'Q': ooo[WHITE] = true; break;
		case 'k': oo[BLACK] = true; break;
		case 'q': ooo[BLACK] = true; break;
		default: return false;
		}
		pos++;
	}
	return true;
}

bool fen_square_to_index(const char fensq[static 2], Square *sq) {
	*sq = OTB;
	if(strlen(fensq) != 2) {
		return false;
	}
	char r = fensq[0];
	if(r < 'a' || r > 'h') {
		return false;
	}
	char f = fensq[1];
	if(f < '1' || f > '8') {
		return false;
	}
	char rank = r - 'a';
	char file = f - '1';

	*sq = rank+file*8;
	return true;
}

bool fen_parse_ep_square(const char fen[static 1], Square *sq) {
	*sq = OTB;
	if(strlen(fen) == 1 && fen[0] == '-') {
		return true;
	}

	if(strlen(fen) != 2) {
		return false;
	}

	// Only rank 3 and 6 can have valid ep squares.
	if(fen[1] != '3' && fen[1] != '6') {
		return false;
	}
	
	return fen_square_to_index(fen, sq);
}

bool fen_parse_half_move_clock(const char fen[static 1], size_t *num) {
	// Half move counter can be 0.
	char *endptr = 0;
	unsigned long ul = strtoul(fen, &endptr, 10);
	if(endptr && *endptr != '\0') {
		return false;
	}
	*num = (size_t)ul;
	return true;
}

bool fen_parse_move_number(const char fen[static 1], size_t *num) {
	// TODO: maximum game length?
	bool ok = fen_parse_half_move_clock(fen, num);
	// Moves start at 1.
	if(!ok || *num == 0) {
		return false;
	}
	return true;
}

