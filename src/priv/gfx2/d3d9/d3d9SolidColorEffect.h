#ifndef __GN_GFX2_D3D9SOLIDCOLOREFFECT_H__
#define __GN_GFX2_D3D9SOLIDCOLOREFFECT_H__
// *****************************************************************************
/// \file
/// \brief   D3D9 solid-color effect
/// \author  chenli@@FAREAST (2007.6.19)
// *****************************************************************************

namespace GN { namespace gfx2
{
    ///
    /// solid color effect
    ///
    class D3D9SolidColorEffect : public D3D9Effect
    {
        EffectParameterHandle mPvw;   ///< 4x4 matrix: project-view-world transformation
        EffectParameterHandle mColor; ///< 4D float vector: [R,G,B,A]
        D3D9RenderTargetPort  mTarget0;
        D3D9DepthBufferPort   mDepth;
        D3D9VtxBufPort        mVtxBuf;
        D3D9IdxBufPort        mIdxBuf; ///< this is optional.

        static Effect * sCreator( GraphicsSystem & gs ) { return new D3D9SolidColorEffect(gs); }

        ///
        /// ctor
        ///
        D3D9SolidColorEffect( GraphicsSystem & gs ) : D3D9Effect( gs )
        {
            // setup parameters
            EffectParameterDesc p;

            p.type  = EFFECT_PARAMETER_TYPE_FLOAT4X4;
            p.count = 1;
            mPvw    = addParameter( "PVW", p );

            p.type  = EFFECT_PARAMETER_TYPE_FLOAT4;
            p.count = 1;
            mColor  = addParameter( "COLOR", p );

            // setup ports
            addPortRef( "TARGET0", &mTarget0 );
            addPortRef( "DEPTH", &mDepth );
            addPortRef( "VTXBUF", &mVtxBuf );
            addPortRef( "IDXBUF", &mIdxBuf );

            // setup vertx buffer port
            mVtxBuf.addRequiredAttribute( "POSITION0" );
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
 
        virtual void render( const EffectParameterSet & param, EffectBinding binding )
        {
            GN_ASSERT( &param.getEffect() == (Effect*)this );

            applyBinding( binding );

            const EffectParameter * m = param.getParameter( mPvw );
            const EffectParameter * c = param.getParameter( mColor );

            GN_UNUSED_PARAM( m );
            GN_UNUSED_PARAM( c );
        }
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX2_D3D9SOLIDCOLOREFFECT_H__
