#include "pch.h"
#include "triangle.h"

using namespace GN;
using namespace GN::input;
using namespace GN::gfx;
using namespace GN::engine;

struct Vertex
{
    Vector3f pos;
};

class VtxBufLoader : public BasicVtxBufLoader
{
public:
    bool decompress( void * & outbuf, size_t & outbytes, const void * inbuf, size_t inbytes, int )
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
    bool decompress( void * & outbuf, size_t & outbytes, const void * inbuf, size_t inbytes, int )
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

const char * vscode =
    "uniform float4x4 pvw;\n"
    "void main( in float4 ipos : POSITION, \n"
    "          out float4 opos : POSITION, \n"
    "          out float4 oclr : COLOR0 ) \n"
    "{ \n"
    "    opos = mul( pvw, ipos ); \n"
    "    oclr = 1; \n"
    "}";

bool TestTriangle::init()
{
    RenderEngine & e = engine();

    // create vertex format handle
    VtxFmtDesc vfd;
    vfd.clear();
    vfd.addAttrib( 0, 0, VTXSEM_POS0, FMT_FLOAT3 );
    vf = gRenderer.createVtxFmt( vfd );
    if( 0 == vf ) return false;

    // create vertex shader
    GraphicsResourceDesc vsdesc;
    vsdesc.name = "vs1";
    vsdesc.type = GRT_SHADER;
    vsdesc.sd.type = SHADER_VS;
    vsdesc.sd.lang = LANG_D3D_HLSL;
    vsdesc.sd.code = vscode;
    vsdesc.sd.vtxfmt.clear();
    vsdesc.sd.vtxfmt.addAttrib( 0, 0, VTXSEM_POS0, FMT_FLOAT3 );
    vs = e.allocResource( vsdesc );
    if( 0 == vs ) return 0;
    AutoRef<ShaderLoader> vsloader( new ShaderLoader );
    e.updateResource( vs, 0, vsloader );

    // create vertex buffer
    GraphicsResourceDesc vbdesc;
    vbdesc.name = "vb1";
    vbdesc.type = GRT_VTXBUF;
    vbdesc.vd.bytes = 3 * sizeof(Vertex);
    vbdesc.vd.dynamic = false;
    vbdesc.vd.readback = false;
    vb = e.allocResource( vbdesc );
    if( 0 == vb ) return false;
    AutoRef<VtxBufLoader> vbloader( new VtxBufLoader );
    e.updateResource( vb, 0, vbloader );

    // create index buffer
    GraphicsResourceDesc ibdesc;
    ibdesc.name = "ib1";
    ibdesc.type = GRT_IDXBUF;
    ibdesc.id.numidx = 3;
    ibdesc.id.dynamic = false;
    ibdesc.id.readback = false;
    ib = e.allocResource( ibdesc );
    if( 0 == ib ) return false;
    AutoRef<IdxBufLoader> ibloader( new IdxBufLoader );
    e.updateResource( ib, 0, ibloader );

    return true;
}

void TestTriangle::quit()
{
}

void TestTriangle::draw()
{
    RenderEngine & e = engine();

    // dispose all
    e.disposeAllResources();

    // set uniform
    Matrix44f m44;
    m44.identity();
    e.setShaderUniform( vs, "pvw", m44 );

    // bind context
    DrawContext ctx;
    ctx.resetToDefault();
    ctx.setVS( (const Shader*)vs );
    ctx.setVtxBuf( 0, (const VtxBuf *)vb, 0, sizeof(Vertex) );
    ctx.setIdxBuf( (const IdxBuf*)ib );
    ctx.setVtxFmt( vf );
    e.setContext( ctx );

    // do draw
    e.drawIndexed( TRIANGLE_LIST, 1, 0, 0, 3, 0 );
}
