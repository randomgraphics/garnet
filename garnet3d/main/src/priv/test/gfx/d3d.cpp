#include "common.h"

class GfxD3DTest : public CxxTest::TestSuite, public GfxTest
{

public:

    void setUp() { d3dInit(); }

    void tearDown() { libFree(); }

    void testExternalWindow() { externalWindow(); }

    void testChangeUserOptions() { changeUserOptions(); }

    void testFullscreen() { fullscreen(); }

    void testDefaultBackbufferSize() { defaultBackbufferSize(); }

    void testMultiRenderer() { multiRenderer(); }
};
