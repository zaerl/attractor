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
    struct timespec start, end;
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

    timespec_get(&start, TIME_UTC);
    att_set_verbose(2);
    att_run_tests(filter);

    int failed = att_report();

    timespec_get(&end, TIME_UTC);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    printf("Execution time: %.4f seconds\n", elapsed);

    return failed;
}
