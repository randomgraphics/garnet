#include "testCase.h"

using namespace GN::engine;
using namespace GN::scene;

///
/// simple triangle test case
///
class TestTriangle : public TestCase
{
    UIntPtr context;

    GraphicsResource * param;

public:

    //@{

    TestTriangle( RenderEngine & re )
        : TestCase( re )
        , context( 0 )
    {}

    virtual bool init();
    virtual void quit();
    virtual void render();

    //@}
};
