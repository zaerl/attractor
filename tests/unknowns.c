/**
 * The Attractor Unit Test library
 *
 * This file is distributed under the MIT License. See LICENSE for details.
 */

#include "../attractor.h"
#include <stdio.h>

typedef struct unknown_struct {
    int unknown;
} unknown_struct;

int assert_unknown(void* result, void* expected, att_op op, const char *description) {
    int equal = ((unknown_struct*)result)->unknown == ((unknown_struct*)expected)->unknown;

    return op == ATT_OP_NE ? !equal : equal;
}

void *test_unknowns(void *arg) {
    typedef int num;
    num a = 1;
    num b = 1;
    ATT_ASSERT(a, b, "Unknown typedef int num a = b");

    typedef float float_num;
    float_num c = 1.0;
    float_num d = 1.0;
    ATT_ASSERT(c, d, "Unknown typedef float num c = d");

    unknown_struct sa;
    unknown_struct sb;

    ATT_ASSERT(&sa, &sa, "Unknown struct sa = sa");

    sa.unknown = 1;
    sb.unknown = 1;

    att_set_generic_callback(&assert_unknown);
    ATT_ASSERT(&sa, &sb, "Unknown struct sa = sb");

    sb.unknown = 2;
    ATT_ASSERT_NE(&sa, &sb, "Unknown struct sa != sb via callback");
    att_set_generic_callback(NULL);

    return NULL;
}
