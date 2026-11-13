/**
 * The Attractor Unit Test library
 *
 * This file is distributed under the MIT License. See LICENSE for details.
 */

#include "../attractor.h"
#include <float.h>
#include <math.h>
#include <stdio.h>

float float_0(float result) {
    return result;
}

ATT_TEST(floats) {
    ATT_ASSERT(1.0, 1.0, "1.0 = 1.0");
    ATT_ASSERT(float_0(1.0), 1.0, "Fn 1.0 = 1.0");
    ATT_ASSERT((double)1.0, (double)1.0, "(double)1.0 = (double)1.0");

    double a = 1.0;
    ATT_ASSERT(a, 1.0, "a = 1.0");
}

ATT_TEST(floats_epsilon) {
    double sum = 0.1 + 0.2;

    ATT_ASSERT(sum == 0.3, 0, "0.1 + 0.2 differs from 0.3 exactly");
    ATT_ASSERT(att_get_float_epsilon() == 0, 1, "epsilon defaults to 0");

    // Widen the tolerance at runtime.
    att_set_float_epsilon(1e-6);

    ATT_ASSERT(att_get_float_epsilon() > 0, 1, "epsilon updated at runtime");
    ATT_ASSERT(sum, 0.3, "0.1 + 0.2 == 0.3 within runtime epsilon");

    att_set_float_epsilon(0);
    ATT_ASSERT(att_get_float_epsilon() == 0, 1, "epsilon reset to 0");
}

ATT_TEST(floats_double_epsilon) {
    // The double accessors read and write the same tolerance as the long double ones.
    att_set_double_epsilon(0.25);
    ATT_ASSERT(att_get_double_epsilon(), 0.25, "double epsilon is set");
    ATT_ASSERT_TRUE(att_get_float_epsilon() == 0.25L, "long double epsilon follows");
    ATT_ASSERT(1.0, 1.2, "1.0 = 1.2 within 0.25");
    att_set_double_epsilon(0);
    ATT_ASSERT_NE(1.0, 1.2, "1.0 != 1.2 exactly");
}

ATT_TEST(floats_edge) {
    double nan_v = NAN;
    double inf_v = INFINITY;

    ATT_ASSERT_NE(nan_v, nan_v, "NaN != NaN");
    ATT_ASSERT_NE(nan_v, 1.0, "NaN != 1.0");

    ATT_ASSERT(inf_v, inf_v, "inf = inf");
    ATT_ASSERT_NE(-inf_v, inf_v, "-inf != inf");
    ATT_ASSERT_NE(inf_v, DBL_MAX, "inf != DBL_MAX");

    ATT_ASSERT(0.0, -0.0, "0.0 = -0.0");

    att_set_float_epsilon(1e9);
    ATT_ASSERT_NE(nan_v, nan_v, "NaN != NaN with epsilon");
    ATT_ASSERT(inf_v, inf_v, "inf = inf with epsilon");
    ATT_ASSERT_NE(inf_v, 1.0, "inf != 1.0 with epsilon");
    ATT_ASSERT_NE(DBL_MAX, -DBL_MAX, "DBL_MAX != -DBL_MAX with epsilon");
    att_set_float_epsilon(0);

    att_set_float_epsilon(0.5);
    ATT_ASSERT(1.0, 1.5, "diff of exactly epsilon is equal");
    ATT_ASSERT_NE(1.0, 1.75, "diff beyond epsilon is not equal");
    att_set_float_epsilon(0);

    float nan_f = NAN;
    float inf_f = INFINITY;

    ATT_ASSERT_NE(nan_f, nan_f, "float NaN != NaN");
    ATT_ASSERT(inf_f, inf_f, "float inf = inf");

    long double nan_l = NAN;
    long double inf_l = INFINITY;

    ATT_ASSERT_NE(nan_l, nan_l, "long double NaN != NaN");
    ATT_ASSERT(inf_l, inf_l, "long double inf = inf");
}
