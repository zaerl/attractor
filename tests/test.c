/**
 * The Attractor Unit Test library
 *
 * This file is distributed under the MIT License. See LICENSE for details.
 */

#include <stdio.h>
#include <string.h>
#include <time.h>

// The single translation unit that carries the library implementation. The test
// cases live in the other tests/*.c files and register themselves via ATT_TEST.
#define ATT_IMPLEMENTATION
#include "../attractor.h"

int main(int argc, const char *argv[]) {
    // clock() is used instead of timespec_get(), which the legacy msvcrt.dll does not provide.
    clock_t start, end;
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

    start = clock();
    att_set_verbose(2);
    att_run_tests(filter);

    int failed = att_report();

    end = clock();
    elapsed = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Execution time: %.4f seconds\n", elapsed);

    return failed;
}
