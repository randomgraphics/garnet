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
    }

    void testInvalidLogArgument()
    {
        GN::LogDesc desc;
        desc.level = (GN::LogLevel)100;
        desc.cate = 0;
        desc.func = 0;
        desc.file = 0;
        desc.line = 0;
        GN::doLog( desc, "haha" );
    }
};
