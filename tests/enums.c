/**
 * The Attractor Unit Test library
 *
 * This file is distributed under the MIT License. See LICENSE for details.
 */

#include "../attractor.h"

typedef enum enum_color { ENUM_RED, ENUM_GREEN, ENUM_BLUE } enum_color;

typedef enum enum_signed { ENUM_MINUS = -1, ENUM_ZERO, ENUM_PLUS } enum_signed;

#ifdef __cplusplus
enum class enum_scoped { first, second, third };

enum class enum_small : unsigned char { low = 1, high = 255 };

enum class enum_big : unsigned long long { max = 18446744073709551615ull };
#endif

static enum_color enum_next(enum_color color) {
    return color == ENUM_BLUE ? ENUM_RED : (enum_color)(color + 1);
}

// Enumerations are compared by their value, in C (where they are integers) and in C++.
ATT_TEST(enums) {
    enum_color green = ENUM_GREEN;
    enum_signed minus = ENUM_MINUS;

    ATT_ASSERT(green, ENUM_GREEN, "green = ENUM_GREEN");
    ATT_ASSERT(ENUM_GREEN, green, "ENUM_GREEN = green");
    ATT_ASSERT(green, green, "green = green");
    ATT_ASSERT(enum_next(green), ENUM_BLUE, "next(green) = ENUM_BLUE");
    ATT_ASSERT_NE(green, ENUM_RED, "green != ENUM_RED");
    ATT_ASSERT_GT(green, ENUM_RED, "green > ENUM_RED");
    ATT_ASSERT_LE(green, ENUM_BLUE, "green <= ENUM_BLUE");

    // Against an integer.
    ATT_ASSERT(green, 1, "green = 1");
    ATT_ASSERT(1, green, "1 = green");
    ATT_ASSERT_LT(green, 2u, "green < 2u");

    ATT_ASSERT(minus, ENUM_MINUS, "minus = ENUM_MINUS");
    ATT_ASSERT(minus, -1, "minus = -1");
    ATT_ASSERT_LT(minus, ENUM_ZERO, "minus < ENUM_ZERO");
    ATT_ASSERT_LT(minus, 1u, "minus < 1u");

#ifdef __cplusplus
    enum_scoped second = enum_scoped::second;

    ATT_ASSERT(second, enum_scoped::second, "second = enum_scoped::second");
    ATT_ASSERT(enum_scoped::second, second, "enum_scoped::second = second");
    ATT_ASSERT_NE(second, enum_scoped::first, "second != enum_scoped::first");
    ATT_ASSERT_GT(second, enum_scoped::first, "second > enum_scoped::first");
    ATT_ASSERT_LE(second, enum_scoped::third, "second <= enum_scoped::third");
    ATT_ASSERT(second, 1, "second = 1");
    ATT_ASSERT(1, second, "1 = second");

    // The underlying type is kept: no truncation and no sign change.
    ATT_ASSERT(enum_small::high, enum_small::high, "enum_small::high = enum_small::high");
    ATT_ASSERT(enum_small::high, 255, "enum_small::high = 255");
    ATT_ASSERT_GT(enum_small::high, enum_small::low, "enum_small::high > enum_small::low");
    ATT_ASSERT(enum_big::max, enum_big::max, "enum_big::max = enum_big::max");
    ATT_ASSERT_NE(enum_big::max, -1, "enum_big::max != -1");
#endif
}
