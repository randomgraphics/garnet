#ifndef __GN_GFX2_D3D9HLSLEFFECT_H__
#define __GN_GFX2_D3D9HLSLEFFECT_H__
// *****************************************************************************
/// \file
/// \brief   d3d9 HLSL effect, which accepts HLSL shader code as parameter
/// \author  chenli@@FAREAST (2007.6.22)
// *****************************************************************************

namespace GN { namespace gfx2
{
    ///
    /// parameter set for D3D9 hlsl effect
    ///
    class D3D9HlslEffectParameterSet : public BaseEffectParameterSet
    {
        D3D9GraphicsSystem               & mGs;
        AutoComPtr<IDirect3DVertexShader9> mVs;
        AutoComPtr<IDirect3DPixelShader9>  mPs;
        AutoComPtr<ID3DXConstantTable>     mVsConsts, mPsConsts;

        EffectParameterHandle mVsHandle, mPsHandle;

    public:

        ///
        /// ctor
        ///
        D3D9HlslEffectParameterSet( D3D9GraphicsSystem & gs, Effect & e, size_t count )
            : BaseEffectParameterSet( e, count )
            , mGs( gs )
            , mVsHandle( e.getParameterHandle( "VS" ) )
            , mPsHandle( e.getParameterHandle( "PS" ) )
        {
        }

        ///
        /// dtor
        ///
        ~D3D9HlslEffectParameterSet() {}

        //@{
        IDirect3DVertexShader9 * vs() const { return mVs; }
        IDirect3DPixelShader9  * ps() const { return mPs; }
        //@}

        /// \name from parent class
        //@{
        virtual const EffectParameter * getParameter( EffectParameterHandle ) const { return 0; }
        virtual void setParameter( EffectParameterHandle handle, const EffectParameter & value );
        virtual void unsetParameter( EffectParameterHandle handle );
        //@}
    };

    ///
    /// general effect that accepts shader code as parameter
    ///
    class D3D9HlslEffect : public D3D9Effect
    {
        EffectParameterHandle mVs, mPs;
        EffectParameterHandle mVsConstants, mPsConstants;
        EffectParameterHandle mPrimType, mPrimCount, mBaseIndex, mBaseVertex, mVertexCount;

        D3D9RenderTargetPort mRenderTarget0;
        D3D9RenderTargetPort mRenderTarget1;
        D3D9RenderTargetPort mRenderTarget2;
        D3D9RenderTargetPort mRenderTarget3;
        D3D9DepthBufferPort  mDepthBuffer;
        D3D9TexturePort      mTexture0;
        D3D9TexturePort      mTexture1;
        D3D9TexturePort      mTexture2;
        D3D9TexturePort      mTexture3;
        D3D9TexturePort      mTexture4;
        D3D9TexturePort      mTexture5;
        D3D9TexturePort      mTexture6;
        D3D9TexturePort      mTexture7;
        D3D9VtxBufPort       mVtxBuf0;
        D3D9VtxBufPort       mVtxBuf1;
        D3D9VtxBufPort       mVtxBuf2;
        D3D9VtxBufPort       mVtxBuf3;
        D3D9VtxBufPort       mVtxBuf4;
        D3D9VtxBufPort       mVtxBuf5;
        D3D9VtxBufPort       mVtxBuf6;
        D3D9VtxBufPort       mVtxBuf7;
        D3D9IdxBufPort       mIdxBuf;

        static Effect * sCreator( GraphicsSystem & gs ) { return new D3D9HlslEffect(GN_SAFE_CAST<D3D9GraphicsSystem&>(gs)); }

        ///
        /// ctor
        ///
        D3D9HlslEffect( D3D9GraphicsSystem & gs );
 
    public:

        ///
        /// get effect factory
        ///
        static EffectFactory sGetFactory()
        {
            EffectFactory f;
            f.quality = 0;
            f.creator = &sCreator;
            return f;
        }

        // from base class
        //@{
        virtual EffectParameterSet * createParameterSet() { return new D3D9HlslEffectParameterSet( d3d9gs(), *this, getParameterCount() ); }
        virtual void                 render( const EffectParameterSet & param, EffectBinding binding );
        //@}

    private:

        inline void applyVS( IDirect3DVertexShader9 * vs );
        inline void applyPS( IDirect3DPixelShader9 * ps );
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX2_D3D9HLSLEFFECT_H__
