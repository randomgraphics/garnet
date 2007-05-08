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

    AutoRef<DummyLoader> dummyloader( new DummyLoader );

    GraphicsResourceDesc desc;

    // create vertex format handle
    desc.name = "vf1";
    desc.type = GRT_VTXFMT;
    desc.fd.clear();
    desc.fd.addAttrib( 0, 0, VTXSEM_POS0, FMT_FLOAT3 );
    vf = e.allocResource( desc );
    if( 0 == vf ) return false;
    e.updateResource( vf, 0, dummyloader );

    // create vertex shader
    desc.name = "vs1";
    desc.type = GRT_SHADER;
    desc.sd.type = SHADER_VS;
    desc.sd.lang = LANG_D3D_HLSL;
    desc.sd.code = vscode;
    vs = e.allocResource( desc );
    if( 0 == vs ) return 0;
    e.updateResource( vs, 0, dummyloader );

    // create vertex buffer
    desc.name = "vb1";
    desc.type = GRT_VTXBUF;
    desc.vd.bytes = 3 * sizeof(Vertex);
    desc.vd.dynamic = false;
    desc.vd.readback = false;
    vb = e.allocResource( desc );
    if( 0 == vb ) return false;
    AutoRef<VtxBufLoader> vbloader( new VtxBufLoader );
    e.updateResource( vb, 0, vbloader );

    // create index buffer
    desc.name = "ib1";
    desc.type = GRT_IDXBUF;
    desc.id.numidx = 3;
    desc.id.dynamic = false;
    desc.id.readback = false;
    ib = e.allocResource( desc );
    if( 0 == ib ) return false;
    AutoRef<IdxBufLoader> ibloader( new IdxBufLoader );
    e.updateResource( ib, 0, ibloader );

    // success
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
    ctx.setVtxFmt( (VtxFmtHandle)vf );
    e.setContext( ctx );

    // do draw
    e.drawIndexed( TRIANGLE_LIST, 1, 0, 0, 3, 0 );
}
