# Attractor

Attractor is a single function unit test framework written in C11.

## Installation

Copy `attractor.c` and `attractor.h` in your test folder.

## Usage

Attractor provide a `ATT_ASSERT` macro that you can use to test your code. The macro outputs a
number different from zero if the test passes:

```c
ATT_ASSERT(the_value_to_test, the_expected_value, "description")
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

In case of unknown type the input is assumed to be `int`.

## API

1. `ATT_ASSERT(value, expected, description)` you pass the value to test, the expected one and a
   description of the test.
2. `unsigned int att_get_valid_tests(void)` return the count of valid tests.
3. `unsigned int att_get_total_tests(void)` return the count of run tests.
4. `void att_set_verbose(unsigned int verbose)` set the verbose.

## Examples

```c
#include <attractor.h>

int var_to_test_1 = 1;
float var_to_test_2 = 2.0;

ATT_ASSERT(var_to_test_1, 1, "one == one")
ATT_ASSERT(var_to_test_2, 2.0, "2.0 == 2.0")
ATT_ASSERT(3ll, 3ll, "long long 3 == long long 3")

printf("\nTests valid/run: %d/%d\n", att_get_valid_tests(), att_get_total_tests());
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
[%d] one == one:                                                              OK
[%f] 2.0 == 2.0:                                                              OK
[%lld] long long 3 == long long 3:                                            OK

Tests valid/run: 3/3
```

## Examples with errors

If we change `var_to_test_2` in `1.0` this is the output with `verbose = 1`:

```c
.F
Expected 2.000000, got 3.000000

.
Tests valid/run: 2/3
```

And with `verbose = 2`:

```
[%d] one == one:                                                              OK
[%f] 2.0 == 2.0:                                                            FAIL
Expected 2.000000, got 3.000000

[%lld] long long 3 == long long 3:                                            OK

Tests valid/run: 2/3
```

## Advance usage

Attractor treats strings as strings. So the content of the string is output to the terminal as it is
in case of error. By defining `#define ATT_STRING_AS_POINTERS 1`, it will simply output the pointer
address.

Check [tests/test.c](tests/test.c) for an advanced example that uses multi-threaded tests.
