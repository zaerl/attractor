#include "../attractor.h"
#include <stdio.h>

float float_0(float result) {
    return result;
}

void *test_floats(void *arg) {
    ATT_ASSERT(1.0, 1.0, "1.0 = 1.0")
    ATT_ASSERT(float_0(1.0), 1.0, "Fn 1.0 = 1.0")
    ATT_ASSERT((double)1.0, (double)1.0, "(double)1.0 = (double)1.0")

    double a = 1.0;
    ATT_ASSERT(a, 1.0, "a = 1.0")

    return NULL;
}

void *test_floats_epsilon(void *arg) {
    double sum = 0.1 + 0.2;

    ATT_ASSERT(sum == 0.3, 0, "0.1 + 0.2 differs from 0.3 exactly")
    ATT_ASSERT(att_get_float_epsilon() == 0, 1, "epsilon defaults to 0")

    // Widen the tolerance at runtime.
    att_set_float_epsilon(1e-6);

    ATT_ASSERT(att_get_float_epsilon() > 0, 1, "epsilon updated at runtime")
    ATT_ASSERT(sum, 0.3, "0.1 + 0.2 == 0.3 within runtime epsilon")

    att_set_float_epsilon(0);
    ATT_ASSERT(att_get_float_epsilon() == 0, 1, "epsilon reset to 0")

    return NULL;
}
