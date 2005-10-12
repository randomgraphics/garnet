#include "common.h"

class GfxOGLTest : public CxxTest::TestSuite, public GfxTest
{

public:

    void setUp() { oglInit(); }

    void tearDown() { libFree(); }

    //void testMultiRenderer() { multiRenderer(); }
};
