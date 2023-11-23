#ifndef IOCONTROLLER_H
#define IOCONTROLLER_H

#include <stdbool.h>
#include <stdint.h>

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

void IOController__loop(IOController * controller);
void IOController__process_uci_commands(IOController * controller);

#endif
