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
    std::auto_ptr<File> fp( openFile( "media::texture\\earth.jpg", "rb" ) );
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
    rc.clear();

    // create GPU program
    GpuProgramDesc gpd;
    gpd.lang = GPL_GLSL;
    gpd.vs.code = vscode;
    gpd.ps.code = pscode;
    rc.gpuProgram.attach( rndr.createGpuProgram( gpd ) );
    if( !rc.gpuProgram ) return false;

    // create uniform
    const GpuProgramParameterDesc & gppd = rc.gpuProgram->getParameterDesc();
    rc.uniforms.resize( gppd.numUniforms );
    for( size_t i = 0; i < gppd.numUniforms; ++i )
    {
        rc.uniforms[i].attach( rndr.createUniform( gppd.uniformSizes[i] ) );
        if( !rc.uniforms[i] ) return false;
    }

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
        rc.textures[0]->updateMipmap( 0, 0, 0, md.rowPitch, md.slicePitch, &texels[0], SURFACE_UPDATE_DEFAULT );
    }
    strcpy_s( rc.texbinds[0], "t0" );

    // create vertex buffer
    static float vertices[] =
    {
        0,0,0,1,
        1,0,0,1,
        1,1,0,1,
        0,1,0,1,
    };
    VtxBufDesc vbd = {
        sizeof(vertices),
        false,
        false
    };
    rc.vtxbufs[0].attach( rndr.createVtxBuf( vbd ) );
    if( NULL == rc.vtxbufs[0] ) return false;
    rc.vtxbufs[0]->update( 0, 0, vertices );

    // create index buffer
    UInt16 indices[] = { 0, 1, 3, 2 };
    IdxBufDesc ibd = { 4, false, false, false };
    rc.idxbuf.attach( rndr.createIdxBuf( ibd ) );
    if( !rc.idxbuf ) return false;
    rc.idxbuf->update( 0, 0, indices );

    return true;
}

void quit( Renderer & )
{
    rc.clear();
}

void draw( Renderer & r )
{
    size_t ui = rc.gpuProgram->getUniformIndex( "transform" );
    if( GpuProgram::PARAMETER_NOT_FOUND == ui ) return;

    Matrix44f m;

    // DRAW_UP: triangle at left top corner
    static float vertices[] =
    {
        0,0,0,1,
        1,0,0,1,
        1,1,0,1,
        0,1,0,1,
    };
    m.translate( -1.0f, -0.0f, 0 );
    rc.uniforms[ui]->update( m );
    r.bindContext( rc );
    r.drawUp( TRIANGLE_LIST, 3, vertices, 4*sizeof(float) );

    // DRAW_INDEXED_UP : triangle at left bottom
    static UInt16 indices[] = { 0, 1, 3 };
    m.translate( -1.0f, -1.0f, 0 );
    rc.uniforms[ui]->update( m );
    r.bindContext( rc );
    r.drawIndexedUp( TRIANGLE_STRIP, 3, 4, vertices, 4*sizeof(float), indices );

    // DRAW: triangle at right top corner
    m.identity();
    rc.uniforms[ui]->update( m );
    r.bindContext( rc );
    r.draw( TRIANGLE_LIST, 3, 0 );

    // DRAW_INDEXED : quad at right bottom corner
    m.translate( 0.5f, -1.5f, 0 );
    rc.uniforms[ui]->update( m );
    r.bindContext( rc );
    r.drawIndexed( TRIANGLE_STRIP, 4, 0, 0, 4, 0 );
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
