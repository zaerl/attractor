// This is a script used to generate functions declarations for the attractor library

const types = [
    {
        name: 'char',
        signed: true,
        unsigned: true,
        ptr: true,
        formats: [ 'c', 'c' ]
    }, {
        name: 'short',
        signed: true,
        unsigned: true,
        formats: [ 'hd', 'hu' ]
    }, {
        name: 'int',
        signed: true,
        unsigned: true,
        formats: [ 'd', 'u' ]
    }, {
        name: 'long',
        signed: true,
        unsigned: true,
        formats: [ 'ld', 'lu' ]
    }, {
        name: 'long long',
        signed: true,
        unsigned: true,
        formats: [ 'lld', 'llu' ]
    }, {
        name: 'float',
        signed: true,
        formats: [ 'f' ]
    }, {
        name: 'double',
        signed: true,
        formats: [ 'lf' ]
    }, {
        name: 'long double',
        signed: true,
        formats: [ 'Lf' ]
    }, {
        name: 'void',
        ptr: true,
        formats: [ 'p' ]
    }, {
        name: '_Bool',
        signed: true,
        formats: [ 'b' ],
        alt_formats: [ 'd' ]
    }, {
        name: 'default',
        alt_name: 'int',
        signed: true,
        formats: [ 'unknown' ],
        alt_formats: [ 'd' ]
    }
];

const generics = [];
const decls = [];
const contents = [];

const today = new Date();
const date = today.toISOString().split('T')[0];

function type_name(type_info, unsigned) {
    return (unsigned ? 'unsigned ': '') + type_info.name;
}

function format_name(type_info, unsigned, alt) {
    const index = unsigned ? 1 : 0;

    if(alt && type_info.alt_formats) {
        return type_info.alt_formats[index];
    }

    return type_info.formats[index];
}

function fn_name(type_info, unsigned) {
    const format = format_name(type_info, unsigned);

    return `att_assert${unsigned ? '_u' : ''}${type_info.name.endsWith('*') ? '_p' : ''}_${format}`;
}

function fn_decl(type_info, unsigned) {
    let type = type_name(type_info, unsigned);
    const fn = fn_name(type_info, unsigned);

    if(type_info.alt_name) {
        type = type_info.alt_name;
    }

    return `ATT_API unsigned int ${fn}(${type} result, ${type} expected, const char *description)`;
}

function fn_content(type_info, unsigned) {
    let comparison = 'result == expected';
    const format = format_name(type_info, unsigned, true);
    let s_format = '"%' + (type_info.name.endsWith('*') ? 'p' : format) + '"';

    if(type_info.name === 'char*') {
        comparison = '((result == expected) || ((result && expected) ? strcmp(result, expected) == 0 : 0))';
        s_format = 'ATT_STRING_AS_POINTERS == 1 ? "%p" : "\\"%s\\""';
    }

    return fn_decl(type_info, unsigned) + ` {
    int test = att_assert("${type_info.name}", ${comparison}, description);

    if(!test) {
        ATT_ERROR_MESSAGE(result, ${s_format}, expected);
    }

    return test;
}`;
}

function generic_decl(type_info, unsigned) {
    const type = type_name(type_info, unsigned);
    const fn = fn_name(type_info, unsigned);

    return `${type}: ${fn}`;
}

function header() {
    return `/**
 * ${date}
 *
 * The attractor unit test library
 */
`
}

const generate_c = process.argv.length === 3 && process.argv[2] === 'c';

for(const type_info of types) {
    if(type_info.signed) {
        decls.push(fn_decl(type_info, false));
        generics.push(generic_decl(type_info, false));
        contents.push(fn_content(type_info, false));
    }

    if(type_info.unsigned) {
        decls.push(fn_decl(type_info, true));
        generics.push(generic_decl(type_info, true));
        contents.push(fn_content(type_info, true));
    }

    if(type_info.ptr) {
        type_info.name = type_info.name + '*';
        decls.push(fn_decl(type_info, false));
        generics.push(generic_decl(type_info, false));
        contents.push(fn_content(type_info, false));
    }
}

if(generate_c) {
    console.log(`
${header()}
#include "attractor.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if ATT_USE_IOCTL > 0
#include <sys/ioctl.h>
#include <unistd.h>
#endif

#define ATT_ERROR_MESSAGE(RESULT, FORMAT, EXPECTED) \\
if(att_verbose >= 1 && att_show_error) { \\
    fputs("Expected \\x1B[32m", stdout); \\
    printf(FORMAT, EXPECTED); \\
    fputs("\\x1B[0m, got \\x1B[31m", stdout); \\
    printf(FORMAT, RESULT); \\
    fputs("\\x1B[0m\\n\\n", stdout); \\
}

static unsigned int att_valid_tests = 0;
static unsigned int att_total_tests = 0;
static unsigned int att_verbose = ATT_VERBOSE;
static unsigned int att_show_error = ATT_SHOW_ERROR;
static unsigned int att_columns = 80;

unsigned int att_get_valid_tests(void) {
    return att_valid_tests;
}

unsigned int att_get_total_tests(void) {
    return att_total_tests;
}

void att_set_verbose(unsigned int verbose) {
    att_verbose = verbose;
}

void att_set_show_error(unsigned int show_error) {
    att_show_error = show_error;
}

int att_assert(const char *type, int test, const char *description);

${contents.join('\n\n')}

int att_assert(const char *format, int test, const char *description) {
    ++att_total_tests;

#if ATT_USE_IOCTL > 0
    if(att_total_tests == 1) {
        struct winsize w;

        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

        if(w.ws_col > 0) {
            att_columns = w.ws_col;
        }
    }
#endif

    if(test) {
        ++att_valid_tests;
    }

    if(att_verbose == 0) {
        return test;
    } else if(att_verbose == 1) {
        fputs(test ? "." : "\\x1B[31mF\\x1B[0m", stdout);

        if(!test) {
            fputs("\\n", stdout);
        }
    } else {
        const char *ok = "\\x1B[32mOK\\x1B[0m";
        const char *fail = "\\x1B[31mFAIL\\x1B[0m";
        int length = att_columns - (strlen(format) + strlen(description) + (test ? 2 : 4) + 5);

        if(length <= 0) {
            length = 2;
        }

        char spaces[length + 1];
        spaces[length] = '\\0';

        for(int i = 0; i < length; i++) {
            spaces[i] = ' ';
        }

        printf("[%s] \\x1b[34m%s\\x1b[0m: %s%s\\n", format, description, spaces, test ? ok : fail);
    }

    return test;
}
`);

    return;
}

console.log(`/**
 * ${date}
 *
 * The attractor unit test library
 *
 * Usage:
 *
 * #include <attractor.h>
 *
 * int var_to_test = 1;
 * int expected_value = 1;
 *
 * ATT_ASSERT(var_to_test, expected_value, "one == one");
 */

#ifndef ATT_TEST_H
#define ATT_TEST_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef ATT_API
#define ATT_API
#endif

#ifndef ATT_VERBOSE
#define ATT_VERBOSE 1
#endif

#ifndef ATT_SHOW_ERROR
#define ATT_SHOW_ERROR 1
#endif

#ifndef ATT_STRING_AS_POINTERS
#define ATT_STRING_AS_POINTERS 0
#endif

#ifndef ATT_USE_IOCTL
#define ATT_USE_IOCTL 0
#endif

#define ATT_ASSERT(VALUE, EXPECTED, MESSAGE) _Generic((0, VALUE), \\
    ${generics.join(', \\\n    ')} \\
)(VALUE, EXPECTED, MESSAGE);

${decls.join(';\n')};

unsigned int att_get_valid_tests(void);
unsigned int att_get_total_tests(void);

void att_set_verbose(unsigned int verbose);
void att_set_show_error(unsigned int show_error);

#ifdef __cplusplus
}
#endif

#endif /* ATT_TEST_H */`);
