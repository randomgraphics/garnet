#include "common.h"

class GfxOGLTest : public CxxTest::TestSuite, public GfxTest
{

public:

    void setUp() { libInit("OGL"); }

    void tearDown() { libFree(); }

    //void testMultiRenderer() { multiRenderer(); }
};
