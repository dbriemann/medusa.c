#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "../src/board.h"
#include "../src/engine.h"
#include "../src/errors.h"
#include "../src/perft.h"

int main(void) {
	Engine engine = {
		.io_controller =
			{
							.protocol = IDLE,
							},
	};

	Engine__io_loop(&engine);

	return EXIT_SUCCESS;
}
