#include "gputest.h"

class OGLGpuTest : public CxxTest::TestSuite, public GpuTest
{

public:

    OGLGpuTest() : GpuTest( GN::gfx::GpuAPI::OGL, false ) {}

    void testExternalWindow()
    {
        #if GN_BUILD_GPU_API_HAS_OGL
        externalWindow();
        #endif
    }
};
