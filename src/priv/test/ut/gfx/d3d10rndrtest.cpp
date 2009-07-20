#include "rndrtest.h"

class D3D10RendererTest : public CxxTest::TestSuite, public RendererTest
{

public:

    D3D10RendererTest() : RendererTest( GN::gfx::RendererAPI::D3D10, false ) {}

    void testExternalWindow()
    {
        #ifdef GN_BUILD_RENDERER_API_HAS_D3D10
        externalWindow();
        #endif
    }
};
