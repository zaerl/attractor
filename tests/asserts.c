/**
 * The Attractor Unit Test library
 *
 * This file is distributed under the MIT License. See LICENSE for details.
 */

#include "../attractor.h"

#include <stdio.h>
#include <string.h>

void *test_asserts(void *arg) {
    unsigned char buffer_1[4] = { 0x01, 0x02, 0x03, 0x04 };
    unsigned char buffer_2[4] = { 0x01, 0x02, 0x03, 0x04 };

    ATT_ASSERT_TRUE(1, "1 is true");
    ATT_ASSERT_TRUE(1 == 1, "1 == 1 is true");
    ATT_ASSERT_TRUE(strcmp("a", "b") < 0, "\"a\" < \"b\"");

    ATT_ASSERT_MEM(buffer_1, buffer_2, sizeof(buffer_1), "Equal buffers");
    ATT_ASSERT_MEM(buffer_1, buffer_1, sizeof(buffer_1), "Same buffer");
    ATT_ASSERT_MEM(NULL, NULL, 4, "Two NULL buffers");
    ATT_ASSERT_MEM("abc", "abc", 4, "Equal strings as memory");

    // Assertion macros are expressions and return non-zero on success.
    unsigned int result = ATT_ASSERT(1, 1, "ATT_ASSERT is an expression");
    ATT_ASSERT_TRUE(result != 0, "ATT_ASSERT returns non-zero on success");

    if(!ATT_ASSERT_MEM(buffer_1, buffer_2, sizeof(buffer_1), "ATT_ASSERT_MEM is an expression")) {
        ATT_ASSERT_TRUE(0, "Unreachable");
    }

    return NULL;
}
