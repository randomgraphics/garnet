#include "testCase.h"

using namespace GN;
using namespace GN::input;
using namespace GN::gfx;
using namespace GN::engine;
using namespace GN::scene;

///
/// simple cube test case
///
class TestCube : public TestCase
{
    BitmapFont       font;
    engine::Drawable cube;
    engine::Entity * tex;

public:

    //@{

    TestCube( EntityManager & em, RenderEngine & re, QuadRenderer & qr )
        : TestCase( em, re, qr )
        , font( qr )
        , tex( 0 )
    {
    }

    virtual bool init();
    virtual void quit();
    virtual void draw();

    //@}
};
