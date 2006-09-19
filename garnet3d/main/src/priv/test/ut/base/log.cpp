#include "../testCommon.h"

class LogTest : public CxxTest::TestSuite
{
public:
    void testLogMacro()
    {
        GN_ERROR(sLogger)("error");
        GN_WARN(sLogger)("warn");
        GN_INFO(sLogger)("info");
        GN_ERROR(sLogger)(NULL);
    }
};
