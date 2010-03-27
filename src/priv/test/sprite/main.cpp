#include "pch.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::input;
using namespace GN::util;

SpriteRenderer * sr = NULL;
AutoRef<Texture> tex;

bool Init( Gpu & gpu )
{
    // create sprite renderer
    sr = new SpriteRenderer( gpu );
    if( !sr->Init() ) return false;

    // create texture
    tex.Attach( loadTextureFromFile( gpu, "media::texture\\rabit.png" ) );
    if( !tex ) return false;

    // success
    return true;
}

void Quit( Gpu & )
{
    SafeDelete( sr );
    tex.Clear();
}

static float pos_x;
static float pos_y;
static float angle = 0;

void Update( float timestep )
{
    static bool paused = false;

    Input & in = gInput;
    KeyEvent ke = in.PopLastKeyEvent();
    if( ke.status.down && ( KeyCode::SPACEBAR == ke.code || KeyCode::XB360_A == ke.code ) )
    {
        paused = !paused;
    }

    if( !paused )
    {
        angle += timestep * GN_HALF_PI;
        angle = fmod( angle, GN_TWO_PI );
        pos_x = sin( angle ) * 20.0f;
        pos_y = cos( angle ) * 20.0f;
        pos_x += 20.0f;
        pos_y += 20.0f;
    }
}

void Draw( Gpu & )
{
    const TextureDesc & td = tex->GetDesc();

    sr->drawSingleTexturedSprite(
        tex,
        GN::gfx::SpriteRenderer::DEFAULT_OPTIONS,
        pos_x, pos_y, // x1, y1
        (float)td.width, (float)td.height ); // z
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

        if( in.GetKeyStatus( KeyCode::ESCAPE ).down || in.GetKeyStatus( KeyCode::XB360_B ).down )
        {
            gogogo = false;
        }

        gpu.ClearScreen( Vector4f(0,0.5f,0.5f,1.0f) );
        double frameTime = fps.getLastFrameElasped();
        Update( (float)frameTime );
        Draw( gpu );
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
