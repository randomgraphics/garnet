#include "pch.h"
#include "xenonRenderer.h"
#include "xenonRenderTargetMgr.h"
#include "xenonShader.h"
#include "xenonTexture.h"
#include "xenonVertexDecl.h"
#include "xenonVtxBuf.h"
#include "xenonIdxBuf.h"

// *****************************************************************************
// local functions
// *****************************************************************************

// *****************************************************************************
// init/shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::XenonRenderer::contextInit()
{
    GN_GUARD;

    // initialize render target manager
    //GN_ASSERT( 0 == mRTMgr );
    //mRTMgr = new XenonRenderTargetMgr( *this );
    //if( !mRTMgr->init() ) return false;

    // bind default context to device
    rebindContext();

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::XenonRenderer::contextQuit()
{
    GN_GUARD;

    // reset context
    mContext.clear();

    // delete all vertex formats
    mVertexFormats.clear();

    //safeDelete( mRTMgr );

    GN_UNGUARD;
}

// *****************************************************************************
// from BasicRenderer
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::XenonRenderer::bindContextImpl(
    const RendererContext & context,
    bool                    skipDirtyCheck )
{
    GN_GUARD_SLOW;

    PIXPERF_FUNCTION_EVENT();

    //
    // Parameter check
    //
    if( paramCheckEnabled() )
    {
        GN_TODO( "verify renderer context data" );
    }

    if( !bindContextRenderTargetsAndViewport( context, skipDirtyCheck ) ) return false;

    if( !bindContextRenderStates( context, skipDirtyCheck ) ) return false;

    if( !bindContextShaders( context, skipDirtyCheck ) ) return false;

    if( !bindContextResources( context, skipDirtyCheck ) ) return false;

    // success
    return true;

    GN_UNGUARD_SLOW;
}

// *****************************************************************************
// private methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
inline bool
GN::gfx::XenonRenderer::bindContextRenderTargetsAndViewport(
    const RendererContext & newContext,
    bool                    skipDirtyCheck )
{
    GN_UNUSED_PARAM( newContext );
    GN_UNUSED_PARAM( skipDirtyCheck );
    GN_UNIMPL_WARNING();
    return true;

    //bool needRebindViewport;
    //mRTMgr->bind( mContext, newContext, skipDirtyCheck, needRebindViewport );

/*

#if GN_XENON
	if( newFlags.colorBuffers )
    {
        static const RendererContext::SurfaceDesc sNullSurface = { 0, 0, 0, 0 };
        const RendererContext::SurfaceDesc *newSurf, *oldSurf;

        for( UInt32 i = 0; i < 4; ++i )
        {
            newSurf = (i >= newContext.numColorBuffers) ? &sNullSurface : &newContext.colorBuffers[i];
            oldSurf = (i >= mContext.numColorBuffers) ? &sNullSurface : &mContext.colorBuffers[i];
            if( *newSurf != *oldSurf || forceRebind )
            {
                // copy content from RT to old/current RTT
                if( *oldSurf )
                {
                    GN_TODO( "resolve to RTT" );
                }

                // setup new RT
                if( 0 == newSurf->texture )
                {
                    // restore to default RT
                    if( 0 == i )
                    {
                        GN_DX9_CHECK( mDevice->SetRenderTarget( 0, mAutoColor0 ) );
                    }
                    else
                    {
                        GN_DX9_CHECK( mDevice->SetRenderTarget( 0, 0 ) );
                    }
                }
                else
                {
                    // setup RT based on RTT
                    //const Vector3<UInt32> & sz = newSurf->texture->getMipSize( newSurf->level );
                    //setupRT( sz.x, sz.y, newSurf->texture->getDesc().format );
                }

            }
        }

        // setup default render targets
        GN_DX9_CHECK( mDevice->SetRenderTarget( 0, mAutoColor0 ) );
        GN_DX9_CHECK( mDevice->SetRenderTarget( 1, 0 ) );
        GN_DX9_CHECK( mDevice->SetRenderTarget( 2, 0 ) );
        GN_DX9_CHECK( mDevice->SetRenderTarget( 3, 0 ) );
	}

    if( newFlags.depthBuffer )
    {
        const RendererContext::SurfaceDesc *newSurf, *oldSurf;

        // bind depth buffer
        newSurf = &newContext.depthBuffer;
        oldSurf = &mContext.depthBuffer;
        if( 0 == newSurf->texture )
        {
            AutoComPtr<IDirect3DSurface9> rt0;
            GN_DX9_CHECK( mDevice->GetRenderTarget( 0, &rt0 ) );
            GN_ASSERT( rt0 );
            D3DSURFACE_DESC rt0Desc;
            GN_DX9_CHECK( rt0->GetDesc( &rt0Desc ) );
            if( mAutoDepth )
            {
                D3DSURFACE_DESC depthDesc;
                GN_DX9_CHECK( mAutoDepth->GetDesc( &depthDesc ) );
                if( depthDesc.Width < rt0Desc.Width ||
                    depthDesc.Height < rt0Desc.Height ||
                    forceRebind )
                {
                    // release old depth surface
                    GN_DX9_CHECK( mDevice->SetDepthStencilSurface( NULL ) );
                    mAutoDepth.clear();

                    // create new depth buffer
                    GN_DX9_CHECK_R( mDevice->CreateDepthStencilSurface(
                        max(depthDesc.Width, rt0Desc.Width),
                        max(depthDesc.Height, rt0Desc.Height),
                        depthDesc.Format,
                        depthDesc.MultiSampleType,
                        depthDesc.MultiSampleQuality,
                        TRUE, // discardable depth buffer
                        &mAutoDepth, 0 ) );
                    GN_DX9_CHECK( mDevice->SetDepthStencilSurface( mAutoDepth ) );
                }
            }
            else
            {
                GN_DX9_CHECK_R( mDevice->CreateDepthStencilSurface(
                    rt0Desc.Width,
                    rt0Desc.Height,
                    DEFAULT_DEPTH_FORMAT, // TODO: enumerate appropriate depth buffer format.
                    mPresentParameters.MultiSampleType,
                    mPresentParameters.MultiSampleQuality,
                    TRUE,
                    &mAutoDepth, 0 ) );
                GN_DX9_CHECK( mDevice->SetDepthStencilSurface( mAutoDepth ) );
            }
        }
        else if( *newSurf != *oldSurf || forceRebind )
        {
            GN_UNIMPL();
        }
    }

#else

#endif
*/
}

static const D3DCULL CULL_TO_D3D[]=
{
    D3DCULL_NONE, // FRONT_CCW, CULL_NONE
    D3DCULL_CCW,  // FRONT_CCW, CULL_FRONT,
    D3DCULL_CW,   // FRONT_CCW, CULL_BACK,
    D3DCULL_NONE, // FRONT_CW,  CULL_NONE,
    D3DCULL_CW,   // FRONT_CW,  CULL_FRONT,
    D3DCULL_CCW,  // FRONT_CW,  CULL_BACK,
};
GN_CASSERT( GN_ARRAY_COUNT(CULL_TO_D3D) == GN::gfx::RendererContext::NUM_FRONT_FACE_MODES * GN::gfx::RendererContext::NUM_CULL_MODES );

//
//
// -----------------------------------------------------------------------------
inline bool
GN::gfx::XenonRenderer::bindContextRenderStates(
    const RendererContext & newContext,
    bool                    skipDirtyCheck )
{
    GN_UNUSED_PARAM( newContext );
    GN_UNUSED_PARAM( skipDirtyCheck );
    GN_UNIMPL_WARNING();

    // cull state
    D3DCULL cullMode = CULL_TO_D3D[newContext.frontFace*RendererContext::NUM_CULL_MODES + newContext.cullMode];
    mDevice->SetRenderState( D3DRS_CULLMODE, cullMode );

    /*
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
                    GN::safeCastPtr<const GN::gfx::XenonBasicShader>(n)->apply();
                }
                else switch( i )
                {
                    case SHADER_VS : GN_DX9_CHECK( mDevice->SetVertexShader( 0 ) ); break;
                    case SHADER_PS : GN_DX9_CHECK( mDevice->SetPixelShader( 0 ) ); break;
                    case SHADER_GS : break; // do nothing for geometry shader
                    default : GN_UNEXPECTED();
                }
            }
            else if( n )
            {
                GN::safeCastPtr<const GN::gfx::XenonBasicShader>(n)->applyDirtyUniforms();
            }
        } else if( o )
        {
            GN::safeCastPtr<const GN::gfx::XenonBasicShader>(o)->applyDirtyUniforms();
        }
    }

    //
    // bind render states
    //
    if( newFlags.rsb )
    {
        const RenderStateBlockDesc & newrsb = newContext.rsb;
        GN_ASSERT( newrsb.valid() );

        if( forceRebind )
        {
            #define GNGFX_DEFINE_RS( tag, type, defval, minVal, maxVal ) \
                if( newrsb.isSet(RS_##tag) ) sSet_##tag( *this, newrsb.get(RS_##tag) );
            #include "garnet/gfx/renderStateMeta.h"
            #undef GNGFX_DEFINE_RS
        }
        else
        {
            const RenderStateBlockDesc & oldrsb = mContext.rsb;

            #define GNGFX_DEFINE_RS( tag, type, defval, minVal, maxVal ) \
                GN_ASSERT( oldrsb.isSet( RS_##tag ) ); \
                if( newrsb.isSet(RS_##tag) && newrsb.get(RS_##tag) != oldrsb.get(RS_##tag) ) \
                    sSet_##tag( *this, newrsb.get(RS_##tag) );
            #include "garnet/gfx/renderStateMeta.h"
            #undef GNGFX_DEFINE_RS
        }
    }

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

            sSetupXenonViewport( mDevice, l , t, r, b );
        }
    }
    else if( needRebindViewport )
    {
        float l = mContext.viewport.x;
        float t = mContext.viewport.y;
        float r = l + mContext.viewport.w;
        float b = t + mContext.viewport.h;
        sSetupXenonViewport( mDevice, l, t, r, b );
    }*/

    return true;
}

//
//
// -----------------------------------------------------------------------------
inline bool
GN::gfx::XenonRenderer::bindContextShaders(
    const RendererContext & newContext,
    bool                    skipDirtyCheck )
{
    if( newContext.gpuProgram )
    {
        const XenonBasicGpuProgram * prog = (const XenonBasicGpuProgram *)newContext.gpuProgram.get();

        if( skipDirtyCheck || mContext.gpuProgram != newContext.gpuProgram )
        {
            prog->apply();
        }

        const SysMemUniform * const * uniforms = (const SysMemUniform * const *)newContext.uniforms.cptr();
        prog->applyUniforms( uniforms, newContext.uniforms.size() );

        prog->applyTextures( newContext.textures.cptr(), newContext.textures.MAX_SIZE, skipDirtyCheck );
    }
    else
    {
        if( skipDirtyCheck || mContext.gpuProgram )
        {
            // re-apply shader pointer to NULL, only when dirty check is disabled,
            // or last GPU program is not NULL.
            mDevice->SetVertexShader( NULL );
            mDevice->SetPixelShader( NULL );
        }
    }

    return true;
}

//
//
// -----------------------------------------------------------------------------
inline bool
GN::gfx::XenonRenderer::bindContextResources(
    const RendererContext & newContext,
    bool                    skipDirtyCheck )
{
    //
    // bind vertex format
    //
    if( skipDirtyCheck || newContext.vtxfmt != mContext.vtxfmt )
    {
        AutoComPtr<IDirect3DVertexDeclaration9> & decl = mVertexFormats[newContext.vtxfmt];
        if( !decl )
        {
            decl.attach( createXenonVertexDecl( *mDevice, newContext.vtxfmt ) );
            if( !decl ) return false;
        }

        // apply to D3D device
        GN_DX9_CHECK( mDevice->SetVertexDeclaration( decl ) );
    }

    ///
    /// bind vertex buffers
    ///
    for( UINT i = 0; i < RendererContext::MAX_VERTEX_BUFFERS; ++i )
    {
        const AutoRef<VtxBuf> & vb     = newContext.vtxbufs[i];
        UINT                    stride = newContext.strides[i];

        if( skipDirtyCheck || vb != mContext.vtxbufs[i] || stride != mContext.strides[i] )
        {
            GN_DX9_CHECK( mDevice->SetStreamSource(
                i,
                vb ? safeCastPtr<const XenonVtxBuf>(vb.get())->getD3DBuffer() : NULL,
                0, // offset
                stride ) );
        }
    }

    //
    // bind index buffer
    //
    if( skipDirtyCheck || newContext.idxbuf != mContext.idxbuf )
    {
        GN_DX9_CHECK( mDevice->SetIndices( newContext.idxbuf
            ? safeCastPtr<const XenonIdxBuf>(newContext.idxbuf.get())->getD3DBuffer()
            : NULL ) );
    }

    GN_TODO( "apply textures and samplers" );

    /*
    // bind samplers
    //
    if( newFlags.samplers )
    {
        UINT maxStages = getCaps(CAPS_MAX_TEXTURE_STAGES);
        UINT numTex = min<UINT>( (UINT)newContext.numTextures, maxStages );
        UINT stage;
        for( stage = 0; stage < numTex; ++stage )
        {
            SamplerHandle samp = newContext.samplers[stage];
            if( samp != mContext.samplers[stage] ||
                stage > mContext.numSamplers ||
                !forceRebind )
            {
                if( 0 == samp ) samp = mDefaultSampler;
                mSamplers[samp].bind( stage );
            }
        }
    }*/

    return true;
}
