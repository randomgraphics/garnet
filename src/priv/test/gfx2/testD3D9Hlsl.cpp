#include "pch.h"
#include "testD3D9Hlsl.h"

using namespace GN;
using namespace GN::gfx;

// *****************************************************************************
// local code
// *****************************************************************************

struct Vertex
{
    float x, y, z;
    float n[3];
    float u, v;
};

struct TextureData
{
    AutoObjPtr<File> file;
    ImageReader      ir;
    ImageDesc        id;
    DynaArray<UInt8> buf;

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

static const char * fxcode =
"uniform float4x4 gPvw : register(c0);             \n"
"sampler s0 : register(s0);                        \n"
"                                                  \n"
"struct vsi                                        \n"
"{                                                 \n"
"    float4 pos : POSITION;                        \n"
"    float3 nml : NORMAL;                          \n"
"    float2 uv  : TEXCOORD0;                       \n"
"};                                                \n"
"                                                  \n"
"struct vso                                        \n"
"{                                                 \n"
"    float4 pos : POSITION;                        \n"
"    float4 clr : COLOR0;                          \n"
"    float2 uv  : TEXCOORD0;                       \n"
"};                                                \n"
"                                                  \n"
"vso vsmain( vsi i )                               \n"
"{                                                 \n"
"    vso o;                                        \n"
"    o.pos = mul( i.pos, gPvw );                   \n"
"    float3 n = 2*abs(i.nml) + i.nml;              \n"
"    o.clr = float4( n/3.0, 1.0 );                 \n"
"    o.uv = i.uv;                                  \n"
"    return o;                                     \n"
"}                                                 \n"
"                                                  \n"
"float4 psmain( vso i ) : COLOR0                   \n"
"{                                                 \n"
"	return tex2D(s0, i.uv) * i.clr;                \n"
"}                                                 \n"
"                                                  \n"
"technique                                         \n"
"{                                                 \n"
"	pass                                           \n"
"	{                                              \n"
"		vertexshader = compile vs_2_0 vsmain();    \n"
"		pixelshader  = compile ps_2_0 psmain();    \n"
"	}                                              \n"
"}";

// *****************************************************************************
// public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool TestD3D9Hlsl::init( GraphicsSystem & gs )
{
    // initialize matrix
    Matrix44f view, proj;
    view.translate( 0, 0, 200 );
    proj.perspectiveD3DLh( 1.0f, 4.0f/3.0f, 10.0f, 1000.0f );
    mProjView = proj * view;
    mArcBall.setHandness( util::LEFT_HAND );
    mArcBall.setViewMatrix( view );
    mArcBall.connectToInput();
    mArcBall.setMouseMoveWindow( 0, 0, gs.getDesc().width, gs.getDesc().height );

    // create cube mesh
    static Vertex vertices[24];
    static UInt16 indices[36];
    createBox(
        100, 100, 100, // box size
        &vertices[0].x, sizeof(Vertex),
        &vertices[0].u, sizeof(Vertex),
         vertices[0].n, sizeof(Vertex),
        0, 0, 0, 0, // tangend and binormal
        indices,
        0 // quad list
        );

    // create kernel
    mKernel = gs.getKernel<Hlsl9Kernel>( "HLSL9" );
    if( 0 == mKernel ) return false;

    // create parameters
    mParam = mKernel->createParameterSet();
    if( 0 == mParam ) return false;
    mParam->setFx( fxcode );
    mParam->setPrimType( TRIANGLE_LIST ); // D3DPT_TRIANGLELIST
    mParam->setPrimCount( 12 );
    mParam->setBaseVertex( 0 );
    mParam->setVertexCount( 24 );
    mParam->setBaseIndex( 0 );

    // create vertex buffer
    SurfaceCreationParameter scp;
    scp.bindTo( "HLSL9", "VTXBUF0" );
    scp.forcedAccessFlags = SURFACE_ACCESS_HOST_WRITE;
    scp.layout.dim = SURFACE_DIMENSION_1D;
    scp.layout.levels = 1;
    scp.layout.faces = 1;
    scp.layout.basemap.width = GN_ARRAY_COUNT(vertices);
    scp.layout.basemap.height = 1;
    scp.layout.basemap.depth = 1;
    scp.layout.basemap.rowBytes = sizeof(vertices);
    scp.layout.basemap.sliceBytes = scp.layout.basemap.rowBytes;
    scp.layout.format.attribs[0].semantic.set( "POS0" );
    scp.layout.format.attribs[0].offset = 0;
    scp.layout.format.attribs[0].format = FMT_FLOAT3;
    scp.layout.format.attribs[1].semantic.set( "NML0" );
    scp.layout.format.attribs[1].offset = 12;
    scp.layout.format.attribs[1].format = FMT_FLOAT3;
    scp.layout.format.attribs[2].semantic.set( "TEX0" );
    scp.layout.format.attribs[2].offset = 24;
    scp.layout.format.attribs[2].format = FMT_FLOAT2;
    scp.layout.format.count = 3;
    scp.layout.format.stride = sizeof(Vertex);
    mVtxBuf = gs.createSurface( scp );
    if( 0 == mVtxBuf ) return false;
    mVtxBuf->download(
        0,
        0,
        vertices,
        sizeof(vertices),
        sizeof(vertices) );

    // create index buffer
    scp.bindings.clear();
    scp.bindTo( "HLSL9", "IDXBUF" );
    scp.forcedAccessFlags = SURFACE_ACCESS_HOST_WRITE;
    scp.layout.dim = SURFACE_DIMENSION_1D;
    scp.layout.levels = 1;
    scp.layout.faces = 1;
    scp.layout.basemap.width = GN_ARRAY_COUNT(indices);
    scp.layout.basemap.height = 1;
    scp.layout.basemap.depth = 1;
    scp.layout.basemap.rowBytes = sizeof(indices);
    scp.layout.basemap.sliceBytes = scp.layout.basemap.rowBytes;
    scp.layout.format.attribs[0].semantic.set( "INDEX" );
    scp.layout.format.attribs[0].offset = 0;
    scp.layout.format.attribs[0].format = FMT_R_16_UINT;
    scp.layout.format.count = 1;
    scp.layout.format.stride = sizeof(short);
    mIdxBuf = gs.createSurface( scp );
    if( 0 == mIdxBuf ) return false;
    mIdxBuf->download(
        0,
        0,
        indices,
        sizeof(indices),
        sizeof(indices) );

    // create texture
    TextureData td;
    if( !td.load( "media::/texture/rabit.png" ) ) return false;
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
    mTexture = gs.createSurface( scp );
    if( 0 == mTexture ) return false;
    for( size_t f = 0; f < scp.layout.faces; ++f )
    for( size_t l = 0; l < scp.layout.levels; ++l )
    {
        const MipmapDesc & mmd = td.id.getMipmap( f, l );
        mTexture->download(
            calcSubSurfaceIndex( f, l, scp.layout.levels ),
            0,
            td.buf.cptr() + td.id.getMipmapOffset( f, l ),
            mmd.rowPitch,
            mmd.slicePitch );
    }

    // create binding
    Hlsl9PortBinding bd;
    bd.vtxbufs[0].set( mVtxBuf, 0, 1, 0, 1 );
    bd.idxbuf.set( mIdxBuf, 0, 1, 0, 1 );
    bd.textures[0].set( mTexture, 0, td.id.numLevels, 0, td.id.numFaces );
    mBinding = mKernel->createPortBinding( bd );
    if( 0 == mBinding ) return false;

    // success
    return true;
}

//
//
// -----------------------------------------------------------------------------
void TestD3D9Hlsl::quit( GraphicsSystem & )
{
    if( mBinding )
    {
        GN_ASSERT( mKernel );
        mKernel->deletePortBinding( mBinding );
        mBinding = 0;
    }
    safeDelete( mVtxBuf );
    safeDelete( mIdxBuf );
    safeDelete( mTexture );
    safeDelete( mParam );
    mKernel = 0;
}

//
//
// -----------------------------------------------------------------------------
void TestD3D9Hlsl::draw( GraphicsSystem & )
{
    GN_ASSERT( mKernel );
    GN_ASSERT( mParam );

    Matrix44f world = mArcBall.getRotationMatrix44();
    Matrix44f pvw = mProjView * world;

    mParam->setVSConstF( 0, 4, pvw[0] );

    mKernel->render( *mParam, mBinding );
}
