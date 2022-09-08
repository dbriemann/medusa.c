#ifndef MINBOARD_H
#define MINBOARD_H

#include <stdbool.h>
#include <stdint.h>

#include "base.h"

typedef struct MinBoard MinBoard;
struct MinBoard {
	Piece squares[64];
	Color color;
	bool castle_short[2];
	bool castle_long[2];
	Square ep_square;
	// TODO: should these be just (u) int? (promotion thingy)
	uint16_t half_moves;
	uint16_t move_num;
};

// Methods:
MinBoard MinBoard__create();

#endif
