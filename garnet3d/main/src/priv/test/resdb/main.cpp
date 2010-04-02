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
    "struct VSOUTPUT \n"
    "{ \n"
    "   float4 hpos      : POSITION0;  // vertex position in homogenous space \n"
    "   float2 texcoords : TEXCOORD; \n"
    "}; \n"
    "float4 main( in VSOUTPUT i ) : COLOR0 { \n"
    "   float4  tex  = tex2D( s0, i.texcoords ); \n"
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

    ed.gpuprograms["glsl"].gpd.lang = GpuProgramLanguage::GLSL;
    ed.gpuprograms["glsl"].gpd.vs.source = glslvscode;
    ed.gpuprograms["glsl"].gpd.ps.source = glslpscode;
    ed.gpuprograms["glsl"].uniforms["pvw"] = "MATRIX_PVW";
    ed.gpuprograms["glsl"].textures["t0"] = "ALBEDO_TEXTURE";

    ed.gpuprograms["hlsl"].gpd.lang = GpuProgramLanguage::HLSL9;
    ed.gpuprograms["hlsl"].gpd.vs.source = hlslvscode;
    ed.gpuprograms["hlsl"].gpd.vs.entry = "main";
    ed.gpuprograms["hlsl"].gpd.ps.source = hlslpscode;
    ed.gpuprograms["hlsl"].gpd.ps.entry = "main";
    ed.gpuprograms["hlsl"].uniforms["pvw"] = "MATRIX_PVW";
    ed.gpuprograms["hlsl"].textures["s0"] = "ALBEDO_TEXTURE";

    ed.techniques["glsl"].passes.Resize( 1 );
    ed.techniques["glsl"].passes[0].gpuprogram = "glsl";

    ed.techniques["hlsl"].passes.Resize( 1 );
    ed.techniques["hlsl"].passes[0].gpuprogram = "hlsl";
}

void initMeshDesc( MeshResourceDesc & md )
{
    struct Vertex { float x, y, u, v; };

    static Vertex vertices[] = {
        { -1.0f,  1.0f, 0.0f, 0.0f },
        { -1.0f, -1.0f, 0.0f, 1.0f },
        {  1.0f,  1.0f, 1.0f, 0.0f },
        {  1.0f, -1.0f, 1.0f, 1.0f },
    };

    static UInt16 indices[] = { 0, 1, 2, 2, 1, 3 };

    md.prim = PrimitiveType::TRIANGLE_LIST;
    md.numvtx = GN_ARRAY_COUNT( vertices );
    md.numidx = GN_ARRAY_COUNT( indices );
    md.vtxfmt = VertexFormat::XY_UV();
    md.vertices[0] = vertices;
    md.indices = indices;
}

bool Init( Gpu & g )
{
    db = new GpuResourceDatabase( g );

    EffectResourceDesc ed;
    initEffectDesc( ed );
    AutoRef<EffectResource> e = db->CreateResource<EffectResource>( "e0" );
    if( !e || !e->Reset( &ed ) ) return false;

    MeshResourceDesc med;
    initMeshDesc( med );
    AutoRef<MeshResource> mesh = db->CreateResource<MeshResource>( "m0" );
    if( !mesh || !mesh->Reset( &med ) ) return false;

    ModelResourceDesc mod;
    mod.effect = "e0";
    mod.mesh = "m0";
    mod.textures["ALBEDO_TEXTURE"].resourceName = "media::/texture/rabit.png";
    mod.uniforms["MATRIX_PVW"].size = sizeof(Matrix44f);

    model = db->CreateResource<ModelResource>( "m0" ).Detach();
    if( 0 == model ) return false;

    if( !model->Reset( &mod ) ) return false;

    tex[0].Attach( LoadTextureFromFile( db->GetGpu(), "media::/texture/rabit.png" ) );
    tex[1].Attach( LoadTextureFromFile( db->GetGpu(), "media::/texture/earth.jpg" ) );

    // success
    return true;
}

void Quit( Gpu & )
{
    tex[0].Clear();
    tex[1].Clear();
    SafeDecref( model );
    SafeDelete( db );
}

void Update( Input & in )
{
    KeyEvent k = in.PopLastKeyEvent();

    if( KeyCode::SPACEBAR == k.code && k.status.down )
    {
        static int i = 0;
        i = ( i + 1 ) % 2;

        AutoRef<TextureResource> t( db->FindResource<TextureResource>( "media::/texture/rabit.png" ) );
        if( t ) t->SetTexture( tex[i] );
    }
}

void Draw( Gpu & )
{
    AutoRef<UniformResource> u( model->GetUniformResource( "MATRIX_PVW" ) );
    if( u ) u->GetUniform()->Update( Matrix44f::sIdentity() );

    model->Draw();
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

        Update( in );

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
