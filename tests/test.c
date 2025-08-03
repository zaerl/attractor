/**
 * The warudo tests
 *
 * This file is distributed under the MIT License. See LICENSE for details.
 */

#include <pthread.h>
#include <stdio.h>
#include <time.h>

#include "../attractor.h"

#define DECLARE_TEST(NAME) void *test_##NAME(void *arg);

DECLARE_TEST(chars)
DECLARE_TEST(callback)
DECLARE_TEST(floats)
DECLARE_TEST(integers)
DECLARE_TEST(strings)
DECLARE_TEST(voids)
DECLARE_TEST(unknowns)

int main(int argc, const char *argv[]) {
    struct timespec start, end;
    double elapsed;

    clock_gettime(CLOCK_MONOTONIC, &start);
    att_set_verbose(2);

    #define RUN_TEST(NAME) \
        pthread_t thread_##NAME; \
        pthread_create(&thread_##NAME, NULL, test_##NAME, NULL); \
        pthread_join(thread_##NAME, NULL);

    RUN_TEST(callback)
    RUN_TEST(chars)
    RUN_TEST(floats)
    RUN_TEST(integers)
    RUN_TEST(strings)
    RUN_TEST(voids)
    RUN_TEST(unknowns)

    unsigned int tests_valid = att_get_valid_tests();
    unsigned int tests_total = att_get_total_tests();
    int valid = tests_valid == tests_total;

    /* Green if valid and red if not */
    const char *color_code = valid ? "\x1B[32m" : "\x1B[31m";

    printf("Tests valid/run: %s%d/%d\n\x1B[0m", color_code, tests_valid, tests_total);

    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    printf("Execution time: %.4f seconds\n", elapsed);

    return valid ? 0 : -1;
}
