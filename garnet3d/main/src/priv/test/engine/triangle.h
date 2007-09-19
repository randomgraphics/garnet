#include "testCase.h"

using namespace GN::engine;
using namespace GN::scene;

///
/// simple triangle test case
///
class TestTriangle : public TestCase
{
    Drawable drawable;

    GraphicsResource * param;

public:

    //@{

    TestTriangle( RenderEngine & re )
        : TestCase( re )
        , drawable( 0 )
    {}

    virtual bool init();
    virtual void quit();
    virtual void render();

    //@}
};
