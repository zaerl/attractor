/**
 * The Attractor Unit Test library
 *
 * This file is distributed under the MIT License. See LICENSE for details.
 */

#include "../attractor.h"
#include <stddef.h>

ATT_TEST(att_case_alpha) {
    ATT_ASSERT(1, 1, "case alpha runs");
}

ATT_TEST(att_case_beta) {
    ATT_ASSERT("beta", "beta", "case beta runs");
}

ATT_TEST(other_case_gamma) {
    ATT_ASSERT(2, 2, "case gamma runs");
}

static unsigned int start_calls = 0;
static const char *last_started = NULL;

static void test_started(const char *name) {
    ++start_calls;
    last_started = name;
}

ATT_TEST(cases) {
    unsigned int before;
    int ret;

#ifdef ATT_TEST_MANUAL
    // No constructor support on this compiler: register the cases by hand.
    att_register_test("att_case_alpha", att_test_fn_att_case_alpha);
    att_register_test("att_case_beta", att_test_fn_att_case_beta);
    att_register_test("other_case_gamma", att_test_fn_other_case_gamma);
#endif

    ATT_ASSERT_GE(att_get_test_count(), 3u, "three cases registered");

    // Test names are enumerable, e.g. to implement a --list option.
    ATT_ASSERT_NE(att_get_test_name(0), NULL, "first test name is enumerable");
    ATT_ASSERT(att_get_test_name(att_get_test_count()), NULL, "out of range index gives NULL");

    // A filter runs only the matching cases; each case here has one assertion.
    before = att_get_total_tests();
    ret = att_run_tests("att_case_");
    ATT_ASSERT(att_get_total_tests() - before, 2u, "filter runs the two matching cases");
    ATT_ASSERT(ret, 0, "filtered cases pass");

    before = att_get_total_tests();
    ret = att_run_tests("other_case_");
    ATT_ASSERT(att_get_total_tests() - before, 1u, "filter runs the one matching case");
    ATT_ASSERT(ret, 0, "filtered case passes");

    // The start callback runs before each test case with its name.
    att_set_test_start_callback(test_started);
    ret = att_run_tests("other_case_");
    att_set_test_start_callback(NULL);

    ATT_ASSERT(start_calls, 1u, "start callback called once");
    ATT_ASSERT(last_started, "other_case_gamma", "start callback receives the test name");
    ATT_ASSERT(ret, 0, "run with start callback passes");
}
