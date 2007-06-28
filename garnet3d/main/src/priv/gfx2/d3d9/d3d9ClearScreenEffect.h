#ifndef __GN_GFX2_D3D9CLEARSCREENEFFECT_H__
#define __GN_GFX2_D3D9CLEARSCREENEFFECT_H__
// *****************************************************************************
/// \file
/// \brief   D3D9 clear screen effect
/// \author  chenli@@FAREAST (2007.6.19)
// *****************************************************************************

namespace GN { namespace gfx2
{
    ///
    /// clear screen effect
    ///
    class D3D9ClearScreenEffect : public D3D9Effect
    {
        D3D9RenderTargetPort mTarget0;
        D3D9DepthBufferPort  mDepth;

        EffectParameterHandle mColorValue, mDepthValue, mStencilValue;

        static Effect * sCreator( GraphicsSystem & gs ) { return new D3D9ClearScreenEffect( GN_SAFE_CAST<D3D9GraphicsSystem&>(gs) ); }

        ///
        /// ctor
        ///
        D3D9ClearScreenEffect( D3D9GraphicsSystem & gs )
            : D3D9Effect(gs)
            , mTarget0(gs)
            , mDepth(gs)
        {
            // setup ports
            addPortRef( "TARGET0", &mTarget0 );
            addPortRef( "DEPTH", &mDepth );

            // setup parameters
            EffectParameterDesc c;
            c.type  = EFFECT_PARAMETER_TYPE_FLOAT4;
            mColorValue = addParameter( "COLOR_VALUE", c );
            //setParameter( mColorValue, EffectParameter(.0f, .0f, .0f, .0f) );

            EffectParameterDesc z;
            z.type  = EFFECT_PARAMETER_TYPE_FLOAT1;
            mDepthValue = addParameter( "DEPTH_VALUE", z );
            //setParameter( mDepthValue, .0f );

            EffectParameterDesc s;
            s.type  = EFFECT_PARAMETER_TYPE_INT1;
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

            D3D9EffectBinding & b = getPortBinding( binding );
            D3D9GraphicsSystem & gs = d3d9gs();
            IDirect3DDevice9  * dev = gs.d3ddev();

            b.apply();

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

            if( b.hasZBuf() )
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
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX2_D3D9CLEARSCREENEFFECT_H__
