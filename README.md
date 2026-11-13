# Attractor

Attractor is a lightweight single-header unit test framework written in C11, usable from C++17 (or
later) too.

## Installation

Attractor is a single-header library: copy `attractor.h` in your test folder. In exactly one .c/.cpp
source file define `ATT_IMPLEMENTATION` before including the header:

```c
// define ATT_IMPLEMENTATION for only *one* .c/.cpp
#define ATT_IMPLEMENTATION
#include "attractor.h"
```

Every other source file just includes `attractor.h`.

## Usage

Attractor provides an `ATT_ASSERT` macro that you can use to test your code. The macro is an
expression that evaluates to a number different from zero if the test passes:

```c
ATT_ASSERT(the_value_to_test, the_expected_value, "description");

// Or, being an expression:
if(!ATT_ASSERT(the_value_to_test, the_expected_value, "description")) {
    // React to the failure
}
```

The `ATT_ASSERT` macro accepts these types:

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
17. `const void*`
18. `_Bool`

In case of unknown type the input is assumed to be `const void*`.

Beyond equality, the `ATT_ASSERT_NE`, `ATT_ASSERT_LT`, `ATT_ASSERT_LE`, `ATT_ASSERT_GT` and
`ATT_ASSERT_GE` macros compare with the other operators, accepting the same types and keeping the
typed failure output (`Expected < 10, got 12`):

```c
ATT_ASSERT_NE(errno, 0, "errno was set");
ATT_ASSERT_LT(elapsed_ms, 100l, "fast enough");
ATT_ASSERT_GE(count, 1u, "at least one result");
```

Strings compare lexicographically (via `strcmp`), so `ATT_ASSERT_LT("abc", "abd", ...)` passes.
Floats keep the epsilon semantics for `NE` (values equal within the tolerance are not "not equal"),
while `LT`/`LE`/`GT`/`GE` compare plainly. `void*`, `_Bool` and unknown types support only equality
and inequality; the other operators fail with an `Operator < is not supported for this type`
message.

Two more assertion macros cover the common cases that are not a typed comparison:

```c
ATT_ASSERT_TRUE(is_valid(input), "input is valid");
ATT_ASSERT_MEM(buffer, expected_buffer, sizeof(buffer), "buffers are equal");
```

`ATT_ASSERT_TRUE` passes when the condition is true. `ATT_ASSERT_MEM` compares two memory regions
byte by byte and, on failure, reports the offset and value of the first differing byte.

## Test cases

Assertions can be grouped into named test cases. `ATT_TEST(name)` defines a test case function and
registers it automatically before `main()` runs:

```c
#define ATT_IMPLEMENTATION
#include "attractor.h"

ATT_TEST(math_add) {
    ATT_ASSERT(2 + 2, 4, "2 + 2 == 4");
}

ATT_TEST(strings_hello) {
    ATT_ASSERT("hello", "hello", "hello == hello");
}

int main(void) {
    att_run_tests(NULL);
    return att_report();
}
```

`att_run_tests(filter)` runs the registered cases whose name contains `filter`, or all of them when
`filter` is `NULL`. A test case fails when at least one of its assertions fails; failed cases are
listed in a recap and counted in a `Test cases valid/run: X/Y` summary. The function returns the
number of failed cases and composes with `att_report()`, which keeps printing the assertion summary
and producing the exit code:

```
$ ./test
Test: math_add .
Test: strings_hello .
Test cases valid/run: 2/2
Tests valid/run: 2/2
```

The library takes no command line options itself; the host program stays in charge of its `argv`.
`att_get_test_name(index)` enumerates the registered names (returning `NULL` past the end), which
is all a `--list` option needs. See [tests/test.c](tests/test.c) for a small runner implementing
`--list`/`-l` and `--filter <substring>`/`-f`/`--filter=<substring>` on top of these two calls.

For per-case setup or logging, `att_set_test_start_callback()` registers a callback that
`att_run_tests()` calls before each test case, passing the case name. Assertions made inside the
callback count toward the case that is about to run.

Registration happens through compiler constructors (a static initializer in C++,
`__attribute__((constructor))` on GCC/Clang, the CRT initializer section on MSVC compiling C). On a
C compiler with none of these, the header defines `ATT_TEST_MANUAL` and you register the cases
yourself with `att_register_test("name", att_test_fn_name)`. `att_register_test_data("name", fn,
data)` registers a `void fn(void *data)` case that is called with `data`, which is what bindings
from other languages need to register closures. Names are not copied. The registry holds `ATT_MAX_TESTS`
cases (512 by default); define a bigger value in the `ATT_IMPLEMENTATION` file if you need more. In
TAP mode each case is announced with a `# test: name` diagnostic line.

## API

1. `ATT_ASSERT(value, expected, description)` you pass the value to test, the expected one and a
   description of the test.
2. `ATT_ASSERT_NE/LT/LE/GT/GE(value, expected, description)` same as `ATT_ASSERT` but compare with
   `!=`, `<`, `<=`, `>` or `>=`.
3. `ATT_ASSERT_TRUE(condition, description)` assert that a condition is true.
4. `ATT_ASSERT_MEM(value, expected, size, description)` assert that two memory regions of `size`
   bytes are equal.
5. `int att_report(void)` print the `Tests valid/run: X/Y` summary and return `0` if every test
   passed, `1` otherwise. Meant to be used as the return value of `main()`.
6. `void att_set_verbose(unsigned int verbose)` set verbosity (0 to 4).
7. `void att_set_show_error(unsigned int show_error)` set to show errors.
8. `void att_set_generic_callback(att_generic_callback callback)` set a callback to run for generic
   tests
9. `void att_set_test_callback(att_test_callback callback)` set a callback to call for all tests.
10. `unsigned int att_get_valid_tests(void)` return the count of valid tests.
11. `unsigned int att_get_total_tests(void)` return the count of run tests.
12. `void att_set_float_epsilon(long double epsilon)` set the tolerance for float comparisons.
13. `long double att_get_float_epsilon(void)` return the current float comparison tolerance.
14. `void att_set_tap(unsigned int tap)` enable TAP (Test Anything Protocol) output.
15. `ATT_TEST(name)` define a named test case and register it automatically.
16. `int att_run_tests(const char *filter)` run the registered test cases whose name contains
    `filter` (all of them when `NULL`); return the number of failed cases.
17. `unsigned int att_get_test_count(void)` return the count of registered test cases.
18. `const char *att_get_test_name(unsigned int index)` return the name of the test case at
    `index`, or `NULL` when out of range.
19. `void att_register_test(const char *name, att_test_fn fn)` register a test case manually.
20. `void att_set_test_start_callback(att_test_start_callback callback)` set a callback to call
    before each test case run by `att_run_tests`.
21. `void att_register_test_data(const char *name, att_test_data_fn fn, void *data)` register a
    test case that is called with `data`.
22. `void att_set_double_epsilon(double epsilon)` and `double att_get_double_epsilon(void)` the
    float tolerance as a `double`, for callers without a `long double` type.
23. `void att_set_output_callback(att_output_callback callback, void *data)` send the output to a
    callback instead of stdout.

## Examples

```c
#define ATT_IMPLEMENTATION
#include "attractor.h"

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

The default verbosity is `2`. Each level adds output on top of the previous one:

- `0` prints only the failures.
- `1` adds the test case names, printed by `att_run_tests()` before each case.
- `2` adds a `.` per valid assertion and an `F` per failed one, on the same line as the case name.
- `3` prints a `OK`/`NO` line per assertion instead of the dots.
- `4` behaves like 3 and additionally closes every test case with its own `Valid/run: X/Y`
  summary counting only that case's assertions.

You can change the level using:

```c
// By defining:
#define ATT_VERBOSE 2 // 0, 1, 2, 3, 4

// Or by calling:
att_set_verbose(2); // 0, 1, 2, 3, 4
```

Outside of `att_run_tests()` there are no case names, so levels 0 and 1 print only the failures and
level 2 prints the plain dots:

```
...
Tests valid/run: 3/3
```

With `verbose = 3` the code outputs:

```
OK [int] one == one
OK [float] 2.0 == 2.0
OK [long long] long long 3 == long long 3

Tests valid/run: 3/3
```

And this is `verbose = 4` with two test cases:

```
Test: math_add
OK [int] 2 + 2 == 4
Valid/run: 1/1

Test: strings_hello
OK [char *] hello == hello
Valid/run: 1/1

Test cases valid/run: 2/2
Tests valid/run: 2/2
```

## Examples with errors

Failures are reported independently of the verbosity level: they are shown whenever `att_show_error`
is set (the default). Set `att_set_show_error(0)` if you really want complete silence.

If we change `var_to_test_2` in `1.0` this is the output with `verbose = 2`. Each failure is prefixed
with the source `file:line` and, below `verbose` 3, the test description, so it is self-contained:

```
.F
test.c:9: 2.0 == 2.0: Expected 2, got 1

.
Tests valid/run: 2/3
```

With `verbose = 0` only the failures are printed (no `.` for the passing tests):

```
test.c:9: 2.0 == 2.0: Expected 2, got 1

Tests valid/run: 2/3
```

And with `verbose = 3` the description is already on the result line, so it is not repeated:

```
OK [int] one == one
NO [float] 2.0 == 2.0
test.c:9: Expected 2, got 1

OK [long long] long long 3 == long long 3
Tests valid/run: 2/3
```

## Advanced usage

Check the [tests](tests) folder for advanced examples: every file defines its test cases with
`ATT_TEST` and [tests/test.c](tests/test.c) is the runner. The failure messages are tested by
[tests/golden/failures.c](tests/golden/failures.c), a program whose assertions all fail: `make test`
compares its output with [tests/golden/failures.expected](tests/golden/failures.expected).

### Threading

The valid/total test counters are atomic, so `ATT_ASSERT` can be used from several threads at once
without losing updates. `att_get_valid_tests()` and `att_get_total_tests()` therefore report the
exact number of assertions run, even when tests execute concurrently. `att_run_tests()` itself runs
the registered test cases sequentially, on the calling thread.

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

### TAP output

For machine-readable output, enable TAP (Test Anything Protocol) mode with `#define ATT_TAP 1` or
`att_set_tap(1)`. It replaces the dot/OK reporting: every assertion prints an `ok N - description`
or `not ok N - description` line, error details become `# ` diagnostic lines, and `att_report()`
emits the trailing `1..N` plan. The output plugs directly into `prove` and other TAP harnesses:

```
ok 1 - one == one
not ok 2 - 2.0 == 2.5
# example.c:12: Expected 2.5, got 2
1..2
# Tests valid/run: 1/2
```

### Output callback

By default everything is printed to stdout. `att_set_output_callback(callback, data)` sends the
output to a function instead, for example to capture it or to print it through another runtime's
own stdout. The callback receives fragments of text, not necessarily whole lines:

```c
static void to_stderr(const char *text, void *data) {
    fputs(text, stderr);
}

att_set_output_callback(to_stderr, NULL);
```

Pass `NULL` to print to stdout again.

### Custom callback

You can register a custom function to be called when a type is not one of the list.

```c
#include "attractor.h"

typedef struct unknown_struct {
    int unknown;
} unknown_struct;

int assert_unknown(void* result, void* expected, att_op op, const char *description) {
    if(!result || !expected) {
        return 0;
    }

    int equal = ((unknown_struct*)result)->unknown == ((unknown_struct*)expected)->unknown;

    // The callback receives the operator, so ATT_ASSERT_NE works too
    return op == ATT_OP_NE ? !equal : equal;
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
