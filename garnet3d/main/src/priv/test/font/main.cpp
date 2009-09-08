#include "pch.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::input;
using namespace GN::util;

SpriteRenderer      * sr = NULL;
FontFaceCreationDesc ffc = { "font::simsun.ttc", (UInt16)64, (UInt16)64, FFQ_ANTIALIASED };
BitmapFont    ascii, ttf;
wchar_t              textw[256];

bool initTTF()
{
    // create font
    AutoRef<FontFace> ff( createFontFace(ffc) );
    if( !ff ) return false;

    // initialize bitmap font renderer
    ttf.quit();
    return ttf.init( sr, ff );
}

bool init( Gpu & gpu )
{
    // create sprite renderer
    sr = new SpriteRenderer( gpu );
    if( !sr->init() ) return false;

    // initialize ascii font
    AutoRef<FontFace> ff( createSimpleAsciiFontFace() );
    if( !ff ) return false;
    if( !ascii.init( sr, ff ) ) return false;

    // initialize TTF font
    if( !initTTF() ) return false;

    CharacterEncodingConverter cec( CharacterEncodingConverter::GBK, CharacterEncodingConverter::WIDECHAR );
    cec( textw, "ÄãºÃ!¹þ¹þ" );

    // success
    return true;
}

void quit( Gpu & )
{
    ascii.quit();
    ttf.quit();
    safeDelete( sr );
}

void onKeyPress( KeyEvent ke )
{
    if( !ke.status.down ) return;

    switch( ke.code )
    {
        case KeyCode::UP :
            ffc.height *= 2;
            initTTF();
            break;

        case KeyCode::DOWN:
            ffc.height /= 2;
            if( 0 == ffc.height ) ffc.height = 1;
            initTTF();
            break;

        case KeyCode::LEFT :
            ffc.width /= 2;
            if( 0 == ffc.width ) ffc.width = 1;
            initTTF();
            break;

        case KeyCode::RIGHT :
            ffc.width *= 2;
            initTTF();
            break;

        case KeyCode::A :
            ffc.quality = FFQ_ANTIALIASED;
            initTTF();
            break;

        case KeyCode::M :
            ffc.quality = FFQ_MONOCHROM;
            initTTF();
            break;

        default:
            break;
    }
}

void draw( Gpu &, const wchar_t * fps )
{
    ascii.drawText( fps, 10, 13 );

    if( ttf.ok() )
    {
        ttf.drawText( textw, 100, 100 );
        ttf.drawText( L"Hello!", 200, 200 );
    }
}

int run( Gpu & gpu )
{
    if( !init( gpu ) ) { quit( gpu ); return -1; }

    bool gogogo = true;

    FpsCalculator fps;
    getLogger("GN.util.fps")->setLevel( Logger::VERBOSE ); // enable FPS logger

    while( gogogo )
    {
        gpu.processRenderWindowMessages( false );

        Input & in = gInput;

        in.processInputEvents();

        if( in.getKeyStatus( KeyCode::ESCAPE ).down )
        {
            gogogo = false;
        }

        gpu.clearScreen( Vector4f(0,0.5f,0.5f,1.0f) );
        draw( gpu, fps.getFpsString() );
        gpu.present();

        fps.onFrame();
    }

    quit( gpu );

    return 0;
}

struct InputInitiator
{
    InputInitiator( Gpu & r )
    {
        initializeInputSystem( InputAPI::NATIVE );
        const DispDesc & dd = r.getDispDesc();
        gInput.attachToWindow( dd.displayHandle, dd.windowHandle );
        gInput.sigKeyPress.connect( onKeyPress );
    }

    ~InputInitiator()
    {
        shutdownInputSystem();
    }
};

int main( int argc, const char * argv[] )
{
    enableCRTMemoryCheck();

    CommandLineArguments cmdargs( argc, argv );
    switch( cmdargs.status )
    {
        case CommandLineArguments::SHOW_HELP:
            cmdargs.showDefaultHelp();
            return 0;

        case CommandLineArguments::INVALID_COMMAND_LINE:
            return -1;

        case CommandLineArguments::CONTINUE_EXECUTION:
            // do nothing
            break;

        default:
            GN_UNEXPECTED();
            return -1;
    }

    Gpu * r;
    if( cmdargs.useMultiThreadGpu )
        r = createMultiThreadGpu( cmdargs.rendererOptions );
    else
        r = createSingleThreadGpu( cmdargs.rendererOptions );
    if( NULL == r ) return -1;

    InputInitiator ii(*r);

    int result = run( *r );

    deleteGpu( r );

    return result;
}
