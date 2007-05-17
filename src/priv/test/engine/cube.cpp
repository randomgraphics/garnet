#include "pch.h"
#include "cube.h"

using namespace GN;
using namespace GN::input;
using namespace GN::gfx;
using namespace GN::engine;

static GN::Logger * sLogger = GN::getLogger("GN.test.engine.Cube");

static const char * vscode =
    "uniform float4x4 pvw;\n"
    "void main( in float4 ipos : POSITION,   \n"
    "           in float2 itex : TEXCOORD0,  \n"
    "          out float4 opos : POSITION,   \n"
    "          out float2 otex : TEXCOORD0 ) \n"
    "{ \n"
    "    opos = mul( pvw, ipos ); \n"
    "    otex = itex; \n"
    "}";

static const char * pscode =
    "sampler s0 : register(s0); \n"
    "float4 main( in float2 itex : TEXCOORD0 ) : COLOR0 \n"
    "{ \n"
    "   return tex2D( s0, itex ); \n"
    "}";

bool TestCube::init()
{
    EntityManager & em = entityManager();
    RenderEngine & re = renderEngine();

    // create vertex shader
    vs = createShader( re, SHADER_VS, LANG_D3D_HLSL, vscode, "", "vs1" );

    // create pixel shader
    ps = createShader( re, SHADER_PS, LANG_D3D_HLSL, pscode, "", "ps1" );

    // create mesh entity
    mesh = loadMeshEntityFromFile( em, re, "media::/cube/cube.mesh.xml" );
    if( 0 == mesh ) return false;

    // create texture entity
    tex = loadTextureEntityFromFile( em, re, "media::/texture/earth.jpg" );
    if( 0 == tex ) return false;

    // success
    return true;
}

void TestCube::quit()
{
    // make sure that all resources are unbinded from render engine.
    clearDrawContext( renderEngine() );

    deleteEntity( mesh );
    deleteEntity( tex );
}

void TestCube::draw()
{
    RenderEngine & re = renderEngine();

    // dispose all
    re.disposeAllResources();

    // set uniform
    Matrix44f proj,view;
    proj.perspectiveD3D( 1.0f, 4.0f/3.0f, 10.0f, 500.0f );
    view.lookAt( Vector3f(100.0f,100.0f,100.0f), Vector3f(0,0,0), Vector3f(0,1.0f,0) );
    re.setShaderUniform( vs, "pvw", proj*view );

    engine::Mesh * m = entity2Object<engine::Mesh*>(mesh,0);

    // bind context
    DrawContext ctx;
    ctx.resetToDefault();
    ctx.setVS( (const Shader*)vs );
    ctx.setPS( (const Shader*)ps );
    ctx.setTexture( 0, (const Texture*)entity2Texture(tex) );
    m->updateContext( ctx );
    re.setContext( ctx );

    // do draw
    m->draw();
}
