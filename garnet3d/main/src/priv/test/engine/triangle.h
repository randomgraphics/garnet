#include "testCase.h"

///
/// simple triangle test case
///
class TestTriangle : public TestCase
{
    GraphicsResource *vf, *vs, *ps, *vb, *ib;

    Entity * tex;

    AsciiFont font;

public:

    //@{

    TestTriangle( EntityManager & em, RenderEngine & re, QuadRenderer & qr )
        : TestCase( em, re, qr )
        , font( qr )
    {}

    virtual bool init();
    virtual void quit();
    virtual void draw();

    //@}
};
