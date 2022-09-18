#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "base.h"
#include "board.h"
#include "errors.h"

int main() {
	const char *input_fen = "1nbqkb1r/1ppppp1p/r4n2/p5pP/8/2N4R/PPPPPPP1/R1BQKBN1 w Qk g6 0 5";
	Board		ob;

	Error error = Board__set_fen(&ob, input_fen);

	return EXIT_SUCCESS;
}
