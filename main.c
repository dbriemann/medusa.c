#include <stdio.h>
#include <stdlib.h>

#include "base.h"
#include "fen.h"

int main() {
    Piece squares[64];

    bool ok = parse_fen_pieces("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR",
                               squares);

    for(int i = 0; i < 64; i++) {
        if(i % 8 == 0) {
            printf("\n");
        }
        printf("%d ", squares[i]);
    }
    printf("\n");

    return EXIT_SUCCESS;
}
