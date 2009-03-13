#include "pch.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::input;
using namespace GN::util;

SpriteRenderer * sr = NULL;
AutoRef<Texture> tex;

/// read image from file
inline bool
readImageFromFile( ImageDesc & desc, std::vector<UInt8> & data )
{
    std::auto_ptr<File> fp( fs::openFile( "media::texture\\rabit.png", "rb" ) );
    if( NULL == fp.get() ) return false;

    ImageReader ir;
    if( !ir.reset( *fp ) ) return false;

    if( !ir.readHeader( desc ) ) return false;

    data.resize( desc.getTotalBytes() );
    if( !ir.readImage( &data[0] ) ) return false;

    return true;
}

bool init( Renderer & rndr )
{
    // create sprite renderer
    sr = new SpriteRenderer( rndr );
    if( !sr->init() ) return false;

    // create texture
    tex.attach( loadTextureFromFile( rndr, "media::texture\\rabit.png" ) );
    if( !tex ) return false;

    // success
    return true;
}

void quit( Renderer & )
{
    safeDelete( sr );
    tex.clear();
}

static float pos_x;
static float pos_y;
static float angle = 0;

void update( float timestep )
{
    angle += timestep * GN_HALF_PI;
    angle = fmod( angle, GN_TWO_PI );
    pos_x = sin( angle ) * 20.0f;
    pos_y = cos( angle ) * 20.0f;
    pos_x += 20.0f;
    pos_y += 20.0f;
}

void draw( Renderer & )
{
    const TextureDesc & td = tex->getDesc();

    sr->drawSingleTexturedSprite(
        tex,
        0, // options
        pos_x, pos_y, // x1, y1
        (float)td.width, (float)td.height ); // z
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
        double frameTime = fps.getLastFrameElasped();
        update( (float)frameTime );
        draw( rndr );
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

    Renderer * r = createMultiThreadRenderer( o );
    //Renderer * r = createSingleThreadRenderer( o );
    if( NULL == r ) return -1;

    InputInitiator ii(*r);

    int result = run( *r );

    deleteRenderer( r );

    return result;
}
