#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "../src/board.h"
#include "../src/errors.h"
#include "../src/iocontroller.h"
#include "../src/perft.h"

int main(void) {
	IOController controller = {
		.protocol = IDLE,
	};

	IOController__loop(&controller);

	return EXIT_SUCCESS;
}
