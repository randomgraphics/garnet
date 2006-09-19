#include "common.h"

class GfxD3D9Test : public CxxTest::TestSuite, public GfxTest
{

public:

    void setUp() { d3d9Init(); }

    void tearDown() { GN::gfx::deleteRenderer(); }

    void testExternalWindow() { externalWindow(); }

    void testChangeUserOptions() { changeOptions(); }

    //void testFullscreen() { fullscreen(); }

    void testDefaultBackbufferSize() { defaultBackbufferSize(); }

    void testRenderStateBlock() { renderStateBlock(); }

    void testVtxBuf() { vtxBuf(); }
};
