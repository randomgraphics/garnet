#include "testCase.h"

///
/// simple cube test case
///
class TestCube : public TestCase
{
    engine::Drawable cube;
    engine::Entity * tex;

public:

    //@{

    TestCube( EntityManager & em, RenderEngine & re, QuadRenderer & qr )
        : TestCase( em, re, qr )
        , tex( 0 )
    {
    }

    virtual bool init();
    virtual void quit();
    virtual void draw();

    //@}
};
