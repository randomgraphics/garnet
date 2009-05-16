#include "pch.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::util;
using namespace GN::input;

static GN::Logger * sLogger = GN::getLogger( "GN.test.RenderToTexture" );

class RenderToTexture
{
    Renderer       & rndr;
    SpriteRenderer   sr;
    AutoRef<Texture> c0, ds;
    AutoRef<Texture> tex0, tex1;
    RendererContext  context;

public:

    RenderToTexture( Renderer & r )
        : rndr(r)
        , sr(r)
    {
    }

    ~RenderToTexture()
    {
        quit();
    }

    bool init()
    {
        // create sprite renderer
        if( !sr.init() ) return false;

        // create render targets
        c0.attach( rndr.create2DTexture( 256, 256, 1, ColorFormat::RGBA32, TextureUsages::COLOR_RENDER_TARGET() ) );
        if( c0.empty() )
        {
            GN_ERROR(sLogger)( "Current graphics hardware does not support render-to-texture at all." );
            return false;
        }

        ds.attach( rndr.create2DTexture( 256, 256, 1, ColorFormat::UNKNOWN, TextureUsages::DEPTH_RENDER_TARGET() ) );
        if( ds.empty() )
        {
            GN_WARN(sLogger)( "Current graphics hardware does not support depth-texture. All tests related depth-texture are disabled." );
        }

        // load textures
        tex0.attach( loadTextureFromFile( rndr, "media::texture/rabit.png" ) );
        tex1.attach( loadTextureFromFile( rndr, "media::texture/earth.jpg" ) );
        if( 0 == tex0 || 0 == tex1 ) return false;

        return true;
    }

    void quit()
    {
        c0.clear();
        ds.clear();
        tex0.clear();
        tex1.clear();
        context.clear();
        sr.quit();
    }

    void render()
    {
        // draw tex0 to c0
        context.rendertargets.colors.resize( 1 );
        context.rendertargets.colors[0].texture = c0;
        rndr.bindContext( context );
        rndr.clearScreen();
        sr.drawSingleTexturedSprite( tex0, GN::gfx::SpriteRenderer::DEFAULT_OPTIONS, -1.0f, 1.0f, 2.0f, 2.0f );

        // draw c0 to left half of the screen
        context.rendertargets.colors.clear();
        rndr.bindContext( context );
        rndr.clearScreen();
        sr.drawSingleTexturedSprite( c0, GN::gfx::SpriteRenderer::DEFAULT_OPTIONS, 0, 0.0, 0.0, 0.5, 1.0 );

        // draw tex1 to color render target
        context.rendertargets.colors.resize( 1 );
        context.rendertargets.colors[0].texture = c0;
        rndr.bindContext( context );
        rndr.clearScreen();
        sr.drawSingleTexturedSprite( tex1, GN::gfx::SpriteRenderer::DEFAULT_OPTIONS, -1.0f, 1.0f, 2.0f, 2.0f );

        // draw c0 to right half of the screen
        context.rendertargets.colors.clear();
        rndr.bindContext( context );
        sr.drawSingleTexturedSprite( c0, GN::gfx::SpriteRenderer::DEFAULT_OPTIONS, 0, 0.5, 0.0, 1.0, 1.0 );
    }
};

int run( Renderer & rndr )
{
    RenderToTexture scene( rndr );

    if( !scene.init() ) return -1;

    bool gogogo = true;

    FpsCalculator fps;
    getLogger("GN.util.fps")->setLevel( Logger::LL_VERBOSE ); // enable FPS logger

    while( gogogo )
    {
        rndr.processRenderWindowMessages( false );

        Input & in = gInput;

        in.processInputEvents();

        if( in.getKeyStatus( KeyCode::ESCAPE ).down )
        {
            gogogo = false;
        }

        rndr.clearScreen( Vector4f(0,0.5f,0.5f,1.0f) );
        scene.render();
        rndr.present();

        fps.onFrame();
    }

    return 0;
}

struct InputInitiator
{
    InputInitiator( Renderer & r )
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

    Renderer * r;
    if( cmdargs.useMultiThreadRenderer )
        r = createMultiThreadRenderer( cmdargs.rendererOptions );
    else
        r = createSingleThreadRenderer( cmdargs.rendererOptions );
    if( NULL == r ) return -1;

    InputInitiator ii(*r);

    int result = run( *r );

    deleteRenderer( r );

    return result;
}

