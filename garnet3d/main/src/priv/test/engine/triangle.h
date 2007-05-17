#include "testCase.h"

using namespace GN;
using namespace GN::engine;

///
/// simple triangle test case
///
class TestTriangle : public TestCase
{
    GraphicsResource *vf, *vs, *ps, *vb, *ib;

    Entity * tex;

public:

    //@{

    TestTriangle( EntityManager & em, RenderEngine & re ) : TestCase( em, re ) {}

    virtual bool init();
    virtual void quit();
    virtual void draw();

    //@}
};
