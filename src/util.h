#ifndef UTIL_H
#define UTIL_H

#include <stdbool.h>
#include <stdio.h>

enum LOG_LEVEL {
	INFO,
	DEBUG,
	WARN,
	ERROR,
};
typedef enum LOG_LEVEL LOG_LEVEL;

static const char * LOG_LEVEL_NAMES[] = {
	"INFO",
	"DEBUG",
	"WARN",
	"ERROR",
};

static inline void LOG(const LOG_LEVEL level, const char msg[], bool debug) {
	if (debug || level >= WARN) {
		printf("LOG[%s]: %s\n", LOG_LEVEL_NAMES[level], msg);
	}
}

#endif
