#include "rndrtest.h"

class OGLRendererTest : public CxxTest::TestSuite, public RendererTest
{

public:

    OGLRendererTest() : RendererTest( GN::gfx::RendererAPI::OGL, false ) {}

    void testExternalWindow()
    {
        #ifdef GN_BUILD_RENDERER_API_HAS_OGL
        externalWindow();
        #endif
    }
};
