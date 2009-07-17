#include "rndrtest.h"

class OGLRendererTest : public CxxTest::TestSuite, public RendererTest
{

public:

    OGLRendererTest() : RendererTest( GN::gfx::RendererAPI::OGL, false ) {}

    void testExternalWindow()
    {
        #ifdef HAS_OGL
        externalWindow();
        #endif
    }
};
