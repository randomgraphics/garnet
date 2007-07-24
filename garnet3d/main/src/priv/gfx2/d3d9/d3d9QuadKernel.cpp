#include "pch.h"
#include "d3d9QuadKernel.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx2.D3D9QuadKernel");

namespace GN { namespace gfx
{
    static inline void sEnableBlend( D3D9RenderStateBlock & rsb )
    {
        rsb.setRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
        rsb.setRenderState( D3DRS_BLENDOP         , D3DBLENDOP_ADD );
        rsb.setRenderState( D3DRS_SRCBLEND        , D3DBLEND_SRCALPHA );
        rsb.setRenderState( D3DRS_DESTBLEND       , D3DBLEND_INVSRCALPHA );
        rsb.setRenderState( D3DRS_BLENDOPALPHA    , D3DBLENDOP_ADD );
        rsb.setRenderState( D3DRS_SRCBLENDALPHA   , D3DBLEND_ONE );
        rsb.setRenderState( D3DRS_DESTBLENDALPHA  , D3DBLEND_ZERO );
    }

    class TransparentParameter : public BoolKernelParameter
    {
        D3D9RenderStateBlock & mRsb;

    public:

        TransparentParameter( const KernelParameterDesc & desc, bool initial, D3D9RenderStateBlock & rsb )
            : BoolKernelParameter( desc, initial )
            , mRsb( rsb )
        {
        }

        void setb( size_t offset, size_t count, const bool * values )
        {
            if( 0 != offset || 1 != count || NULL == values )
            {
                GN_ERROR(getLogger("GN.gfx2.TransparentParameter"))( "invalid parameter value." );
                return;
            }

            value = *values;

            if( value )
            {
                sEnableBlend( mRsb );
            }
            else
            {
                mRsb.setRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
            }
        }
    };

    ///
    /// parameter set for D3D9 quad kernel
    ///
    class D3D9QuadKernelParameterSet : public KernelParameterSet
    {
        BoolKernelParameter   mTransparent;
        D3D9RenderStateBlock  mRsb;

        void setDefaultRsb()
        {
            sEnableBlend( mRsb );

            mRsb.setRenderState( D3DRS_ZENABLE         , FALSE );
            mRsb.setRenderState( D3DRS_ZWRITEENABLE    , FALSE );
            mRsb.setRenderState( D3DRS_CULLMODE        , D3DCULL_CW );

            mRsb.setSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_POINT );
            mRsb.setSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT );
            mRsb.setSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_NONE );
        }

    public:

        //@{

        D3D9QuadKernelParameterSet( D3D9Kernel & k )
            : KernelParameterSet( k )
            , mTransparent( *k.getParameterDesc(0), true )
            , mRsb( k.d3d9gs() )
        {
            setDefaultRsb();
        }

        ~D3D9QuadKernelParameterSet()
        {
        }

        void applyRsb() const { mRsb.apply(); }

        virtual KernelParameter & get( size_t index )
        {
            switch( index )
            {
                case 0  : return mTransparent;
                default :
                    GN_ERROR(sLogger)( "parameter index is out of range!" );
                    return DummyKernelParameter::sGetInstance();
            }
        }

        //@}
    };
}}

// *****************************************************************************
// D3D9QuadStream
// *****************************************************************************

GN::gfx::D3D9QuadStream::D3D9QuadStream( D3D9GraphicsSystem & gs, const char * name )
    : D3D9UnstableResource( gs )
{
    clear();

    mDesc.name = name;
    mDesc.format.attribs[0].set( "POSITION",  0, FMT_FLOAT3 );
    mDesc.format.attribs[1].set( "COLOR"   , 12, FMT_RGBA32 );
    mDesc.format.attribs[2].set( "TEXCOORD", 16, FMT_FLOAT2 );
    mDesc.format.count = 3;
    mDesc.format.stride = 32;
    mDesc.maxBytes = MAX_QUADS * sizeof(QuadVertex);
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D9QuadStream::init()
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::D3D9QuadStream, () );

    IDirect3DDevice9 * dev = d3d9gs().d3ddev();

    // create vb
    for( int i = 0; i < NUM_VTXBUFS; ++i )
    {
        GN_DX9_CHECK_RV(
            dev->CreateVertexBuffer( MAX_QUADS * 4 * sizeof(QuadVertex), 0, 0, D3DPOOL_MANAGED, &mVtxBufs[i], 0 ),
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
void GN::gfx::D3D9QuadStream::quit()
{
    GN_GUARD;

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
void GN::gfx::D3D9QuadStream::push( const void * source, size_t bytes )
{
    GN_GUARD_SLOW;

    // check parameter
    if( 0 == source )
    {
        GN_ERROR(sLogger)( "null data pointer." );
        return;
    }
    if( bytes > freeBytes() )
    {
        GN_ERROR(sLogger)( "no enough space." );
        return;
    }

    IDirect3DVertexBuffer9 * vb = mVtxBufs[mActiveVB];
    GN_ASSERT( vb );

    UInt32 newQuads = (UInt32)( bytes / sizeof(QuadVertex) / 4 );

    void * dest;

    GN_DX9_CHECK_R( vb->Lock( (UInt32)mNumQuads * 4 * sizeof(QuadVertex), (UInt32)newQuads * 4 * sizeof(QuadVertex), &dest, 0 ) );

    memcpy( dest, source, bytes );

    vb->Unlock();

    mNumQuads += newQuads;
    GN_ASSERT( mNumQuads <= MAX_QUADS );

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
inline void GN::gfx::D3D9QuadStream::draw()
{
    IDirect3DDevice9 * dev = d3d9gs().d3ddev();

    // bind vertex buffer
    dev->SetStreamSource( 0, mVtxBufs[mActiveVB], 0, sizeof(QuadVertex) );

    // do rendering
    dev->DrawIndexedPrimitive(
        D3DPT_TRIANGLELIST,
        0, // base vertex index
        0, // min index
        (UInt32)mNumQuads * 4, // num vertex
        0, // start index
        (UInt32)mNumQuads * 2 ); // prim count

    mActiveVB = ( mActiveVB + 1 ) & 0x7F; // (n+1)%128;
    mNumQuads = 0;
}

// *****************************************************************************
// D3D9QuadKernel
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::gfx::D3D9QuadKernel::D3D9QuadKernel( D3D9GraphicsSystem & gs )
    : D3D9Kernel( KERNEL_NAME(), gs )
    , mTarget0( gs, "TARGET0", 0 )
    , mDepth( gs, "DEPTH" )
    , mTexture( gs, "TEXTURE0", 0 )
    , mQuads( gs, "QUADS" )
{
    // setup ports
    addPortRef( mTarget0 );
    addPortRef( mDepth );
    addPortRef( mTexture );

    // setup streams
    addStreamRef( mQuads );

    // setup parameters
    addParameter( "TRANSPARENT", KERNEL_PARAMETER_TYPE_BOOL, 1 );
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D9QuadKernel::init()
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::D3D9QuadKernel, () );

    IDirect3DDevice9 * dev = d3d9gs().d3ddev();

    const GraphicsSystemDesc & gsdesc = d3d9gs().getDesc();

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
        dev->CreateIndexBuffer( D3D9QuadStream::MAX_QUADS * 6 * sizeof(short), 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &mIdxBuf, 0 ),
        failure() );
    UInt16 * indices;
    GN_DX9_CHECK_RV( mIdxBuf->Lock( 0, 0, (void**)&indices, 0 ), failure() );
    for( UInt16 i = 0; i < D3D9QuadStream::MAX_QUADS; ++i )
    {
        indices[i*6+0] = i*4+0;
        indices[i*6+1] = i*4+1;
        indices[i*6+2] = i*4+2;
        indices[i*6+3] = i*4+0;
        indices[i*6+4] = i*4+2;
        indices[i*6+5] = i*4+3;
    }
    mIdxBuf->Unlock();

    // initialize quads
    if( !mQuads.init() ) return failure();

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
    mQuads.quit();

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::KernelParameterSet * GN::gfx::D3D9QuadKernel::createParameterSet()
{
    return new D3D9QuadKernelParameterSet(*this);
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

    p.applyRsb();

    IDirect3DDevice9 * dev = d3d9gs().d3ddev();

    dev->SetVertexShader( mVs );
    dev->SetPixelShader( mPs );
    dev->SetVertexDeclaration( mDecl );
    dev->SetIndices( mIdxBuf );

    mQuads.draw();

    GN_UNGUARD_SLOW;
}
