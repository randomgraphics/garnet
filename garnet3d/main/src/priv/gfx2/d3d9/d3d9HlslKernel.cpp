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
    class D3D9HlslKernelParameterSet : public Hlsl9KernelParameterSet
    {
        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        D3D9HlslKernelParameterSet( D3D9KernelBase & k )
            : mDev( k.gfxsys().d3ddev() )
        {
            mVscfUpdate.clear();
            mPscfUpdate.clear();
        }
        ~D3D9HlslKernelParameterSet() {}
        //@}

        // ********************************
        // public functions
        // ********************************
    public:

        virtual void setFx( const StrA & s )
        {
            mFx.attach( d3d9::compileEffect( mDev, s.cptr(), s.size() ) );
        }

        virtual void setVSConstF( size_t firstRegister, size_t numRegisters, const float * data )
        {
            mVscfUpdate.merge( (UInt32)firstRegister, (UInt32)numRegisters );
            memcpy( &mVscf[firstRegister], data, numRegisters * sizeof(float)*4 );
        }

        virtual void setPSConstF( size_t firstRegister, size_t numRegisters, const float * data )
        {
            mPscfUpdate.merge( (UInt32)firstRegister, (UInt32)numRegisters );
            memcpy( &mPscf[firstRegister], data, numRegisters * sizeof(float)*4 );
        }

        virtual void setPrimType( PrimitiveType pt ) { mPrimType = pt2d3d( pt ); }
        virtual void setPrimCount( size_t n ) { mPrimCount = (UINT)n; }
        virtual void setBaseVertex( size_t n ) { mBaseVertex = (UINT)n; }
        virtual void setBaseIndex( size_t n ) { mBaseIndex = (UINT)n; }
        virtual void setVertexCount( size_t n ) { mVertexCount = (UINT)n; }


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

                    applyShaderConstants();

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

                    applyShaderConstants();

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
        Vector4f                mVscf[256], mPscf[256];
        ConstUpdate             mVscfUpdate, mPscfUpdate;
        D3DPRIMITIVETYPE        mPrimType;
        UINT                    mPrimCount;
        UINT                    mBaseVertex;
        UINT                    mBaseIndex;
        UINT                    mVertexCount;

        // ********************************
        // private functions
        // ********************************
    private:

        void applyShaderConstants() const
        {
            // apply shader constants
            if( mVscfUpdate.registerCount > 0 )
            {
                mDev->SetVertexShaderConstantF(
                    mVscfUpdate.firstRegister,
                    mVscf[mVscfUpdate.firstRegister],
                    mVscfUpdate.registerCount );
            }
            if( mPscfUpdate.registerCount > 0 )
            {
                mDev->SetPixelShaderConstantF(
                    mPscfUpdate.firstRegister,
                    mPscf[mPscfUpdate.firstRegister],
                    mPscfUpdate.registerCount );
            }
        }

        static inline D3DPRIMITIVETYPE pt2d3d( PrimitiveType pt )
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
    };
}}

// *****************************************************************************
// public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::gfx::D3D9HlslKernel::D3D9HlslKernel( D3D9GraphicsSystem & gs )
    : D3D9KernelBaseT<Hlsl9Kernel>( gs )
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
    gfxsys().setRenderStateBlock( mRsb );

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
