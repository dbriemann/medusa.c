#include <string.h>
#include <stdlib.h>

#include "base.h"
#include "fen.h"
#include "minboard.h"

Error parse_fen(const char fen[], MinBoard *mb) {
	if(fen == NULL || mb == NULL) {
		return ERR_NULL_PTR;
	}

	char *cpy = strdup(fen);
	if(cpy == NULL) {
		return ERR_UNKNOWN;
	}

	char delim[] = " "; // Spaces splits fen into 6 groups.
	Error error = ERR_UNKNOWN;

	do { // This loop runs only ONCE.. so we do not need to free for many returns
		// Group 1 : pieces
		char *strpos = strtok(cpy, delim);
		if(strpos == NULL) {
			error = ERR_INVALID_INPUT;
			break;
		}
		error = fen_parse_pieces(strpos, mb->squares);
		if(error) {
			error = ERR_PARSE_FEN_PIECES;
			break;
		}

		// Group 2 : color to move
		strpos = strtok(NULL, delim);
		if(strpos == NULL) {
			error = ERR_INVALID_INPUT;
			break;
		}
		error = fen_parse_color_to_move(strpos, &mb->color);
		if(error) {
			error = ERR_PARSE_FEN_COLOR;
			break;
		}

		// Group 3 : castling rights
		strpos = strtok(NULL, delim);
		if(strpos == NULL) {
			error = ERR_INVALID_INPUT;
			break;
		}
		error = fen_parse_castling_rights(strpos, mb->castle_short, mb->castle_long);
		if(error) {
			error = ERR_PARSE_FEN_CASTLING;
			break;
		}

		// Group 4 : en passent square
		strpos = strtok(NULL, delim);
		if(strpos == NULL) {
			error = ERR_INVALID_INPUT;
			break;
		}
		error = fen_parse_ep_square(strpos, &mb->ep_square);
		if(error) {
			error = ERR_PARSE_FEN_EP;
			break;
		}

		// Group 5 : half-move clock
		// Counts since last capture or pawn advance.
		strpos = strtok(NULL, delim);
		if(strpos == NULL) {
			error = ERR_INVALID_INPUT;
			break;
		}
		error = fen_parse_half_move_clock(strpos, &mb->half_moves);
		if(error) {
			error = ERR_PARSE_FEN_HALF_MOVE;
			break;
		}

		// Group 6 : move number
		strpos = strtok(NULL, delim);
		if(strpos == NULL) {
			error = ERR_INVALID_INPUT;
			break;
		}
		error = fen_parse_move_number(strpos, &mb->move_num);
		if(error) {
			error = ERR_PARSE_FEN_MOVE;
			break;
		}
		error = OK;
	} while(false);

	free(cpy);
	return error;
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
Error fen_parse_pieces(const char fen[], Piece squares[64]) {
	if(fen == NULL || squares == NULL) {
		return ERR_NULL_PTR;
	}

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
				return ERR_INVALID_INPUT;
			}
		}

		pos++;
		if(token != '/') {
			boardpos++;
		} else {
			// If boardpos is not on file 1 after rank break, the fen is malformed.
			if(boardpos % 8) {
				return ERR_INVALID_INPUT;
			}
		}

		// Check if FEN completed the board.
		if(boardpos == 64) {
			if(pos < strlen(fen)) {
				return ERR_INVALID_INPUT;
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

	return OK;
}

Error fen_parse_color_to_move(const char fen[], Color *color) {
	if(fen == NULL || color == NULL) {
		return ERR_NULL_PTR;
	}

	if(strlen(fen) != 1) {
		return ERR_INVALID_INPUT;
	}
	switch(fen[0]) {
	case 'b': *color = BLACK; return OK;
	case 'w': *color = WHITE; return OK;
	default: return ERR_INVALID_INPUT;
	}
}

Error fen_parse_castling_rights(const char fen[], bool oo[2], bool ooo[2]) {
	if(fen == NULL || oo == NULL || ooo == NULL) {
		return ERR_NULL_PTR;
	}

	if(strlen(fen) < 1 || strlen(fen) > 4) {
		return ERR_INVALID_INPUT;
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
		return OK;
	}
	while(fen[pos] != 0) {
		switch(fen[pos]) {
		case 'K': oo[WHITE] = true; break;
		case 'Q': ooo[WHITE] = true; break;
		case 'k': oo[BLACK] = true; break;
		case 'q': ooo[BLACK] = true; break;
		default: return ERR_INVALID_INPUT;
		}
		pos++;
	}
	return OK;
}

Error fen_square_to_index(const char fensq[static 2], Square *sq) {
	if(fensq == NULL || sq == NULL) {
		return ERR_NULL_PTR;
	}

	*sq = OTB;
	if(strlen(fensq) != 2) {
		return ERR_INVALID_INPUT;
	}
	char r = fensq[0];
	if(r < 'a' || r > 'h') {
		return ERR_INVALID_INPUT;
	}
	char f = fensq[1];
	if(f < '1' || f > '8') {
		return ERR_INVALID_INPUT;
	}
	char rank = r - 'a';
	char file = f - '1';

	*sq = rank + file * 8;
	return OK;
}

Error fen_parse_ep_square(const char fen[], Square *sq) {
	if(fen == NULL || sq == NULL) {
		return ERR_NULL_PTR;
	}

	*sq = OTB;
	if(strlen(fen) == 1 && fen[0] == '-') {
		return OK;
	}

	if(strlen(fen) != 2) {
		return ERR_INVALID_INPUT;
	}

	// Only rank 3 and 6 can have valid ep squares.
	if(fen[1] != '3' && fen[1] != '6') {
		return ERR_INVALID_INPUT;
	}

	return fen_square_to_index(fen, sq);
}

Error fen_parse_half_move_clock(const char fen[], uint16_t *num) {
	if(fen == NULL || num == NULL) {
		return ERR_NULL_PTR;
	}

	// Half move counter can be 0.
	char *		  endptr = NULL;
	unsigned long ul	 = strtoul(fen, &endptr, 10);
	if(endptr != NULL && *endptr != '\0') {
		return ERR_INVALID_INPUT;
	}
	*num = (uint16_t)ul;
	return OK;
}

Error fen_parse_move_number(const char fen[], uint16_t *num) {
	if(fen == NULL || num == NULL) {
		return ERR_NULL_PTR;
	}

	// TODO: maximum game length?
	Error error = fen_parse_half_move_clock(fen, num);
	// Moves start at 1.
	if(error || *num == 0) {
		return ERR_INVALID_INPUT;
	}
	return OK;
}
