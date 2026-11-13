/**
 * The Attractor Unit Test library
 *
 * This file is distributed under the MIT License. See LICENSE for details.
 */

// clock_gettime() is POSIX: strict ISO C modes (-std=c11) hide it without this.
#ifndef _POSIX_C_SOURCE
    #define _POSIX_C_SOURCE 200809L
#endif

#include <stdio.h>
#include <string.h>

#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#else
    #include <time.h>
#endif

// The single translation unit that carries the library implementation. The test
// cases live in the other tests/*.c files and register themselves via ATT_TEST.
#define ATT_IMPLEMENTATION
#include "../attractor.h"

// Monotonic wall-clock time in seconds. The Win32 counter does not depend on the C runtime, so
// it also works with the legacy msvcrt.dll, which has no timespec_get().
static double monotonic_seconds(void) {
#ifdef _WIN32
    LARGE_INTEGER frequency, counter;

    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&counter);

    return (double)counter.QuadPart / (double)frequency.QuadPart;
#else
    struct timespec now;

    clock_gettime(CLOCK_MONOTONIC, &now);

    return (double)now.tv_sec + (double)now.tv_nsec / 1e9;
#endif
}

int main(int argc, const char *argv[]) {
    double start;
    double elapsed;
    const char *filter = NULL;
    int i;

    for(i = 1; i < argc; ++i) {
        const char *arg = argv[i];

        if(strcmp(arg, "--list") == 0 || strcmp(arg, "-l") == 0) {
            const char *name;
            unsigned int index = 0;

            while((name = att_get_test_name(index++)) != NULL) {
                puts(name);
            }

            return 0;
        } else if((strcmp(arg, "--filter") == 0 || strcmp(arg, "-f") == 0) && i + 1 < argc) {
            filter = argv[++i];
        } else if(strncmp(arg, "--filter=", 9) == 0) {
            filter = arg + 9;
        }
    }

    start = monotonic_seconds();
    att_set_verbose(2);
    att_run_tests(filter);

    int failed = att_report();

    elapsed = monotonic_seconds() - start;

    printf("Execution time: %.4f seconds\n", elapsed);

    return failed;
}
