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
		// debug [ on | off ]
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
		// isready
		// TODO: wait for sync with engine before reply.
		Board__set_starting_position(&(engine->board));
		printf("readyok\n");
		return;
	} else if (!strcmp("position", controller->command)) {
		// position [fen <fenstring> | startpos ]  moves <move1> .... <movei>
		char * token = strtok(NULL, spaces_delims);
		if (token == NULL) {
			LOG(DEBUG, "no token for position command", controller->debug);
		} else {
			size_t len        = strlen(token) + 1;
			char * subcommand = calloc(len, sizeof(char));
			if (!subcommand) {
				LOG(ERROR, "out of memory, cannot allocate", controller->debug);
				return;
			}
			memcpy(subcommand, token, len);

			if (!strcmp("startpos", subcommand)) {
				Board__set_starting_position(&(engine->board));
				token = strtok(NULL, newline_delim);
			} else if (!strcmp("fen", subcommand)) {
				token = strtok(NULL, newline_delim);
				// TODO: currently extra string after FEN is ignored.
				// It would be cleaner to cut this off and only pass the FEN to the set_fen function.
				Error err = Board__set_fen(&(engine->board), token);
				if (err != OK) {
					LOG(ERROR, "FEN code invalid", controller->debug);
				}
			}

			free(subcommand);
		}

		// Test if there are moves after the subcommand.
		printf("TOKEN: %s\n", token);
		// char * moves = NULL;
		// if (!strcmp("moves", token)) {
		// 	moves = strtok(NULL, newline_delim);
		// }
		// if (!moves) {
		// 	moves = strstr(token, "moves");
		// 	if (moves) {
		// 		moves += 5; // skip after keyword.
		// 	}
		// }
		// if (moves) {
		// 	moves = strtok(moves, spaces_delims);
		// 	while (moves) {
		// 		printf("move: %s\n", moves);
		// 		moves = strtok(NULL, spaces_delims);
		// 	}
		// }

		return;
	}
}
