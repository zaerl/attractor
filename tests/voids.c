/**
 * The Attractor Unit Test library
 *
 * This file is distributed under the MIT License. See LICENSE for details.
 */

 #include "../attractor.h"
#include <stdio.h>

ATT_TEST(voids) {
    ATT_ASSERT((void*)0, (void*)0, "(void*)0 = (void*)0");
    ATT_ASSERT((void*)1, (void*)1, "(void*)1 = (void*)1");

    const void *cv = "const";
    ATT_ASSERT(cv, cv, "const void* = const void*");
    ATT_ASSERT_NE(cv, NULL, "const void* != NULL");
}
