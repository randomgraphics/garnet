#include "pch.h"
#include "triangle.h"

using namespace GN;
using namespace GN::input;
using namespace GN::gfx;
using namespace GN::engine;

static GN::Logger * sLogger = GN::getLogger("GN.test.engine.Triangle");

struct Vertex
{
    float x, y, z;
};

class VtxBufLoader : public GN::engine::GraphicsResourceLoader
{
public:

    bool load( const GraphicsResourceDesc &, DynaArray<UInt8> & )
    {
        return true;
    }

    bool decompress( const GraphicsResourceDesc &, DynaArray<UInt8> & , DynaArray<UInt8> & )
    {
        return true;
    }

    bool download( GraphicsResource & res, DynaArray<UInt8> & )
    {
        static const Vertex vertices[] =
        {
            { 0, 0, 0 },
            { 1, 0, 0 },
            { 1, 1, 0 },
        };
        res.surface->download( 0, 0, vertices, sizeof(vertices), sizeof(vertices) );
        return true;
    }
};

class IdxBufLoader : public GN::engine::GraphicsResourceLoader
{
public:

    bool load( const GraphicsResourceDesc &, DynaArray<UInt8> & )
    {
        return true;
    }

    bool decompress( const GraphicsResourceDesc &, DynaArray<UInt8> & , DynaArray<UInt8> & )
    {
        return true;
    }

    bool download( GraphicsResource & res, DynaArray<UInt8> & )
    {
        static const UInt16 indices[] = { 0, 1, 2 };
        res.surface->download( 0, 0, indices, sizeof(indices), sizeof(indices) );
        return true;
    }
};

static const char * fxcode =
"uniform float4x4 pvw : register(c0);               \n"
"sampler s0 : register(s0);                         \n"
"                                                   \n"
"void vsmain( in float4 ipos : POSITION,            \n"
"            out float4 opos : POSITION,            \n"
"            out float2 otex : TEXCOORD0,           \n"
"            out float4 oclr : COLOR0 )             \n"
"{                                                  \n"
"    opos = mul( pvw, ipos );                       \n"
"    otex = ipos.xy;                                \n"
"    oclr = 1;                                      \n"
"}                                                  \n"
"                                                   \n"
"void psmain( in float2 itex : TEXCOORD0,           \n"
"            out float4 oclr : COLOR0 )             \n"
"{                                                  \n"
"    oclr = tex2D( s0, itex );                      \n"
"}                                                  \n"
"                                                   \n"
"technique                                          \n"
"{                                                  \n"
"    pass                                           \n"
"    {                                              \n"
"        zenable = false;                           \n"
"        cullmode = none;                           \n"
"        vertexshader = compile vs_2_0 vsmain();    \n"
"        pixelshader  = compile ps_2_0 psmain();    \n"
"    }                                              \n"
"}";

class ParamSetLoader : public GN::engine::GraphicsResourceLoader
{
    bool load( const GraphicsResourceDesc &, DynaArray<UInt8> & )
    {
        return true;
    }

    bool decompress( const GraphicsResourceDesc &, DynaArray<UInt8> & , DynaArray<UInt8> & )
    {
        return true;
    }

    bool download( GraphicsResource & res, DynaArray<UInt8> & )
    {
        GN_ASSERT( GRT_PARAMETER_SET == res.desc.type );
        GN_ASSERT( res.paramset );

        KernelParameterSet & ps = *res.paramset;

        ps["FX"].setString( fxcode );
        ps["PRIM_TYPE"].setAggregate( TRIANGLE_LIST );
        ps["PRIM_COUNT"].setAggregate( 1 );
        ps["BASE_VERTEX"].setAggregate( 0 );
        ps["VERTEX_COUNT"].setAggregate( 3 );
        ps["BASE_INDEX"].setAggregate( 0 );

        return true;
    }
};

static void resetDrawParameters( GraphicsResource *, AutoRef<GraphicsResourceLoader> & reloader )
{
    reloader.attach( new ParamSetLoader );
}

bool TestTriangle::init()
{
    RenderEngine & re = getRenderEngine();

    // get kernel
    GraphicsResource * kernel = re.getKernel( "HLSL9" );
    if( 0 == kernel ) return false;

    // create parameter set
    param = re.createParameterSet( "triangle ps", *kernel );
    param->sigReload.connect( &resetDrawParameters );

    // create vertex buffer
    SurfaceElementFormat vtxfmt;
    vtxfmt.attribs.resize( 1 );
    vtxfmt.attribs[0].semantic.set( "POS0" );
    vtxfmt.attribs[0].offset = 0;
    vtxfmt.attribs[0].format = COLOR_FORMAT_FLOAT3;
    vtxfmt.stride = sizeof(Vertex);
    AutoRef<VtxBufLoader> vbloader( new VtxBufLoader );
    GraphicsResource * vb = re.createVtxBuf( "triangle vb", vtxfmt, 3, vbloader );
    if( 0 == vb ) return false;

    // create index buffer
    AutoRef<IdxBufLoader> ibloader( new IdxBufLoader );
    GraphicsResource * ib = re.createIdxBuf( "triangle ib", 3, ibloader );
    if( 0 == ib ) return false;

    // create texture
    GraphicsResource * tex = re.createTextureFromImageFile( "media::/texture/earth.jpg" );

    // create binding
    std::map<StrA,SurfaceResourceView> views;
    views["VTXBUF0"].set( vb, 0, 1, 0, 1 );
    views["IDXBUF"].set( ib, 0, 1, 0, 1 );
    views["TEXTURE0"].set( tex, 0, tex->desc.surface.creation.desc.layout.levels, 0, tex->desc.surface.creation.desc.layout.faces );
    GraphicsResource * binding = re.createPortBinding( "triangle binding", *kernel, views );
    if( 0 == binding ) return false;

    // create drawable
    drawable = re.createDrawable( kernel, param, binding );

    // success
    return true;
}

void TestTriangle::quit()
{
    getRenderEngine().deleteDrawable( drawable );
}

void TestTriangle::render()
{
    RenderEngine & re = getRenderEngine();

    // dispose all
    static int k = 0;
    if( 100 < (k % 200) ) re.disposeAllResources();
    ++k;

    static float scale = 0.1f;
    scale += 0.005f;
    if( scale > 1.0f ) scale = 0.1f;
    Matrix44f m44;
    m44.identity();
    m44[0][0] = scale;
    m44[1][1] = scale;
    m44[2][2] = scale;

    re.setParameterT( param, "VSCF", m44 );

    re.render( drawable );
}
