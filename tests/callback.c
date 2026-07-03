/**
 * The Attractor Unit Test library
 *
 * This file is distributed under the MIT License. See LICENSE for details.
 */

#include "../attractor.h"
#include <stddef.h>
#include <stdio.h>

static int callback_called = 0;
static const char *last_expression = NULL;

int callback(int test, const char *description, const char *expression, const char *file,
    unsigned int line) {
    callback_called = 1;
    last_expression = expression;

    return 0;
}

void *test_callback(void *arg) {
    att_set_test_callback(callback);

    ATT_ASSERT(1, 1, "Callback was set")
    ATT_ASSERT(callback_called, 1, "Callback was called")
    ATT_ASSERT(last_expression, "callback_called", "\"callback_called\" = \"callback_called\"")

    att_set_test_callback(NULL);

    return NULL;
}
