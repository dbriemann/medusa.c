#ifndef ENGINE_H
#define ENGINE_H

#include <stdbool.h>
#include <stdint.h>

#include "board.h"

enum IOProtocol {
	IDLE,
	UCI,
};
typedef enum IOProtocol IOProtocol;

typedef struct IOController IOController;
struct IOController {
	IOProtocol protocol;
	bool       debug;
	char *     command;
};

typedef struct Engine Engine;
struct Engine {
	Board        board;
	IOController io_controller;
};

void Engine__io_loop(Engine * engine);
void Engine__process_uci_commands(Engine * engine);

#endif
