#include "common.h"

class GfxD3DTest : public CxxTest::TestSuite, public GfxTest
{

public:

    void setUp() { libInit("D3D"); }

    void tearDown() { libFree(); }

    void testExternalWindow() { externalWindow(); }

    void testChangeDevice() { changeDevice(); }

    void testFullscreen() { fullscreen(); }

    void testDefaultBackbufferSize() { defaultBackbufferSize(); }

    void testMultiRenderer() { multiRenderer(); }
};
