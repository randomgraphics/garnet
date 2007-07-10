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
    class D3D9HlslKernelParameterSet : public Hlsl9ParameterSet
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
            mVscfUpdate.merge( firstRegister, numRegisters );
            memcpy( &mVscf[firstRegister], data, numRegisters * sizeof(float)*4 );
        }

        virtual void setPSConstF( size_t firstRegister, size_t numRegisters, const float * data )
        {
            mPscfUpdate.merge( firstRegister, numRegisters );
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
    : D3D9KernelBase( gs )
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
GN::gfx::Hlsl9ParameterSet *
GN::gfx::D3D9HlslKernel::createParameterSet()
{
    GN_GUARD;

    return new D3D9HlslKernelParameterSet( *this );

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::KernelPortBinding
GN::gfx::D3D9HlslKernel::createPortBinding( const Hlsl9PortBinding & b )
{
    D3D9KernelPortBindingDesc desc;
    desc.bindings["TARGET0"]  = b.renderTargets[0];
    desc.bindings["TARGET1"]  = b.renderTargets[1];
    desc.bindings["TARGET2"]  = b.renderTargets[2];
    desc.bindings["TARGET3"]  = b.renderTargets[3];
    desc.bindings["DEPTH"]    = b.depth;
    desc.bindings["TEXTURE0"] = b.textures[0];
    desc.bindings["TEXTURE1"] = b.textures[1];
    desc.bindings["TEXTURE2"] = b.textures[2];
    desc.bindings["TEXTURE3"] = b.textures[3];
    desc.bindings["TEXTURE4"] = b.textures[4];
    desc.bindings["TEXTURE5"] = b.textures[5];
    desc.bindings["TEXTURE6"] = b.textures[6];
    desc.bindings["TEXTURE7"] = b.textures[7];
    desc.bindings["VTXBUF0"]  = b.vtxbufs[0];
    desc.bindings["VTXBUF1"]  = b.vtxbufs[1];
    desc.bindings["VTXBUF2"]  = b.vtxbufs[2];
    desc.bindings["VTXBUF3"]  = b.vtxbufs[3];
    desc.bindings["VTXBUF4"]  = b.vtxbufs[4];
    desc.bindings["VTXBUF5"]  = b.vtxbufs[5];
    desc.bindings["VTXBUF6"]  = b.vtxbufs[6];
    desc.bindings["VTXBUF7"]  = b.vtxbufs[7];
    desc.bindings["IDXBUF"]   = b.idxbuf;
    return D3D9KernelBase::createPortBinding( desc );
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

    const D3D9HlslKernelParameterSet & p = safeCast<const D3D9HlslKernelParameterSet &>(param);

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
