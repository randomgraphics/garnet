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

        D3D9FxParameter( D3D9GraphicsSystem & gs )
            : BaseKernelParameter( getKernelReflection(D3D9HlslKernel::KERNEL_NAME()).parameters["FX"] )
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

        ID3DXEffect * getD3DXEffect() const { return mFx; }

        bool set( size_t offset, size_t bytes, const void * values )
        {
            if( !BaseKernelParameter::set( offset, bytes, values ) ) return false;

            mFx.attach( d3d9::compileEffect( mDev, (const char*)values ) );

            return !mFx.empty();
        }

        void unset()
        {
            BaseKernelParameter::unset();
            mFx.clear();
        }
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

        ConstUpdate  mUpdate;

    public:

        D3D9ConstParameter( const char * name )
            : BaseKernelParameter( getKernelReflection(D3D9HlslKernel::KERNEL_NAME()).parameters[name] )
        {
            mUpdate.clear();
        }

        void setVscf( IDirect3DDevice9 * dev ) const
        {
            if( mUpdate.registerCount > 0 )
            {
                dev->SetVertexShaderConstantF(
                    mUpdate.firstRegister,
                    getPtr<const Vector4f>()[mUpdate.firstRegister],
                    mUpdate.registerCount );
            }
        }

        void setPscf( IDirect3DDevice9 * dev ) const
        {
            if( mUpdate.registerCount > 0 )
            {
                dev->SetPixelShaderConstantF(
                    mUpdate.firstRegister,
                    getPtr<const Vector4f>()[mUpdate.firstRegister],
                    mUpdate.registerCount );
            }
        }

        bool set( size_t offset, size_t bytes, const void * values )
        {
            if( !BaseKernelParameter::set( offset, bytes, values ) ) return false;

            UINT firstRegister = (UINT)offset / 16;
            UINT numRegisters  = (UINT)bytes / 16;

            GN_ASSERT( firstRegister < 256 );
            GN_ASSERT( ( firstRegister + numRegisters ) <= 256 );

            mUpdate.merge( firstRegister, numRegisters );

            return true;
        }
    };

    ///
    /// primitive type parameter
    ///
    class D3D9PrimTypeParameter : public TypedKernelParameter<PrimitiveType>
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
            if( pt < 0 || pt > TRIANGLE_STRIP )
            {
                GN_ERROR(sLogger)( "invalid primitive type: %d", pt );
                return D3DPT_FORCE_DWORD;
            }
            else
            {
                return sPrimMap[pt];
            }
        }

        D3DPRIMITIVETYPE mD3DPrimType;

    public:

        D3D9PrimTypeParameter( PrimitiveType initial )
            : TypedKernelParameter<PrimitiveType>( getKernelReflection(D3D9HlslKernel::KERNEL_NAME()).parameters["PRIM_TYPE"], initial )
        {
        }

        bool set( size_t offset, size_t bytes, const void * values )
        {
            if( !BaseKernelParameter::set( offset, bytes, values ) ) return false;

            mD3DPrimType = pt2d3d( getRef<int>() );

            return D3DPT_FORCE_DWORD != mD3DPrimType;
        }

        const D3DPRIMITIVETYPE & d3dpt() const { return mD3DPrimType; }
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
            , mFx( k.d3d9gs() )
            , mVscf( "VSCF" )
            , mPscf( "PSCF" )
            , mPrimType( TRIANGLE_LIST )
            , mPrimCount( getKernelReflection(D3D9HlslKernel::KERNEL_NAME()).parameters["PRIM_COUNT"], 0 )
            , mBaseVertex( getKernelReflection(D3D9HlslKernel::KERNEL_NAME()).parameters["BASE_VERTEX"], 0 )
            , mBaseIndex( getKernelReflection(D3D9HlslKernel::KERNEL_NAME()).parameters["BASE_INDEX"], 0 )
            , mVertexCount( getKernelReflection(D3D9HlslKernel::KERNEL_NAME()).parameters["VERTEX_COUNT"], 0 )
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
            ID3DXEffect * effect = mFx.getD3DXEffect();

            if( 0 == effect ) return;

            UINT passes;
            if( SUCCEEDED( effect->Begin( &passes, 0 ) ) )
            {
                for( UINT i = 0; i < passes; ++i )
                {
                    {
                        D3D9DebugMuter muter;
                        effect->BeginPass( i );
                        effect->CommitChanges();
                    }

                    // apply shader constants
                    mVscf.setVscf( mDev );
                    mPscf.setPscf( mDev );

                    GN_DX9_CHECK( mDev->DrawIndexedPrimitive(
                        mPrimType.d3dpt(),
                        mBaseVertex,
                        0, // min index
                        mVertexCount,
                        mBaseIndex,
                        mPrimCount ) );

                    effect->EndPass();
                }

                {
                    D3D9DebugMuter muter;
                    effect->End();
                }
            }
        }


        void draw() const
        {
            ID3DXEffect * effect = mFx.getD3DXEffect();

            if( 0 == effect ) return;

            UINT passes;

            if( SUCCEEDED( effect->Begin( &passes, 0 ) ) )
            {
                for( UINT i = 0; i < passes; ++i )
                {
                    {
                        D3D9DebugMuter muter;
                        effect->BeginPass( i );
                        effect->CommitChanges();
                    }

                    // apply shader constants
                    mVscf.setVscf( mDev );
                    mPscf.setPscf( mDev );

                    GN_DX9_CHECK( mDev->DrawPrimitive(
                        mPrimType.d3dpt(),
                        mBaseVertex,
                        mPrimCount ) );

                    effect->EndPass();
                }

                {
                    D3D9DebugMuter muter;
                    effect->End();
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
    , mRenderTarget0( gs, baseref(), "TARGET0", 0 )
    , mRenderTarget1( gs, baseref(), "TARGET1", 1 )
    , mRenderTarget2( gs, baseref(), "TARGET2", 2 )
    , mRenderTarget3( gs, baseref(), "TARGET3", 3 )
    , mDepthBuffer( gs, baseref(), "DEPTH" )
    , mTexture0( gs, baseref(), "TEXTURE0", 0 )
    , mTexture1( gs, baseref(), "TEXTURE1", 1 )
    , mTexture2( gs, baseref(), "TEXTURE2", 2 )
    , mTexture3( gs, baseref(), "TEXTURE3", 3 )
    , mTexture4( gs, baseref(), "TEXTURE4", 4 )
    , mTexture5( gs, baseref(), "TEXTURE5", 5 )
    , mTexture6( gs, baseref(), "TEXTURE6", 6 )
    , mTexture7( gs, baseref(), "TEXTURE7", 7 )
    , mVtxBuf0( gs, baseref(), "VTXBUF0", 0 )
    , mVtxBuf1( gs, baseref(), "VTXBUF1", 1 )
    , mVtxBuf2( gs, baseref(), "VTXBUF2", 2 )
    , mVtxBuf3( gs, baseref(), "VTXBUF3", 3 )
    , mVtxBuf4( gs, baseref(), "VTXBUF4", 4 )
    , mVtxBuf5( gs, baseref(), "VTXBUF5", 5 )
    , mVtxBuf6( gs, baseref(), "VTXBUF6", 6 )
    , mVtxBuf7( gs, baseref(), "VTXBUF7", 7 )
    , mIdxBuf( gs, baseref(), "IDXBUF" )
    , mRsb( gs )
{
    GN_ASSERT( validate() );
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
void GN::gfx::D3D9HlslKernel::render( const KernelParameterSet & param, const KernelPortBinding * binding )
{
    GN_GUARD_SLOW;

    PIXPERF_FUNCTION_EVENT();

    const D3D9KernelPortBinding * b = binding ? safeCastPtr<const D3D9KernelPortBinding>( binding ) : getDefaultPortBinding();
    GN_ASSERT( b );
    b->apply();

    const D3D9HlslKernelParameterSet & p = safeCastRef<const D3D9HlslKernelParameterSet>(param);

    // restore to default render states
    mRsb.apply();

    // do rendering
    if( b->hasIdxBuf() )
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
