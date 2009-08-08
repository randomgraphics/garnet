#include "gputest.h"

class D3D10GpuTest : public CxxTest::TestSuite, public GpuTest
{

public:

    D3D10GpuTest() : GpuTest( GN::gfx::GpuAPI::D3D10, false ) {}

    void testExternalWindow()
    {
        #if GN_BUILD_GPU_API_HAS_D3D10
        externalWindow();
        #endif
    }
};
