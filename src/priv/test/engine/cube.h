#include "testCase.h"

using namespace GN;
using namespace GN::engine;

///
/// simple cube test case
///
class TestCube : public TestCase
{
    Drawable cube;
    Entity * tex;

public:

    //@{

    TestCube( EntityManager & em, RenderEngine & re )
        : TestCase( em, re )
        , tex( 0 )
    {
    }

    virtual bool init();
    virtual void quit();
    virtual void draw();

    //@}
};
