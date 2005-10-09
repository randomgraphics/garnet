#include "../testCommon.h"

class LogTest : public CxxTest::TestSuite
{
public:
    void testLogMacro()
    {
        GN_ERROR("error");
        GN_WARN("warn");
        GN_INFO("info");
        GN_ERROR(NULL);
        GN::doLog( (GN::LogLevel)100, 0, "haha", 0, 0, 0 );
    }
};
