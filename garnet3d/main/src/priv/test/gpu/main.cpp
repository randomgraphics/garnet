#include "pch.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::input;
using namespace GN::win;
using namespace GN::util;

bool blankScreen = false;
GpuContext rc;

const char * hlsl_vscode =
    "uniform float4x4 transform; \n"
    "struct VSOUT { float4 pos : POSITION0; float2 uv : TEXCOORD; }; \n"
    "VSOUT main( in float4 pos : POSITION ) { \n"
    "   VSOUT o; \n"
    "   o.pos = mul( transform, pos ); \n"
    "   o.uv  = pos.xy; \n"
    "   return o; \n"
    "}";

const char * hlsl_pscode =
    "sampler t0; \n"
    "struct VSOUT { float4 pos : POSITION0; float2 uv : TEXCOORD; }; \n"
    "float4 main( in VSOUT i ) : COLOR0 { \n"
    "   return tex2D( t0, i.uv ); \n"
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

bool Init( Gpu & gpu )
{
    if( blankScreen ) return true;

    rc.Clear();

    // create GPU program
    GpuProgramDesc gpd;
    if( GpuAPI::OGL == gpu.GetOptions().api )
    {
        gpd.lang = GpuProgramLanguage::GLSL;
        gpd.vs.source = glsl_vscode;
        gpd.ps.source = glsl_pscode;
    }
    else
    {
        gpd.lang = GpuProgramLanguage::HLSL9;
        gpd.vs.source = hlsl_vscode;
        gpd.ps.source = hlsl_pscode;
        gpd.vs.entry  = "main";
        gpd.ps.entry  = "main";
    }
    rc.gpuProgram.Attach( gpu.CreateGpuProgram( gpd ) );
    if( !rc.gpuProgram ) return false;

    // create uniform
    rc.uniforms.Resize( 1 );
    rc.uniforms[0].Attach( gpu.CreateUniform( sizeof(Matrix44f) ) );
    if( !rc.uniforms[0] ) return false;

    // setup vertex format
    rc.vtxfmt.numElements = 1;
    rc.vtxfmt.elements[0].BindTo( "position", 0 );
    rc.vtxfmt.elements[0].format = ColorFormat::FLOAT4;
    rc.vtxfmt.elements[0].offset = 0;
    rc.vtxfmt.elements[0].stream = 0;

    // create texture
    rc.textures[0].texture.Attach( loadTextureFromFile( gpu, "media::texture\\earth.jpg" ) );

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
    rc.vtxbufs[0].vtxbuf.Attach( gpu.CreateVtxBuf( vbd ) );
    if( NULL == rc.vtxbufs[0].vtxbuf ) return false;
    rc.vtxbufs[0].vtxbuf->Update( 0, 0, vertices );

    // create index buffer
    UInt16 indices[] = { 0, 1, 3, 2 };
    IdxBufDesc ibd = { 4, false, false };
    rc.idxbuf.Attach( gpu.CreateIdxBuf( ibd ) );
    if( !rc.idxbuf ) return false;
    rc.idxbuf->Update( 0, 0, indices );

    return true;
}

void Quit( Gpu & )
{
    rc.Clear();
}

void Draw( Gpu & r )
{
    if( blankScreen ) return;

    Matrix44f m;

    // DRAW_UP: triangle at left top corner
    static float vertices[] =
    {
        0,0,0,1,
        1,0,0,1,
        1,1,0,1,
        0,1,0,1,
    };
    m.Translate( -1.0f, -0.0f, 0 );
    rc.uniforms[0]->Update( m );
    r.BindContext( rc );
    r.DrawUp( PrimitiveType::TRIANGLE_LIST, 3, vertices, 4*sizeof(float) );

    // DRAW_INDEXED_UP : triangle at left bottom
    static UInt16 indices[] = { 0, 1, 3 };
    m.Translate( -1.0f, -1.0f, 0 );
    rc.uniforms[0]->Update( m );
    r.BindContext( rc );
    r.DrawIndexedUp( PrimitiveType::TRIANGLE_STRIP, 3, 4, vertices, 4*sizeof(float), indices );

    // DRAW: triangle at right top corner
    m.Identity();
    rc.uniforms[0]->Update( m );
    r.BindContext( rc );
    r.Draw( PrimitiveType::TRIANGLE_LIST, 3, 0 );

    // DRAW_INDEXED : quad at right bottom corner
    m.Translate( 0.5f, -1.5f, 0 );
    rc.uniforms[0]->Update( m );
    r.BindContext( rc );
    r.DrawIndexed( PrimitiveType::TRIANGLE_STRIP, 4, 0, 0, 4, 0 );
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

        if( in.GetKeyStatus( KeyCode::ESCAPE ).down || in.GetKeyStatus( KeyCode::XB360_A ).down )
        {
            gogogo = false;
        }

        gpu.ClearScreen( Vector4f(0,0.5f,0.5f,1.0f) );
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

void showHelp( CommandLineArguments & ca )
{
    StrA executableName = fs::BaseName( ca.applicationName ) + fs::ExtName( ca.applicationName );
    GN_INFO(ca.logger)( "Usage: %s [options]\n", executableName.ToRawPtr() );
    ca.showStandardCommandLineOptions();
    GN_INFO(ca.logger)(
            "  -b                       Draw blank screen only. Do not create any graphics resources.\n" );
}

int main( int argc, const char * argv[] )
{
    EnableCRTMemoryCheck();

    CommandLineArguments cmdargs( argc, argv );
    switch( cmdargs.status )
    {
        case CommandLineArguments::SHOW_HELP:
            showHelp( cmdargs );
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

    for( size_t i = 0; i < cmdargs.extraArgc; ++i )
    {
        if( 0 == StringCompareI( "-b", cmdargs.extraArgv[i] ) )
        {
            blankScreen = true;
        }
        else
        {
            GN_ERROR(cmdargs.logger)( "Invalid command line argument: %s", cmdargs.extraArgv[i] );
            showHelp( cmdargs );
            return -1;
        }
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
