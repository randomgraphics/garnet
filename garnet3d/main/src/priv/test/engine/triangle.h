#include "pch.h"
#include "testCase.h"

using namespace GN;
using namespace GN::input;
using namespace GN::gfx;
using namespace GN::engine;

class TestTriangle : public TestCase
{
    GraphicsResourceId vs, ps, vb, ib;
    VtxFmtHandle       vf;

public:

    TestTriangle( RenderEngine & e ) : TestCase( e ) {}

    virtual bool init();
    virtual void quit();
    virtual void draw();
};
