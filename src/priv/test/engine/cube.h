#include "testCase.h"

using namespace GN;
using namespace GN::engine;

///
/// simple cube test case
///
class TestCube : public TestCase
{
    GraphicsResource * vs, * ps;
    Entity           * tex, * mesh;

public:

    //@{

    TestCube( EntityManager & em, RenderEngine & re )
        : TestCase( em, re )
        , vs( 0 )
        , ps( 0 )
        , tex( 0 )
        , mesh( 0 )
    {
    }

    virtual bool init();
    virtual void quit();
    virtual void draw();

    //@}
};
