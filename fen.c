#include <string.h>

#include "minboard.h"

bool parse_fen(char fen[static 100], MinBoard *mb) {
    char group_delim[] = " "; // Spaces splits fen into 6 groups.

    char *strpos = strtok(fen, group_delim);
    if (strpos == 0) {
        return false;
    }
    // Group 1 : pieces

    // strpos = strtok(0, group_delim);
}

bool parse_fen_pieces(char fen[static 15], Piece* squares[64]) {
    char pos = 0;
    // Since FEN has different rank order than MinBoard,
    // boardpos starts at field a8: see MinBoard doc.
    size_t boardpos = 56;
    while (fen[pos] != 0) {
        char token = fen[pos];
        // Check if strpos hold 1-8 (number of continues empty fields).
        if (token >= '1' && token <= '8') {
            int amount = token - 48;
            for (size_t i = 0; i < amount; i++) {
                squares[boardpos] = EMPTY;
            }
        } else {
            // Else detect the piece type.
            switch (token) {
            // White pieces:
            case 'P':
                squares[boardpos] = WPAWN;
                break;
            case 'N':
                squares[boardpos] = WKNIGHT;
                break;
            case 'B':
                squares[boardpos] = WBISHOP;
                break;
            case 'R':
                squares[boardpos] = WROOK;
                break;
            case 'Q':
                squares[boardpos] = WQUEEN;
                break;
            case 'K':
                squares[boardpos] = WKING;
                break;
            // Black pieces:
            case 'p':
                squares[boardpos] = BPAWN;
                break;
            case 'n':
                squares[boardpos] = BKNIGHT;
                break;
            case 'b':
                squares[boardpos] = BBISHOP;
                break;
            case 'r':
                squares[boardpos] = BROOK;
                break;
            case 'q':
                squares[boardpos] = BQUEEN;
                break;
            case 'k':
                squares[boardpos] = BKING;
                break;
            // Nex fen group.
            case '/':
                // This is handled after the switch case so we can detect token error in 'default'.
                break;
            // Unknown character (invalid FEN).
            default:
                // Fail: return false.
                return false;
            }
        }

        if(token == '/') {
            // Check if FEN completed the board.
            if(boardpos != 7) {
                // Only if the boardpos ends at 7 (h1) we assigned all fields with a value.
                return false;
            }
            // This is a success.. advance to next fen section.
            break; // while loop
        }

        // Else we continue to next token in pieces group.
        boardpos++;
        // If we reach the end of a rank we need to wrap around in towards the
        // "bottom". E.g. field h8 wraps to a7, h7 to a6, etc...
        if (boardpos % 8 == 0) {
            boardpos -= 8;
        }
    }
}
