#include "../attractor.h"
#include <stdio.h>

void *test_unknowns(void *arg) {
    typedef int num;
    num a = 1;
    num b = 1;
    ATT_ASSERT(a, b, "a = b")

    typedef float float_num;
    float_num c = 1.0;
    float_num d = 1.0;
    ATT_ASSERT(c, d, "c = d")

    return NULL;
}
