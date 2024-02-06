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
