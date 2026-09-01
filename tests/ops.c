/**
 * The Attractor Unit Test library
 *
 * This file is distributed under the MIT License. See LICENSE for details.
 */

#include "../attractor.h"

#include <stdio.h>

ATT_TEST(ops) {
    ATT_ASSERT_NE(1, 2, "1 != 2");
    ATT_ASSERT_LT(1, 2, "1 < 2");
    ATT_ASSERT_LE(2, 2, "2 <= 2");
    ATT_ASSERT_GT(2, 1, "2 > 1");
    ATT_ASSERT_GE(2, 2, "2 >= 2");

    ATT_ASSERT_NE(1u, 2u, "1u != 2u");
    ATT_ASSERT_LT(1ll, 2ll, "1ll < 2ll");
    ATT_ASSERT_GT((unsigned char)2, (unsigned char)1, "(unsigned char)2 > 1");
    ATT_ASSERT_LT('a', 'b', "'a' < 'b'");

    // Strings compare lexicographically via strcmp
    ATT_ASSERT_NE("abc", "abd", "\"abc\" != \"abd\"");
    ATT_ASSERT_LT("abc", "abd", "\"abc\" < \"abd\"");
    ATT_ASSERT_GT("abd", "abc", "\"abd\" > \"abc\"");
    ATT_ASSERT_GE("abc", "abc", "\"abc\" >= \"abc\"");

    // Floats: EQ/NE honor the epsilon, relational operators compare plainly
    ATT_ASSERT_NE(1.0, 2.0, "1.0 != 2.0");
    ATT_ASSERT_LT(1.0f, 2.0f, "1.0f < 2.0f");
    ATT_ASSERT_GE(2.0, 2.0, "2.0 >= 2.0");

    att_set_float_epsilon(0.5);
    ATT_ASSERT(1.0, 1.4, "1.0 == 1.4 within epsilon");
    ATT_ASSERT_NE(1.0, 1.6, "1.0 != 1.6 beyond epsilon");
    att_set_float_epsilon(0);

    // Pointers support NE
    int value = 0;
    int other = 0;
    void *p = &value;
    void *q = &other;
    ATT_ASSERT_NE(p, q, "p != q");

    // The operator macros are expressions too
    if(!ATT_ASSERT_LT(1, 2, "ATT_ASSERT_LT is an expression")) {
        ATT_ASSERT_TRUE(0, "Unreachable");
    }
}
