#include "pch.h"
#include "testD3D9Hlsl.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::input;

struct InputInitiator
{
    InputInitiator( GraphicsSystem * gs )
    {
        Input * i = createInputSystem();

        const GraphicsSystemDesc & desc = gs->getDesc();

        i->attachToWindow( desc.display, desc.window );
    }

    ~InputInitiator()
    {
        delete gInputPtr;
    }
};

struct ClearScreen
{
    Kernel                       * kernel;
    AutoObjPtr<KernelParameterSet> param;

    ClearScreen()
    {
    }

    bool init( GraphicsSystem & gs )
    {
        kernel = gs.getKernel( "CLEAR_SCREEN" );
        if( 0 == kernel ) return false;

        param.attach( kernel->createParameterSet() );
        if( 0 == param ) return false;

        param->setParameter( "COLOR_VALUE", KernelParameter( 0.0f, 0.0f, 0.0f, 1.0f ) );
        param->setParameter( "DEPTH_VALUE", 1.0f );
        param->setParameter( "STENCIL_VALUE", 0 );

        return true;
    }

    void draw()
    {
        kernel->render( *param, 0 );
    }
};

static bool runcase( GraphicsSystem & gs, ClearScreen & cs, Gfx2TestApp & c )
{
    while( 1 )
    {
        gInput.processInputEvents();

        KeyEvent k = gInput.popLastKeyEvent();

        if( k.status.down )
        {
            if( KEY_ESCAPE   == k.code ) return false;
            if( KEY_SPACEBAR == k.code ) return true;
        }

        cs.draw();
        c.draw(gs);
        gs.present();
    }
}

static int run( GraphicsSystem & gs )
{
    // initialize clear kernel
    ClearScreen cs;
    if( !cs.init( gs ) ) return -1;

    Gfx2TestApp * cases[] =
    {
        new TestD3D9Hlsl,
    };

    for( size_t i = 0; i < GN_ARRAY_COUNT(cases); ++i )
    {
        Gfx2TestApp * c = cases[i];
        GN_ASSERT( c );

        bool next = false;

        if( c->init(gs) )
        {
            next = runcase( gs, cs, *c );
        }

        c->quit(gs);
        delete c;
        cases[i] = 0;

        if( !next ) break;
    }

    return 0;
}

int main()
{
    GraphicsSystemCreationParameter gscp =
    {
        FOURCC::sMake( 'D','X','9',0 ),
        0,               // first monitor
        640, 480, 32, 0, // fullscreen properties
        800, 600,        // windowed properties
        false,           // windowed mode
        false,           // no vsync
    };

    GraphicsSystemCreator gs;

    if( !gs.create( gscp ) ) return -1;

    InputInitiator input( gs.get() );

    return run( *gs.get() );
}