#include "pch.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::input;
using namespace GN::win;
using namespace GN::util;

RendererContext rc;

const char * hlsl_vscode =
    "uniform float4x4 transform; \n"
    "float4 main( in float4 pos : POSITION ) : POSITION { \n"
    "   return mul( transform, pos ); \n"
    "}";

const char * hlsl_pscode =
    "float4 main() : COLOR0 { \n"
    "   return 1; \n"
    "}";

const char * glsl_vscode =
    "varying vec2 texcoords; \n"
    "uniform mat4 transform; \n"
    "void main() { \n"
    "   gl_Position = transform * gl_Vertex; \n"
    "   texcoords.xy = gl_Vertex.xy; \n"
    "}";

const char * glsl_pscode =
    "uniform sampler2D t0; \n"
    "varying vec2 texcoords; \n"
    "void main() { \n"
    "   gl_FragColor = texture2D( t0, texcoords ); \n"
    "}";

bool init( Renderer & rndr )
{
    rc.clear();

    // create GPU program
    GpuProgramDesc gpd;
    if( API_OGL == rndr.getOptions().api )
    {
        gpd.lang = GPL_GLSL;
        gpd.vs.source = glsl_vscode;
        gpd.ps.source = glsl_pscode;
    }
    else
    {
        gpd.lang = GPL_HLSL;
        gpd.vs.source = hlsl_vscode;
        gpd.ps.source = hlsl_pscode;
        gpd.vs.entry  = "main";
        gpd.ps.entry  = "main";
    }
    rc.gpuProgram.attach( rndr.createGpuProgram( gpd ) );
    if( !rc.gpuProgram ) return false;

    // create uniform
    const GpuProgramParameterDesc & gppd = rc.gpuProgram->getParameterDesc();
    rc.uniforms.resize( gppd.uniforms.count() );
    for( size_t i = 0; i < gppd.uniforms.count(); ++i )
    {
        rc.uniforms[i].attach( rndr.createUniform( gppd.uniforms[i].size ) );
        if( !rc.uniforms[i] ) return false;
    }

    // setup vertex format
    rc.vtxfmt.numElements = 1;
    rc.vtxfmt.elements[0].bindTo( "position", 0 );
    rc.vtxfmt.elements[0].format = COLOR_FORMAT_FLOAT4;
    rc.vtxfmt.elements[0].offset = 0;
    rc.vtxfmt.elements[0].stream = 0;

    // create texture
    rc.textures[0].attach( loadTextureFromFile( rndr, "media::texture\\earth.jpg" ) );
    rc.bindTexture( 0, "t0" );

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
    };
    rc.vtxbufs[0].attach( rndr.createVtxBuf( vbd ) );
    if( NULL == rc.vtxbufs[0] ) return false;
    rc.vtxbufs[0]->update( 0, 0, vertices );

    // create index buffer
    UInt16 indices[] = { 0, 1, 3, 2 };
    IdxBufDesc ibd = { 4, false, false };
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
    size_t ui = rc.gpuProgram->getParameterDesc().uniforms["transform"];
    if( GPU_PROGRAM_PARAMETER_NOT_FOUND == ui ) return;

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
    //o.api = API_D3D10;

    //Renderer * r = createMultiThreadRenderer( o );
    Renderer * r = createSingleThreadRenderer( o );
    if( NULL == r ) return -1;

    InputInitiator ii(*r);

    int result = run( *r );

    deleteRenderer( r );

    return result;
}
