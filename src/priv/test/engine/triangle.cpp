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

struct TextureData
{
    AutoObjPtr<File> file;
    ImageReader      ir;
    ImageDesc        id;
    DynaArray<UInt8> buf;

    bool loadHeader( const StrA & name )
    {
        file.attach( core::openFile( name, "rb" ) );
        if( !file ) return false;
        if( !ir.reset( *file ) ) return false;
        if( !ir.readHeader( id ) ) return false;
        return true;
    }

    bool load( const StrA & name )
    {
        file.attach( core::openFile( name, "rb" ) );
        if( !file ) return false;
        if( !ir.reset( *file ) ) return false;
        if( !ir.readHeader( id ) ) return false;
        buf.resize( id.getTotalBytes() );
        if( !ir.readImage( buf.cptr() ) ) return false;
        return true;
    }
};

class VtxBufLoader : public GraphicsResourceLoader
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

    bool copy( GraphicsResource & res, DynaArray<UInt8> & )
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

class IdxBufLoader : public GraphicsResourceLoader
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

    bool copy( GraphicsResource & res, DynaArray<UInt8> & )
    {
        static const UInt16 indices[] = { 0, 1, 2 };
        res.surface->download( 0, 0, indices, sizeof(indices), sizeof(indices) );
        return true;
    }
};

class TextureLoader : public GraphicsResourceLoader
{
    StrA        filename;
    TextureData td;

public:

    TextureLoader( const StrA & name ) : filename( name ) {}

    bool load( const GraphicsResourceDesc &, DynaArray<UInt8> & )
    {
        if( !td.load( filename ) ) return false;
        return true;
    }

    bool decompress( const GraphicsResourceDesc &, DynaArray<UInt8> &, DynaArray<UInt8> & )
    {
        return true;
    }

    bool copy( GraphicsResource & res, DynaArray<UInt8> & )
    {
        GN_UNUSED_PARAM( res );
        /*for( size_t f = 0; f < td.id.numFaces; ++f )
        for( size_t l = 0; l < td.id.numLevels; ++l )
        {
            const MipmapDesc & mmd = td.id.getMipmap( f, l );
            res.surface->download(
                calcSubSurfaceIndex( f, l, td.id.numLevels ),
                0,
                td.buf.cptr() + td.id.getMipmapOffset( f, l ),
                mmd.rowPitch,
                mmd.slicePitch );
        }*/
        return true;
    }
};

static const char * fxcode =
"void vsmain( in float4 ipos : POSITION, \n"
"          out float4 opos : POSITION, \n"
"          out float2 otex : TEXCOORD0, \n"
"          out float4 oclr : COLOR0 ) \n"
"{ \n"
"    uniform float4x4 pvw : register(c0);\n"
"    opos = mul( pvw, ipos ); \n"
"    otex = ipos.xy; \n"
"    oclr = 1; \n"
"} \n"
" \n"
"void psmain( in float2 itex : TEXCOORD0, \n"
"          out float4 oclr : COLOR0 ) \n"
"{ \n"
"    sampler s0 : register(s0); \n"
"    oclr = tex2D( s0, itex ); \n"
"}\n"
"techinuque { \n"
"    pass { \n"
"        vertexshader = compile vs_1_1 vsmain(); \n"
"        pixelshader  = compile ps_1_1 psmain(); \n"
"    } \n"
"}";

bool TestTriangle::init()
{
    RenderEngine & re = renderEngine();

    // get kernel
    GraphicsResource * kernel = re.getKernel( "HLSL9" );
    if( 0 == kernel ) return false;

    // create parameter set
    param = re.createParameterSet( "triangle", *kernel );
    //re.setParameter( param, "FX", 0, strLen(fxcode)+1, fxcode );
    re.setParameterT( param, "PRIM_TYPE", TRIANGLE_LIST );
    re.setParameterT( param, "PRIM_COUNT", 1 );
    re.setParameterT( param, "BASE_VERTEX", 0 );
    re.setParameterT( param, "VERTEX_COUNT", 3 );
    re.setParameterT( param, "BASE_INDEX", 0 );

    // create vertex buffer
    SurfaceCreationParameter scp;
    scp.bindTo( "HLSL9", "VTXBUF0" );
    scp.forcedAccessFlags = SURFACE_ACCESS_HOST_WRITE;
    scp.layout.dim = SURFACE_DIMENSION_1D;
    scp.layout.levels = 1;
    scp.layout.faces = 1;
    scp.layout.basemap.width = 3;
    scp.layout.basemap.height = 1;
    scp.layout.basemap.depth = 1;
    scp.layout.basemap.rowBytes = 3 * sizeof(Vertex);
    scp.layout.basemap.sliceBytes = scp.layout.basemap.rowBytes;
    scp.layout.format.attribs[0].semantic.set( "POS0" );
    scp.layout.format.attribs[0].offset = 0;
    scp.layout.format.attribs[0].format = FMT_FLOAT3;
    scp.layout.format.count = 1;
    scp.layout.format.stride = sizeof(Vertex);
    GraphicsResource * vb = re.createSurface( "vb", scp );
    if( 0 == vb ) return false;
    AutoRef<VtxBufLoader> vbloader( new VtxBufLoader );
    re.updateResource( vb, vbloader );

    // create index buffer
    scp.bindings.clear();
    scp.bindTo( "HLSL9", "IDXBUF" );
    scp.forcedAccessFlags = SURFACE_ACCESS_HOST_WRITE;
    scp.layout.dim = SURFACE_DIMENSION_1D;
    scp.layout.levels = 1;
    scp.layout.faces = 1;
    scp.layout.basemap.width = 3;
    scp.layout.basemap.height = 1;
    scp.layout.basemap.depth = 1;
    scp.layout.basemap.rowBytes = 3 * sizeof(UInt16);
    scp.layout.basemap.sliceBytes = scp.layout.basemap.rowBytes;
    scp.layout.format.attribs[0].semantic.set( "INDEX" );
    scp.layout.format.attribs[0].offset = 0;
    scp.layout.format.attribs[0].format = FMT_R_16_UINT;
    scp.layout.format.count = 1;
    scp.layout.format.stride = sizeof(short);
    GraphicsResource * ib = re.createSurface( "idxbuf", scp );
    if( 0 == ib ) return false;
    AutoRef<IdxBufLoader> ibloader( new IdxBufLoader );
    re.updateResource( ib, ibloader );

    // create texture
    TextureData td;
    if( !td.loadHeader( "media::/texture/earth.jpg" ) ) return false;
    scp.bindings.clear();
    scp.bindTo( "HLSL9", "TEXTURE0" );
    scp.layout.dim = SURFACE_DIMENSION_2D;
    scp.layout.levels = td.id.numLevels;
    scp.layout.faces  = td.id.numFaces;
    scp.layout.basemap.width  = td.id.mipmaps[0].width;
    scp.layout.basemap.height = td.id.mipmaps[0].height;
    scp.layout.basemap.depth  = td.id.mipmaps[0].depth;
    scp.layout.basemap.rowBytes = td.id.mipmaps[0].rowPitch;
    scp.layout.basemap.sliceBytes = td.id.mipmaps[0].slicePitch;
    scp.layout.format.attribs[0].semantic.set( "TEXEL" );
    scp.layout.format.attribs[0].offset = 0;
    scp.layout.format.attribs[0].format = td.id.format;
    scp.layout.format.count = 1;
    scp.layout.format.stride = getClrFmtDesc(td.id.format).bits / 8;
    GraphicsResource * tex = re.createSurface( "texture", scp );
    if( 0 == tex ) return false;
    AutoRef<TextureLoader> texloader( new TextureLoader("media::/texture/earth.jpg") );
    re.updateResource( tex, texloader );

    // create binding
    std::map<StrA,SurfaceResourceView> views;
    views["VTXBUF0"].set( vb, 0, 1, 0, 1 );
    views["IDXBUF"].set( ib, 0, 1, 0, 1 );
    views["TEXTURE0"].set( tex, 0, td.id.numLevels, 0, td.id.numFaces );
    GraphicsResource * binding = re.createPortBinding( "binding", *kernel, views );
    if( 0 == binding ) return false;

    // create context
    context = re.createRenderContext( kernel, param, binding );

    // success
    return true;
}

void TestTriangle::quit()
{
    renderEngine().deleteRenderContext( context );
}

void TestTriangle::render()
{
    RenderEngine & re = renderEngine();

    // dispose all
    re.disposeAllResources();

    // update uniform
    Matrix44f m44;
    m44.identity();
    re.setParameterT( param, "VSCF", m44 );

    // do draw
    re.render( context );

    //font.drawText( "render engine triangle test", 10, 10 );
}
