#ifndef PERFT_TEST_H
#define PERFT_TEST_H

#include <stdint.h>

#include "../src/perft.h"

#define MUNIT_ENABLE_ASSERT_ALIASES
#include "../munit/munit.h"

typedef struct PerftTestCase PerftTestCase;
struct PerftTestCase {
	const char *       name;
	const char *       fen;
	const unsigned int depth;
	const uint64_t     nodes;
};

MunitResult test_perft_validate(const MunitParameter params[], void * data);

extern MunitTest test_perft_suite[];

#endif
