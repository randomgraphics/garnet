#include "gputest.h"

class D3D11GpuTest : public CxxTest::TestSuite, public GpuTest
{

public:

    D3D11GpuTest() : GpuTest( GN::gfx::GpuAPI::D3D11, false ) {}

    void testExternalWindow()
    {
        #if GN_BUILD_HAS_D3D11
        externalWindow();
        #endif
    }
};

