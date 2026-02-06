#include "../attractor.h"
#include <stdio.h>

void *test_strings(void *arg) {
    ATT_ASSERT("foo", "foo", "\"foo\" = \"foo\"")
    ATT_ASSERT((char*)"bar", "bar", "Fn \"bar\" = \"bar\"")

    const char* str = "baz";
    ATT_ASSERT(str, "baz", "Var \"baz\" = \"baz\"")

    const char* cstr = "qux";
    ATT_ASSERT(cstr, "qux", "Const \"qux\" = \"qux\"")
    ATT_ASSERT((char*)cstr, "qux", "Const \"qux\" to not const = \"qux\"")

    const char* cstr2 = "qux";
    ATT_ASSERT(cstr, cstr2, "Const \"qux\" = const \"qux\"")

    ATT_ASSERT((char*)NULL, NULL, "NULL")

    return NULL;
}
