#include <stdio.h>
#include <assert.h>

#include "generate.h"

// TODO: somehow test this (currently this is only partially manually tested)
void generateAttackMaps(Piece square_diffs[], Direction diff_dirs[]) {
	assert(square_diffs != NULL);
	assert(diff_dirs != NULL);

	// Zero arrays.
	for(size_t i = 0; i < 240; i++) {
		square_diffs[i] = 0;
		diff_dirs[i]	= 0;
	}

	// Iterate and save all possible from-to square relations for each piece type.

	// Knights
	for(size_t x = 0; x < KNIGHT_DIRS_LEN; x++) {
		Direction dir  = KNIGHT_DIRS[x];
		uint8_t	  diff = (uint8_t)(0x77 + dir);

		square_diffs[diff] |= KNIGHT;
		diff_dirs[diff] = dir;
	}

	// Kings
	for(size_t x = 0; x < KING_DIRS_LEN; x++) {
		Direction dir  = KING_DIRS[x];
		uint8_t	  diff = (uint8_t)(0x77 + dir);

		square_diffs[diff] |= KING;
		diff_dirs[diff] = dir;
	}

	// Orthogonal sliders (rooks, queens)
	for(size_t x = 0; x < ORTHOGONAL_DIRS_LEN; x++) {
		Direction dir = ORTHOGONAL_DIRS[x];
		for(size_t step = 1; step < 8; step++) {
			Direction muldir = dir * (Direction)step;
			uint8_t	  diff	 = (uint8_t)(0x77 + muldir);
			square_diffs[diff] |= ROOK | QUEEN;
			diff_dirs[diff] = dir;
		}
	}

	// Diagonal sliders (bishops, queens)
	for(size_t x = 0; x < DIAGONAL_DIRS_LEN; x++) {
		Direction dir = DIAGONAL_DIRS[x];
		for(size_t step = 1; step < 8; step++) {
			Direction muldir = dir * (Direction)step;
			uint8_t	  diff	 = (uint8_t)(0x77 + muldir);
			square_diffs[diff] |= BISHOP | QUEEN;
			diff_dirs[diff] = dir;
		}
	}
}

