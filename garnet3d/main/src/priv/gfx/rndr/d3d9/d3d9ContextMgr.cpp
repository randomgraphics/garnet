#include "pch.h"
#include "d3d9Renderer.h"
#include "d3d9RenderTargetMgr.h"
#include "d3d9Shader.h"
#include "d3d9Texture.h"
#include "d3d9Sampler.h"
#include "d3d9VertexDecl.h"
#include "d3d9VtxBuf.h"
#include "d3d9IdxBuf.h"

// *****************************************************************************
// local functions
// *****************************************************************************

static inline void sSetColorValue( D3DCOLORVALUE & c, float r, float g, float b, float a )
{
    c.r = r;
    c.g = g;
    c.b = b;
    c.a = a;
}

static inline void sSetColorValue( D3DCOLORVALUE & c, const GN::Vector4f & v )
{
    c.r = v.r;
    c.g = v.g;
    c.b = v.b;
    c.a = v.a;
}

static inline void sSetD3DVector( D3DVECTOR & v, float x, float y, float z, float w )
{
    float k = 1 / w;
    v.x = x * k;
    v.y = y * k;
    v.z = z * k;
}

static inline void sSetD3DVector( D3DVECTOR & dst, const GN::Vector4f & src )
{
    float k = 1.0f / src.w;
    dst.x = src.x * k;
    dst.y = src.y * k;
    dst.z = src.z * k;
}

static DWORD sRenderStateValue2D3D[GN::gfx::NUM_RENDER_STATE_VALUES] =
{
    #define GNGFX_DEFINE_RSV( tag, d3dval, glval ) d3dval,
    #include "garnet/gfx/renderStateValueMeta.h"
    #undef GNGFX_DEFINE_RSV
};

#include "d3d9RenderState.inl"

static D3DTEXTURESTAGESTATETYPE sTextureState2D3D[GN::gfx::NUM_TEXTURE_STATES] =
{
    #define GNGFX_DEFINE_TS( tag, defval0, d3dname, glname1, glname2 )  d3dname,
    #include "garnet/gfx/textureStateMeta.h"
    #undef GNGFX_DEFINE_TS
};
//
static DWORD sTextureStateValue2D3D[GN::gfx::NUM_TEXTURE_STATE_VALUES] =
{
    #define GNGFX_DEFINE_TSV( tag, d3dval, glval1, glval2 ) d3dval,
    #include "garnet/gfx/textureStateValueMeta.h"
    #undef GNGFX_DEFINE_TSV
};

static inline void sSetupD3D9Viewport( LPDIRECT3DDEVICE9 dev, float l, float t, float r, float b )
{
    GN_ASSERT(
        0.0f <= l && l <= 1.0f &&
        0.0f <= t && t <= 1.0f &&
        0.0f <= r && r <= 1.0f &&
        0.0f <= b && b <= 1.0f &&
        l <= r && t <= b );

    float w = r - l;
    float h = b - t;

    // get size of render target 0
    GN::AutoComPtr<IDirect3DSurface9> rt0;
    D3DSURFACE_DESC rt0Desc;
    GN_DX9_CHECK( dev->GetRenderTarget( 0, &rt0 ) );
    GN_DX9_CHECK( rt0->GetDesc( &rt0Desc ) );

    // set d3d viewport
    D3DVIEWPORT9 d3dvp = {
        (DWORD)l * rt0Desc.Width,
        (DWORD)t * rt0Desc.Height,
        (DWORD)w * rt0Desc.Width,
        (DWORD)h * rt0Desc.Height,
        0.0f, 1.0f,
    };

    // update D3D viewport
    GN_DX9_CHECK( dev->SetViewport(&d3dvp) );

    // update D3D scissors
    RECT rc = {
        int( d3dvp.X ),
        int( d3dvp.Y ),
        int( d3dvp.X+d3dvp.Width ),
        int( d3dvp.Y+d3dvp.Height ),
    };
    GN_DX9_CHECK( dev->SetScissorRect( &rc ) );
}

// *****************************************************************************
// init/shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D9Renderer::contextDeviceRestore()
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    // initialize render target manager
    GN_ASSERT( 0 == mRTMgr );
#if GN_XENON
    mRTMgr = new D3D9RTMgrXenon( *this );
#elif GN_PC
    mRTMgr = new D3D9RTMgrPC( *this );
#else
#error Unsupport platform!
#endif
    if( !mRTMgr->init() ) return false;

    // initialize render/texture/sampler states
#if !GN_XENON
    setD3DRenderState( D3DRS_COLORVERTEX, 1 ); // always enable color vertex
#endif

    // rebind context
    bindContext( mContext, mContext.flags, true );

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9Renderer::contextDeviceDispose()
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    safeDelete( mRTMgr );

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9Renderer::contextDeviceDestroy()
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    // unset resources used by D3D device.
	if( mDevice )
	{
		for( UInt32 i = 0; i < getCaps(CAPS_MAX_TEXTURE_STAGES); ++i ) mDevice->SetTexture( i, 0 );
		for( UInt32 i = 0; i < 16; ++i ) mDevice->SetStreamSource( i, 0, 0, 0 );
		mDevice->SetIndices( 0 );
		mDevice->SetVertexDeclaration( 0 );
		mDevice->SetVertexShader( 0 );
		mDevice->SetPixelShader( 0 );
	}

    mContext.resetToDefault();

	clearContextResources();

    GN_UNGUARD;
}

// *****************************************************************************
// from Renderer
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9Renderer::setContext( const RendererContext & newContext )
{
    GN_GUARD_SLOW;

    GN_ASSERT( getCurrentThreadId() == mThreadId );

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
void GN::gfx::D3D9Renderer::rebindContext( RendererContext::FieldFlags flags )
{
    GN_GUARD_SLOW;
    bindContext( mContext, flags, true );
    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
const GN::gfx::RenderStateBlockDesc & GN::gfx::D3D9Renderer::getCurrentRenderStateBlock() const
{
    GN_ASSERT( getCurrentThreadId() == mThreadId );
    GN_ASSERT( mContext.flags.rsb );
    return mContext.rsb;
}

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
inline void GN::gfx::D3D9Renderer::bindContext(
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

#if !GN_XENON
    if( newFlags.ffp ) bindContextFfp( newContext, newFlags, forceRebind );
#endif

    if( newFlags.data ) bindContextData( newContext, newFlags, forceRebind );

    GN_UNGUARD_SLOW;
}


//
//
// -----------------------------------------------------------------------------
inline void GN::gfx::D3D9Renderer::bindContextState(
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
                    GN::safeCastPtr<const GN::gfx::D3D9BasicShader>(n)->apply();
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
                GN::safeCastPtr<const GN::gfx::D3D9BasicShader>(n)->applyDirtyUniforms();
            }
        } else if( o )
        {
            GN::safeCastPtr<const GN::gfx::D3D9BasicShader>(o)->applyDirtyUniforms();
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

            sSetupD3D9Viewport( mDevice, l , t, r, b );
        }
    }
    else if( needRebindViewport )
    {
        float l = mContext.viewport.x;
        float t = mContext.viewport.y;
        float r = l + mContext.viewport.w;
        float b = t + mContext.viewport.h;
        sSetupD3D9Viewport( mDevice, l, t, r, b );
    }

    GN_UNGUARD_SLOW;
}

/*
//
// -----------------------------------------------------------------------------
inline void GN::gfx::D3D9Renderer::bindContextRenderTargetsAndViewport(
    const RendererContext & newContext,
    RendererContext::FieldFlags newFlags,
    bool forceRebind )
{
    bool needRebindViewport = false;

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
}*/

//
//
// -----------------------------------------------------------------------------
inline void GN::gfx::D3D9Renderer::bindContextFfp(
    const RendererContext & newContext,
    RendererContext::FieldFlags newFlags,
    bool forceRebind )
{
#if GN_XENON
    GN_UNUSED_PARAM( newContext );
    GN_UNUSED_PARAM( newFlags );
    GN_UNUSED_PARAM( forceRebind );
#else
    GN_GUARD_SLOW;

    GN_ASSERT( newFlags.ffp );

    if( newFlags.world )
    {
        if( newContext.world != mContext.world || forceRebind )
        {
            Matrix44f mat = Matrix44f::sTranspose( newContext.world );
            GN_DX9_CHECK( mDevice->SetTransform( D3DTS_WORLD, (const D3DMATRIX*)&mat ) );
        }
    }

    if( newFlags.view )
    {
        if( newContext.view != mContext.view || forceRebind )
        {
            Matrix44f mat = Matrix44f::sTranspose( newContext.view );
            GN_DX9_CHECK( mDevice->SetTransform( D3DTS_VIEW, (const D3DMATRIX*)&mat ) );
        }
    }

    if( newFlags.proj )
    {
        if( newContext.proj != mContext.proj || forceRebind )
        {
            Matrix44f mat = Matrix44f::sTranspose( newContext.proj );
            GN_DX9_CHECK( mDevice->SetTransform( D3DTS_PROJECTION, (const D3DMATRIX*)&mat ) );
        }
    }

    if( newFlags.light0Diffuse || newFlags.light0Pos )
    {
        const Vector4f & diff = newFlags.light0Diffuse ? newContext.light0Diffuse : mContext.light0Diffuse;
        const Vector4f & pos = newFlags.light0Pos ? newContext.light0Pos : mContext.light0Pos;
        if( diff != mContext.light0Diffuse ||
            pos != mContext.light0Pos ||
            forceRebind )
        {
            D3DLIGHT9 d3dlight;
            d3dlight.Type = D3DLIGHT_POINT;
            sSetColorValue( d3dlight.Diffuse, diff );
            sSetColorValue( d3dlight.Specular, 0, 0, 0, 0 );
            sSetColorValue( d3dlight.Ambient, 0, 0, 0, 0 );
            sSetD3DVector( d3dlight.Position, pos );
            sSetD3DVector( d3dlight.Direction, 0, 0, 1, 1 );
            d3dlight.Range = sqrt(FLT_MAX);
            d3dlight.Falloff = 1.0f;
            d3dlight.Attenuation0 = 1.0f;
            d3dlight.Attenuation1 = 0.0f;
            d3dlight.Attenuation2 = 0.0f;
            d3dlight.Theta = D3DX_PI;
            d3dlight.Phi = D3DX_PI;
            GN_DX9_CHECK( mDevice->SetLight( 0, &d3dlight ) );
        }
    }

    if( newFlags.materialDiffuse || newFlags.materialSpecular )
    {
        const Vector4f & diff = newFlags.materialDiffuse ? newContext.materialDiffuse : mContext.materialDiffuse;
        const Vector4f & spec = newFlags.materialSpecular ? newContext.materialSpecular : mContext.materialSpecular;
        if( diff != mContext.materialDiffuse ||
            spec != mContext.materialSpecular ||
            forceRebind )
        {
            D3DMATERIAL9 mat;
            sSetColorValue( mat.Diffuse, diff );
            sSetColorValue( mat.Specular, spec );
            sSetColorValue( mat.Ambient, 0, 0, 0, 0 );
            sSetColorValue( mat.Emissive, 0, 0, 0, 0 );
            mat.Power = 1.0f;
            GN_DX9_CHECK( mDevice->SetMaterial( &mat ) );
        }
    }

    if( newFlags.tsb )
    {
        const TextureStateBlockDesc & desc = newContext.tsb;
        DWORD d3dtsv;
        UInt32 maxStages = getD3DCaps( D3D9CAPS_MAX_FFP_TEXTURE_STAGES );
        UInt32 numStages = GN::min<UInt32>( (UInt32)desc.getNumStages(), maxStages );
        for ( UInt32 i = 0; i < numStages; ++i )
        {
            for ( UInt32 j = 0; j < NUM_TEXTURE_STATES; ++j )
            {
                if( desc.isSet( i, (TextureState)j ) )
                {
                    d3dtsv = sTextureStateValue2D3D[ desc.get( i, (TextureState)j ) ];

                    if( D3DTOP_DOTPRODUCT3 == d3dtsv &&
                        !getD3DCaps( D3D9CAPS_DOT3 ) )
                    {
                        GN_DO_ONCE( GN_WARN(sLogger)(
                            "Current D3D device does not support "
                            "dot3 operation! "
                            "Fallback to D3DTOP_SELECTARG1." ) );
                        d3dtsv = D3DTOP_SELECTARG1;
                    }
                    setD3DTextureState( i, sTextureState2D3D[j], d3dtsv );
                }
            }
        }
        // disable remaining stages
        if( numStages < maxStages )
        {
            setD3DTextureState( numStages, D3DTSS_COLOROP, D3DTOP_DISABLE );
            setD3DTextureState( numStages, D3DTSS_ALPHAOP, D3DTOP_DISABLE );
        }
    }

    GN_UNGUARD_SLOW;
#endif // !GN_XENON
}

//
//
// -----------------------------------------------------------------------------
inline void GN::gfx::D3D9Renderer::bindContextData(
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
            const D3D9VtxDeclDesc * decl;
            decl = &mVtxFmts[newContext.vtxfmt];
            GN_ASSERT( decl->decl );
            if( newContext.vtxfmt != mContext.vtxfmt || forceRebind )
            {
                GN_DX9_CHECK( mDevice->SetVertexDeclaration( decl->decl ) );
            }
        }
    }

    ///
    /// bind vertex buffers
    ///
    if( newFlags.vtxbufs )
    {
        for( UINT i = 0; i < newContext.numVtxBufs; ++i )
        {
            const RendererContext::VtxBufDesc & vb = newContext.vtxbufs[i];
            if( vb != mContext.vtxbufs[i] || forceRebind )
            {
                GN_ASSERT( vb.buffer );
                GN_DX9_CHECK( mDevice->SetStreamSource(
                    i,
                    safeCastPtr<const D3D9VtxBuf>(vb.buffer)->getD3DVb(),
                    (UINT)vb.offset,
                    (UINT)vb.stride ) );
            }
        }
    }

    //
    // bind index buffer
    //
    if( newFlags.idxbuf &&
      ( newContext.idxbuf != mContext.idxbuf || forceRebind ) )
    {
        GN_DX9_CHECK( mDevice->SetIndices( newContext.idxbuf
            ? safeCastPtr<const D3D9IdxBuf>(newContext.idxbuf)->getD3DIb()
            : 0 ) );
    }

    //
    // bind textures
    //
    if( newFlags.textures )
    {
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
                    safeCastPtr<const D3D9Texture>(tex)->bind( stage );
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
    }

    //
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
    }

    GN_UNGUARD_SLOW;
}
