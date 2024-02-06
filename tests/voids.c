#include "../attractor.h"
#include <stdio.h>

void *test_voids(void *arg) {
    ATT_ASSERT((void*)0, (void*)0, "(void*)0 = (void*)0")
    ATT_ASSERT((void*)1, (void*)1, "(void*)1 = (void*)1")

    return NULL;
}
