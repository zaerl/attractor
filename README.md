# Attractor

Attractor is a single function unit test framework written in C11.

## Installation

Copy `attractor.c` and `attractor.h` in your test folder.

## Usage

Attractor provide a `ATT_ASSERT` macro that you can use to test your code. The macro is an
expression that evaluates to a number different from zero if the test passes:

```c
ATT_ASSERT(the_value_to_test, the_expected_value, "description");

// Or, being an expression:
if(!ATT_ASSERT(the_value_to_test, the_expected_value, "description")) {
    // React to the failure
}
```

The `ATT_ASSERT` macro accept these types:

1. `char`
2. `unsigned char`
3. `char*`
4. `const char*`
5. `short`
6. `unsigned short`
7. `int`
8. `unsigned int`
9. `long`
10. `unsigned long`
11. `long long`
12. `unsigned long long`
13. `float`
14. `double`
15. `long double`
16. `void*`
17. `_Bool`

In case of unknown type the input is assumed to be `void*`.

Two more assertion macros cover the common cases that are not a typed equality:

```c
ATT_ASSERT_TRUE(strcmp(a, b) < 0, "a sorts before b");
ATT_ASSERT_MEM(buffer, expected_buffer, sizeof(buffer), "buffers are equal");
```

`ATT_ASSERT_TRUE` passes when the condition is true. `ATT_ASSERT_MEM` compares two memory regions
byte by byte and, on failure, reports the offset and value of the first differing byte.

## API

1. `ATT_ASSERT(value, expected, description)` you pass the value to test, the expected one and a
   description of the test.
2. `ATT_ASSERT_TRUE(condition, description)` assert that a condition is true.
3. `ATT_ASSERT_MEM(value, expected, size, description)` assert that two memory regions of `size`
   bytes are equal.
4. `int att_report(void)` print the `Tests valid/run: X/Y` summary and return `0` if every test
   passed, `1` otherwise. Meant to be used as the return value of `main()`.
5. `void att_set_verbose(unsigned int verbose)` set verbosity.
6. `void att_set_show_error(unsigned int show_error)` set to show errors.
7. `void att_set_generic_callback(att_generic_callback callback)` set a callback to run for generic
   tests
8. `void att_set_test_callback(att_test_callback callback)` set a callback to call for all tests.
9. `unsigned int att_get_valid_tests(void)` return the count of valid tests.
10. `unsigned int att_get_total_tests(void)` return the count of run tests.
11. `void att_set_float_epsilon(long double epsilon)` set the tolerance for float comparisons.
12. `long double att_get_float_epsilon(void)` return the current float comparison tolerance.

## Examples

```c
#include <attractor.h>

int main(void) {
    int var_to_test_1 = 1;
    float var_to_test_2 = 2.0;

    ATT_ASSERT(var_to_test_1, 1, "one == one");
    ATT_ASSERT(var_to_test_2, 2.0, "2.0 == 2.0");
    ATT_ASSERT(3ll, 3ll, "long long 3 == long long 3");

    // Print "Tests valid/run: 3/3" and exit non-zero if a test failed
    return att_report();
}
```

The code does not output anything by default. But you can change this using:

```c
// By defining:
#define ATT_VERBOSE 1 // 0, 1, 2

// Or by calling:
att_set_verbose(1); // 0, 1, 2
```

With `verbose = 1` the code output `.` for a valid test and `F` for a failed test:

```
...
Tests valid/run: 3/3
```

With `verbose = 2` the code output:

```
OK [int] one == one
OK [float] 2.0 == 2.0
OK [long long] long long 3 == long long 3

Tests valid/run: 3/3
```

## Examples with errors

Failures are reported independently of the verbosity level: they are shown whenever `att_show_error`
is set (the default). Set `att_set_show_error(0)` if you really want complete silence.

If we change `var_to_test_2` in `1.0` this is the output with `verbose = 1`. Each failure is prefixed
with the source `file:line` and, at `verbose` 0 or 1, the test description, so it is self-contained:

```c
.F
test.c:7: 2.0 == 2.0: Expected 2.000000, got 1.000000

.
Tests valid/run: 2/3
```

With `verbose = 0` only the failures are printed (no `.` for the passing tests):

```
test.c:7: 2.0 == 2.0: Expected 2.000000, got 1.000000

Tests valid/run: 2/3
```

And with `verbose = 2` the description is already on the result line, so it is not repeated:

```
OK [int] one == one
NO [float] 2.0 == 2.0
tests/char.c:25: Expected 2.000000, got 1.000000

OK [long long] long long 3 == long long 3

Tests valid/run: 2/3
```

## Advance usage

Check [tests/test.c](tests/test.c) for an advanced example.

### Threading

The valid/total test counters are atomic, so `ATT_ASSERT` can be used from several threads at once
without losing updates. `att_get_valid_tests()` and `att_get_total_tests()` therefore report the
exact number of assertions run, even when tests execute concurrently.

### Strings

Attractor treats strings as strings. So the content of the string is output to the terminal as it is
in case of error. By defining `#define ATT_STRING_AS_POINTERS 1`, it will simply output the pointer
address.

### Floats

`float`, `double` and `long double` are compared for exact equality by default. Because computed
floating point results are rarely bit-exact, you can define a tolerance. Values are then considered
equal when their absolute difference is within `ATT_FLOAT_EPSILON`:

```c
// 1e-6 is usually enough
#define ATT_FLOAT_EPSILON 1e-6

ATT_ASSERT(0.1 + 0.2, 0.3, "sum within epsilon");
```

The default is `0` (exact equality). The compile-time `ATT_FLOAT_EPSILON` is just the initial value;
you can also change the tolerance at runtime, the same way you change verbosity:

```c
att_set_float_epsilon(1e-6);
ATT_ASSERT(0.1 + 0.2, 0.3, "sum within epsilon");
att_set_float_epsilon(0); // back to exact equality
```

### Custom callback

You can register a custom function to be called when a type is not one of the list.

```c
#include <attractor.h>

typedef struct unknown_struct {
    int unknown;
} unknown_struct;

int assert_unknown(void* result, void* expected, const char *description) {
    if(!result || !expected) {
        return 0;
    }

    return ((unknown_struct*)result)->unknown == ((unknown_struct*)expected)->unknown;
}

void test_unknown_callback() {
    unknown_struct sa;
    unknown_struct sb;

    sa.unknown = 1;
    sb.unknown = 1;

    att_set_generic_callback(&assert_unknown);
    ATT_ASSERT(&sa, &sb, "Unknown struct sa = sb");
    att_set_generic_callback(NULL);
}
```
