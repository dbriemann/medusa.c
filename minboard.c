#include <stdbool.h>

#include "base.h"
#include "minboard.h"

MinBoard MinBoard__create_empty() {
	MinBoard mb = {
		.color = WHITE,
		.castle_short = {false, false},
		.castle_long = {false, false},
		.ep_square = OTB,
		.half_moves = 0,
		.move_num = 0,
	};
	for (int i = 0; i < 64; i++) {
		mb.squares[i] = EMPTY;
	}
	return mb;
}

