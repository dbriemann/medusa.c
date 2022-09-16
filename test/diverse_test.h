#ifndef DIVERSE_TEST_H
#define DIVERSE_TEST_H

#define MUNIT_ENABLE_ASSERT_ALIASES
#include "../munit/munit.h"

MunitResult test_piecelist__add(const MunitParameter[], void *);

extern MunitTest test_diverse_suite[];

#endif
