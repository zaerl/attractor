/**
 * The Attractor Unit Test library
 *
 * This file is distributed under the MIT License. See LICENSE for details.
 *
 * Single-header library. In exactly one source file define ATT_IMPLEMENTATION
 * before including this header:
 *
 * #define ATT_IMPLEMENTATION
 * #include "attractor.h"
 *
 * Every other file just includes the header. Usage:
 *
 * int var_to_test = 1;
 * int expected_value = 1;
 *
 * ATT_ASSERT(var_to_test, expected_value, "one == one");
 */

#ifndef ATTRACTOR_H
#define ATTRACTOR_H

#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
#include <cstddef>
#include <cstdint>
#include <string>
#include <type_traits>

// In C++, _Bool doesn't exist, so map it to bool
#ifndef _Bool
#define _Bool bool
#endif

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

// Tolerance for float, double and long double comparisons. Defaults to 0 (exact
// equality). Define a small value (e.g. 1e-6) to compare within an epsilon.
#ifndef ATT_FLOAT_EPSILON
#define ATT_FLOAT_EPSILON 0
#endif

#ifndef ATT_STRING_AS_POINTERS
#define ATT_STRING_AS_POINTERS 0
#endif

#ifndef ATT_CUSTOM_TYPES
#define ATT_CUSTOM_TYPES
#endif

// clang-format off
#ifndef __cplusplus
#define ATT_ASSERT(VALUE, EXPECTED, MESSAGE) \
    (att_set_assert_context(#VALUE, __FILE__, __LINE__), _Generic(VALUE, \
    ATT_CUSTOM_TYPES \
    char: att_assert_c, \
    unsigned char: att_assert_u_c, \
    char *: att_assert_p_c, \
    const char *: att_assert_cp_c, \
    short: att_assert_hd, \
    unsigned short: att_assert_u_hu, \
    int: att_assert_d, \
    unsigned int: att_assert_u_u, \
    long: att_assert_ld, \
    unsigned long: att_assert_u_lu, \
    long long: att_assert_lld, \
    unsigned long long: att_assert_u_llu, \
    float: att_assert_f, \
    double: att_assert_lf, \
    long double: att_assert_Lf, \
    void *: att_assert_p_p, \
    _Bool: att_assert_b, \
    default: att_assert_unknown \
)(VALUE, EXPECTED, MESSAGE, __FILE__, __LINE__))
#else
#define ATT_ASSERT(VALUE, EXPECTED, MESSAGE) \
    (att_set_assert_context(#VALUE, __FILE__, __LINE__), \
     att_assert_cpp(VALUE, EXPECTED, MESSAGE, __FILE__, __LINE__))
#endif

ATT_API unsigned int att_assert_c(char result, char expected, const char *description, const char *file, unsigned int line);
ATT_API unsigned int att_assert_u_c(unsigned char result, unsigned char expected, const char *description, const char *file, unsigned int line);
ATT_API unsigned int att_assert_p_c(char *result, char *expected, const char *description, const char *file, unsigned int line);
ATT_API unsigned int att_assert_cp_c(const char *result, const char *expected, const char *description, const char *file, unsigned int line);
ATT_API unsigned int att_assert_hd(short result, short expected, const char *description, const char *file, unsigned int line);
ATT_API unsigned int att_assert_u_hu(unsigned short result, unsigned short expected, const char *description, const char *file, unsigned int line);
ATT_API unsigned int att_assert_d(int result, int expected, const char *description, const char *file, unsigned int line);
ATT_API unsigned int att_assert_u_u(unsigned int result, unsigned int expected, const char *description, const char *file, unsigned int line);
ATT_API unsigned int att_assert_ld(long result, long expected, const char *description, const char *file, unsigned int line);
ATT_API unsigned int att_assert_u_lu(unsigned long result, unsigned long expected, const char *description, const char *file, unsigned int line);
ATT_API unsigned int att_assert_lld(long long result, long long expected, const char *description, const char *file, unsigned int line);
ATT_API unsigned int att_assert_u_llu(unsigned long long result, unsigned long long expected, const char *description, const char *file, unsigned int line);
ATT_API unsigned int att_assert_f(float result, float expected, const char *description, const char *file, unsigned int line);
ATT_API unsigned int att_assert_lf(double result, double expected, const char *description, const char *file, unsigned int line);
ATT_API unsigned int att_assert_Lf(long double result, long double expected, const char *description, const char *file, unsigned int line);
ATT_API unsigned int att_assert_p_p(void *result, void *expected, const char *description, const char *file, unsigned int line);
ATT_API unsigned int att_assert_b(_Bool result, _Bool expected, const char *description, const char *file, unsigned int line);
ATT_API unsigned int att_assert_unknown(void * result, void * expected, const char *description, const char *file, unsigned int line);

unsigned int att_get_valid_tests(void);
unsigned int att_get_total_tests(void);

// Assert that a condition is true. Works in both C and C++.
#define ATT_ASSERT_TRUE(VALUE, MESSAGE) \
    (att_set_assert_context(#VALUE, __FILE__, __LINE__), \
     att_assert_true((VALUE) ? 1 : 0, MESSAGE, __FILE__, __LINE__))

// Assert that two memory regions of SIZE bytes are equal. Two NULL pointers are
// considered equal; a NULL and a non-NULL pointer are not.
#define ATT_ASSERT_MEM(VALUE, EXPECTED, SIZE, MESSAGE) \
    (att_set_assert_context(#VALUE, __FILE__, __LINE__), \
     att_assert_mem(VALUE, EXPECTED, SIZE, MESSAGE, __FILE__, __LINE__))

ATT_API unsigned int att_assert_true(int result, const char *description, const char *file, unsigned int line);
ATT_API unsigned int att_assert_mem(const void *result, const void *expected, size_t size, const char *description, const char *file, unsigned int line);

// Print a summary of the tests run ("Tests valid/run: X/Y") and return 0 if all
// tests passed, 1 otherwise. Suitable as the return value of main().
int att_report(void);

void att_set_verbose(unsigned int verbose);
void att_set_show_error(unsigned int show_error);

// Tolerance used for float, double and long double comparisons. Initialized to
// ATT_FLOAT_EPSILON (0 by default, i.e. exact equality) and overridable at runtime.
long double att_get_float_epsilon(void);
void att_set_float_epsilon(long double epsilon);

// A callback to be used when the default comparison fails.
typedef int (*att_generic_callback)(void *result, void *expected, const char *description);

void att_set_generic_callback(att_generic_callback callback);

// A callback to be used when an test occurs.
typedef int (*att_test_callback)(int test, const char *description, const char *expression,
    const char *file, unsigned int line);

void att_set_test_callback(att_test_callback callback);

// Stores the expression for the next assertion. Used internally by ATT_ASSERT.
void att_set_assert_context(const char *expression, const char *file, unsigned int line);

#ifdef __cplusplus
}
#endif

// clang-format on

#ifdef __cplusplus
// Helper to check if common_type exists
template <typename T1, typename T2, typename = void> struct has_common_type : std::false_type {};

template <typename T1, typename T2>
struct has_common_type<T1, T2, std::void_t<typename std::common_type<T1, T2>::type>>
    : std::true_type {};

// C++ template function for att_assert to handle type deduction properly
template <typename T1, typename T2>
inline unsigned int att_assert_cpp(T1 result, T2 expected, const char *description,
    const char *file, unsigned int line) {
    // Check if we can find a common type
    if constexpr(has_common_type<T1, T2>::value) {
        // Convert both to a common type for comparison
        using common_type = std::common_type_t<T1, T2>;
        common_type converted_result = static_cast<common_type>(result);
        common_type converted_expected = static_cast<common_type>(expected);

        // Handle different common types
        if constexpr(std::is_same_v<common_type, char>) {
            return att_assert_c(converted_result, converted_expected, description, file, line);
        } else if constexpr(std::is_same_v<common_type, unsigned char>) {
            return att_assert_u_c(converted_result, converted_expected, description, file, line);
        } else if constexpr(std::is_same_v<common_type, short>) {
            return att_assert_hd(converted_result, converted_expected, description, file, line);
        } else if constexpr(std::is_same_v<common_type, unsigned short>) {
            return att_assert_u_hu(converted_result, converted_expected, description, file, line);
        } else if constexpr(std::is_same_v<common_type, int>) {
            return att_assert_d(converted_result, converted_expected, description, file, line);
        } else if constexpr(std::is_same_v<common_type, unsigned int>) {
            return att_assert_u_u(converted_result, converted_expected, description, file, line);
        } else if constexpr(std::is_same_v<common_type, long>) {
            return att_assert_ld(converted_result, converted_expected, description, file, line);
        } else if constexpr(std::is_same_v<common_type, unsigned long>) {
            return att_assert_u_lu(converted_result, converted_expected, description, file, line);
        } else if constexpr(std::is_same_v<common_type, long long>) {
            return att_assert_lld(converted_result, converted_expected, description, file, line);
        } else if constexpr(std::is_same_v<common_type, unsigned long long>) {
            return att_assert_u_llu(converted_result, converted_expected, description, file, line);
        } else if constexpr(std::is_same_v<common_type, float>) {
            return att_assert_f(converted_result, converted_expected, description, file, line);
        } else if constexpr(std::is_same_v<common_type, double>) {
            return att_assert_lf(converted_result, converted_expected, description, file, line);
        } else if constexpr(std::is_same_v<common_type, long double>) {
            return att_assert_Lf(converted_result, converted_expected, description, file, line);
        } else if constexpr(std::is_same_v<common_type, bool>) {
            return att_assert_b(converted_result, converted_expected, description, file, line);
        } else if constexpr(std::is_same_v<common_type, std::string>) {
            return att_assert_cp_c(result.c_str(), expected.c_str(), description, file, line);
        } else {
            // Unsupported type with common_type - compile error
            static_assert(sizeof(T1) == 0,
                "ATT_ASSERT: Unsupported type. Supported types are: char, short, int, long, long "
                "long (signed/unsigned), float, double, long double, bool, std::string, and "
                "pointer types.");
            return 0;
        }
    } else {
        // No common type - compile error
        static_assert(sizeof(T1) == 0,
            "ATT_ASSERT: Cannot compare incompatible types with no common type.");
        return 0;
    }
}

// Overloads for pointer types (can't use template easily for these)
inline unsigned int att_assert_cpp(char *result, char *expected, const char *description,
    const char *file, unsigned int line) {
    return att_assert_p_c(result, expected, description, file, line);
}

inline unsigned int att_assert_cpp(const char *result, const char *expected,
    const char *description, const char *file, unsigned int line) {
    return att_assert_cp_c(result, expected, description, file, line);
}

inline unsigned int att_assert_cpp(void *result, void *expected, const char *description,
    const char *file, unsigned int line) {
    return att_assert_p_p(result, expected, description, file, line);
}

// Convert the int form of NULL through a pointer-sized integer. On LLP64 platforms (64-bit
// Windows), long is narrower than a pointer.
template <typename T> inline T *att_pointer_from_long(long value) {
    return value == 0 ? nullptr : reinterpret_cast<T *>(static_cast<std::intptr_t>(value));
}

// Special overloads for legacy NULL pointer comparisons
inline unsigned int att_assert_cpp(const char *result, long expected, const char *description,
    const char *file, unsigned int line) {
    return att_assert_cp_c(result, att_pointer_from_long<const char>(expected), description, file,
        line);
}

inline unsigned int att_assert_cpp(char *result, long expected, const char *description,
    const char *file, unsigned int line) {
    return att_assert_p_c(result, att_pointer_from_long<char>(expected), description, file, line);
}

inline unsigned int att_assert_cpp(void *result, long expected, const char *description,
    const char *file, unsigned int line) {
    return att_assert_p_p(result, att_pointer_from_long<void>(expected), description, file, line);
}

// Modern C++ null pointer comparisons
inline unsigned int att_assert_cpp(const char *result, std::nullptr_t, const char *description,
    const char *file, unsigned int line) {
    return att_assert_cp_c(result, nullptr, description, file, line);
}

inline unsigned int att_assert_cpp(char *result, std::nullptr_t, const char *description,
    const char *file, unsigned int line) {
    return att_assert_p_c(result, nullptr, description, file, line);
}

inline unsigned int att_assert_cpp(void *result, std::nullptr_t, const char *description,
    const char *file, unsigned int line) {
    return att_assert_p_p(result, nullptr, description, file, line);
}

// Mixed char pointer types (char* vs const char*)
inline unsigned int att_assert_cpp(char *result, const char *expected, const char *description,
    const char *file, unsigned int line) {
    return att_assert_cp_c(result, expected, description, file, line);
}

inline unsigned int att_assert_cpp(const char *result, char *expected, const char *description,
    const char *file, unsigned int line) {
    return att_assert_cp_c(result, expected, description, file, line);
}

// Generic pointer type catch-all (for any pointer types not covered by specific overloads)
template <typename T>
inline unsigned int att_assert_cpp(T *result, T *expected, const char *description,
    const char *file, unsigned int line) {
    return att_assert_unknown((void *)result, (void *)expected, description, file, line);
}
#endif

#endif /* ATTRACTOR_H */

#ifdef ATT_IMPLEMENTATION
#ifndef ATT_IMPLEMENTATION_INCLUDED
#define ATT_IMPLEMENTATION_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// clang-format off
#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
    #include <io.h>
    #define isatty _isatty
    #ifndef STDOUT_FILENO
        #define STDOUT_FILENO _fileno(stdout)
    #endif
#else
    #include <unistd.h>
#endif

// ATT_FETCH_INCREMENT atomically increments and returns the previous value.
#ifdef __cplusplus
    #include <atomic>
    typedef std::atomic<unsigned int> att_atomic_uint;
    #define ATT_FETCH_INCREMENT(value) ((value)++)
#elif defined(_MSC_VER)
    // MSVC ships no C11 <stdatomic.h> when compiling as C (error C1189 without the
    // /experimental:c11atomics switch), so use the Interlocked intrinsics instead.
    typedef volatile LONG att_atomic_uint;
    #define ATT_FETCH_INCREMENT(value) ((unsigned int)InterlockedIncrement(&(value)) - 1)
#else
    #include <stdatomic.h>
    typedef atomic_uint att_atomic_uint;
    #define ATT_FETCH_INCREMENT(value) ((value)++)
#endif
// clang-format on

#define ATT_ERROR_MESSAGE(RESULT, FORMAT_1, FORMAT_2, EXPECTED) \
    if(att_show_error) { \
        printf(att_show_colors ? "\x1B[90m%s:%u:\x1B[0m " : "%s:%u: ", file, line); \
        if(att_verbose < 2) { \
            printf("%s: ", description); \
        } \
        fputs(att_show_colors ? "Expected \x1B[32m" : "Expected ", stdout); \
        printf(FORMAT_1, EXPECTED); \
        fputs(att_show_colors ? "\x1B[0m, got \x1B[31m" : ", got ", stdout); \
        printf(FORMAT_2, RESULT); \
        fputs(att_show_colors ? "\x1B[0m\n\n" : "\n\n", stdout); \
    }

static att_atomic_uint att_valid_tests = 0;
static att_atomic_uint att_total_tests = 0;
static unsigned int att_verbose = ATT_VERBOSE;
static unsigned int att_show_error = ATT_SHOW_ERROR;
static int att_show_colors = 0;
static long double att_float_epsilon = ATT_FLOAT_EPSILON;

// A callback to be used when the default comparison fails.
static att_generic_callback att_generic_callback_fn = NULL;

// A callback to be used when an test occurs.
static att_test_callback att_test_callback_fn = NULL;

// Variables stored by each assertion to provide context for the next assertion.
static const char *att_assert_expression = "";
static const char *att_assert_file = "";
static unsigned int att_assert_line = 0;

unsigned int att_get_valid_tests(void) {
    return (unsigned int)att_valid_tests;
}

unsigned int att_get_total_tests(void) {
    return (unsigned int)att_total_tests;
}

void att_set_verbose(unsigned int verbose) {
    att_verbose = verbose;
}

void att_set_show_error(unsigned int show_error) {
    att_show_error = show_error;
}

long double att_get_float_epsilon(void) {
    return att_float_epsilon;
}

void att_set_float_epsilon(long double epsilon) {
    att_float_epsilon = epsilon;
}

void att_set_generic_callback(att_generic_callback callback) {
    att_generic_callback_fn = callback;
}

void att_set_test_callback(att_test_callback callback) {
    att_test_callback_fn = callback;
}

void att_set_assert_context(const char *expression, const char *file, unsigned int line) {
    att_assert_expression = expression ? expression : "";
    att_assert_file = file ? file : "";
    att_assert_line = line;
}

// These functions are automatically generated. Do not edit.
// clang-format off
int att_assert(const char *type, int test, const char *description);

ATT_API unsigned int att_assert_c(char result, char expected, const char *description, const char *file, unsigned int line) {
    int test = att_assert("char", result == expected, description);

    if(!test) {
        const char *format_1;
        const char *format_2;

        if(expected < 32 || expected == 127) {
            format_1 = "\\x%02X";
        } else {
            format_1 = "%c";
        }

        if(result < 32 || result == 127) {
            format_2 = "\\x%02X";
        } else {
            format_2 = "%c";
        }

        ATT_ERROR_MESSAGE(result, format_1, format_2, expected);
    }

    return test;
}

ATT_API unsigned int att_assert_u_c(unsigned char result, unsigned char expected, const char *description, const char *file, unsigned int line) {
    int test = att_assert("unsigned char", result == expected, description);

    if(!test) {
        const char *format_1;
        const char *format_2;

        if(expected < 32 || expected == 127) {
            format_1 = "\\x%02X";
        } else {
            format_1 = "%c";
        }

        if(result < 32 || result == 127) {
            format_2 = "\\x%02X";
        } else {
            format_2 = "%c";
        }

        ATT_ERROR_MESSAGE(result, format_1, format_2, expected);
    }

    return test;
}

ATT_API unsigned int att_assert_p_c(char *result, char *expected, const char *description, const char *file, unsigned int line) {
    int test = att_assert("char *", ((result == expected) || ((result && expected) ? strcmp(result, expected) == 0 : 0)), description);

    if(!test) {
        ATT_ERROR_MESSAGE(result, ATT_STRING_AS_POINTERS == 1 ? "%p" : "\"%s\"", ATT_STRING_AS_POINTERS == 1 ? "%p" : "\"%s\"", expected);
    }

    return test;
}

ATT_API unsigned int att_assert_cp_c(const char *result, const char *expected, const char *description, const char *file, unsigned int line) {
    int test = att_assert("const char *", ((result == expected) || ((result && expected) ? strcmp(result, expected) == 0 : 0)), description);

    if(!test) {
        ATT_ERROR_MESSAGE(result, ATT_STRING_AS_POINTERS == 1 ? "%p" : "\"%s\"", ATT_STRING_AS_POINTERS == 1 ? "%p" : "\"%s\"", expected);
    }

    return test;
}

ATT_API unsigned int att_assert_hd(short result, short expected, const char *description, const char *file, unsigned int line) {
    int test = att_assert("short", result == expected, description);

    if(!test) {
        ATT_ERROR_MESSAGE(result, "%hd", "%hd", expected);
    }

    return test;
}

ATT_API unsigned int att_assert_u_hu(unsigned short result, unsigned short expected, const char *description, const char *file, unsigned int line) {
    int test = att_assert("unsigned short", result == expected, description);

    if(!test) {
        ATT_ERROR_MESSAGE(result, "%hu", "%hu", expected);
    }

    return test;
}

ATT_API unsigned int att_assert_d(int result, int expected, const char *description, const char *file, unsigned int line) {
    int test = att_assert("int", result == expected, description);

    if(!test) {
        ATT_ERROR_MESSAGE(result, "%d", "%d", expected);
    }

    return test;
}

ATT_API unsigned int att_assert_u_u(unsigned int result, unsigned int expected, const char *description, const char *file, unsigned int line) {
    int test = att_assert("unsigned int", result == expected, description);

    if(!test) {
        ATT_ERROR_MESSAGE(result, "%u", "%u", expected);
    }

    return test;
}

ATT_API unsigned int att_assert_ld(long result, long expected, const char *description, const char *file, unsigned int line) {
    int test = att_assert("long", result == expected, description);

    if(!test) {
        ATT_ERROR_MESSAGE(result, "%ld", "%ld", expected);
    }

    return test;
}

ATT_API unsigned int att_assert_u_lu(unsigned long result, unsigned long expected, const char *description, const char *file, unsigned int line) {
    int test = att_assert("unsigned long", result == expected, description);

    if(!test) {
        ATT_ERROR_MESSAGE(result, "%lu", "%lu", expected);
    }

    return test;
}

ATT_API unsigned int att_assert_lld(long long result, long long expected, const char *description, const char *file, unsigned int line) {
    int test = att_assert("long long", result == expected, description);

    if(!test) {
        ATT_ERROR_MESSAGE(result, "%lld", "%lld", expected);
    }

    return test;
}

ATT_API unsigned int att_assert_u_llu(unsigned long long result, unsigned long long expected, const char *description, const char *file, unsigned int line) {
    int test = att_assert("unsigned long long", result == expected, description);

    if(!test) {
        ATT_ERROR_MESSAGE(result, "%llu", "%llu", expected);
    }

    return test;
}

ATT_API unsigned int att_assert_f(float result, float expected, const char *description, const char *file, unsigned int line) {
    int test = att_assert("float", (result == expected) || ((result > expected ? result - expected : expected - result) <= att_float_epsilon), description);

    if(!test) {
        ATT_ERROR_MESSAGE(result, "%f", "%f", expected);
    }

    return test;
}

ATT_API unsigned int att_assert_lf(double result, double expected, const char *description, const char *file, unsigned int line) {
    int test = att_assert("double", (result == expected) || ((result > expected ? result - expected : expected - result) <= att_float_epsilon), description);

    if(!test) {
        ATT_ERROR_MESSAGE(result, "%lf", "%lf", expected);
    }

    return test;
}

ATT_API unsigned int att_assert_Lf(long double result, long double expected, const char *description, const char *file, unsigned int line) {
    int test = att_assert("long double", (result == expected) || ((result > expected ? result - expected : expected - result) <= att_float_epsilon), description);

    if(!test) {
        ATT_ERROR_MESSAGE(result, "%Lf", "%Lf", expected);
    }

    return test;
}

ATT_API unsigned int att_assert_p_p(void *result, void *expected, const char *description, const char *file, unsigned int line) {
    int test = att_assert("void *", result == expected, description);

    if(!test) {
        ATT_ERROR_MESSAGE(result, "%p", "%p", expected);
    }

    return test;
}

ATT_API unsigned int att_assert_b(_Bool result, _Bool expected, const char *description, const char *file, unsigned int line) {
    int test = att_assert("_Bool", result == expected, description);

    if(!test) {
        ATT_ERROR_MESSAGE(result, "%d", "%d", expected);
    }

    return test;
}

ATT_API unsigned int att_assert_unknown(void * result, void * expected, const char *description, const char *file, unsigned int line) {
    int test = att_assert(
        att_generic_callback_fn ? "callback" : "default",
        att_generic_callback_fn ? att_generic_callback_fn(result, expected, description) : (result == expected),
        description
    );

    if(!test) {
        ATT_ERROR_MESSAGE(result, "%p", "%p", expected);
    }

    return test;
}

// clang-format on
int att_assert(const char *format, int test, const char *description) {
    // Initialize the library on the first assertion. The fetch-increment returns the
    // previous value atomically, so exactly one thread observes zero here.
    if(ATT_FETCH_INCREMENT(att_total_tests) == 0) {
        if(isatty(STDOUT_FILENO)) {
            const char *no_color = getenv("NO_COLOR");

#ifdef _WIN32
            // On Windows, the TERM env is usually not set, so enable colors by default if NO_COLOR
            // is not set
            att_show_colors = no_color == NULL;
#else
            // On Unix, check TERM environment variable
            const char *term = getenv("TERM");
            att_show_colors = no_color == NULL && term != NULL && strcmp(term, "dumb") != 0;
#endif
        }

#ifdef _WIN32
        if(att_show_colors) {
            // On Windows, we need to enable ANSI escape codes for stdout and stderr
            HANDLE h_out = GetStdHandle(STD_OUTPUT_HANDLE);
            HANDLE h_err = GetStdHandle(STD_ERROR_HANDLE);
            DWORD mode_out, mode_err;

            // Enable ANSI escape codes for stdout
            if(h_out != INVALID_HANDLE_VALUE && GetConsoleMode(h_out, &mode_out)) {
                mode_out |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;

                if(!SetConsoleMode(h_out, mode_out)) {
                    // If we can't enable ANSI codes, disable colors
                    att_show_colors = 0;
                }
            } else {
                // If we can't get console mode, disable colors
                att_show_colors = 0;
            }

            // Enable ANSI escape codes for stderr
            if(att_show_colors && h_err != INVALID_HANDLE_VALUE &&
                GetConsoleMode(h_err, &mode_err)) {
                mode_err |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
                SetConsoleMode(h_err, mode_err);
            }
        }
#endif
    }

    if(test) {
        ATT_FETCH_INCREMENT(att_valid_tests);
    }

    if(att_test_callback_fn) {
        att_test_callback_fn(test, description, att_assert_expression, att_assert_file,
            att_assert_line);
    }

    if(att_verbose == 0) {
        // Do nothing
    } else if(att_verbose == 1) {
        fputs(test ? "." : (att_show_colors ? "\x1B[31mF\x1B[0m" : "F"), stdout);

        if(!test) {
            fputs("\n", stdout);
        }
    } else {
        const char *ok = att_show_colors ? "\x1B[32mOK\x1B[0m" : "OK";
        const char *fail = att_show_colors ? "\x1B[31mNO\x1B[0m" : "NO";

        printf(att_show_colors ? "%s [\x1b[36m%s\x1b[0m] %s\n" : "%s [%s] %s\n", test ? ok : fail,
            format, description);
    }

    return test;
}

ATT_API unsigned int att_assert_true(int result, const char *description, const char *file,
    unsigned int line) {
    int test = att_assert("true", result != 0, description);

    if(!test) {
        ATT_ERROR_MESSAGE("false", "%s", "%s", "true");
    }

    return test;
}

ATT_API unsigned int att_assert_mem(const void *result, const void *expected, size_t size,
    const char *description, const char *file, unsigned int line) {
    int test = att_assert("memory",
        (result == expected) || (result && expected && memcmp(result, expected, size) == 0),
        description);

    if(!test) {
        if(!result || !expected) {
            ATT_ERROR_MESSAGE((void *)result, "%p", "%p", (void *)expected);
        } else if(att_show_error) {
            const unsigned char *res = (const unsigned char *)result;
            const unsigned char *exp = (const unsigned char *)expected;
            size_t offset = 0;

            while(offset < size && res[offset] == exp[offset]) {
                ++offset;
            }

            printf(att_show_colors ? "\x1B[90m%s:%u:\x1B[0m " : "%s:%u: ", file, line);

            if(att_verbose < 2) {
                printf("%s: ", description);
            }

            printf(att_show_colors ?
                    "Byte %zu: expected \x1B[32m0x%02X\x1B[0m, got \x1B[31m0x%02X\x1B[0m\n\n" :
                    "Byte %zu: expected 0x%02X, got 0x%02X\n\n",
                offset, exp[offset], res[offset]);
        }
    }

    return test;
}

int att_report(void) {
    unsigned int total = (unsigned int)att_total_tests;
    unsigned int valid = (unsigned int)att_valid_tests;
    int all_valid = valid == total;

    // The dots printed with verbose = 1 leave the line unterminated.
    if(att_verbose == 1 && total > 0) {
        fputs("\n", stdout);
    }

    printf(att_show_colors ? (all_valid ? "Tests valid/run: \x1B[32m%u/%u\x1B[0m\n" :
                                          "Tests valid/run: \x1B[31m%u/%u\x1B[0m\n") :
                             "Tests valid/run: %u/%u\n",
        valid, total);

    return all_valid ? 0 : 1;
}

#endif /* ATT_IMPLEMENTATION_INCLUDED */
#endif /* ATT_IMPLEMENTATION */
