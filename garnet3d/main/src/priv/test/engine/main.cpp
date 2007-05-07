#include "pch.h"
#include "triangle.h"

using namespace GN;
using namespace GN::input;
using namespace GN::gfx;
using namespace GN::engine;

void runcase( RenderEngine & engine, TestCase & c )
{
    while( 1 )
    {
        gInput.processInputEvents();

        if( gInput.getKeyStatus( KEY_ESCAPE ).down )
        {
            break;
        }
        
        engine.frameBegin();

        engine.clearScreen( Vector4f(0,0,1,0) );

        c.draw();

        engine.frameEnd();
    }
}

void run( RenderEngine & engine )
{
    TestCase * cases[] =
    {
        new TestTriangle(engine),
    };

    for( size_t i = 0; i < GN_ARRAY_COUNT(cases); ++i )
    {
        TestCase * c = cases[i];
        GN_ASSERT( c );

        if( c->init() )
        {
            runcase( engine, *c );
        }

        c->quit();
        delete c;
        cases[i] = 0;
    }
}

struct InputInitiator
{
    InputInitiator()
    {
        createInputSystem( API_NATIVE );
    }

    ~InputInitiator()
    {
        if( gInputPtr ) delete gInputPtr;
    }
};

int main()
{
    InputInitiator input;

    RenderEngine engine;

    UInt32 MB = 1024 * 1024;
    RenderEngineInitParameters reip = { 32*MB, 32*MB, 4*MB };

    RendererOptions ro;

    if( !engine.init(reip) ) return -1;

    if( !engine.resetRenderer( API_D3D9, ro ) ) return -1;

    run( engine );

    // success
    return 0;
}
