#include "pch.h"
#include "d3d9QuadKernel.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx2.D3D9QuadKernel");

namespace GN { namespace gfx
{
    ///
    /// parameter set for D3D9 hlsl kernel
    ///
    class D3D9QuadKernelParameterSet : public QuadKernelParameterSet
    {
        D3D9RenderStateBlock  mRsb;
        bool                  mBlendEnabled;

        void setDefaultRsb()
        {
            setBlend( true );

            mRsb.setRenderState( D3DRS_ZENABLE         , FALSE );
            mRsb.setRenderState( D3DRS_ZWRITEENABLE    , FALSE );
            mRsb.setRenderState( D3DRS_CULLMODE        , D3DCULL_CW );

            mRsb.setSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_POINT );
            mRsb.setSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT );
            mRsb.setSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_NONE );
        }

    public:

        //@{

        D3D9QuadKernelParameterSet( D3D9GraphicsSystem & gs )
            : mRsb( gs )
            , mBlendEnabled( true )
        {
            setDefaultRsb();
        }

        ~D3D9QuadKernelParameterSet()
        {
        }

        void apply() const { mRsb.apply(); }

        virtual void setBlend( bool enabled )
        {
            if( enabled )
            {
                mRsb.setRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
                mRsb.setRenderState( D3DRS_BLENDOP         , D3DBLENDOP_ADD );
                mRsb.setRenderState( D3DRS_SRCBLEND        , D3DBLEND_SRCALPHA );
                mRsb.setRenderState( D3DRS_DESTBLEND       , D3DBLEND_INVSRCALPHA );
                mRsb.setRenderState( D3DRS_BLENDOPALPHA    , D3DBLENDOP_ADD );
                mRsb.setRenderState( D3DRS_SRCBLENDALPHA   , D3DBLEND_ONE );
                mRsb.setRenderState( D3DRS_DESTBLENDALPHA  , D3DBLEND_ZERO );
            }
            else
            {
                mRsb.setRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
            }
        }

        //@}
    };
}}

// *****************************************************************************
// D3D9QuadKernel
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::gfx::D3D9QuadKernel::D3D9QuadKernel( D3D9GraphicsSystem & gs )
    : D3D9KernelBaseT<QuadKernel>( gs )
    , mTarget0(gs,"TARGET",0)
    , mDepth(gs,"DEPTH")
    , mTexture(gs,"TEXTURE",0)
{
    addPortRef( mTarget0 );
    addPortRef( mDepth );
    addPortRef( mTexture );
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D9QuadKernel::init()
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::D3D9QuadKernel, () );

    IDirect3DDevice9 * dev = gfxsys().d3ddev();

    const GraphicsSystemDesc & gsdesc = gfxsys().getDesc();

    StrA vscode;
    vscode.format(
        "struct VSOUT                       \n"
        "{                                  \n"
        "   float4 pos : POSITION;          \n"
        "   float4 clr : COLOR0;            \n"
        "   float2 tex : TEXCOORD0;         \n"
        "};                                 \n"
        "VSOUT main(                        \n"
        "   in float4 pos : POSITION,       \n"
        "   in float4 clr : COLOR0,         \n"
        "   in float2 tex : TEXCOORD0 )     \n"
        "{                                  \n"
        "   VSOUT o;                        \n"
        "   o.pos.x = pos.x *  2.0 - 1.0 - 0.5 / %d; \n"
        "   o.pos.y = pos.y * -2.0 + 1.0 - 0.5 / %d; \n"
        "   o.pos.zw = pos.zw;              \n"
        "   o.clr = clr.zyxw; // BGRA->RGBA \n"
        "   o.tex = tex;                    \n"
        "   return o;                       \n"
        "}", gsdesc.width, gsdesc.height );
    static const char * pscode =
        "struct VSOUT                       \n"
        "{                                  \n"
        "   float4 pos : POSITION;          \n"
        "   float4 clr : COLOR0;            \n"
        "   float2 tex : TEXCOORD0;         \n"
        "};                                 \n"
        "sampler s0 : register(s0);         \n"
        "float4 main( VSOUT i ) : COLOR     \n"
        "{                                  \n"
        "   return i.clr * tex2D( s0, i.tex ); \n"
        "}";

    // create shader
    mVs.attach( d3d9::compileVS( dev, vscode.cptr() ) );
    mPs.attach( d3d9::compilePS( dev, pscode ) );
    if( !mVs || !mPs ) return failure();

    // create decl
    D3DVERTEXELEMENT9 elements[] = {
        { 0,  0, D3DDECLTYPE_FLOAT3  , 0, D3DDECLUSAGE_POSITION, 0 },
        { 0, 12, D3DDECLTYPE_D3DCOLOR, 0, D3DDECLUSAGE_COLOR, 0 },
        { 0, 16, D3DDECLTYPE_FLOAT2  , 0, D3DDECLUSAGE_TEXCOORD, 0 },
        D3DDECL_END()
    };
    GN_DX9_CHECK_RV( dev->CreateVertexDeclaration( elements, &mDecl ), failure() );

    // create idxbuf
    GN_DX9_CHECK_RV(
        dev->CreateIndexBuffer( MAX_QUADS * 6 * sizeof(short), 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &mIdxBuf, 0 ),
        failure() );
    UInt16 * indices;
    GN_DX9_CHECK_RV( mIdxBuf->Lock( 0, 0, (void**)&indices, 0 ), failure() );
    for( UInt16 i = 0; i < MAX_QUADS; ++i )
    {
        indices[i*6+0] = i*4+0;
        indices[i*6+1] = i*4+1;
        indices[i*6+2] = i*4+2;
        indices[i*6+3] = i*4+0;
        indices[i*6+4] = i*4+2;
        indices[i*6+5] = i*4+3;
    }
    mIdxBuf->Unlock();

    // create vb
    for( int i = 0; i < NUM_VTXBUFS; ++i )
    {
        GN_DX9_CHECK_RV(
            dev->CreateVertexBuffer( MAX_QUADS * 4 * sizeof(QuadKernelVertex), 0, 0, D3DPOOL_MANAGED, &mVtxBufs[i], 0 ),
            failure() );
    }

    mActiveVB = 0;
    mNumQuads = 0;

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9QuadKernel::quit()
{
    GN_GUARD;

    mVs.clear();
    mPs.clear();
    mDecl.clear();
    mIdxBuf.clear();
    for( int i = 0; i < NUM_VTXBUFS; ++i )
    {
        mVtxBufs[i].clear();
    }

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::KernelParameterSet * GN::gfx::D3D9QuadKernel::createParameterSet()
{
    return new D3D9QuadKernelParameterSet( gfxsys() );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9QuadKernel::pushVertices( const QuadKernelVertex * vertices, size_t count )
{
    GN_GUARD_SLOW;

    // check parameter
    if( 0 == vertices && count > 0 )
    {
        GN_ERROR(sLogger)( "null data pointer with non-zero count." );
        return;
    }
    if( count > getAvailableVertices() )
    {
        GN_ERROR(sLogger)( "no enough space." );
        return;
    }

    IDirect3DVertexBuffer9 * vb = mVtxBufs[mActiveVB];
    GN_ASSERT( vb );

    size_t newQuads = count / 4;

    void * dest;

    GN_DX9_CHECK_R( vb->Lock( (UInt32)mNumQuads * 4 * sizeof(QuadKernelVertex), (UInt32)newQuads * 4 * sizeof(QuadKernelVertex), &dest, 0 ) );

    memcpy( dest, vertices, count * sizeof(QuadKernelVertex) );

    vb->Unlock();

    mNumQuads += newQuads;
    GN_ASSERT( mNumQuads <= MAX_QUADS );

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
size_t GN::gfx::D3D9QuadKernel::getAvailableVertices() const
{
    return (MAX_QUADS - mNumQuads) * 4;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9QuadKernel::render( const KernelParameterSet & param, KernelPortBinding binding )
{
    GN_GUARD_SLOW;

    PIXPERF_FUNCTION_EVENT();

    D3D9KernelPortBinding & b = getPortBinding( binding );
    b.apply();

    const D3D9QuadKernelParameterSet & p = safeCastRef<const D3D9QuadKernelParameterSet>(param);

    p.apply();

    IDirect3DDevice9 * dev = gfxsys().d3ddev();

    dev->SetVertexShader( mVs );
    dev->SetPixelShader( mPs );
    dev->SetVertexDeclaration( mDecl );
    dev->SetIndices( mIdxBuf );
    dev->SetStreamSource( 0, mVtxBufs[mActiveVB], 0, sizeof(QuadKernelVertex) );

    // do rendering
    dev->DrawIndexedPrimitive(
        D3DPT_TRIANGLELIST,
        0, // base vertex index
        0, // min index
        (UInt32)mNumQuads * 4, // num vertex
        0, // start index
        (UInt32)mNumQuads * 2 ); // prim count

    // switch to next vertex buffer
    mActiveVB = ( mActiveVB + 1 ) & 0x7F; // (n+1)%128;
    mNumQuads = 0;

    GN_UNGUARD_SLOW;
}
