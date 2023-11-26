#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "board.h"
#include "engine.h"
#include "util.h"

const char spaces_delims[] = " \t\n";
const char newline_delim[] = "\n";

void Engine__io_loop(Engine * engine) {

	while (true) {
		char * input   = NULL;
		size_t bufsize = 0;
		// TODO: do we need the character count?
		__attribute__((unused)) const ssize_t characters = getline(&input, &bufsize, stdin);

		IOController * controller = &(engine->io_controller);

		char * token = strtok(input, spaces_delims);
		if (token == NULL) {
			LOG(DEBUG, "no token in main loop", controller->debug);
		} else if (!strcmp("uci", token)) {
			printf("id name Medusa\n");
			printf("id author David Linus Briemann\n");
			// TODO: send engine options.
			controller->protocol = UCI;
		} else if (!strcmp("quit", token)) {
			free(input);
			return;
		} else if (!strcmp("d", token)) {
			char * boardstr = NULL;
			Board__to_string(&(engine->board), &boardstr);
			printf("%s", boardstr);
			free(boardstr);
		} else if (controller->protocol == UCI) {
			controller->command = token;
			Engine__process_uci_commands(engine);
		} else {
			// No other protocols/commands available.
			LOG(WARN, "unknown command", controller->debug);
		}

		free(input);
	}
}

void Engine__process_uci_commands(Engine * engine) {
	IOController * controller = &(engine->io_controller);

	printf("uci command received: %s\n", controller->command);
	if (controller->command == NULL) {
		LOG(DEBUG, "no command in uci handler", controller->debug);
		return;
	} else if (!strcmp("debug", controller->command)) {
		char * token = strtok(NULL, spaces_delims);
		if (token == NULL) {
			LOG(DEBUG, "no token for debug command", controller->debug);
			return;
		} else if (!strcmp("on", token)) {
			controller->debug = true;
		} else if (!strcmp("off", token)) {
			controller->debug = false;
		} else {
			LOG(DEBUG, "unexpected token for debug command", controller->debug);
		}
		return;
	} else if (!strcmp("isready", controller->command)) {
		// TODO: wait for sync with engine before reply.
		printf("readyok\n");
		return;
	} else if (!strcmp("position", controller->command)) {
		char * token = strtok(NULL, spaces_delims);
		if (token == NULL) {
			LOG(DEBUG, "no token for position command", controller->debug);
		} else if (!strcmp("startpos", token)) {
			Board__set_starting_position(&(engine->board));
		} else if (!strcmp("fen", token)) {
			token     = strtok(NULL, newline_delim);
			Error err = Board__set_fen(&(engine->board), token);
			if (err != OK) {
				LOG(ERROR, "FEN code invalid", controller->debug);
			}
		} else if (!strcmp("moves", token)) {
			// TODO:
		}

		return;
	}
}
