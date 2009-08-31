#include "pch.h"
#include "gpures.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::input;
using namespace GN::win;
using namespace GN::util;

GpuResourceDatabase * db = NULL;
GpuResourceHandle  model = 0;

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

    ed.techniques["glsl"].passes.resize( 1 );
    ed.techniques["glsl"].passes[0].shader = "glsl";
}

void initMeshDesc( MeshResourceDesc & md )
{
    struct Vertex { float x, y, u, v; };

    const Vertex vertices[] = {
        { -1.0f,  1.0f, 0.0f, 0.0f },
        { -1.0f, -1.0f, 0.0f, 1.0f },
        {  1.0f,  1.0f, 1.0f, 0.0f },
        {  1.0f, -1.0f, 1.0f, 1.0f },
    };

    const UInt16 indices[] = { 0, 1, 2, 0, 2, 3 };

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

    model = ModelResource::create( *db, "m0",  md );

    // success
    return true;
}

void quit( Gpu & )
{
    safeDelete( db );
}

void draw( Gpu & )
{
    if( 0 != model )
    {
        ModelResource * m = GpuResource::castTo<ModelResource>( db->getResource(model) );

        //UniformResource * u = GpuResource::castTo<UniformResource>( db->getResource( m->getUniform( "pvw" ) ) );
        //if( u ) u->getUniform()->update( Matrix44f::sIdentity() );

        m->draw();
    }
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
