// Tests for the assert and do-once macro families in garnet/base/debug.h and garnet/base/misc.h.
//
// These are mostly compile-time assertions. The build treats warnings as errors, so an expansion
// that nested an if/else inside an if would fail to compile here with -Wdangling-else. The runtime
// checks confirm that a following else still binds to the outer if rather than being swallowed.
//
// Every condition tested is deliberately true: GN_REQUIRE fires in all build configurations, and a
// firing assert would prompt or abort.

#include <catch2/catch_test_macros.hpp>

#include <garnet/GNbase.h>

#include <string>
#include <vector>

TEST_CASE("assert macros expand to a single statement", "[debug]") {
    const bool yes = true;
    const bool no  = false;
    int        taken;

    SECTION("GN_ASSERT in an unbraced if body, with a following else") {
        taken = 0;
        if (yes)
            GN_ASSERT(yes);
        else
            taken = 1;
        CHECK(taken == 0);

        if (no)
            GN_ASSERT(yes);
        else
            taken = 2;
        CHECK(taken == 2);
    }

    SECTION("GN_ASSERT with a message") {
        // The message is a single string, not a format argument list.
        if (yes)
            GN_ASSERT(yes, "a literal message");
        else
            FAIL("else branch bound to the wrong if");

        // ... and the std::string overload, which is how a formatted message is passed
        if (yes)
            GN_ASSERT(yes, fmt::format("a formatted message: {}", 1));
        else
            FAIL("else branch bound to the wrong if");
    }

    SECTION("GN_REQUIRE in an unbraced if body") {
        // GN_REQUIRE asserts in every build configuration, not just debug.
        taken = 0;
        if (yes)
            GN_REQUIRE(yes);
        else
            taken = 1;
        CHECK(taken == 0);
    }

    SECTION("unbraced loop and switch bodies") {
        int count = 0;
        for (int i = 0; i < 3; ++i) GN_ASSERT(yes);
        while (no) GN_ASSERT(no);
        switch (1) {
        case 1:
            GN_ASSERT(yes);
            ++count;
            break;
        default:
            break;
        }
        CHECK(count == 1);
    }
}

TEST_CASE("do-once macros expand to a single statement", "[debug]") {
    const bool yes = true;
    const bool no  = false;

    SECTION("GN_DO_LIMITED_TIMES runs exactly n times") {
        int count = 0;
        for (int i = 0; i < 10; ++i) GN_DO_LIMITED_TIMES(3, ++count);
        CHECK(count == 3);
    }

    SECTION("GN_DO_ONCE in an unbraced if body, with a following else") {
        int once  = 0;
        int other = 0;
        for (int i = 0; i < 5; ++i) {
            if (yes)
                GN_DO_ONCE(++once);
            else
                ++other;
        }
        CHECK(once == 1);
        CHECK(other == 0);

        for (int i = 0; i < 5; ++i) {
            if (no)
                GN_DO_ONCE(++once);
            else
                ++other;
        }
        CHECK(once == 1);
        CHECK(other == 5);
    }

    SECTION("GN_TODO and GN_UNIMPL_WARNING in unbraced if bodies") {
        // These reach a logger, so they also cover the trailing semicolons that used to be baked
        // into their definitions -- those would have orphaned the else below.
        int taken = 0;
        if (yes)
            GN_TODO("a todo");
        else
            taken = 1;
        CHECK(taken == 0);

        if (no)
            GN_UNIMPL_WARNING();
        else
            taken = 2;
        CHECK(taken == 2);
    }
}
