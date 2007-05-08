#include "pch.h"
#include "testCase.h"

using namespace GN;
using namespace GN::input;
using namespace GN::gfx;
using namespace GN::engine;

class TestTriangle : public TestCase
{
    GraphicsResourceId vf, vs, ps, vb, ib;

public:

    TestTriangle( RenderEngine & e ) : TestCase( e ) {}

    virtual bool init();
    virtual void quit();
    virtual void draw();
};
