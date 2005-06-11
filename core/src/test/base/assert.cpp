#include "../testCommon.h"

class AssertTest : public CxxTest::TestSuite
{
public:
    void testSuccess()
    {
        GN_ASSERT(1);
    }

    void TestFailure()
    {
        TS_WARN( "TODO: implement this test!" );
    }
};
