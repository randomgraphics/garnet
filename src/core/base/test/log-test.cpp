// Tests for the GN log macros and GN::Logger::LogHelper.
//
// Ported from the retiring CxxTest suite at src/test/ut/base/log.cpp, which contributed
// testLogMacro, testLogPrintf and testLogStringFormatter, and extended to cover the
// comma-parameter-list / streaming / mixed call forms and the log-is-off guarantees.

#include <catch2/catch_test_macros.hpp>

#include <garnet/GNbase.h>

#include <string>
#include <type_traits>
#include <vector>

namespace {

/// Captures everything reported through the logger under test.
struct Capture : GN::Logger::Receiver {
    std::vector<std::string>  narrow;
    std::vector<std::wstring> wide;

    void onLog(GN::Logger &, const GN::Logger::LogLocation &, const char * msg) override { narrow.push_back(msg); }
    void onLog(GN::Logger &, const GN::Logger::LogLocation &, const wchar_t * msg) override { wide.push_back(msg); }

    void clear() {
        narrow.clear();
        wide.clear();
    }
};

/// Attaches a capture to a logger of its own, so that changing the level here cannot disturb
/// other tests. The level is restored on teardown.
struct LogFixture {
    GN::Logger * logger = GN::getLogger("GN.test.logmacro");
    Capture      capture;
    const int    savedLevel;

    LogFixture(): savedLevel(logger->getLevel()) {
        logger->addReceiver(&capture);
        logger->setEnabled(true);
        logger->setLevel(GN::Logger::BABBLE);
    }

    ~LogFixture() {
        logger->removeReceiver(&capture);
        logger->setLevel(savedLevel);
    }

    /// The single message that was reported. Fails the test if there was not exactly one.
    const std::string & onlyNarrow() const {
        REQUIRE(capture.narrow.size() == 1);
        return capture.narrow[0];
    }

    const std::wstring & onlyWide() const {
        REQUIRE(capture.wide.size() == 1);
        return capture.wide[0];
    }
};

int gEvaluations = 0;

/// Stands in for a format argument with a side effect, to prove a disabled log does not touch it.
int countedValue(int value) {
    ++gEvaluations;
    return value;
}

/// Stands in for a logger expression with a side effect, e.g. GN_ERROR(getLogger("name"), ...).
GN::Logger * countedLogger(GN::Logger * logger) {
    ++gEvaluations;
    return logger;
}

} // namespace

TEST_CASE("log macro: comma parameter list", "[log]") {
    LogFixture f;

    SECTION("no format arguments") {
        GN_ERROR(f.logger, "plain message");
        CHECK(f.onlyNarrow() == "plain message");
    }

    SECTION("fmt brace syntax") {
        GN_ERROR(f.logger, "a = {}, b = {}", 1, "two");
        CHECK(f.onlyNarrow() == "a = 1, b = two");
    }

    SECTION("brace escape is not a replacement field") {
        GN_ERROR(f.logger, "{{literal}}");
        CHECK(f.onlyNarrow() == "{literal}");
    }

    SECTION("many arguments") {
        GN_ERROR(f.logger, "{}{}{}{}{}{}{}{}{}{}{}{}{}{}", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14);
        CHECK(f.onlyNarrow() == "1234567891011121314");
    }

    SECTION("message longer than the small string optimization") {
        GN_ERROR(f.logger, "{}", std::string(200, 'x'));
        CHECK(f.onlyNarrow() == std::string(200, 'x'));
    }

    SECTION("wide") {
        GN_WARN(f.logger, L"a = {}", 3);
        CHECK(f.onlyWide() == L"a = 3");
    }

    SECTION("every level macro reports") {
        GN_FATAL(f.logger, "fatal");
        GN_ERROR(f.logger, "error");
        GN_WARN(f.logger, "warn");
        GN_INFO(f.logger, "info");
        GN_VERBOSE(f.logger, "verbose");
        GN_BABBLE(f.logger, "babble");
        CHECK(f.capture.narrow == std::vector<std::string> {"fatal", "error", "warn", "info", "verbose", "babble"});
    }
}

TEST_CASE("log macro: printf syntax", "[log]") {
    LogFixture f;

    SECTION("narrow") {
        GN_INFO(f.logger, "error: value=%d, string=%s", 42, "hello");
        CHECK(f.onlyNarrow() == "error: value=42, string=hello");
    }

    SECTION("narrow, precision") {
        GN_INFO(f.logger, "warn: pi=%.2f", 3.1415);
        CHECK(f.onlyNarrow() == "warn: pi=3.14");
    }

    SECTION("narrow, alternate form") {
        GN_INFO(f.logger, "info: hex=%#x", 0xABCD);
        CHECK(f.onlyNarrow() == "info: hex=0xabcd");
    }

    SECTION("wide") {
        GN_INFO(f.logger, L"unicode error: value=%d, str=%s", 88, L"world");
        CHECK(f.onlyWide() == L"unicode error: value=88, str=world");
    }

    SECTION("wide, zero padding") {
        GN_INFO(f.logger, L"unicode warn: id=%08d", 123);
        CHECK(f.onlyWide() == L"unicode warn: id=00000123");
    }

    SECTION("wide, string and float") {
        GN_INFO(f.logger, L"unicode info: %s, val=%f", L"number", 9.81);
        CHECK(f.onlyWide() == L"unicode info: number, val=9.810000");
    }

    SECTION("GN_PRINTF_EX forces printf even for a brace-bearing string") {
        GN_PRINTF_EX(f.logger, GN::Logger::INFO, GN_FUNCTION, __FILE__, __LINE__, "%d {}", 7);
        CHECK(f.onlyNarrow() == "7 {}");
    }
}

TEST_CASE("log macro: printf and fmt syntax resolution", "[log]") {
    LogFixture f;

    SECTION("a conversion specifier and no brace field selects printf") {
        GN_ERROR(f.logger, "a = %d", 1);
        CHECK(f.onlyNarrow() == "a = 1");
    }

    SECTION("a brace field wins when the string carries both") {
        // Ambiguous input. fmt is chosen, so %d stays literal and only {} is substituted.
        GN_ERROR(f.logger, "{} %d", 1);
        CHECK(f.onlyNarrow() == "1 %d");
    }

    SECTION("a %% escape counts as printf syntax and is unescaped") {
        GN_ERROR(f.logger, "100%% done");
        CHECK(f.onlyNarrow() == "100% done");
        f.capture.clear();
        // ... matching what the explicit printf macro does
        GN_PRINTF_EX(f.logger, GN::Logger::ERROR_, GN_FUNCTION, __FILE__, __LINE__, "100%% done");
        CHECK(f.onlyNarrow() == "100% done");
    }

    SECTION("a brace field still wins over a lone %% escape") {
        // fmt does not unescape %%, so it stays doubled.
        GN_ERROR(f.logger, "{}%% done", 50);
        CHECK(f.onlyNarrow() == "50%% done");
    }

    SECTION("a trailing bare percent is not a conversion specifier") {
        GN_ERROR(f.logger, "50 percent %");
        CHECK(f.onlyNarrow() == "50 percent %");
    }
}

TEST_CASE("log macro: streaming form", "[log]") {
    LogFixture f;

    SECTION("streaming only") {
        GN_INFO(f.logger) << "x = " << 42 << ", y = " << 3.5;
        CHECK(f.onlyNarrow() == "x = 42, y = 3.5");
    }

    SECTION("a logger with no message reports nothing") {
        GN_ERROR(f.logger);
        CHECK(f.capture.narrow.empty());
        CHECK(f.capture.wide.empty());
    }
}

TEST_CASE("log macro: comma parameter list and streaming combined", "[log]") {
    LogFixture f;

    // Regression guard. emit() must not report on its own: a streaming tail may still follow, and
    // reporting early splits one statement into two log entries.
    SECTION("fmt prefix then streamed tail is one message") {
        GN_ERROR(f.logger, "a = {}", 1) << ", b = " << 2;
        CHECK(f.onlyNarrow() == "a = 1, b = 2");
    }

    SECTION("printf prefix then streamed tail is one message") {
        GN_ERROR(f.logger, "a = %d", 1) << ", b = " << 2;
        CHECK(f.onlyNarrow() == "a = 1, b = 2");
    }

    SECTION("several streamed operands after the format prefix") {
        GN_ERROR(f.logger, "a = {}", 1) << ", b = " << 2 << ", c = " << 3;
        CHECK(f.onlyNarrow() == "a = 1, b = 2, c = 3");
    }

    SECTION("format prefix longer than the small string optimization is preserved") {
        GN_ERROR(f.logger, "{}", std::string(200, 'x')) << "|tail";
        CHECK(f.onlyNarrow() == std::string(200, 'x') + "|tail");
    }

    SECTION("GN_PRINTF_EX also composes with a streamed tail") {
        GN_PRINTF_EX(f.logger, GN::Logger::INFO, GN_FUNCTION, __FILE__, __LINE__, "a = %d", 1) << ", b = " << 2;
        CHECK(f.onlyNarrow() == "a = 1, b = 2");
    }

    SECTION("a wide format string is terminal, by design") {
        // The accumulators are narrow, so folding a wide message into them would need a lossy
        // conversion. Returning void makes `GN_ERROR(logger, L"..") << x` a compile error rather than
        // silently producing something wrong. No call site mixes the two.
        GN::Logger::LogHelper helper(f.logger, GN::Logger::INFO, GN_FUNCTION, __FILE__, __LINE__);
        static_assert(std::is_void_v<decltype(helper.emit(L"wide"))>);
        static_assert(std::is_void_v<decltype(helper.emitPrintf(L"wide"))>);
        static_assert(std::is_same_v<decltype(helper.emit("narrow")), GN::Logger::LogHelper &>);
        static_assert(std::is_same_v<decltype(helper.emit()), GN::Logger::LogHelper &>);
    }
}

TEST_CASE("log macro: nothing happens when the level is off", "[log]") {
    LogFixture f;
    f.logger->setLevel(GN::Logger::FATAL); // ERROR and below are off

    SECTION("no message is reported") {
        GN_VERBOSE(f.logger, "hidden");
        GN_VERBOSE(f.logger) << "hidden";
        GN_VERBOSE(f.logger, "hidden") << " tail";
        CHECK(f.capture.narrow.empty());
        CHECK(f.capture.wide.empty());
    }

    SECTION("format arguments are not evaluated") {
        gEvaluations = 0;
        GN_VERBOSE(f.logger, "hidden {}", countedValue(1));
        CHECK(gEvaluations == 0);
    }

    SECTION("streamed operands are not evaluated") {
        gEvaluations = 0;
        GN_VERBOSE(f.logger) << "hidden " << countedValue(1);
        CHECK(gEvaluations == 0);
    }

    SECTION("a disabled logger is off at every level") {
        f.logger->setLevel(GN::Logger::BABBLE);
        f.logger->setEnabled(false);
        gEvaluations = 0;
        GN_FATAL(f.logger, "hidden {}", countedValue(1));
        CHECK(gEvaluations == 0);
        CHECK(f.capture.narrow.empty());
        f.logger->setEnabled(true);
    }
}

TEST_CASE("log macro: the logger expression is evaluated exactly once", "[log]") {
    LogFixture f;

    SECTION("when the level is on") {
        gEvaluations = 0;
        GN_ERROR(countedLogger(f.logger), "message");
        CHECK(gEvaluations == 1);
        CHECK(f.onlyNarrow() == "message");
    }

    SECTION("when the level is off") {
        f.logger->setLevel(GN::Logger::FATAL);
        gEvaluations = 0;
        GN_VERBOSE(countedLogger(f.logger), "message");
        CHECK(gEvaluations == 1);
        CHECK(f.capture.narrow.empty());
    }
}

TEST_CASE("log macro: expands to a single statement", "[log]") {
    LogFixture f;

    // Compiling this file at all is most of the assertion: an expansion that nested an if/else
    // inside an if would trip -Wdangling-else, which is an error in this build. These cases also
    // check that a following else binds to the outer if rather than being swallowed.
    const bool yes = true;
    const bool no  = false;

    SECTION("if/else, comma parameter list") {
        if (yes)
            GN_ERROR(f.logger, "then");
        else
            GN_ERROR(f.logger, "else");
        CHECK(f.onlyNarrow() == "then");
        f.capture.clear();
        if (no)
            GN_ERROR(f.logger, "then");
        else
            GN_ERROR(f.logger, "else");
        CHECK(f.onlyNarrow() == "else");
    }

    SECTION("if/else, streaming") {
        if (yes)
            GN_ERROR(f.logger) << "then";
        else
            GN_ERROR(f.logger) << "else";
        CHECK(f.onlyNarrow() == "then");
    }

    SECTION("unbraced loop and switch bodies") {
        for (int i = 0; i < 2; ++i) GN_INFO(f.logger, "loop {}", i);
        while (no) GN_INFO(f.logger, "never");
        switch (1) {
        case 1:
            GN_INFO(f.logger, "switch");
            break;
        default:
            GN_INFO(f.logger, "default");
            break;
        }
        CHECK(f.capture.narrow == std::vector<std::string> {"loop 0", "loop 1", "switch"});
    }

    SECTION("GN_DO_ONCE in an unbraced if body") {
        // GN_DO_LIMITED_TIMES carried the same dangling-else defect and was fixed alongside.
        for (int i = 0; i < 3; ++i) {
            if (yes)
                GN_DO_ONCE(GN_INFO(f.logger, "once"));
            else
                GN_INFO(f.logger, "never");
        }
        CHECK(f.capture.narrow == std::vector<std::string> {"once"});
    }
}

TEST_CASE("log macro: string types are accepted without .data() or .c_str()", "[log]") {
    LogFixture f;

    const std::string  stdNarrow = "std::string";
    const std::wstring stdWide   = L"std::wstring";
    const GN::StrA     gnNarrow  = "StrA";
    const GN::StrW     gnWide    = L"StrW";

    SECTION("std::string") {
        GN_ERROR(f.logger, "value = {}", stdNarrow);
        CHECK(f.onlyNarrow() == "value = std::string");
    }

    SECTION("std::wstring") {
        GN_ERROR(f.logger, L"value = {}", stdWide);
        CHECK(f.onlyWide() == L"value = std::wstring");
    }

    SECTION("GN::StrA") {
        GN_ERROR(f.logger, "value = {}", gnNarrow);
        CHECK(f.onlyNarrow() == "value = StrA");
    }

    SECTION("GN::StrW in a wide format string") {
        // This did not work before: fmt::formatter<GN::StrW> was specialized on the narrow character
        // type, leaving GN::StrW unformattable inside a wide format string.
        GN_ERROR(f.logger, L"value = {}", gnWide);
        CHECK(f.onlyWide() == L"value = StrW");
    }

    SECTION("GN::Str is formatted as a string, not as a range of characters") {
        GN_ERROR(f.logger, "{}", gnNarrow);
        CHECK(f.onlyNarrow() == "StrA");
    }

    SECTION("streamed") {
        GN_ERROR(f.logger) << stdNarrow << "/" << gnNarrow;
        CHECK(f.onlyNarrow() == "std::string/StrA");
    }
}

TEST_CASE("StringFormatter chooses between its preallocated buffer and the heap", "[log]") {
    // make sure StringFormatter is using preallocated buffer for small strings
    GN::internal::StringFormatter<char> formatter("Hello, {}! The answer is {}.", "World", 42);
    CHECK(formatter.isPreallocated());
    CHECK(std::string(formatter.result()) == "Hello, World! The answer is 42.");

    // when string is too long, it should fall back to dynamic allocation
    GN::internal::StringFormatter<char, 16> longFormatter("This is a long string: {} {} {}.", 1, 2, 3);
    CHECK(!longFormatter.isPreallocated());
    CHECK(std::string(longFormatter.result()) == "This is a long string: 1 2 3.");

    // do the same test for wchar_t version
    GN::internal::StringFormatter<wchar_t> wformatter(L"Hello, {}! The answer is {}.", L"World", 42);
    CHECK(wformatter.isPreallocated());
    CHECK(std::wstring(wformatter.result()) == L"Hello, World! The answer is 42.");

    GN::internal::StringFormatter<wchar_t, 16> wlongFormatter(L"This is a long string: {} {} {}.", 1, 2, 3);
    CHECK(!wlongFormatter.isPreallocated());
    CHECK(std::wstring(wlongFormatter.result()) == L"This is a long string: 1 2 3.");
}

TEST_CASE("log macro: format string syntax detection helpers", "[log]") {
    using GN::internal::hasFmtField;
    using GN::internal::hasPrintfSpecifier;

    CHECK(hasPrintfSpecifier("a = %d"));
    CHECK(hasPrintfSpecifier("%-8.3f"));
    CHECK(hasPrintfSpecifier("%*d"));
    CHECK(hasPrintfSpecifier("%llu"));
    CHECK(hasPrintfSpecifier("100%%"));
    CHECK(!hasPrintfSpecifier("a = {}"));
    CHECK(!hasPrintfSpecifier("50 percent %"));
    CHECK(!hasPrintfSpecifier(""));
    CHECK(!hasPrintfSpecifier<char>(nullptr));

    CHECK(hasFmtField("a = {}"));
    CHECK(hasFmtField("{:>10}"));
    CHECK(!hasFmtField("{{escaped}}"));
    CHECK(!hasFmtField("a = %d"));
    CHECK(!hasFmtField(""));
    CHECK(!hasFmtField<char>(nullptr));

    // the wide overloads share the implementation, so one spot check each is enough
    CHECK(hasPrintfSpecifier(L"a = %d"));
    CHECK(hasFmtField(L"a = {}"));
}
