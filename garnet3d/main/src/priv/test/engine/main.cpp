#include "pch.h"
#include "triangle.h"
#include "cube.h"

using namespace GN;
using namespace GN::input;
using namespace GN::gfx;
using namespace GN::engine;
using namespace GN::scene;

bool runcase( RenderEngine & re, TestCase & c )
{
    while( 1 )
    {
        gInput.processInputEvents();

        KeyEvent k = gInput.popLastKeyEvent();
        if( k.status.down )
        {
            if( KEY_ESCAPE == k.code ) return false;
            if( KEY_SPACEBAR == k.code ) return true;
        }

        re.frameBegin();

        re.clearScreen( Vector4f(0,0,1,0) );

        c.draw();

        re.frameEnd();
    }
}

void run( EntityManager & em, RenderEngine & re, QuadRenderer & qr )
{
    TestCase * cases[] =
    {
        new TestCube(em,re,qr),
        new TestTriangle(em,re,qr),
    };

    for( size_t i = 0; i < GN_ARRAY_COUNT(cases); ++i )
    {
        TestCase * c = cases[i];
        GN_ASSERT( c );

        bool next = false;

        if( c->init() )
        {
            next = runcase( re, *c );
        }

        c->quit();
        delete c;
        cases[i] = 0;

        if( !next ) break;
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
    QuadRenderer  qr;

    UInt32 MB = 1024 * 1024;
    RenderEngineInitParameters reip = { 32*MB, 32*MB, 4*MB };

    RendererOptions ro;

    if( !re.init(reip) ) return -1;

    if( !re.resetRenderer( API_D3D9, ro ) ) return -1;

    if( !qr.init( re ) ) return -1;

    run( em, re, qr );

    // success
    return 0;
}
