#ifndef PERFT_H
#define PERFT_H

#include <stdint.h>

#include "board.h"

// typedef struct PerftData PerftData;
// struct PerftData {
// 	int64_t nodes;
// 	int64_t captures;
// 	int64_t eps;
// 	int64_t promos;
// 	int64_t checks;
// 	int64_t double_checks;
// 	int64_t mates;
// 	int64_t castles;
// };

// void perft_analyze(Board b, unsigned int max_depth, PerftData *pdata);
// void perft_validate(Board b, unsigned int depth, PerftData *pdata, bool debug);
uint64_t perft(Board b, unsigned int depth);

#endif
