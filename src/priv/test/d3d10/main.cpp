#include "pch.h"

#if GN_MSVC
#if GN_DEBUG_BUILD
#pragma comment( lib, "d3dx10d.lib" )
#else
#pragma comment( lib, "d3dx10.lib" )
#endif
#endif

using namespace GN;
using namespace GN::gfx;
using namespace GN::engine;
using namespace GN::app;
using namespace GN::scene;

static const char * vs_code =
"uniform float4x4 gPvw : register(c0);      \n"
"struct vsi                                 \n"
"{                                          \n"
"    float4 pos : POSITION;                 \n"
"    float3 nml : NORMAL;                   \n"
"    float2 uv  : TEXCOORD0;                \n"
"};                                         \n"
"struct vso                                 \n"
"{                                          \n"
"    float4 pos : POSITION;                 \n"
"    float4 clr : COLOR0;                   \n"
"    float2 uv  : TEXCOORD0;                \n"
"};                                         \n"
"vso main( vsi i )                          \n"
"{                                          \n"
"    vso o;                                 \n"
"    o.pos = mul( gPvw, i.pos );            \n"
"    float3 n = 2*abs(i.nml) + i.nml;       \n"
"    o.clr = float4( n/3.0, 1.0 );          \n"
"    o.uv = i.uv;                           \n"
"    return o;                              \n"
"};";

static const char * ps_code =
"struct vso                     \n"
"{                              \n"
"    float4 pos : POSITION;     \n"
"    float4 clr : COLOR0;       \n"
"};                             \n"
"float4 main( vso i ) : COLOR0  \n"
"{                              \n"
"	return i.clr;               \n"
"}";

class MyApp : public SampleApp
{
    GraphicsResource * vs, * ps;
    Mesh * mesh;
    Matrix44f world, view, proj;
    util::ArcBall arcball;
    DrawContext ctx;

public:

    MyApp() : mesh(0) {}

    void onDetermineInitParam( InitParam & ip )
    {
        ip.rapi = API_D3D10;
        //ip.ro.reference = true;
    }

    bool onInit()
    {
        RenderEngine & re = getRenderEngine();

        world.identity();
        view.translate( 0, 0, 200 );
        proj.perspectiveD3DLh( 1.0f, 4.0f/3.0f, 10.0f, 1000.0f );
        arcball.setHandness( util::LEFT_HAND );
        arcball.setViewMatrix( view );
        arcball.connectToInput();

        // load shaders
        vs = re.createShader( "vs", SHADER_VS, LANG_D3D_HLSL, vs_code );
        ps = re.createShader( "ps", SHADER_PS, LANG_D3D_HLSL, ps_code );
        if( !vs || !ps ) return false;

        // create cube mesh
        mesh = new Mesh(re);
        if( !mesh->loadFromFile( "media::/cube/cube.mesh.xml" ) ) return false;

        // initialize context
        ctx.resetToDefault();
        ctx.setShaders( vs, ps, 0 );
        mesh->updateContext( ctx );

        // initial arcball window
        const DispDesc & dd = re.getDispDesc();
        arcball.setMouseMoveWindow( 0, 0, (int)dd.width, (int)dd.height );

        return true;
    }

    void onQuit()
    {
        safeFreeGraphicsResource( vs );
        safeFreeGraphicsResource( ps );
        safeDelete( mesh );
    }

    void onUpdate()
    {
        world = arcball.getRotationMatrix44();
        Matrix44f pvw = proj * view * world;
        getRenderEngine().setShaderUniform( vs, "gPvw", pvw );
    }

    void onRender()
    {
        RenderEngine & re = getRenderEngine();

        re.clearScreen( Vector4f(0,0,0,1) );

        re.setContext( ctx );
        mesh->draw();
    }
};

int main( int argc, const char * argv[] )
{
    MyApp app;
    return app.run( argc, argv );
}
