#include "../attractor.h"
#include <stdio.h>

char* string_0(char* result) {
    return result;
}

void *test_strings(void *arg) {
    ATT_ASSERT("foo", "foo", "\"foo\" = \"foo\"")
    ATT_ASSERT(string_0("bar"), "bar", "Fn \"bar\" = \"bar\"")

    char* str = "baz";
    ATT_ASSERT(str, "baz", "Var \"baz\" = \"baz\"")

    const char* cstr = "qux";
    ATT_ASSERT(cstr, "qux", "Const \"qux\" = \"qux\"")
    ATT_ASSERT((char*)cstr, "qux", "Const \"qux\" to not const = \"qux\"")

    ATT_ASSERT((char*)NULL, NULL, "NULL")

    return NULL;
}
