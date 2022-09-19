#ifndef DIVERSE_TEST_H
#define DIVERSE_TEST_H

#define MUNIT_ENABLE_ASSERT_ALIASES
#include "../munit/munit.h"

MunitResult test_piecelist__add(const MunitParameter params[], void *data);
MunitResult test_piecelist__del(const MunitParameter params[], void *data);
MunitResult test_piecelist__del_index(const MunitParameter params[], void *data);

extern MunitTest test_diverse_suite[];

#endif
