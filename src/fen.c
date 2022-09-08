#include <string.h>

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
		// Check if strpos hold 1-8 (number of continues empty fields).
		if(token >= '1' && token <= '8') {
			int empty_fields = token - '0';
			for(size_t i = 0; i < empty_fields; i++) {
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
