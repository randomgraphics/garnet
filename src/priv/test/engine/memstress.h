#include "testCase.h"

using namespace GN;
using namespace GN::engine;

///
/// create large number of resources to check memory overhead of render engine.
///
class TestMemoryStress : public TestCase
{
    DynaArray<GraphicsResource*> resources;

public:

    //@{

    TestMemoryStress( RenderEngine & re )
        : TestCase( re )
    {}

    virtual bool init();
    virtual void quit();
    virtual void render() {}

    //@}
};

