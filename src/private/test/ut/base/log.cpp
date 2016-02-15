#include "../testCommon.h"

class LogTest : public CxxTest::TestSuite
{
public:
    void testLogMacro()
    {
        GN_ERROR(sLogger)("error");
        GN_WARN(sLogger)("warn");
        GN_INFO(sLogger)("info");
        GN_ERROR(sLogger)(L"unicode error");
        GN_WARN(sLogger)(L"unicode warn");
        GN_INFO(sLogger)(L"unicode info");
        GN_ERROR(sLogger)((const char*)NULL);
        GN_ERROR(sLogger)((const wchar_t*)NULL);
    }
};
