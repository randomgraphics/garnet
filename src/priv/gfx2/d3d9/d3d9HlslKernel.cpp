#include "pch.h"
#include "d3d9HlslKernel.h"
#include "garnet/GNgfx.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx2.D3D9HlslKernel");

// *****************************************************************************
// local functions
// *****************************************************************************

struct D3D9DebugMuter
{
    BOOL old;

    D3D9DebugMuter() : old( D3DXDebugMute(true) )
    {
    }

    ~D3D9DebugMuter()
    {
        D3DXDebugMute( old );
    }
};

// *****************************************************************************
// D3D9HlslKernelParameterSet
// *****************************************************************************

namespace GN { namespace gfx
{
    ///
    /// parameter set for D3D9 hlsl kernel
    ///
    class D3D9HlslKernelParameterSet : public BaseKernelParameterSet, public SlotBase
    {
        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        D3D9HlslKernelParameterSet( D3D9Kernel & k )
            : BaseKernelParameterSet( k )
            , mDev( k.d3d9gs().d3ddev() )
        {
            mVscfUpdate.clear();
            mPscfUpdate.clear();

            mParamFx = getBaseParameterByName( "FX" );
            mParamFx->sigValueSet.connect( this, &D3D9HlslKernelParameterSet::onFxSet );

            mParamVscf = getBaseParameterByName( "VSCF" );
            mParamVscf->sigValueSet.connect( this, &D3D9HlslKernelParameterSet::onVscfSet );

            mParamPscf = getBaseParameterByName( "PSCF" );
            mParamPscf->sigValueSet.connect( this, &D3D9HlslKernelParameterSet::onPscfSet );
        }
        ~D3D9HlslKernelParameterSet() {}
        //@}

        // ********************************
        // public functions
        // ********************************
    public:

        ID3DXEffect * getEffect() const { return mFx; }

        void applyShaderConstants() const
        {
            // apply shader constants
            const float * v;
            if( mVscfUpdate.registerCount > 0 )
            {
                v = mParamVscf->toFloat();
                mDev->SetVertexShaderConstantF(
                    mVscfUpdate.firstRegister,
                    v + mVscfUpdate.firstRegister * 4,
                    mVscfUpdate.registerCount );
            }
            if( mPscfUpdate.registerCount > 0 )
            {
                v = mParamPscf->toFloat();
                mDev->SetPixelShaderConstantF(
                    mPscfUpdate.firstRegister,
                    v + mPscfUpdate.firstRegister * 4,
                    mPscfUpdate.registerCount );
            }
        }

        // ********************************
        // private variables
        // ********************************
    private:

        ///
        /// record offset and range of shader const update
        ///
        struct ConstUpdate
        {
            //@{
            UInt32 firstRegister;
            UInt32 registerCount;
            void clear() { firstRegister = 0; registerCount = 0; }
            void merge( UInt32 f, UInt32 c )
            {
                if( 0 == registerCount )
                {
                    firstRegister = f;
                    registerCount = c;
                }
                else
                {
                    firstRegister = min( firstRegister, f );
                    registerCount = max( registerCount, c ) - firstRegister;
                }
            }
            //@}
        };

        IDirect3DDevice9      * mDev;
        AutoComPtr<ID3DXEffect> mFx;
        ConstUpdate             mVscfUpdate, mPscfUpdate;
        BaseKernelParameter   * mParamFx, * mParamVscf, * mParamPscf;

        // ********************************
        // private functions
        // ********************************
    private:

        void onFxSet( size_t, size_t, size_t )
        {
            GN_ASSERT( KERNEL_PARAMETER_TYPE_STRING == mParamFx->getDesc().type );

            const StrA * s = mParamFx->toString();
            if( 0 == s || s->empty() )
            {
                GN_ERROR(sLogger)( "Null FX string." );
                return;
            }

            mFx.attach( d3d9::compileEffect( mDev, s->cptr(), s->size() ) );
        }

        void onVscfSet( size_t, size_t offset, size_t count )
        {
            mVscfUpdate.merge( (UInt32)offset/4, (UInt32)count/4 );
        }

        void onPscfSet( size_t, size_t offset, size_t count )
        {
            mPscfUpdate.merge( (UInt32)offset/4, (UInt32)count/4 );
        }
    };
}}

// *****************************************************************************
// public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::gfx::D3D9HlslKernel::D3D9HlslKernel( D3D9GraphicsSystem & gs )
    : D3D9Kernel( gs )
    , mRenderTarget0( gs )
    , mRenderTarget1( gs )
    , mRenderTarget2( gs )
    , mRenderTarget3( gs )
    , mDepthBuffer( gs )
    , mTexture0( gs, 0 )
    , mTexture1( gs, 1 )
    , mTexture2( gs, 2 )
    , mTexture3( gs, 3 )
    , mTexture4( gs, 4 )
    , mTexture5( gs, 5 )
    , mTexture6( gs, 6 )
    , mTexture7( gs, 7 )
    , mVtxBuf0( gs, 0 )
    , mVtxBuf1( gs, 1 )
    , mVtxBuf2( gs, 2 )
    , mVtxBuf3( gs, 3 )
    , mVtxBuf4( gs, 4 )
    , mVtxBuf5( gs, 5 )
    , mVtxBuf6( gs, 6 )
    , mVtxBuf7( gs, 7 )
    , mIdxBuf( gs )
    , mRsb( gs )
{
    // setup parameters
    addParameter( "FX", KERNEL_PARAMETER_TYPE_STRING, 1 );

    addParameter( "VSCF", KERNEL_PARAMETER_TYPE_FLOAT, 256 * 4 );
    addParameter( "PSCF", KERNEL_PARAMETER_TYPE_FLOAT, 256 * 4 );

    mPrimType = addParameter( "PRIM_TYPE", KERNEL_PARAMETER_TYPE_INT, 1 );
    mPrimCount = addParameter( "PRIM_COUNT", KERNEL_PARAMETER_TYPE_INT, 1 );
    mBaseVertex = addParameter( "BASE_VERTEX", KERNEL_PARAMETER_TYPE_INT, 1 );
    mBaseIndex = addParameter( "BASE_INDEX", KERNEL_PARAMETER_TYPE_INT, 1 );
    mVertexCount = addParameter( "VERTEX_COUNT", KERNEL_PARAMETER_TYPE_INT, 1 );

    // setup ports
    addPortRef( "TARGET0"  , &mRenderTarget0 );
    addPortRef( "TARGET1"  , &mRenderTarget1 );
    addPortRef( "TARGET2"  , &mRenderTarget2 );
    addPortRef( "TARGET3"  , &mRenderTarget3 );
    addPortRef( "DEPTH"    , &mDepthBuffer );
    addPortRef( "TEXTURE0" , &mTexture0 );
    addPortRef( "TEXTURE1" , &mTexture1 );
    addPortRef( "TEXTURE2" , &mTexture2 );
    addPortRef( "TEXTURE3" , &mTexture3 );
    addPortRef( "TEXTURE4" , &mTexture4 );
    addPortRef( "TEXTURE5" , &mTexture5 );
    addPortRef( "TEXTURE6" , &mTexture6 );
    addPortRef( "TEXTURE7" , &mTexture7 );
    addPortRef( "VTXBUF0"  , &mVtxBuf0 );
    addPortRef( "VTXBUF1"  , &mVtxBuf1 );
    addPortRef( "VTXBUF2"  , &mVtxBuf2 );
    addPortRef( "VTXBUF3"  , &mVtxBuf3 );
    addPortRef( "VTXBUF4"  , &mVtxBuf4 );
    addPortRef( "VTXBUF5"  , &mVtxBuf5 );
    addPortRef( "VTXBUF6"  , &mVtxBuf6 );
    addPortRef( "VTXBUF7"  , &mVtxBuf7 );
    addPortRef( "IDXBUF"   , &mIdxBuf );
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::KernelParameterSet *
GN::gfx::D3D9HlslKernel::createParameterSet()
{
    GN_GUARD;

    return new D3D9HlslKernelParameterSet( *this );

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9HlslKernel::render(
    const KernelParameterSet & param, KernelBinding binding )
{
    GN_GUARD_SLOW;

    PIXPERF_FUNCTION_EVENT();

    const D3D9HlslKernelParameterSet & p = safeCast<const D3D9HlslKernelParameterSet &>(param);
    ID3DXEffect * fx = p.getEffect();
    if( 0 == fx ) return;

    D3D9KernelPortBinding & b = getPortBinding( binding );
    b.apply();

    D3D9GraphicsSystem & gs = d3d9gs();
    IDirect3DDevice9  * dev = gs.d3ddev();

    const BaseKernelParameter
        * pt = GN_SAFE_CAST<BaseKernelParameter*>( param.getParameter( mPrimType ) ),
        * pc = GN_SAFE_CAST<BaseKernelParameter*>( param.getParameter( mPrimCount ) ),
        * bv = GN_SAFE_CAST<BaseKernelParameter*>( param.getParameter( mBaseVertex ) );
    GN_ASSERT( pt && !pt->empty() );
    GN_ASSERT( pc && !pc->empty() );
    GN_ASSERT( bv && !bv->empty() );

    // restore to default render states
    gs.setRenderStateBlock( mRsb );

    if( b.hasIdxBuf() )
    {
        const BaseKernelParameter
            * bi = GN_SAFE_CAST<BaseKernelParameter*>( param.getParameter( mBaseIndex ) ),
            * vc = GN_SAFE_CAST<BaseKernelParameter*>( param.getParameter( mVertexCount ) );
        GN_ASSERT( bi && !bi->empty() );
        GN_ASSERT( vc && !vc->empty() );

        UINT passes;
        if( SUCCEEDED( fx->Begin( &passes, 0 ) ) )
        {
            for( UINT i = 0; i < passes; ++i )
            {
                {
                    D3D9DebugMuter muter;
                    fx->BeginPass( i );
                    fx->CommitChanges();
                }

                p.applyShaderConstants();

                GN_DX9_CHECK( dev->DrawIndexedPrimitive(
                    (D3DPRIMITIVETYPE)*pt->toInt(),
                    *bv->toInt(),
                    0, // min index
                    *vc->toUInt(),
                    *bi->toUInt(),
                    *pc->toUInt() ) );

                fx->EndPass();
            }

            {
                D3D9DebugMuter muter;
                fx->End();
            }
        }

    }
    else
    {
        UINT passes;
        if( SUCCEEDED( fx->Begin( &passes, 0 ) ) )
        {
            for( UINT i = 0; i < passes; ++i )
            {
                {
                    D3D9DebugMuter muter;
                    fx->BeginPass( i );
                    fx->CommitChanges();
                }

                p.applyShaderConstants();

                GN_DX9_CHECK( dev->DrawPrimitive(
                    (D3DPRIMITIVETYPE)*pt->toInt(),
                    *bv->toUInt(),
                    *pc->toUInt() ) );

                fx->EndPass();
            }

            {
                D3D9DebugMuter muter;
                fx->End();
            }
        }
    }

    GN_UNGUARD_SLOW;
}

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
