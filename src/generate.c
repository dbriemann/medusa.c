#include <stdio.h>

#include "generate.h"

void generateAttackMaps(Piece square_diffs[static 1], Direction diff_dirs[static 1]) {
	Direction diff_dir = 0;

	for(size_t f = 0; f < 64; f++) {
		Square from = LOOKUP_0x88[f];

		for(size_t t = 0; t < 64; t++) {
			Square to = LOOKUP_0x88[t];

			if(from == to) {
				continue;
			}

			Square diff	 = (Square)(0x77 + (Direction)from - (Direction)to);
			Piece  pbits = 0; // Resulting bitset.
			
			// Test if diff is a possible target for all types of pieces.
			
			// Kings
			for(size_t x = 0; x < KING_DIRS_LEN; x++) {
				Direction dir = KING_DIRS[x];
				Square target = (Square)((Direction)from + dir);

				if(target == to) {
					pbits |= KING;
					break;
				}
			}

			// Diagonal sliders
			for(size_t x = 0; x < DIAGONAL_DIRS_LEN; x++) {	
				Direction dir = DIAGONAL_DIRS[x];
				Direction steps = 1;
				while(true) {
					Square target = (Square)((Direction)from + steps*dir);
					if(!on_board(target)) {
						// We slided off the board -> next dir
						break;
					}

					if(target == to) {
						diff_dir = dir;
						// Can be reached by diagonal sliders.
						pbits |= BISHOP | QUEEN;
						// The goto acts as a double break. Skipping other pieces.
						goto INSPECTION_FINISHED;
					}
					steps++;
				}
			}

			// Orthogonal sliders
			for(size_t x = 0; x < ORTHOGONAL_DIRS_LEN; x++) {	
				Direction dir = ORTHOGONAL_DIRS[x];
				Direction steps = 1;
				while(true) {
					Square target = (Square)((Direction)from + steps*dir);
					if(!on_board(target)) {
						// We slided off the board -> next dir
						break;
					}

					if(target == to) {
						diff_dir = dir;
						// Can be reached by diagonal sliders.
						pbits |= ROOK | QUEEN;
						// The goto acts as a double break. Skipping other pieces.
						goto INSPECTION_FINISHED;
					}
					steps++;
				}
			}


			// Knights
			for(size_t x = 0; x < KNIGHT_DIRS_LEN; x++) {
				Direction dir = KNIGHT_DIRS[x];
				Square target = (Square)((Direction)from + dir);

				if(target == to) {
					diff_dir = dir;
					pbits |= KNIGHT;
					break;
				}
			}

		INSPECTION_FINISHED:
			diff_dirs[diff] = diff_dir;
			square_diffs[diff] = pbits;
		}
	}
}

