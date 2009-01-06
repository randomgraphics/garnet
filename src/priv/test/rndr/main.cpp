#include "pch.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::input;
using namespace GN::win;
using namespace GN::util;

RendererContext rc;

const char * vscode =
    "varying vec2 texcoords; \n"
    "uniform mat4 transform; \n"
    "void main() { \n"
    "   gl_Position = transform * gl_Vertex; \n"
    "   texcoords.xy = gl_Vertex.xy; \n"
    "}";

const char * pscode =
    "uniform sampler2D t0; \n"
    "varying vec2 texcoords; \n"
    "void main() { \n"
    "   gl_FragColor = texture2D( t0, texcoords ); \n"
    "}";

/// read image from file
inline bool
readImageFromFile( ImageDesc & desc, std::vector<UInt8> & data )
{
    std::auto_ptr<File> fp( core::openFile( "media::texture\\earth.jpg", "rb" ) );
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
    rc.resetToDefault();

    // create GPU program
    GpuProgramDesc gpd;
    gpd.vs.lang = GPL_GLSL;
    gpd.vs.code = vscode;
    gpd.ps.lang = GPL_GLSL;
    gpd.ps.code = pscode;
    rc.gpuProgram.attach( rndr.createGpuProgram( gpd ) );
    if( !rc.gpuProgram ) return false;

    // setup vertex format
    rc.vtxfmt.numElements = 1;
    strcpy_s( rc.vtxfmt.elements[0].binding, "position" );
    rc.vtxfmt.elements[0].bindingIndex = 0;
    rc.vtxfmt.elements[0].format = COLOR_FORMAT_FLOAT4;
    rc.vtxfmt.elements[0].offset = 0;
    rc.vtxfmt.elements[0].stream = 0;

    // load image
    ImageDesc id;
    std::vector<UInt8> texels;
    if( !readImageFromFile( id, texels ) ) return false;

    // create texture
    TextureDesc td;
    td.fromImageDesc( id );
    rc.textures[0].attach( rndr.createTexture( td ) );
    if( rc.textures[0] )
    {
        const MipmapDesc & md = id.getMipmap( 0, 0 );
        rc.textures[0]->updateMipmap( 0, 0, 0, md.rowPitch, md.slicePitch, &texels[0], UPDATE_DEFAULT );
    }
    strcpy_s( rc.texbinds[0], "t0" );

    return true;
}

void quit( Renderer & )
{
    rc.resetToDefault();
}

void draw( Renderer & r )
{
    Matrix44f m;
    m.translate( -0.5f, -0.5f, 0 );
    rc.gpuProgram->setParameter( "transform", m );
    r.bindContext( rc );

    static float vb[] =
    {
        0,0,0,1,
        1,0,0,1,
        1,1,0,1,
    };

    r.drawUp( TRIANGLE_LIST, 1, vb, 4*sizeof(float) );
}

int run( Renderer & rndr )
{
    if( !init( rndr ) ) { quit( rndr ); return -1; }

    WindowHandle win = rndr.getDispDesc().windowHandle;

    bool gogogo = true;

    while( gogogo )
    {
        processWindowMessages( win, false );

        Input & in = gInput;

        in.processInputEvents();

        if( in.getKeyStatus( KEY_ESCAPE ).down )
        {
            gogogo = false;
        }

        rndr.clearScreen( Vector4f(0,0.5f,0.5f,1.0f) );
        draw( rndr );
        rndr.present();
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

    Renderer * r = createSingleThreadRenderer( o );
    if( NULL == r ) return -1;

    InputInitiator ii(*r);

    int result = run( *r );

    deleteRenderer( r );

    return result;
}
