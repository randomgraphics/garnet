#ifndef __GN_GFX2_D3D9SCRIPTEFFECT_H__
#define __GN_GFX2_D3D9SCRIPTEFFECT_H__
// *****************************************************************************
//! \file    d3d9/d3d9ScriptEffect.h
//! \brief   d3d9 script effect, which accepts shader code as parameter
//! \author  chenli@@FAREAST (2007.6.22)
// *****************************************************************************

namespace GN { namespace gfx2
{
    ///
    /// general effect that accepts shader code as parameter
    ///
    class D3D9ScriptEffect : public D3D9Effect
    {
        EffectParameterHandle mVs, mPs;
        EffectParameterHandle mVsConstants, mPsConstants;
        EffectParameterHandle mPrimType, mPrimCount, mBaseIndex, mBaseVertex, mVertexCount;

        D3D9RenderTargetPort mRenderTargets[4];
        D3D9DepthBufferPort  mDepthBuffer;
        D3D9TexturePort      mTextures[8];
        D3D9VtxBufPort       mVtxBufs[8];
        D3D9IdxBufPort       mIdxBuf;

        static Effect * sCreator( GraphicsSystem & gs ) { return new D3D9ScriptEffect(gs); }

        ///
        /// ctor
        ///
        D3D9ScriptEffect( GraphicsSystem & gs ) : D3D9Effect( gs )
        {
            // setup parameters
            EffectParameterDesc p;

            p.type  = EFFECT_PARAMETER_TYPE_STRING;
            p.count = 1;
            mVs = addParameter( "VS", p );

            p.type  = EFFECT_PARAMETER_TYPE_STRING;
            p.count = 1;
            mPs = addParameter( "PS", p );

            p.type = EFFECT_PARAMETER_TYPE_FLOAT4;
            p.count = 256;
            mVsConstants = addParameter( "VSC", p );

            p.type = EFFECT_PARAMETER_TYPE_FLOAT4;
            p.count = 256;
            mPsConstants = addParameter( "PSC", p );

            p.type = EFFECT_PARAMETER_TYPE_INT1;
            p.count = 1;
            mPrimType = addParameter( "PRIM_TYPE", p );

            p.type = EFFECT_PARAMETER_TYPE_INT1;
            p.count = 1;
            mPrimCount = addParameter( "PRIM_COUNT", p );

            p.type = EFFECT_PARAMETER_TYPE_INT1;
            p.count = 1;
            mBaseIndex = addParameter( "BASE_INDEX", p );

            p.type = EFFECT_PARAMETER_TYPE_INT1;
            p.count = 1;
            mBaseVertex = addParameter( "BASE_VERTEX", p );

            p.type = EFFECT_PARAMETER_TYPE_INT1;
            p.count = 1;
            mVertexCount = addParameter( "VERTEX_COUNT", p );

            // setup ports
            addPortRef( "TARGET0"  , &mRenderTargets[0] );
            addPortRef( "TARGET1"  , &mRenderTargets[1] );
            addPortRef( "TARGET2"  , &mRenderTargets[2] );
            addPortRef( "TARGET3"  , &mRenderTargets[3] );
            addPortRef( "DEPTH"    , &mDepthBuffer );
            addPortRef( "TEXTURE0" , &mTextures[0] );
            addPortRef( "TEXTURE1" , &mTextures[1] );
            addPortRef( "TEXTURE2" , &mTextures[2] );
            addPortRef( "TEXTURE3" , &mTextures[3] );
            addPortRef( "TEXTURE4" , &mTextures[4] );
            addPortRef( "TEXTURE5" , &mTextures[5] );
            addPortRef( "TEXTURE6" , &mTextures[6] );
            addPortRef( "TEXTURE7" , &mTextures[7] );
            addPortRef( "VTXBUF0"  , &mVtxBufs[0] );
            addPortRef( "VTXBUF1"  , &mVtxBufs[1] );
            addPortRef( "VTXBUF2"  , &mVtxBufs[2] );
            addPortRef( "VTXBUF3"  , &mVtxBufs[3] );
            addPortRef( "VTXBUF4"  , &mVtxBufs[4] );
            addPortRef( "VTXBUF5"  , &mVtxBufs[5] );
            addPortRef( "VTXBUF6"  , &mVtxBufs[6] );
            addPortRef( "VTXBUF7"  , &mVtxBufs[7] );
            addPortRef( "IDXBUF"   , &mIdxBuf );

            // setup properties
            setProperty( "RENDER_TARGET_COUNT", 4 );
            setProperty( "TEXTURE_COUNT", 4 );
            setProperty( "VTXBUF_COUNT", 4 );
        }
 
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

        virtual void render( const EffectParameterSet & param, EffectBinding binding );


    private:

        inline void applyVS( const EffectParameter * );
        inline void applyPS( const EffectParameter * );
    };
}}

// *****************************************************************************
//                           End of d3d9ScriptEffect.h
// *****************************************************************************
#endif // __GN_GFX2_D3D9SCRIPTEFFECT_H__
