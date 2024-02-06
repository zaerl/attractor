#include "../attractor.h"
#include <stdio.h>

int int_0(int result) {
    return result;
}

void *test_integers(void *arg) {
    // ATT_ASSERT("Zero", 1, ret_0, 1)
    ATT_ASSERT(1, 1, "1 = 1")
    ATT_ASSERT(1u, 1u, "1u = 1u")
    ATT_ASSERT(int_0(1), 1, "Fn 1 = 1")

    ATT_ASSERT((short)1, (short)1, "(short)1 = (short)1")
    ATT_ASSERT((unsigned short)1, (unsigned short)1, "(unsigned short)1 = (unsigned short)1")

    ATT_ASSERT(1l, 1l, "1l = 1l")
    ATT_ASSERT(1ul, 1ul, "1ul = 1ul")
    ATT_ASSERT(1ll, 1ll, "1ll = 1ll")
    ATT_ASSERT(1ll, 1ll, "1ull = 1ull")

    return NULL;
}
