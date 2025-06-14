#include "../attractor.h"
#include <stdio.h>

struct unknown_struct {
    int unknown;
};

void *test_unknowns(void *arg) {
    typedef int num;
    num a = 1;
    num b = 1;
    ATT_ASSERT(a, b, "Unknown typedef int num a = b")

    typedef float float_num;
    float_num c = 1.0;
    float_num d = 1.0;
    ATT_ASSERT(c, d, "Unknown typedef float num c = d")

    struct unknown_struct sa;

    ATT_ASSERT(&sa, &sa, "Unknown struct sa = sa")

    return NULL;
}
