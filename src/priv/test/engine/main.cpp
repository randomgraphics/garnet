#include "pch.h"
#include "triangle.h"

using namespace GN;
using namespace GN::input;
using namespace GN::gfx;
using namespace GN::engine;

void runcase( RenderEngine & re, TestCase & c )
{
    while( 1 )
    {
        gInput.processInputEvents();

        if( gInput.getKeyStatus( KEY_ESCAPE ).down )
        {
            break;
        }
        
        re.frameBegin();

        re.clearScreen( Vector4f(0,0,1,0) );

        c.draw();

        re.frameEnd();
    }
}

void run( EntityManager & em, RenderEngine & re )
{
    TestCase * cases[] =
    {
        new TestTriangle(em,re),
    };

    for( size_t i = 0; i < GN_ARRAY_COUNT(cases); ++i )
    {
        TestCase * c = cases[i];
        GN_ASSERT( c );

        if( c->init() )
        {
            runcase( re, *c );
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

    EntityManager em;
    RenderEngine  re;

    UInt32 MB = 1024 * 1024;
    RenderEngineInitParameters reip = { 32*MB, 32*MB, 4*MB };

    RendererOptions ro;

    if( !re.init(reip) ) return -1;

    if( !re.resetRenderer( API_D3D9, ro ) ) return -1;

    run( em, re );

    // success
    return 0;
}
