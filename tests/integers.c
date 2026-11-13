/**
 * The Attractor Unit Test library
 *
 * This file is distributed under the MIT License. See LICENSE for details.
 */

#include "../attractor.h"

#include <stdio.h>

int int_0(int result) {
    return result;
}

ATT_TEST(integers) {
    // ATT_ASSERT("Zero", 1, ret_0, 1)
    ATT_ASSERT(1, 1, "1 = 1");
    ATT_ASSERT(1u, 1u, "1u = 1u");
    ATT_ASSERT(int_0(1), 1, "Fn 1 = 1");

    ATT_ASSERT((short)1, (short)1, "(short)1 = (short)1");
    ATT_ASSERT((unsigned short)1, (unsigned short)1, "(unsigned short)1 = (unsigned short)1");

    ATT_ASSERT(1l, 1l, "1l = 1l");
    ATT_ASSERT(1ul, 1ul, "1ul = 1ul");
    ATT_ASSERT(1ll, 1ll, "1ll = 1ll");
    ATT_ASSERT(1ull, 1ull, "1ull = 1ull");
}

// The expected value is never silently converted to the type of the tested one.
ATT_TEST(integers_mixed) {
    unsigned int max = 4294967295u;
    unsigned long long big = 18446744073709551615ull;
    int two = 2;
    int minus = -1;
    size_t length = 3;

    // Signed against unsigned compares the mathematical values.
    ATT_ASSERT_NE(max, -1, "UINT_MAX != -1");
    ATT_ASSERT_GT(max, -1, "UINT_MAX > -1");
    ATT_ASSERT_NE(big, -1ll, "ULLONG_MAX != -1ll");
    ATT_ASSERT_NE(minus, max, "-1 != UINT_MAX");
    ATT_ASSERT_LT(minus, 1u, "-1 < 1u");
    ATT_ASSERT_LE(minus, big, "-1 <= ULLONG_MAX");
    ATT_ASSERT(length, 3, "size_t 3 = int 3");
    ATT_ASSERT(two, 2u, "int 2 = 2u");
    ATT_ASSERT_GE(length, 3, "size_t 3 >= int 3");
    ATT_ASSERT_GT(big, 1, "ULLONG_MAX > 1");

    // An integer against a floating point value compares as floating point.
    ATT_ASSERT_NE(two, 2.9, "2 != 2.9");
    ATT_ASSERT_LT(two, 2.9, "2 < 2.9");
    ATT_ASSERT(two, 2.0, "2 = 2.0");
    ATT_ASSERT_NE(max, 0.5f, "UINT_MAX != 0.5f");
}
