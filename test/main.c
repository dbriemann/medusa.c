#include <stdbool.h>

#define MUNIT_ENABLE_ASSERT_ALIASES
#include "../munit/munit.h"

#include "../fen.h"

static MunitResult test_parse_fen_pieces(const MunitParameter params[], void *data) {
    Piece squares[64];

    bool ok = parse_fen_pieces("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR",
                               squares);
    munit_assert_int(true, ==, ok);
    return MUNIT_OK;
}

// Test suite is array of tests.
static MunitTest test_suite_tests[] = {
    {(char *)"/fen/parse_fen_pieces", test_parse_fen_pieces, 0, 0,
     MUNIT_TEST_OPTION_NONE, 0},

    // End of tests.
    {NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}};

static const MunitSuite test_suite = {
    // String to prepend for all test cases in this suite.
    (char *)"",
    // Array of test suites.
    test_suite_tests,
    // Other test suites.
    NULL,
    // Number of test iterations.
    1,
    // Suite options.
    MUNIT_SUITE_OPTION_NONE};

int main(int argc, char *argv[MUNIT_ARRAY_PARAM(argc + 1)]) {
    /* Finally, we'll actually run our test suite!  That second argument
     * is the user_data parameter which will be passed either to the
     * test or (if provided) the fixture setup function. */
    return munit_suite_main(&test_suite, (void *)"µnit", argc, argv);
}
