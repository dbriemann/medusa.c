#include "iocontroller.h"
#include "util.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char delim[] = " \t\n";

void IOController__loop(IOController * controller) {

	while (true) {
		char *  input      = NULL;
		size_t  bufsize    = 0;
		ssize_t characters = getline(&input, &bufsize, stdin);

		char * token = strtok(input, delim);
		if (token == NULL) {
			LOG(DEBUG, "no token in main loop", controller->debug);
		} else if (!strcmp("uci", token)) {
			printf("set mode uci\n");
			controller->protocol = UCI;
		} else if (!strcmp("quit", token)) {
			free(input);
			return;
		} else if (controller->protocol == UCI) {
			controller->command = token;
			IOController__process_uci_commands(controller);
		} else {
			// No other protocols/commands available.
			LOG(WARN, "unknown command", controller->debug);
		}

		// TODO: log this stuff
		printf("io controller state\n");
		printf("    mode: %d\n", controller->protocol);
		printf("    debug: %s\n", controller->debug ? "true" : "false");
		printf("command: %s, bufsize: %zu, chars read: %zu\n", token, bufsize, characters);

		free(input);
	}
}

void IOController__process_uci_commands(IOController * controller) {
	printf("uci command received: %s\n", controller->command);
	if (controller->command == NULL) {
		LOG(DEBUG, "no command in uci handler", controller->debug);
		return;
	} else if (!strcmp("debug", controller->command)) {
		char * token = strtok(NULL, delim);
		if (token == NULL) {
			LOG(DEBUG, "no token for debug command", controller->debug);
			return;
		} else if (!strcmp("on", token)) {
			controller->debug = true;
		} else if (!strcmp("off", token)) {
			controller->debug = false;
		} else {
			LOG(DEBUG, "unexpected token for debug command", controller->debug);
			return;
		}
	} else if (!strcmp("isready", controller->command)) {
	}
}
