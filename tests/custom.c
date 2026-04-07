typedef struct custom_struct {
    int value;
} custom_struct;

#define ATT_CUSTOM_TYPES custom_struct*: att_assert_p_p,

#include "../attractor.h"
#include <stdio.h>

void *test_custom(void *arg) {
    custom_struct s;
    s.value = 1;

    ATT_ASSERT(&s, &s, "Custom struct pointer same address")

    return NULL;
}
