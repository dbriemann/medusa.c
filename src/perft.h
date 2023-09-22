#ifndef PERFT_H
#define PERFT_H

#include <bits/stdint-uintn.h>
#include <stdint.h>

#include "board.h"

typedef struct PerftData PerftData;
struct PerftData {
	uint64_t nodes;
	uint64_t captures;
	uint64_t eps;
	uint64_t promos;
	uint64_t checks;
	uint64_t double_checks;
	uint64_t mates;
	uint64_t castles;
};

void perft__validate(Board b, unsigned int depth, PerftData *pdata);

#endif
