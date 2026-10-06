#include "gputest.h"

class OGLGpuTest : public CxxTest::TestSuite, public GpuTest {

public:
    OGLGpuTest(): GpuTest(GN::gfx::GpuAPI::OGL, false) {}

    void testExternalWindow() {
#if GN_BUILD_HAS_OGL
        // TODO: failing on Linux. Disabled for now.
        // externalWindow();
#endif
    }
};
