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

class TextureLoader : public GraphicsResourceLoader
{
    const StrA mFileName;

public:

    TextureLoader( const StrA & filename ) : mFileName(filename) {}

    virtual bool load( const GraphicsResourceDesc &, void * & outbuf, size_t & outbytes, int )
    {
        // open file
        AutoObjPtr<File> fp( core::openFile( mFileName, "rb" ) );
        if( 0 == fp ) return false;

        // create temporary buffer
        size_t bytes = fp->size();
        AutoTypePtr<UInt8> buf( new UInt8[bytes] );

        // read file
        size_t readen;
        if( !fp->read( buf, bytes, &readen ) || readen != bytes ) return false;

        // success
        outbuf = buf.detach();
        outbytes = bytes;
        return true;
    }

    virtual bool decompress( const GraphicsResourceDesc &, void * & outbuf, size_t & outbytes, const void * inbuf, size_t inbytes, int )
    {
        MemFile<const UInt8> file( (const UInt8*)inbuf, inbytes );

        ImageReader ir;
        ImageDesc   id;

        if( !ir.reset( file ) ) return false;

        if( !ir.readHeader( id ) ) return false;

        // create temporary buffer
        size_t mipcount = id.numFaces*id.numLevels;
        size_t imgbytes = id.getTotalBytes();
        size_t mipbytes = sizeof(MipmapDesc) * mipcount;
        size_t bytes = imgbytes + sizeof(ImageDesc) + mipbytes;
        AutoTypePtr<UInt8> buf( new UInt8[bytes] );

        // fill header of output buffer
        ImageDesc  * outdesc = (ImageDesc*)buf.get();
        MipmapDesc * outmips = (MipmapDesc*)(outdesc+1);
        UInt8      * data    = (UInt8*)(outmips+mipcount);
        memcpy( outdesc, &id, sizeof(ImageDesc) );
        memcpy( outmips, id.mipmaps, mipbytes );
        outdesc->mipmaps = outmips;

        // read image data
        if( !ir.readImage( data ) ) return false;

        // success
        outbuf = buf.detach();
        outbytes = bytes;
        return true;
    }

    virtual bool copy( GraphicsResource & res, const void * inbuf, size_t inbytes, int )
    {
        Texture * tex = res.texture;
        GN_ASSERT( tex );

        const ImageDesc  * indesc = (ImageDesc*)inbuf;
        size_t           imgbytes = indesc->getTotalBytes();
        size_t             offset = sizeof(ImageDesc) + sizeof(MipmapDesc) * indesc->numFaces * indesc->numLevels;
        const UInt8      * data   = (const UInt8*)inbuf + offset;

        if( offset + imgbytes > inbytes )
        {
            GN_ERROR(sLogger)( "input buffer is incomplete!" );
            return false;
        }

        GN_ASSERT( indesc->numFaces == tex->getDesc().faces );
        GN_ASSERT( indesc->numLevels <= tex->getDesc().levels );

        for( size_t face = 0; face < indesc->numFaces; ++face )
        {
            for( size_t level = 0; level < indesc->numLevels; ++level )
            {
                const MipmapDesc & md = indesc->getMipmap( face, level );

                const UInt8 * src = data + indesc->getLevelOffset( face, level );

                TexLockedResult tlr;
                if( !tex->lock( tlr, face, level, NULL, LOCK_DISCARD ) ) return false;

                if( tlr.sliceBytes == md.slicePitch )
                {
                    memcpy( tlr.data, src, tlr.sliceBytes * md.depth );
                }
                else
                {
                    GN_UNIMPL_WARNING();
                }

                tex->unlock();
            }
        }

        // success
        return true;
    }

    virtual void freebuf( void * inbuf, size_t )
    {
        safeDeleteArray( inbuf );
    }
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
    "          out float2 otex : TEXCOORD0, \n"
    "          out float4 oclr : COLOR0 ) \n"
    "{ \n"
    "    opos = mul( pvw, ipos ); \n"
    "    otex = ipos.xy; \n"
    "    oclr = 1; \n"
    "}";

bool TestTriangle::init()
{
    RenderEngine & re = renderEngine();

    AutoRef<DummyLoader> dummyloader( new DummyLoader );

    GraphicsResourceDesc desc;

    // create vertex format handle
    desc.name = "vf1";
    desc.type = GRT_VTXFMT;
    desc.fd.clear();
    desc.fd.addAttrib( 0, 0, VTXSEM_POS0, FMT_FLOAT3 );
    vf = re.allocResource( desc );
    if( 0 == vf ) return false;
    re.updateResource( vf, 0, dummyloader );

    // create vertex shader
    desc.name = "vs1";
    desc.type = GRT_SHADER;
    desc.sd.type = SHADER_VS;
    desc.sd.lang = LANG_D3D_HLSL;
    desc.sd.code = vscode;
    vs = re.allocResource( desc );
    if( 0 == vs ) return 0;
    re.updateResource( vs, 0, dummyloader );

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

    // create texture
    ImageReader ir;
    ImageDesc   id;
    AutoObjPtr<File> fp( core::openFile( "media::/texture/rabit.png", "rb" ) );
    if( 0 == fp ) return false;
    if( !ir.reset( *fp ) ) return false;
    if( !ir.readHeader( id ) ) return false;
    fp.clear();
    desc.name = "tex";
    desc.type = GRT_TEXTURE;
    if( !desc.td.fromImageDesc( id ) ) return false;
    tex = re.allocResource( desc );
    if( 0 == tex ) return 0;
    AutoRef<TextureLoader> texloader( new TextureLoader("media::/texture/rabit.png") );
    re.updateResource( tex, 0, texloader );

    // success
    return true;
}

void TestTriangle::quit()
{
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
    ctx.setVS( (const Shader*)vs );
    ctx.setVtxBuf( 0, (const VtxBuf *)vb, 0, sizeof(Vertex) );
    ctx.setIdxBuf( (const IdxBuf*)ib );
    ctx.setVtxFmt( (VtxFmtHandle)vf );
    ctx.setTexture( 0, (const Texture*)tex );
    re.setContext( ctx );

    // do draw
    re.drawIndexed( TRIANGLE_LIST, 1, 0, 0, 3, 0 );
}
