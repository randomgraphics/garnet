#include "pch.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::input;
using namespace GN::util;

SpriteRenderer * sr = NULL;
AutoRef<Texture> tex;

bool init( Gpu & gpu )
{
    // create sprite renderer
    sr = new SpriteRenderer( gpu );
    if( !sr->init() ) return false;

    // create texture
    tex.attach( loadTextureFromFile( gpu, "media::texture\\rabit.png" ) );
    if( !tex ) return false;

    // success
    return true;
}

void quit( Gpu & )
{
    SafeDelete( sr );
    tex.clear();
}

static float pos_x;
static float pos_y;
static float angle = 0;

void update( float timestep )
{
    static bool paused = false;

    Input & in = gInput;
    KeyEvent ke = in.popLastKeyEvent();
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

void draw( Gpu & )
{
    const TextureDesc & td = tex->getDesc();

    sr->drawSingleTexturedSprite(
        tex,
        GN::gfx::SpriteRenderer::DEFAULT_OPTIONS,
        pos_x, pos_y, // x1, y1
        (float)td.width, (float)td.height ); // z
}

int run( Gpu & gpu )
{
    if( !init( gpu ) ) { quit( gpu ); return -1; }

    bool gogogo = true;

    FpsCalculator fps;
    GetLogger("GN.util.fps")->SetLevel( Logger::VERBOSE ); // enable FPS logger

    while( gogogo )
    {
        gpu.processRenderWindowMessages( false );

        Input & in = gInput;

        in.processInputEvents();

        if( in.getKeyStatus( KeyCode::ESCAPE ).down || in.getKeyStatus( KeyCode::XB360_B ).down )
        {
            gogogo = false;
        }

        gpu.clearScreen( Vector4f(0,0.5f,0.5f,1.0f) );
        double frameTime = fps.getLastFrameElasped();
        update( (float)frameTime );
        draw( gpu );
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
    }

    ~InputInitiator()
    {
        shutdownInputSystem();
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
        r = createMultiThreadGpu( cmdargs.rendererOptions );
    else
        r = createSingleThreadGpu( cmdargs.rendererOptions );
    if( NULL == r ) return -1;

    InputInitiator ii(*r);

    int result = run( *r );

    deleteGpu( r );

    return result;
}
