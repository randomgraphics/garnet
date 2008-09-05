#include "pch.h"
#include "d3d10Renderer.h"
#include "d3d10RenderTargetMgr.h"
#include "d3d10StateObject.h"
#include "d3d10Shader.h"
#include "d3d10Texture.h"
#include "d3d10VtxLayout.h"
#include "d3d10Buffer.h"

// *****************************************************************************
// local functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
static inline void sSetupD3D10Viewport(
    ID3D10Device * dev,
    const GN::Vector2<UInt32> & rtsize, // render target size
    float l, float t, float r, float b )
{
    GN_ASSERT(
        0.0f <= l && l <= 1.0f &&
        0.0f <= t && t <= 1.0f &&
        0.0f <= r && r <= 1.0f &&
        0.0f <= b && b <= 1.0f &&
        l <= r && t <= b );

    float w = r - l;
    float h = b - t;

    D3D10_VIEWPORT d3dvp = {
        (INT)l * rtsize.x,
        (INT)t * rtsize.y,
        (UINT)w * rtsize.x,
        (UINT)h * rtsize.y,
        0.0f,
        1.0f,
    };

    // update D3D viewport
    dev->RSSetViewports( 1, &d3dvp );

    // update D3D scissors
    D3D10_RECT rc = {
        (long)( d3dvp.TopLeftX ),
        (long)( d3dvp.TopLeftY ),
        (long)( d3dvp.TopLeftX+d3dvp.Width ),
        (long)( d3dvp.TopLeftY+d3dvp.Height ),
    };
    dev->RSSetScissorRects( 1, &rc );
}

// *****************************************************************************
// init/shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D10Renderer::contextDeviceCreate()
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    mSOMgr = new D3D10StateObjectManager( *this );
    if( 0 == mSOMgr ) return false;

    // create render target manager
    mRTMgr = new D3D10RTMgr( *this );
    if( !mRTMgr->init() ) return false;

    // bind default context
    bindContext( mContext, mContext.flags, true );

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D10Renderer::contextDeviceDestroy()
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    mContext.resetToDefault();

	clearContextResources();

    safeDelete( mRTMgr );

    safeDelete( mSOMgr );

    GN_UNGUARD;
}

// *****************************************************************************
// from Renderer
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D10Renderer::setContext( const RendererContext & newContext )
{
    GN_GUARD_SLOW;

#if GN_BUILD_DEBUG
    // make sure bindContext() does not rely on flags in tmp structure.
    RendererContext tmp = newContext;
    RendererContext::FieldFlags flags = tmp.flags;
    tmp.flags.u32 = 0;
    bindContext( tmp, flags, false );
#else
    bindContext( newContext, newContext.flags, false );
#endif

    mContext.mergeWith( newContext );
    holdContextResources( newContext );

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D10Renderer::rebindContext( RendererContext::FieldFlags flags )
{
    GN_GUARD_SLOW;
    bindContext( mContext, flags, true );
    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
const GN::gfx::RenderStateBlockDesc & GN::gfx::D3D10Renderer::getCurrentRenderStateBlock() const
{
    GN_ASSERT( mContext.flags.rsb );
    return mContext.rsb;
}

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
inline void GN::gfx::D3D10Renderer::bindContext(
    const RendererContext & newContext,
    RendererContext::FieldFlags newFlags,
    bool forceRebind )
{
    GN_GUARD_SLOW;

    PIXPERF_FUNCTION_EVENT();

    //
    // Parameter check
    //
    if( parameterCheckEnabled() )
    {
        // TODO: verify data in new context
        // TODO: make sure all fields in current context are valid.
    }

    if( newFlags.state ) bindContextState( newContext, newFlags, forceRebind );

    if( newFlags.data ) bindContextData( newContext, newFlags, forceRebind );

    GN_UNGUARD_SLOW;
}


//
//
// -----------------------------------------------------------------------------
inline void GN::gfx::D3D10Renderer::bindContextState(
    const RendererContext & newContext,
    RendererContext::FieldFlags newFlags,
    bool forceRebind )
{
    GN_GUARD_SLOW;

    GN_ASSERT( newFlags.state );

    //
    // bind shaders
    //
    for( int i = 0; i < NUM_SHADER_TYPES; ++i )
    {
        const GN::gfx::Shader * o = mContext.shaders[i];
        if( newFlags.shaderBit(i) )
        {
            const GN::gfx::Shader * n = newContext.shaders[i];
            if( o != n || forceRebind )
            {
                if( n )
                {
                    GN::safeCastPtr<const GN::gfx::D3D10BasicShader>(n)->apply();
                }
                else switch( i )
                {
                    case SHADER_VS : mDevice->VSSetShader( 0 ); break;
                    case SHADER_PS : mDevice->PSSetShader( 0 ); break;
                    case SHADER_GS : mDevice->GSSetShader( 0 ); break;
                    default : GN_UNEXPECTED();
                }
            }
            else if( n )
            {
                GN::safeCastPtr<const GN::gfx::D3D10BasicShader>(n)->applyDirtyUniforms();
            }
        } else if( o )
        {
            GN::safeCastPtr<const GN::gfx::D3D10BasicShader>(o)->applyDirtyUniforms();
        }
    }

    /*
    // bind render states
    //
    if( newFlags.rsb )
    {
        const RenderStateBlockDesc & newrsb = newContext.rsb;
        GN_ASSERT( newrsb.valid() );

        if( forceRebind )
        {
            #define GNGFX_DEFINE_RS( tag, type, defval, minVal, maxVal ) \
                if( newrsb.isSet(RS_##tag) ) mSOMgr->Set_##tag( newrsb.get(RS_##tag) );
            #include "garnet/gfx/renderStateMeta.h"
            #undef GNGFX_DEFINE_RS
        }
        else
        {
            const RenderStateBlockDesc & oldrsb = mContext.rsb;

            #define GNGFX_DEFINE_RS( tag, type, defval, minVal, maxVal ) \
                GN_ASSERT( oldrsb.isSet( RS_##tag ) ); \
                if( newrsb.isSet(RS_##tag) && newrsb.get(RS_##tag) != oldrsb.get(RS_##tag) ) \
                    mSOMgr->Set_##tag( newrsb.get(RS_##tag) );
            #include "garnet/gfx/renderStateMeta.h"
            #undef GNGFX_DEFINE_RS
        }

        mSOMgr->apply();
    }*/

    //
    // bind render targets
    //
    bool needRebindViewport = false;
    if( newFlags.renderTargets )
    {
        mRTMgr->bind( mContext.renderTargets, newContext.renderTargets, forceRebind, needRebindViewport );
    }

    // bind viewport
    if( newFlags.viewport )
    {
        if( needRebindViewport || newContext.viewport != mContext.viewport || forceRebind )
        {
            float l = newContext.viewport.x;
            float t = newContext.viewport.y;
            float r = l + newContext.viewport.w;
            float b = t + newContext.viewport.h;

            // clamp viewport in valid range
            clamp<float>( l, 0.0f, 1.0f );
            clamp<float>( b, 0.0f, 1.0f );
            clamp<float>( r, 0.0f, 1.0f );
            clamp<float>( t, 0.0f, 1.0f );

            sSetupD3D10Viewport( mDevice, mRTMgr->getRenderTargetSize(), l , t, r, b );
        }
    }
    else if( needRebindViewport )
    {
        float l = mContext.viewport.x;
        float t = mContext.viewport.y;
        float r = l + mContext.viewport.w;
        float b = t + mContext.viewport.h;
        sSetupD3D10Viewport( mDevice, mRTMgr->getRenderTargetSize(), l, t, r, b );
    }

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
inline void GN::gfx::D3D10Renderer::bindContextData(
    const RendererContext & newContext,
    RendererContext::FieldFlags newFlags,
    bool forceRebind )
{
    GN_GUARD_SLOW;

    GN_ASSERT( newFlags.data );

    //
    // bind vertex format
    //
    if( newFlags.vtxfmt )
    {
        if( newContext.vtxfmt )
        {
            const D3D10VtxLayoutDesc * layout;
            layout = &mVtxFmts[newContext.vtxfmt];
            GN_ASSERT( layout->layout );
            if( newContext.vtxfmt != mContext.vtxfmt || forceRebind )
            {
                mDevice->IASetInputLayout( layout->layout );
            }
        }
    }

    ///
    /// bind vertex buffers
    ///
    if( newFlags.vtxbufs )
    {
        ID3D10Buffer * buf[16];
        UINT           stride[16];
        UINT           offset[16];

        GN_ASSERT( newContext.numVtxBufs <= 16 );

        bool bind = false;
        for( UINT i = 0; i < newContext.numVtxBufs; ++i )
        {
            const RendererContext::VtxBufDesc & vb = newContext.vtxbufs[i];
            if( vb != mContext.vtxbufs[i] || forceRebind )
            {
                bind      = true;
                buf[i]    = safeCastPtr<const D3D10VtxBuf>(vb.buffer)->getD3DBuffer();
                stride[i] = vb.stride;
                offset[i] = vb.offset;
            }
        }

        if( bind )
        {
            mDevice->IASetVertexBuffers( 0, newContext.numVtxBufs, buf, stride, offset );
        }
    }

    //
    // bind index buffer
    //
    if( newFlags.idxbuf &&
      ( newContext.idxbuf != mContext.idxbuf || forceRebind ) )
    {
        mDevice->IASetIndexBuffer(
            newContext.idxbuf ? safeCastPtr<const D3D10IdxBuf>(newContext.idxbuf)->getD3DBuffer() : 0,
            DXGI_FORMAT_R16_UINT,
            0 );
    }

    //
    // bind textures
    //
    if( newFlags.textures )
    {
        /*
        UINT maxStages = getCaps(CAPS_MAX_TEXTURE_STAGES);
        UINT numTex = min<UINT>( (UINT)newContext.numTextures, maxStages );
        UINT stage;
        for( stage = 0; stage < numTex; ++stage )
        {
            const Texture * tex = newContext.textures[stage];
            if( tex != mContext.textures[stage] ||
                stage > mContext.numTextures ||
                !forceRebind )
            {
                if( tex )
                {
                    safeCastPtr<const D3D10Texture>(tex)->bind( stage );
                }
                else
                {
                    mDevice->SetTexture( stage, NULL );
                }
            }
        }
        // clear unused stages
        numTex = min<UINT>( (UINT)mContext.numTextures, maxStages );
        for( ; stage < numTex; ++stage )
        {
            mDevice->SetTexture( stage, 0 );
        }
        */
    }

    GN_UNGUARD_SLOW;
}
