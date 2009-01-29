#include "pch.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::input;
using namespace GN::util;

SpriteRenderer * sr = NULL;
FontFaceDesc     ffd = { "font::simsun.ttc", (UInt16)64, (UInt16)64, FFQ_ANTIALIASED };
BitmapFont       font;

bool initFont()
{
    // create font
    AutoRef<FontFace> ff( createFont(ffd) );
    if( !ff ) return false;

    // initialize bitmap font renderer
    font.quit();
    return font.init( sr, ff );
}

bool init( Renderer & rndr )
{
    // create sprite renderer
    sr = new SpriteRenderer( rndr );
    if( !sr->init() ) return false;

    if( !initFont() ) return false;

    // success
    return true;
}

void quit( Renderer & )
{
    safeDelete( sr );
    font.quit();
}

void onKeyPress( KeyEvent ke )
{
    if( !ke.status.down ) return;

    switch( ke.code )
    {
        case KEY_UP :
            ffd.height *= 2;
            initFont();
            break;

        case KEY_DOWN:
            ffd.height /= 2;
            if( 0 == ffd.height ) ffd.height = 1;
            initFont();
            break;

        case KEY_LEFT :
            ffd.width /= 2;
            if( 0 == ffd.width ) ffd.width = 1;
            initFont();
            break;

        case KEY_RIGHT :
            ffd.width *= 2;
            initFont();
            break;

        case KEY_A :
            ffd.quality = FFQ_ANTIALIASED;
            initFont();
            break;

        case KEY_M :
            ffd.quality = FFQ_MONOCHROM;
            initFont();
            break;

        default:
            break;
    }
}

void draw( Renderer &, const wchar_t * fps )
{
    if( font.ok() )
    {
        font.drawText( fps, 0, 0 );
        font.drawText( L"ÄãºÃ!¹þ¹þ", 100, 100 );
        font.drawText( L"Hello!", 200, 200 );
    }
}

int run( Renderer & rndr )
{
    if( !init( rndr ) ) { quit( rndr ); return -1; }

    bool gogogo = true;

    FpsCalculator fps;
    getLogger("GN.util.fps")->setLevel( Logger::LL_VERBOSE ); // enable FPS logger

    while( gogogo )
    {
        rndr.processRenderWindowMessages( false );

        Input & in = gInput;

        in.processInputEvents();

        if( in.getKeyStatus( KEY_ESCAPE ).down )
        {
            gogogo = false;
        }

        rndr.clearScreen( Vector4f(0,0.5f,0.5f,1.0f) );
        draw( rndr, fps.getFpsString() );
        rndr.present();

        fps.onFrame();
    }

    quit( rndr );

    return 0;
}

struct InputInitiator
{
    InputInitiator( Renderer & r )
    {
        initializeInputSystem( API_NATIVE );
        const DispDesc & dd = r.getDispDesc();
        gInput.attachToWindow( dd.displayHandle, dd.windowHandle );
        gInput.sigKeyPress.connect( onKeyPress );
    }

    ~InputInitiator()
    {
        shutdownInputSystem();
    }
};

int main( int, const char *[] )
{
    enableCRTMemoryCheck();

    RendererOptions o;
    o.api = API_OGL;

    //Renderer * r = createMultiThreadRenderer( o );
    Renderer * r = createSingleThreadRenderer( o );
    if( NULL == r ) return -1;

    InputInitiator ii(*r);

    int result = run( *r );

    deleteRenderer( r );

    return result;
}
