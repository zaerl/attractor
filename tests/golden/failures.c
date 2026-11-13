/**
 * The Attractor Unit Test library
 *
 * This file is distributed under the MIT License. See LICENSE for details.
 *
 * Every assertion here fails on purpose. The output is compared with failures.expected by
 * `make test-golden`, so that the failure messages are tested too.
 */

#define ATT_IMPLEMENTATION
#include "../../attractor.h"

ATT_TEST(first_failure) {
    ATT_ASSERT(1, 2, "first case fails");
}

ATT_TEST(second_failure) {
    ATT_ASSERT_TRUE(0, "second case fails");
}

ATT_TEST(passing) {
    ATT_ASSERT(1, 1, "passing case");
}

static void values(void) {
    char negative = (char)0x80;
    char newline = '\n';
    int two = 2;
    int minus = -1;
    unsigned int max = 4294967295u;
    const char *null_string = NULL;
    unsigned char buffer_1[3] = { 1, 2, 3 };
    unsigned char buffer_2[3] = { 1, 9, 3 };
    bool flag = 1;

    ATT_ASSERT(negative, 'a', "negative char");
    ATT_ASSERT(newline, 'a', "control char");
    ATT_ASSERT(two, 3, "int");
    ATT_ASSERT_GE(two, 3, "int with operator");
    ATT_ASSERT(two, 2.5, "int against double");
    ATT_ASSERT(max, -1, "unsigned against negative");
    ATT_ASSERT_GT(minus, 1u, "negative against unsigned");
    ATT_ASSERT(1.5, 2.5, "double");
    ATT_ASSERT("foo", "bar", "strings");
    ATT_ASSERT(null_string, "bar", "NULL string");
    ATT_ASSERT("foo", null_string, "NULL expected string");
    ATT_ASSERT_LT(flag, flag, "unsupported operator");
    ATT_ASSERT_TRUE(two == 3, "condition");
    ATT_ASSERT_MEM(buffer_1, buffer_2, sizeof(buffer_1), "memory");
    ATT_ASSERT(1.5L, 2.5L, "long double");
}

int main(void) {
    values();

    // The recap of a run lists only the cases that failed in that run.
    att_run_tests("first_");
    att_run_tests("second_");
    att_run_tests("passing");

    // A filter that matches nothing is a failure.
    att_run_tests("no_such_case");

    att_set_tap(1);
    values();
    ATT_ASSERT(1, 2, "has a # hash\nand a line break");
    att_run_tests("no_such_case");

    return att_report();
}
