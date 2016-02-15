#include "gputest.h"

class OGLGpuTest : public CxxTest::TestSuite, public GpuTest
{

public:

    OGLGpuTest() : GpuTest( GN::gfx::GpuAPI::OGL, false ) {}

    void testExternalWindow()
    {
        #if GN_PLATFORM_HAS_OGL
        externalWindow();
        #endif
    }
};
