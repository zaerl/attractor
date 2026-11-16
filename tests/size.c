/**
 * The Attractor Unit Test library
 *
 * This file is distributed under the MIT License. See LICENSE for details.
 */

#include "../attractor.h"

ATT_TEST(size) {
    size_t result_1 = sizeof(int);
    size_t result_2 = sizeof(int);

    ATT_ASSERT(result_1, result_2, "ATT_ASSERT works with size_t");
}
