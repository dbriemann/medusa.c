#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "base.h"
#include "board.h"
#include "errors.h"

int main() {
	Board out_board;

	// Add white rook at h5 (0x47)
	Board__add_piece(&out_board, 0x47, WROOK);
	printf("%d\n", out_board.squares[0x47]);
	printf("%zu\n", out_board.rooks_size[WHITE]);
	printf("%d\n", out_board.rooks[WHITE][0]);

	return EXIT_SUCCESS;
}
