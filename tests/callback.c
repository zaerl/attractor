#include "../attractor.h"
#include <stddef.h>

static int callback_called = 0;

int callback(int result, const char *description) {
    callback_called = 1;

    return 0;
}

void *test_callback(void *arg) {
    att_set_test_callback(callback);

    ATT_ASSERT(1, 1, "Callback was set")
    ATT_ASSERT(callback_called, 1, "Callback was called")

    att_set_test_callback(NULL);

    return NULL;
}
