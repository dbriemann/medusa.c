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
    return false;
}

bool parse_fen_pieces(char fen[static 15], Piece squares[64]) {
    char pos = 0;
    size_t boardpos = 0;
    while (fen[pos] != 0) {
        char token = fen[pos];
        // Check if strpos hold 1-8 (number of continues empty fields).
        if (token >= '1' && token <= '8') {
            int empty_fields = token - '0';
            for (size_t i = 0; i < empty_fields; i++) {
                squares[boardpos] = EMPTY;
                boardpos++;
            }
            pos++;
            continue;
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
            case '/': // Indicates next rank.
                // We can ignore the / because the FEN string should fill the ranks correctly.
                break;
            // Unknown character (invalid FEN).
            default:
                // Fail: return false.
                return false;
            }
        }

        // Check if FEN completed the board.
        if(boardpos == 63) {
            return true;
        }

        pos++;

        if(token == '/') {
            continue;
        }

        boardpos++;
    }

    // TODO: mirror ranks to have layout that fits engine board layout.

    return true;
}
