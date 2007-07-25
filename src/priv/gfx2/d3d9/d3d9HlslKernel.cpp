#include "pch.h"
#include "d3d9HlslKernel.h"

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
    /// FX parameter
    ///
    class D3D9FxParameter : public BaseKernelParameter, public D3D9UnstableResource
    {
        IDirect3DDevice9      * mDev;
        AutoComPtr<ID3DXEffect> mFx;

    public:

        D3D9FxParameter( const KernelParameterDesc & desc, D3D9GraphicsSystem & gs )
            : BaseKernelParameter( desc )
            , D3D9UnstableResource( gs )
            , mDev( gs.d3ddev() )
        {
        }

        virtual bool onRestore()
        {
            if( mFx ) GN_DX9_CHECK_RV( mFx->OnResetDevice(), false );
            return true;
        }

        virtual void onDispose()
        {
            if( mFx ) GN_DX9_CHECK( mFx->OnLostDevice() );
        }

        ID3DXEffect * operator->() const { GN_ASSERT( mFx ); return mFx; }

        void sets( size_t offset, size_t count, const char * const * values )
        {
            if( 0 != offset || 1 != count || NULL == values )
            {
                GN_ERROR(getLogger("GN.gfx2.D3D9FxParameter"))( "invalid parameter value." );
                return;
            }

            mFx.attach( d3d9::compileEffect( mDev, *values ) );
        }

        void unset() { mFx.clear(); }
    };

    ///
    /// Const parameter
    ///
    class D3D9ConstParameter : public BaseKernelParameter
    {
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

        Vector4f     mConstants[256];
        ConstUpdate  mUpdate;

    public:

        D3D9ConstParameter( const KernelParameterDesc & desc ) : BaseKernelParameter( desc ) { mUpdate.clear(); }

        void setVscf( IDirect3DDevice9 * dev ) const
        {
            if( mUpdate.registerCount > 0 )
            {
                dev->SetVertexShaderConstantF(
                    mUpdate.firstRegister,
                    mConstants[mUpdate.firstRegister],
                    mUpdate.registerCount );
            }
        }

        void setPscf( IDirect3DDevice9 * dev ) const
        {
            if( mUpdate.registerCount > 0 )
            {
                dev->SetPixelShaderConstantF(
                    mUpdate.firstRegister,
                    mConstants[mUpdate.firstRegister],
                    mUpdate.registerCount );
            }
        }

        void setf( size_t offset, size_t count, const float * values )
        {
            if( offset >= 256*4 || (offset+count) > 256*4 || NULL == values )
            {
                GN_ERROR(sLogger)( "invalid parameter value." );
                return;
            }

            UINT firstRegister = (UINT)offset / 4;
            UINT numRegisters  = (UINT)count / 4;

            mUpdate.merge( firstRegister, numRegisters );
            memcpy( &mConstants[firstRegister], values, numRegisters * sizeof(float) * 4 );
        }
    };

    ///
    /// primitive type parameter
    ///
    class D3D9PrimTypeParameter : public TypedKernelParameter<D3DPRIMITIVETYPE>
    {
        static inline D3DPRIMITIVETYPE pt2d3d( int pt )
        {
            static D3DPRIMITIVETYPE sPrimMap[GN::gfx::NUM_PRIMITIVES] =
            {
                D3DPT_POINTLIST,
                D3DPT_LINELIST,
                D3DPT_LINESTRIP,
                D3DPT_TRIANGLELIST,
                D3DPT_TRIANGLESTRIP,
            };
            if( pt > TRIANGLE_STRIP )
            {
                GN_ERROR(sLogger)( "invalid primitive type: %d", pt );
                return D3DPT_FORCE_DWORD;
            }
            else
            {
                return sPrimMap[pt];
            }
        }

    public:

        D3D9PrimTypeParameter( const KernelParameterDesc & desc, D3DPRIMITIVETYPE initial )
            : TypedKernelParameter<D3DPRIMITIVETYPE>( desc, initial )
        {
        }

        void seti( size_t offset, size_t count, const int * values )
        {
            if( 0 != offset || 1 != count || NULL == values )
            {
                GN_ERROR(getLogger("GN.gfx2.D3D9PrimTypeParameter"))( "invalid parameter value." );
                return;
            }

            value = pt2d3d( *values );
        }
    };

    ///
    /// parameter set for D3D9 hlsl kernel
    ///
    class D3D9HlslKernelParameterSet : public KernelParameterSet
    {
        IDirect3DDevice9                   * mDev;
        D3D9FxParameter                      mFx;
        D3D9ConstParameter                   mVscf, mPscf;
        D3D9PrimTypeParameter                mPrimType;
        IntKernelParameter<UINT>             mPrimCount;
        IntKernelParameter<UINT>             mBaseVertex;
        IntKernelParameter<UINT>             mBaseIndex;
        IntKernelParameter<UINT>             mVertexCount;

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        D3D9HlslKernelParameterSet( D3D9Kernel & k )
            : KernelParameterSet( k )
            , mDev( k.d3d9gs().d3ddev() )
            , mFx( *k.getParameterDesc(0), k.d3d9gs() )
            , mVscf( *k.getParameterDesc(1) )
            , mPscf( *k.getParameterDesc(2) )
            , mPrimType( *k.getParameterDesc(3), D3DPT_TRIANGLELIST )
            , mPrimCount( *k.getParameterDesc(4), 0 )
            , mBaseVertex( *k.getParameterDesc(5), 0 )
            , mBaseIndex( *k.getParameterDesc(6), 0 )
            , mVertexCount( *k.getParameterDesc(7), 0 )
        {
        }
        ~D3D9HlslKernelParameterSet() {}
        //@}

        // ********************************
        // public functions
        // ********************************
    public:

        void drawIndexed() const
        {
            UINT passes;
            if( SUCCEEDED( mFx->Begin( &passes, 0 ) ) )
            {
                for( UINT i = 0; i < passes; ++i )
                {
                    {
                        D3D9DebugMuter muter;
                        mFx->BeginPass( i );
                        mFx->CommitChanges();
                    }

                    // apply shader constants
                    mVscf.setVscf( mDev );
                    mPscf.setPscf( mDev );

                    GN_DX9_CHECK( mDev->DrawIndexedPrimitive(
                        mPrimType,
                        mBaseVertex,
                        0, // min index
                        mVertexCount,
                        mBaseIndex,
                        mPrimCount ) );

                    mFx->EndPass();
                }

                {
                    D3D9DebugMuter muter;
                    mFx->End();
                }
            }
        }


        void draw() const
        {
            UINT passes;
            if( SUCCEEDED( mFx->Begin( &passes, 0 ) ) )
            {
                for( UINT i = 0; i < passes; ++i )
                {
                    {
                        D3D9DebugMuter muter;
                        mFx->BeginPass( i );
                        mFx->CommitChanges();
                    }

                    // apply shader constants
                    mVscf.setVscf( mDev );
                    mPscf.setPscf( mDev );

                    GN_DX9_CHECK( mDev->DrawPrimitive(
                        mPrimType,
                        mBaseVertex,
                        mPrimCount ) );

                    mFx->EndPass();
                }

                {
                    D3D9DebugMuter muter;
                    mFx->End();
                }
            }
        }

        virtual KernelParameter & get( size_t index )
        {
            switch( index )
            {
                case 0  : return mFx;
                case 1  : return mVscf;
                case 2  : return mPscf;
                case 3  : return mPrimType;
                case 4  : return mPrimCount;
                case 5  : return mBaseVertex;
                case 6  : return mBaseIndex;
                case 7  : return mVertexCount;
                default :
                    GN_ERROR(sLogger)( "parameter index is out of range!" );
                    return DummyKernelParameter::sGetInstance();
            }
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
    : D3D9Kernel( KERNEL_NAME(), gs )
    , mRenderTarget0( gs, "TARGET0", 0 )
    , mRenderTarget1( gs, "TARGET1", 1 )
    , mRenderTarget2( gs, "TARGET2", 2 )
    , mRenderTarget3( gs, "TARGET3", 3 )
    , mDepthBuffer( gs, "DEPTH" )
    , mTexture0( gs, "TEXTURE0", 0 )
    , mTexture1( gs, "TEXTURE1", 1 )
    , mTexture2( gs, "TEXTURE2", 2 )
    , mTexture3( gs, "TEXTURE3", 3 )
    , mTexture4( gs, "TEXTURE4", 4 )
    , mTexture5( gs, "TEXTURE5", 5 )
    , mTexture6( gs, "TEXTURE6", 6 )
    , mTexture7( gs, "TEXTURE7", 7 )
    , mVtxBuf0( gs, "VTXBUF0", 0 )
    , mVtxBuf1( gs, "VTXBUF1", 1 )
    , mVtxBuf2( gs, "VTXBUF2", 2 )
    , mVtxBuf3( gs, "VTXBUF3", 3 )
    , mVtxBuf4( gs, "VTXBUF4", 4 )
    , mVtxBuf5( gs, "VTXBUF5", 5 )
    , mVtxBuf6( gs, "VTXBUF6", 6 )
    , mVtxBuf7( gs, "VTXBUF7", 7 )
    , mIdxBuf( gs, "IDXBUF" )
    , mRsb( gs )
{
    // setup parameters
    addParameter( "FX", KERNEL_PARAMETER_TYPE_STRING, 1 );
    addParameter( "VSCF", KERNEL_PARAMETER_TYPE_FLOAT, 256 * 4 );
    addParameter( "PSCF", KERNEL_PARAMETER_TYPE_FLOAT, 256 * 4 );
    addParameter( "PRIM_TYPE", KERNEL_PARAMETER_TYPE_INT, 1 );
    addParameter( "PRIM_COUNT", KERNEL_PARAMETER_TYPE_INT, 1 );
    addParameter( "BASE_VERTEX", KERNEL_PARAMETER_TYPE_INT, 1 );
    addParameter( "BASE_INDEX", KERNEL_PARAMETER_TYPE_INT, 1 );
    addParameter( "VERTEX_COUNT", KERNEL_PARAMETER_TYPE_INT, 1 );

    // setup ports
    addPortRef( mRenderTarget0 );
    addPortRef( mRenderTarget1 );
    addPortRef( mRenderTarget2 );
    addPortRef( mRenderTarget3 );
    addPortRef( mDepthBuffer );
    addPortRef( mTexture0 );
    addPortRef( mTexture1 );
    addPortRef( mTexture2 );
    addPortRef( mTexture3 );
    addPortRef( mTexture4 );
    addPortRef( mTexture5 );
    addPortRef( mTexture6 );
    addPortRef( mTexture7 );
    addPortRef( mVtxBuf0 );
    addPortRef( mVtxBuf1 );
    addPortRef( mVtxBuf2 );
    addPortRef( mVtxBuf3 );
    addPortRef( mVtxBuf4 );
    addPortRef( mVtxBuf5 );
    addPortRef( mVtxBuf6 );
    addPortRef( mVtxBuf7 );
    addPortRef( mIdxBuf );
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::KernelParameterSet * GN::gfx::D3D9HlslKernel::createParameterSet()
{
    GN_GUARD;

    return new D3D9HlslKernelParameterSet( *this );

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9HlslKernel::render(
    const KernelParameterSet & param, KernelPortBinding binding )
{
    GN_GUARD_SLOW;

    PIXPERF_FUNCTION_EVENT();

    D3D9KernelPortBinding & b = getPortBinding( binding );
    b.apply();

    const D3D9HlslKernelParameterSet & p = safeCastRef<const D3D9HlslKernelParameterSet>(param);

    // restore to default render states
    mRsb.apply();

    // do rendering
    if( b.hasIdxBuf() )
    {
        p.drawIndexed();
    }
    else
    {
        p.draw();
    }

    GN_UNGUARD_SLOW;
}

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
