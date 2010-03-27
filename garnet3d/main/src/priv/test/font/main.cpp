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
    AutoRef<FontFace> ff( CreateFontFace(ffc) );
    if( !ff ) return false;

    // initialize bitmap font renderer
    ttf.Quit();
    return ttf.Init( sr, ff );
}

bool Init( Gpu & gpu )
{
    // create sprite renderer
    sr = new SpriteRenderer( gpu );
    if( !sr->Init() ) return false;

    // initialize ascii font
    AutoRef<FontFace> ff( CreateSimpleAsciiFontFace() );
    if( !ff ) return false;
    if( !ascii.Init( sr, ff ) ) return false;

    // initialize TTF font
    if( !initTTF() ) return false;

    CharacterEncodingConverter cec( CharacterEncodingConverter::GBK, CharacterEncodingConverter::WIDECHAR );
    cec( textw, "ÄãºÃ!¹þ¹þ" );

    // success
    return true;
}

void Quit( Gpu & )
{
    ascii.Quit();
    ttf.Quit();
    SafeDelete( sr );
}

void OnKeyPress( KeyEvent ke )
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

void Draw( Gpu &, const wchar_t * fps )
{
    ascii.DrawText( fps, 10, 13 );

    if( ttf.Ok() )
    {
        ttf.DrawText( textw, 100, 100 );
        ttf.DrawText( L"Hello!", 200, 200 );
    }
}

int Run( Gpu & gpu )
{
    if( !Init( gpu ) ) { Quit( gpu ); return -1; }

    bool gogogo = true;

    FpsCalculator fps;
    GetLogger("GN.util.fps")->SetLevel( Logger::VERBOSE ); // enable FPS logger

    while( gogogo )
    {
        gpu.ProcessRenderWindowMessages( false );

        Input & in = gInput;

        in.ProcessInputEvents();

        if( in.GetKeyStatus( KeyCode::ESCAPE ).down )
        {
            gogogo = false;
        }

        gpu.ClearScreen( Vector4f(0,0.5f,0.5f,1.0f) );
        Draw( gpu, fps.getFpsString() );
        gpu.Present();

        fps.onFrame();
    }

    Quit( gpu );

    return 0;
}

struct InputInitiator
{
    InputInitiator( Gpu & r )
    {
        InitializeInputSystem( InputAPI::NATIVE );
        const DispDesc & dd = r.GetDispDesc();
        gInput.AttachToWindow( dd.displayHandle, dd.windowHandle );
        gInput.sigKeyPress.Connect( OnKeyPress );
    }

    ~InputInitiator()
    {
        ShutdownInputSystem();
    }
};

int main( int argc, const char * argv[] )
{
    EnableCRTMemoryCheck();

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
        r = CreateMultiThreadGpu( cmdargs.rendererOptions );
    else
        r = CreateSingleThreadGpu( cmdargs.rendererOptions );
    if( NULL == r ) return -1;

    InputInitiator ii(*r);

    int result = Run( *r );

    DeleteGpu( r );

    return result;
}
