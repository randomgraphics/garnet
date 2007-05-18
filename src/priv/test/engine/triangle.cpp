#include "pch.h"
#include "triangle.h"

using namespace GN;
using namespace GN::input;
using namespace GN::gfx;
using namespace GN::engine;

static GN::Logger * sLogger = GN::getLogger("GN.test.engine.Triangle");

struct Vertex
{
    Vector3f pos;
};

class VtxBufLoader : public BasicVtxBufLoader
{
public:
    bool decompress( const GraphicsResourceDesc &, void * & outbuf, size_t & outbytes, const void * inbuf, size_t inbytes, int )
    {
        GN_ASSERT( 0 == inbuf && 0 == inbytes );
        Vertex * data = new Vertex[3];
        data[0].pos.set( 0, 0, 0 );
        data[1].pos.set( 1, 0, 0 );
        data[2].pos.set( 1, 1, 0 );
        outbuf = data;
        outbytes = sizeof(Vertex) * 3;
        return true;
    }
};

class IdxBufLoader : public BasicIdxBufLoader
{
public:
    bool decompress( const GraphicsResourceDesc &, void * & outbuf, size_t & outbytes, const void * inbuf, size_t inbytes, int )
    {
        GN_ASSERT( 0 == inbuf && 0 == inbytes );
        UInt16 * data = new UInt16[3];
        data[0] = 0;
        data[1] = 1;
        data[2] = 2;
        outbuf = data;
        outbytes = sizeof(UInt16) * 3;
        return true;
    }
};

static const char * vscode =
    "uniform float4x4 pvw;\n"
    "void main( in float4 ipos : POSITION, \n"
    "          out float4 opos : POSITION, \n"
    "          out float2 otex : TEXCOORD0, \n"
    "          out float4 oclr : COLOR0 ) \n"
    "{ \n"
    "    opos = mul( pvw, ipos ); \n"
    "    otex = ipos.xy; \n"
    "    oclr = 1; \n"
    "}";

static const char * pscode =
    "sampler s0 : register(s0); \n"
    "void main( in float2 itex : TEXCOORD0, \n"
    "          out float4 oclr : COLOR0 ) \n"
    "{ \n"
    "    oclr = tex2D( s0, itex ); \n"
    "}";

bool TestTriangle::init()
{
    RenderEngine & re = renderEngine();

    GraphicsResourceDesc desc;

    // create vertex format
    VtxFmtDesc vfd;
    vfd.clear();
    vfd.addAttrib( 0, 0, VTXSEM_POS0, FMT_FLOAT3 );
    vf = re.createVtxFmt( "vf1", vfd );
    if( 0 == vf ) return false;

    // create shader
    vs = re.createShader( "vs1", SHADER_VS, LANG_D3D_HLSL, vscode );
    ps = re.createShader( "ps1", SHADER_PS, LANG_D3D_HLSL, pscode );

    // create vertex buffer
    desc.name = "vb1";
    desc.type = GRT_VTXBUF;
    desc.vd.bytes = 3 * sizeof(Vertex);
    desc.vd.dynamic = false;
    desc.vd.readback = false;
    vb = re.allocResource( desc );
    if( 0 == vb ) return false;
    AutoRef<VtxBufLoader> vbloader( new VtxBufLoader );
    re.updateResource( vb, 0, vbloader );

    // create index buffer
    desc.name = "ib1";
    desc.type = GRT_IDXBUF;
    desc.id.numidx = 3;
    desc.id.dynamic = false;
    desc.id.readback = false;
    ib = re.allocResource( desc );
    if( 0 == ib ) return false;
    AutoRef<IdxBufLoader> ibloader( new IdxBufLoader );
    re.updateResource( ib, 0, ibloader );

    // create texture entity
    tex = loadTextureEntityFromFile( entityManager(), re, "media::/texture/earth.jpg" );
    if( 0 == tex ) return false;

    // create ascii font
    if( !font.init() ) return false;

    // success
    return true;
}

void TestTriangle::quit()
{
    // make sure that all resources are unbinded from render engine.
    renderEngine().clearDrawContext();

    deleteEntity( tex );
}

void TestTriangle::draw()
{
    RenderEngine & re = renderEngine();

    // dispose all
    re.disposeAllResources();

    // set uniform
    Matrix44f m44;
    m44.identity();
    re.setShaderUniform( vs, "pvw", m44 );

    // bind context
    DrawContext ctx;
    ctx.resetToDefault();
    ctx.setVS( vs );
    ctx.setPS( ps );
    ctx.setVtxBuf( 0, vb, 0, sizeof(Vertex) );
    ctx.setIdxBuf( ib );
    ctx.setVtxFmt( vf );
    ctx.setTexture( 0, entity2Texture(tex) );
    re.setContext( ctx );

    // do draw
    re.drawIndexed( TRIANGLE_LIST, 1, 0, 0, 3, 0 );

    font.drawText( "render engine triangle test", 10, 10 );
}
