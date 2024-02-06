
/**
 * 2024-02-06
 *
 * The attractor unit test library
 */

#include "attractor.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ATT_ERROR_MESSAGE(RESULT, FORMAT, EXPECTED) \
if(att_verbose >= 1) { \
    fputs("Expected \x1B[32m", stdout); \
    printf(FORMAT, EXPECTED); \
    fputs("\x1B[0m, got \x1B[31m", stdout); \
    printf(FORMAT, RESULT); \
    fputs("\x1B[0m\n\n", stdout); \
}

static unsigned int att_valid_tests = 0;
static unsigned int att_total_tests = 0;
static unsigned int att_verbose = ATT_VERBOSE;

unsigned int att_get_valid_tests(void) {
    return att_valid_tests;
}

unsigned int att_get_total_tests(void) {
    return att_total_tests;
}

void att_set_verbose(unsigned int verbose) {
    att_verbose = verbose;
}

int att_assert(const char *type, int test, const char *description);

ATT_API unsigned int att_assert_c(char result, char expected, const char *description) {
    int test = att_assert("%c", result == expected, description);

    if(!test) {
        ATT_ERROR_MESSAGE(result, "%c", expected);
    }

    return test;
}

ATT_API unsigned int att_assert_u_c(unsigned char result, unsigned char expected, const char *description) {
    int test = att_assert("%c", result == expected, description);

    if(!test) {
        ATT_ERROR_MESSAGE(result, "%c", expected);
    }

    return test;
}

ATT_API unsigned int att_assert_p_c(char* result, char* expected, const char *description) {
    int test = att_assert("%p", result && expected ? strcmp(result, expected) == 0 : 0, description);

    if(!test) {
        ATT_ERROR_MESSAGE(result, "%p", expected);
    }

    return test;
}

ATT_API unsigned int att_assert_hd(short result, short expected, const char *description) {
    int test = att_assert("%hd", result == expected, description);

    if(!test) {
        ATT_ERROR_MESSAGE(result, "%hd", expected);
    }

    return test;
}

ATT_API unsigned int att_assert_u_hu(unsigned short result, unsigned short expected, const char *description) {
    int test = att_assert("%hu", result == expected, description);

    if(!test) {
        ATT_ERROR_MESSAGE(result, "%hu", expected);
    }

    return test;
}

ATT_API unsigned int att_assert_d(int result, int expected, const char *description) {
    int test = att_assert("%d", result == expected, description);

    if(!test) {
        ATT_ERROR_MESSAGE(result, "%d", expected);
    }

    return test;
}

ATT_API unsigned int att_assert_u_u(unsigned int result, unsigned int expected, const char *description) {
    int test = att_assert("%u", result == expected, description);

    if(!test) {
        ATT_ERROR_MESSAGE(result, "%u", expected);
    }

    return test;
}

ATT_API unsigned int att_assert_ld(long result, long expected, const char *description) {
    int test = att_assert("%ld", result == expected, description);

    if(!test) {
        ATT_ERROR_MESSAGE(result, "%ld", expected);
    }

    return test;
}

ATT_API unsigned int att_assert_u_lu(unsigned long result, unsigned long expected, const char *description) {
    int test = att_assert("%lu", result == expected, description);

    if(!test) {
        ATT_ERROR_MESSAGE(result, "%lu", expected);
    }

    return test;
}

ATT_API unsigned int att_assert_lld(long long result, long long expected, const char *description) {
    int test = att_assert("%lld", result == expected, description);

    if(!test) {
        ATT_ERROR_MESSAGE(result, "%lld", expected);
    }

    return test;
}

ATT_API unsigned int att_assert_u_llu(unsigned long long result, unsigned long long expected, const char *description) {
    int test = att_assert("%llu", result == expected, description);

    if(!test) {
        ATT_ERROR_MESSAGE(result, "%llu", expected);
    }

    return test;
}

ATT_API unsigned int att_assert_f(float result, float expected, const char *description) {
    int test = att_assert("%f", result == expected, description);

    if(!test) {
        ATT_ERROR_MESSAGE(result, "%f", expected);
    }

    return test;
}

ATT_API unsigned int att_assert_lf(double result, double expected, const char *description) {
    int test = att_assert("%lf", result == expected, description);

    if(!test) {
        ATT_ERROR_MESSAGE(result, "%lf", expected);
    }

    return test;
}

ATT_API unsigned int att_assert_Lf(long double result, long double expected, const char *description) {
    int test = att_assert("%Lf", result == expected, description);

    if(!test) {
        ATT_ERROR_MESSAGE(result, "%Lf", expected);
    }

    return test;
}

ATT_API unsigned int att_assert_p_p(void* result, void* expected, const char *description) {
    int test = att_assert("%p", result == expected, description);

    if(!test) {
        ATT_ERROR_MESSAGE(result, "%p", expected);
    }

    return test;
}

ATT_API unsigned int att_assert_b(_Bool result, _Bool expected, const char *description) {
    int test = att_assert("%d", result == expected, description);

    if(!test) {
        ATT_ERROR_MESSAGE(result, "%d", expected);
    }

    return test;
}

ATT_API unsigned int att_assert_unknown(int result, int expected, const char *description) {
    int test = att_assert("%d", result == expected, description);

    if(!test) {
        ATT_ERROR_MESSAGE(result, "%d", expected);
    }

    return test;
}

int att_assert(const char *format, int test, const char *description) {
    ++att_total_tests;

    if(test) {
        ++att_valid_tests;
    }

    if(att_verbose == 0) {
        return test;
    } else if(att_verbose == 1) {
        fputs(test ? "." : "\x1B[31mF\x1B[0m", stdout);

        if(!test) {
            fputs("\n", stdout);
        }
    } else {
        const char *ok = "\x1B[32mOK\x1B[0m";
        const char *fail = "\x1B[31mFAIL\x1B[0m";
        int length = 80 - (strlen(format) + strlen(description) + (test ? 2 : 4) + 5);

        if(length <= 0) {
            length = 2;
        }

        char spaces[length + 1];
        spaces[length] = '\0';

        for(int i = 0; i < length; i++) {
            spaces[i] = ' ';
        }

        printf("[%s] \x1b[34m%s\x1b[0m: %s%s\n", format, description, spaces, test ? ok : fail);
    }

    return test;
}

