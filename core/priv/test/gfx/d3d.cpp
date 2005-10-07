#include "common.h"

class GfxD3DTest : public CxxTest::TestSuite, public GfxTest
{

public:

    void setUp() { libInit("D3D"); }

    void tearDown() { libFree(); }

    void testChangeDevice() { changeDevice(); }

    void testFullscreen() { fullscreen(); }

    void testMultiRenderer() { multiRenderer(); }
};
