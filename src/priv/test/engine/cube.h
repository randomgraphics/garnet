#include "testCase.h"

using namespace GN;
using namespace GN::engine;

///
/// simple cube test case
///
class TestCube : public TestCase
{
    engine::Effect     eff;
    Entity           * tex, * mesh;

public:

    //@{

    TestCube( EntityManager & em, RenderEngine & re )
        : TestCase( em, re )
        , eff(re)
        , tex( 0 )
        , mesh( 0 )
    {
    }

    virtual bool init();
    virtual void quit();
    virtual void draw();

    //@}
};
