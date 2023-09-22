#ifndef PERFT_TEST_H
#define PERFT_TEST_H

#include "../src/perft.h"

#define MUNIT_ENABLE_ASSERT_ALIASES
#include "../munit/munit.h"

typedef struct PerftTestCase PerftTestCase;
struct PerftTestCase {
	const char *name;
	const char *fen;
	const unsigned int depth;
	const PerftData target_pdata;
};

MunitResult test_perft_validate(const MunitParameter params[], void *data);

extern MunitTest test_perft_suite[];

#endif
