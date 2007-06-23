#ifndef __GN_GFX2_D3D9BUILDINEFFECTS_H__
#define __GN_GFX2_D3D9BUILDINEFFECTS_H__
// *****************************************************************************
//! \file    d3d9/d3d9BuildInEffects.h
//! \brief   D3D9 build-in effects
//! \author  chenli@@FAREAST (2007.6.19)
// *****************************************************************************

namespace GN { namespace gfx2
{
    ///
    /// build-in clear effect
    ///
    class D3D9ClearEffect : public D3D9Effect
    {
        D3D9RenderTargetPort mTarget0;
        D3D9DepthBufferPort  mDepth;

        EffectParameterHandle mColorValue, mDepthValue, mStencilValue;

        static Effect * sCreator( GraphicsSystem & gs ) { return new D3D9ClearEffect(gs); }

        ///
        /// ctor
        ///
        D3D9ClearEffect( GraphicsSystem & gs ) : D3D9Effect(gs)
        {
            // setup ports
            addPortRef( "TARGET0", &mTarget0 );
            addPortRef( "DEPTH", &mDepth );

            // setup parameters
            EffectParameterDesc c;
            c.type  = EFFECT_PARAMETER_TYPE_FLOAT4;
            c.count = 1;
            mColorValue = addParameter( "COLOR_VALUE", c );
            //setParameter( mColorValue, EffectParameter(.0f, .0f, .0f, .0f) );

            EffectParameterDesc z;
            z.type  = EFFECT_PARAMETER_TYPE_FLOAT1;
            z.count = 1;
            mDepthValue = addParameter( "DEPTH_VALUE", z );
            //setParameter( mDepthValue, .0f );

            EffectParameterDesc s;
            s.type  = EFFECT_PARAMETER_TYPE_INT1;
            s.count = 1;
            mStencilValue = addParameter( "STENCIL_VALUE", s );
            //setParameter( mStencilValue, 0 );
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

            D3D9GraphicsSystem & gs = d3d9gs();
            IDirect3DDevice9  * dev = gs.d3ddev();

            const EffectParameter * c = param.getParameter( mColorValue );
            const EffectParameter * z = param.getParameter( mDepthValue );
            const EffectParameter * s = param.getParameter( mStencilValue );

            DWORD flags   = 0;
            DWORD color   = 0;
            float depth   = .0f;
            DWORD stencil = 0;

            if( c )
            {
                flags |= D3DCLEAR_TARGET;
                color = D3DCOLOR_COLORVALUE( c->float4[0], c->float4[1], c->float4[2], c->float4[3] );
            }

            if( gs.getD3D9Desc().zbuffer )
            {
                if( z )
                {
                    flags |= D3DCLEAR_ZBUFFER;
                    depth = z->float1;
                }

                if( s )
                {
                    flags |= D3DCLEAR_STENCIL;
                    stencil = (DWORD)s->int1;
                }
            }

            if( flags ) dev->Clear( 0, 0, flags, color, depth, stencil );
        }
    };

    ///
    /// build-in simple solid color effect
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
//                           End of d3d9BuildInEffects.h
// *****************************************************************************
#endif // __GN_GFX2_D3D9BUILDINEFFECTS_H__

