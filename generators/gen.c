#include <stdio.h>
#include <stdlib.h>

#include "../src/base.h"
#include "../src/generate.h"

int main() {
	Piece square_diffs[240];
	Direction diff_dirs[240];

	generateAttackMaps(square_diffs, diff_dirs);

	printf("\nsquare_diffs:\n");
	printf("{\n");
	for(size_t i = 1; i <= 240; i++) {
		printf("%d, ", (int)square_diffs[i]);
		if(i % 10 == 0) {
			printf("\n");
		}
	}
	printf("}\n");


	printf("\ndiff_dirs\n");
	printf("{\n");
	for(size_t i = 1; i <= 240; i++) {
		printf("%d, ", (int)diff_dirs[i]);
		if(i % 10 == 0) {
			printf("\n");
		}
	}
	printf("}\n");

	return EXIT_SUCCESS;
}
