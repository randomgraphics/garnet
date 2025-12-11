#include "../testCommon.h"

class LogTest : public CxxTest::TestSuite {
public:
    // A simple log receiver to receive test log message
    struct TestLogReceiver : public GN::Logger::Receiver {
        GN::StrA     message;
        GN::StrW     wmessage;
        virtual void onLog(GN::Logger &, const GN::Logger::LogLocation &, const char * msg) { message = msg; }
        virtual void onLog(GN::Logger &, const GN::Logger::LogLocation &, const wchar_t * msg) { wmessage = msg; }
    };
    TestLogReceiver mTestLogReceiver;

    LogTest() {
        // register test log receiver
        GN::getRootLogger()->addReceiver(&mTestLogReceiver);
    }

    ~LogTest() {
        // unregister test log receiver
        GN::getRootLogger()->removeReceiver(&mTestLogReceiver);
    }

    void testLogMacro() {
        GN_ERROR(sLogger)("error");
        GN_WARN(sLogger)("warn");
        GN_INFO(sLogger)("info");
        GN_ERROR(sLogger)(L"unicode error");
        GN_WARN(sLogger)(L"unicode warn");
        GN_INFO(sLogger)(L"unicode info");
    }

#define TEST_PRINTF GN_PRINTF_EX(sLogger, GN::Logger::INFO, GN_FUNCTION, __FILE__, __LINE__)

    // Test printf variants for the log macro, verify that the log message is properly formatted by checking the receiver.
    void testLogPrintf() {
        // char version
        TEST_PRINTF("error: value=%d, string=%s", 42, "hello");
        TS_ASSERT_EQUALS(mTestLogReceiver.message, "error: value=42, string=hello");

        TEST_PRINTF("warn: pi=%.2f", 3.1415);
        TS_ASSERT_EQUALS(mTestLogReceiver.message, "warn: pi=3.14");

        TEST_PRINTF("info: hex=%#x", 0xABCD);
        TS_ASSERT_EQUALS(mTestLogReceiver.message, "info: hex=0xabcd");

        // wchar_t version
        TEST_PRINTF(L"unicode error: value=%d, str=%s", 88, L"world");
        TS_ASSERT_EQUALS(mTestLogReceiver.wmessage, L"unicode error: value=88, str=world");

        TEST_PRINTF(L"unicode warn: id=%08d", 123);
        TS_ASSERT_EQUALS(mTestLogReceiver.wmessage, L"unicode warn: id=00000123");

        TEST_PRINTF(L"unicode info: %s, val=%f", L"number", 9.81);
        TS_ASSERT_EQUALS(mTestLogReceiver.wmessage, L"unicode info: number, val=9.810000");
    }
};
