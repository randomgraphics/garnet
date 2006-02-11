#include "common.h"

class GfxD3DTest : public CxxTest::TestSuite, public GfxTest
{

public:

    void setUp() { d3dInit(); }

    void tearDown() { GN::gfx::deleteRenderer(); }

    void testExternalWindow() { externalWindow(); }

    void testChangeUserOptions() { changeOptions(); }

    void testFullscreen() { fullscreen(); }

    void testDefaultBackbufferSize() { defaultBackbufferSize(); }

    void testRenderStateBlock() { renderStateBlock(); }

    void testVtxBuf() { vtxBuf(); }
};
