#include <stdio.h>
#include <stdlib.h>

#include "../src/base.h"
#include "../src/generate.h"

int main() {
	Piece square_diffs[240];
	Direction diff_dirs[240];

	generateAttackMaps(square_diffs, diff_dirs);

	printf("\nsquare_diffs:\n");
	printf("{");
	for(size_t i = 0; i < 240; i++) {
		if(i % 20 == 0) {
			printf("\n");
		}
		printf("%d, ", (int)square_diffs[i]);
	}
	printf("\n}\n");


	printf("\ndiff_dirs\n");
	printf("{");
	for(size_t i = 0; i < 240; i++) {
		if(i % 20 == 0) {
			printf("\n");
		}
		printf("%d, ", (int)diff_dirs[i]);
	}
	printf("\n}\n");

	return EXIT_SUCCESS;
}
