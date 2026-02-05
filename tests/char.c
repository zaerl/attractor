#include "../attractor.h"
#include <stdio.h>

void *test_chars(void *arg) {
    ATT_ASSERT((char)'1', (char)'1', "(char)1 = (char)1")
    ATT_ASSERT((unsigned char)'1', (unsigned char)'1', "(unsigned char)1 = (unsigned char)1")

    char a = 'a';
    ATT_ASSERT(a, 'a', "a = a")

    ATT_ASSERT((char)'\0', (char)'\0', "(char)\0 = (char)0")
    ATT_ASSERT((char)'\n', (char)'\n', "(char)\\\n = (char)1")
    ATT_ASSERT((char)'\t', (char)'\t', "(char)\\\\t = (char)1")

    return NULL;
}
