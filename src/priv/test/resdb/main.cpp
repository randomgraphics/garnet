#include "pch.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::input;
using namespace GN::win;
using namespace GN::util;

GpuResourceDatabase * db = NULL;
ModelResource * model = 0;
AutoRef<Texture> tex[2];

static const char * hlslvscode =
    "uniform float4x4 pvw; \n"
    "struct VSOUTPUT \n"
    "{ \n"
    "   float4 hpos      : POSITION0;  // vertex position in homogenous space \n"
    "   float2 texcoords : TEXCOORD; \n"
    "}; \n"
    "struct VSINPUT \n"
    "{ \n"
    "   float4 position  : POSITION; \n"
    "   float2 texcoords : TEXCOORD; \n"
    "}; \n"
    "VSOUTPUT main( in VSINPUT i ) { \n"
    "   VSOUTPUT o; \n"
    "   o.hpos      = mul( pvw, i.position ); \n"
    "   o.texcoords = i.texcoords; \n"
    "   return o; \n"
    "}";

static const char * hlslpscode =
    "sampler s0; \n"
    "Texture2D<float4> t0; \n"
    "struct VSOUTPUT \n"
    "{ \n"
    "   float4 hpos      : POSITION0;  // vertex position in homogenous space \n"
    "   float2 texcoords : TEXCOORD; \n"
    "}; \n"
    "float4 main( in VSOUTPUT i ) : COLOR0 { \n"
    "   float4  tex  = t0.Sample( s0, i.texcoords ); \n"
    "   return tex; \n"
    "}";

static const char * glslvscode =
    "uniform mat4 pvw; \n"
    "varying vec2 texcoords; \n"
    "void main() { \n"
    "   gl_Position = pvw * gl_Vertex; \n"
    "   texcoords   = gl_MultiTexCoord0.xy; \n"
    "}";

static const char * glslpscode =
    "uniform sampler2D t0; \n"
    "varying vec2 texcoords; \n"
    "void main() { \n"
    "   vec4  tex    = texture2D( t0, texcoords ); \n"
    "   gl_FragColor = tex; \n"
    "}";

void initEffectDesc( EffectResourceDesc & ed )
{
    ed.uniforms["MATRIX_PVW"];
    ed.textures["ALBEDO_TEXTURE"];

    ed.shaders["glsl"].gpd.lang = GpuProgramLanguage::GLSL;
    ed.shaders["glsl"].gpd.vs.source = glslvscode;
    ed.shaders["glsl"].gpd.ps.source = glslpscode;
    ed.shaders["glsl"].uniforms["pvw"] = "MATRIX_PVW";
    ed.shaders["glsl"].textures["t0"] = "ALBEDO_TEXTURE";

    ed.shaders["hlsl"].gpd.lang = GpuProgramLanguage::HLSL9;
    ed.shaders["hlsl"].gpd.vs.source = hlslvscode;
    ed.shaders["hlsl"].gpd.vs.entry = "main";
    ed.shaders["hlsl"].gpd.ps.source = hlslpscode;
    ed.shaders["hlsl"].gpd.ps.entry = "main";
    ed.shaders["hlsl"].uniforms["pvw"] = "MATRIX_PVW";
    ed.shaders["hlsl"].textures["t0"] = "ALBEDO_TEXTURE";

    ed.techniques["glsl"].passes.resize( 1 );
    ed.techniques["glsl"].passes[0].shader = "glsl";

    ed.techniques["hlsl"].passes.resize( 1 );
    ed.techniques["hlsl"].passes[0].shader = "hlsl";
}

void initMeshDesc( MeshResourceDesc & md )
{
    struct Vertex { float x, y, u, v; };

    static const Vertex vertices[] = {
        { -1.0f,  1.0f, 0.0f, 0.0f },
        { -1.0f, -1.0f, 0.0f, 1.0f },
        {  1.0f,  1.0f, 1.0f, 0.0f },
        {  1.0f, -1.0f, 1.0f, 1.0f },
    };

    static const UInt16 indices[] = { 0, 1, 2, 2, 1, 3 };

    md.prim = PrimitiveType::TRIANGLE_LIST;
    md.numvtx = GN_ARRAY_COUNT( vertices );
    md.numidx = GN_ARRAY_COUNT( indices );
    md.vtxfmt = VertexFormat::XY_UV();
    md.vertices[0] = vertices;
    md.indices = indices;
}

bool init( Gpu & g )
{
    db = new GpuResourceDatabase( g );

    ModelResourceDesc md;
    initEffectDesc( md.effectResourceDesc );
    initMeshDesc( md.meshResourceDesc );
    md.textures["ALBEDO_TEXTURE"].resourceName = "media::/texture/rabit.png";
    md.uniforms["MATRIX_PVW"].size = sizeof(Matrix44f);

    model = db->createResource<ModelResource>( "m0" ).detach();
    if( 0 == model ) return false;

    if( !model->reset( &md ) ) return false;

    tex[0].attach( loadTextureFromFile( db->gpu(), "media::/texture/rabit.png" ) );
    tex[1].attach( loadTextureFromFile( db->gpu(), "media::/texture/earth.jpg" ) );

    // success
    return true;
}

void quit( Gpu & )
{
    tex[0].clear();
    tex[1].clear();
    safeDecref( model );
    safeDelete( db );
}

void update( Input & in )
{
    KeyEvent k = in.popLastKeyEvent();

    if( KeyCode::SPACEBAR == k.code && k.status.down )
    {
        static int i = 0;
        i = ( i + 1 ) % 2;

        AutoRef<TextureResource> t( db->findResource<TextureResource>( "media::/texture/rabit.png" ) );
        if( t ) t->setTexture( tex[i] );
    }
}

void draw( Gpu & )
{
    AutoRef<UniformResource> u( model->getUniform( "MATRIX_PVW" ) );
    if( u ) u->getUniform()->update( Matrix44f::sIdentity() );

    model->draw();
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

        update( in );

        gpu.clearScreen( Vector4f(0,0.5f,0.5f,1.0f) );
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
