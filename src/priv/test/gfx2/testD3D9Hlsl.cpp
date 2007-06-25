#include "pch.h"
#include "testD3D9Hlsl.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::gfx2;

// *****************************************************************************
// local code
// *****************************************************************************

struct Vertex
{
    float x, y, z;
    float n[3];
    float u, v;
};

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

// *****************************************************************************
// public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool TestD3D9Hlsl::init( GraphicsSystem & gs )
{
    // create effect
    mEffect = gs.getEffect( "D3D9_HLSL" );
    if( 0 == mEffect ) return false;

    // create parameters
    mParam = mEffect->createParameterSet();
    if( 0 == mParam ) return false;
    mParam->setParameter( "VS", vs_code );
    mParam->setParameter( "PS", ps_code );

    // create surfaces
    SurfaceCreationParameter scp;
    scp.bindings.append( SurfaceBindingParameter("D3D9_HLSL","VTXBUF0") );
    scp.forcedAccessFlags = SURFACE_ACCESS_HOST_WRITE;
    scp.layout.dim = SURFACE_DIMENSION_1D;
    scp.layout.levels = 1;
    scp.layout.faces = 1;
    scp.layout.basemap.width = 24; // 24 vertices
    scp.layout.basemap.height = 1;
    scp.layout.basemap.depth = 1;
    scp.layout.basemap.rowBytes = 24 * sizeof(Vertex);
    scp.layout.basemap.sliceBytes = scp.layout.basemap.rowBytes;
    scp.layout.elements.attribs[0].semantic.set( "POSITION0" );
    scp.layout.elements.attribs[0].offset = 0;
    scp.layout.elements.attribs[0].format = FMT_FLOAT3;
    scp.layout.elements.attribs[1].semantic.set( "NORMAL0" );
    scp.layout.elements.attribs[1].offset = 12;
    scp.layout.elements.attribs[1].format = FMT_FLOAT3;
    scp.layout.elements.attribs[1].semantic.set( "TEXCOORD0" );
    scp.layout.elements.attribs[1].offset = 24;
    scp.layout.elements.attribs[1].format = FMT_FLOAT2;
    scp.layout.elements.count = 3;
    scp.layout.elements.stride = sizeof(Vertex);
    mVtxBuf = gs.createSurface( scp );
    if( 0 == mVtxBuf ) return false;

    // create binding
    EffectBindingDesc bd;
    mBinding = mEffect->createBinding( bd );
    if( 0 == mBinding ) return false;

    mProj.perspectiveD3DLh( 1.0f, 4.0f/3.0f, 10.0f, 1000.0f );
    mView.translate( 0, 0, 200 );
    mWorld.identity();

    return true;
}

//
//
// -----------------------------------------------------------------------------
void TestD3D9Hlsl::quit( GraphicsSystem & )
{
    if( mBinding )
    {
        GN_ASSERT( mEffect );
        mEffect->deleteBinding( mBinding );
        mBinding = 0;
    }
    safeDelete( mVtxBuf );
    safeDelete( mIdxBuf );
    safeDelete( mTexture );
    safeDelete( mParam );
    mEffect = 0;
}

//
//
// -----------------------------------------------------------------------------
void TestD3D9Hlsl::draw( GraphicsSystem & )
{
    GN_ASSERT( mEffect );
    GN_ASSERT( mParam );
    mEffect->render( *mParam, mBinding );
}
