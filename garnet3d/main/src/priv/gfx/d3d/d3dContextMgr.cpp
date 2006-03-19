#include "pch.h"
#include "d3dRenderer.h"
#if !GN_ENABLE_INLINE
#include "d3dContextMgr.inl"
#endif
#include "d3dShader.h"
#include "d3dTexture.h"
#include "d3dVertexDecl.h"
#include "d3dVtxBuf.h"
#include "d3dIdxBuf.h"

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

#include "d3dRenderState.inl"

static D3DTEXTURESTAGESTATETYPE sTextureState2D3D[GN::gfx::NUM_TEXTURE_STATES] =
{
    #define GNGFX_DEFINE_TS( tag, defval0, defval, d3dname, glname1, glname2 )  d3dname,
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

// *****************************************************************************
// init/shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3DRenderer::contextClear()
{
    _GNGFX_DEVICE_TRACE();
    mContextState.resetToDefault();
    mContextData.resetToEmpty();
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3DRenderer::contextDeviceRestore()
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    // initialize all render/texture/sampler states
    for( int i = 0; i < MAX_D3D_RENDER_STATES; ++i ) mRenderStates[i].clear();
    for( int s = 0; s < MAX_TEXTURE_STAGES; ++s )
        for( int i = 0; i < MAX_D3D_SAMPLER_STATES; ++i ) mSamplerStates[s][i].clear();
#if !GN_XENON
    for( int s = 0; s < MAX_TEXTURE_STAGES; ++s )
        for( int i = 0; i < MAX_D3D_TEXTURE_STATES; ++i ) mTextureStates[s][i].clear();
    setD3DRenderState( D3DRS_COLORVERTEX, 1 ); // always enable color vertex
#endif

    // rebind context and data
    bindContextState( mContextState, mContextState.flags, true );
    bindContextData( mContextData, mContextData.flags, true );

    // success
    return true;

    GN_UNGUARD;
}

// *****************************************************************************
// from Renderer
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3DRenderer::setContextState( const ContextState & newState )
{
    GN_GUARD_SLOW;

#if GN_DEBUG
    // make sure bindContextState() does not rely on flags in tmp structure.
    ContextState tmp = newState;
    ContextState::FieldFlags flags = tmp.flags;
    tmp.flags.u32 = 0;
    bindContextState( tmp, flags, false );
#else
    bindContextState( newState, newState.flags, false );
#endif

    mContextState.mergeWith( newState );
    holdContextState( newState );

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3DRenderer::setContextData( const ContextData & newData )
{
    GN_GUARD_SLOW;

#if GN_DEBUG
    // make sure bindContextData() does not rely on flags in tmp structure.
    ContextData tmp = newData;
    ContextData::FieldFlags flags = tmp.flags;
    tmp.flags.u32 = 0;
    bindContextData( tmp, flags, false );
#else
    bindContextData( newData, newData.flags, false );
#endif

    mContextData.mergeWith( newData );
    holdContextData( newData );

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3DRenderer::rebindContextState( ContextState::FieldFlags flags )
{
    GN_GUARD_SLOW;
    bindContextState( mContextState, flags, true );
    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3DRenderer::rebindContextData( ContextData::FieldFlags flags )
{
    GN_GUARD_SLOW;
    bindContextData( mContextData, flags, true );
    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
const GN::gfx::RenderStateBlockDesc & GN::gfx::D3DRenderer::getCurrentRenderStateBlock() const
{
    GN_GUARD_SLOW;
    GN_ASSERT( mContextState.flags.rsb );
    return mContextState.rsb;
    GN_UNGUARD_SLOW;
}

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN_INLINE void GN::gfx::D3DRenderer::bindContextState(
    const ContextState & newState,
    ContextState::FieldFlags newFlags,
    bool forceRebind )
{
    GN_GUARD_SLOW;

    //
    // Parameter check
    //
    if( isParameterCheckEnabled() )
    {
        // TODO: verify data in new context
        // TODO: make sure all fields in current context are valid.
    }

    //
    // bind vertex shader
    //
    if( newFlags.vtxShader )
    {
        newFlags.vtxShader = 0;
        const GN::gfx::Shader * o = mContextState.shaders[VERTEX_SHADER];
        const GN::gfx::Shader * n = newState.shaders[VERTEX_SHADER];
        if( o != n || forceRebind )
        {
            if( n )
            {
                GN::safeCast<const GN::gfx::D3DBasicShader*>(n)->apply();
            }
            else
            {
                GN_DX_CHECK( mDevice->SetVertexShader( 0 ) );
            }
        }
        else if( n )
        {
            GN::safeCast<const GN::gfx::D3DBasicShader*>(n)->applyDirtyUniforms();
        }
    }

    //
    // bind pixel shader
    //
    if( newFlags.pxlShader )
    {
        newFlags.pxlShader = 0;
        const GN::gfx::Shader * o = mContextState.shaders[PIXEL_SHADER];
        const GN::gfx::Shader * n = newState.shaders[PIXEL_SHADER];
        if( o != n || forceRebind )
        {
            if( n )
            {
                GN::safeCast<const GN::gfx::D3DBasicShader*>(n)->apply();
            }
            else
            {
                GN_DX_CHECK( mDevice->SetPixelShader( 0 ) );
            }
        }
        else if( n )
        {
            GN::safeCast<const GN::gfx::D3DBasicShader*>(n)->applyDirtyUniforms();
        }
    }

    //
    // bind render states
    //
    if( newFlags.rsb )
    {
        newFlags.rsb = 0;
        GN::gfx::RenderStateValue rsv;
        #define GNGFX_DEFINE_RS( tag, defvalue )         \
            rsv = newState.rsb.rs[RS_##tag];             \
            GN_ASSERT( rsv < NUM_RENDER_STATE_VALUES );  \
            if( rsv != RSV_EMPTY ) sSet_##tag( *this, rsv );
        #include "garnet/gfx/renderStateMeta.h"
        #undef GNGFX_DEFINE_RS
    }

    //
    // TODO: bind render targets
    //
    if( newFlags.renderTargets )
    {
        newFlags.renderTargets = 0;
    }

    //
    // bind viewport
    //
    if( newFlags.viewport )
    {
        newFlags.viewport = 0;
        if( newState.viewport != mContextState.viewport || forceRebind )
        {
            // clamp viewport in valid range
            float l = newState.viewport.x;
            float t = newState.viewport.y;
            float r = l + newState.viewport.w;
            float b = t + newState.viewport.h;
            clamp<float>( l, 0.0f, 1.0f );
            clamp<float>( b, 0.0f, 1.0f );
            clamp<float>( r, 0.0f, 1.0f );
            clamp<float>( t, 0.0f, 1.0f );
            float w = r - l;
            float h = b - t;

            // get size of render target 0
            AutoComPtr<IDirect3DSurface9> rt0;
            D3DSURFACE_DESC rt0Desc;
            GN_DX_CHECK( mDevice->GetRenderTarget( 0, &rt0 ) );
            GN_DX_CHECK( rt0->GetDesc( &rt0Desc ) );

            // set d3d viewport
            D3DVIEWPORT9 d3dvp = {
                (DWORD)l * rt0Desc.Width,
                (DWORD)t * rt0Desc.Height,
                (DWORD)w * rt0Desc.Width,
                (DWORD)h * rt0Desc.Height,
                0.0f, 1.0f,
            };

            // update D3D viewport
            GN_DX_CHECK( mDevice->SetViewport(&d3dvp) );

            // update D3D scissors
            RECT rc = {
                int( d3dvp.X ),
                int( d3dvp.Y ),
                int( d3dvp.X+d3dvp.Width ),
                int( d3dvp.Y+d3dvp.Height ),
            };
            GN_DX_CHECK( mDevice->SetScissorRect( &rc ) );
        }
    }

    //
    // bind FFP parameters
    //
#if !GN_XENON

    // When using programmable pipeline, FFP states should not change too often.
    // So here we add a check point to skip FFP states update once and for all.
    if( 0 == newFlags.u32 ) return;

    if( newFlags.world )
    {
        if( newState.world != mContextState.world || forceRebind )
        {
            Matrix44f mat = Matrix44f::sTranspose( newState.world );
            GN_DX_CHECK( mDevice->SetTransform( D3DTS_WORLD, (const D3DMATRIX*)&mat ) );
        }
    }

    if( newFlags.view )
    {
        if( newState.view != mContextState.view || forceRebind )
        {
            Matrix44f mat = Matrix44f::sTranspose( newState.view );
            GN_DX_CHECK( mDevice->SetTransform( D3DTS_VIEW, (const D3DMATRIX*)&mat ) );
        }
    }

    if( newFlags.proj )
    {
        if( newState.proj != mContextState.proj || forceRebind )
        {
            Matrix44f mat = Matrix44f::sTranspose( newState.proj );
            GN_DX_CHECK( mDevice->SetTransform( D3DTS_PROJECTION, (const D3DMATRIX*)&mat ) );
        }
    }

    if( newFlags.light0Diffuse || newFlags.light0Pos )
    {
        const Vector4f & diff = newFlags.light0Diffuse ? newState.light0Diffuse : mContextState.light0Diffuse;
        const Vector4f & pos = newFlags.light0Pos ? newState.light0Pos : mContextState.light0Pos;
        if( diff != mContextState.light0Diffuse ||
            pos != mContextState.light0Pos ||
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
            GN_DX_CHECK( mDevice->SetLight( 0, &d3dlight ) );
        }
    }

    if( newFlags.materialDiffuse || newFlags.materialSpecular )
    {
        const Vector4f & diff = newFlags.materialDiffuse ? newState.materialDiffuse : mContextState.materialDiffuse;
        const Vector4f & spec = newFlags.materialSpecular ? newState.materialSpecular : mContextState.materialSpecular;
        if( diff != mContextState.materialDiffuse ||
            spec != mContextState.materialSpecular ||
            forceRebind )
        {
            D3DMATERIAL9 mat;
            sSetColorValue( mat.Diffuse, diff );
            sSetColorValue( mat.Specular, spec );
            sSetColorValue( mat.Ambient, 0, 0, 0, 0 );
            sSetColorValue( mat.Emissive, 0, 0, 0, 0 );
            mat.Power = 1.0f;
            GN_DX_CHECK( mDevice->SetMaterial( &mat ) );
        }
    }

    if( newFlags.textureStates )
    {
        if( newState.textureStates != mContextState.textureStates || forceRebind )
        {
            // apply all TSSs to API
            uint32_t i;
            const TextureStateBlockDesc & desc = newState.textureStates;
            TextureStateValue tsv;
            DWORD d3dtsv;
            uint32_t numStages = GN::min<uint32_t>( MAX_TEXTURE_STAGES, getCaps( CAPS_MAX_TEXTURE_STAGES ) );
            for ( i = 0; i < numStages; ++i )
            {
                for ( uint32_t j = 0; j < NUM_TEXTURE_STATES; ++j )
                {
                    tsv = desc.ts[i][j];
                    if( TSV_EMPTY != tsv )
                    {
                        d3dtsv = sTextureStateValue2D3D[tsv];

                        if( D3DTOP_DOTPRODUCT3 == d3dtsv &&
                            !getD3DCaps( D3DCAPS_DOT3 ) )
                        {
                            GN_DO_ONCE( GN_WARN(
                                "Current D3D device does not support "
                                "dot3 operation! "
                                "Fallback to D3DTOP_SELECTARG1." ) );
                            d3dtsv = D3DTOP_SELECTARG1;
                        }
                        else if( D3DTA_CONSTANT == (d3dtsv&D3DTA_SELECTMASK) &&
                            !getCaps( CAPS_PER_STAGE_CONSTANT ) )
                        {
                            GN_DO_ONCE( GN_WARN(
                                    "Current D3D device does not support "
                                    "per-stage constant! "
                                    "Fallback to D3DTA_TFACTOR." ) );
                            d3dtsv = D3DTA_TFACTOR;
                        }
                        setD3DTextureState( i, sTextureState2D3D[j], d3dtsv );
                    }
                }
            }
        }
    }
#endif // !GN_XENON

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
GN_INLINE void GN::gfx::D3DRenderer::bindContextData(
    const ContextData & newData,
    ContextData::FieldFlags newFlags,
    bool forceRebind )
{
    GN_GUARD_SLOW;

    //
    // Parameter check
    //
    if( isParameterCheckEnabled() )
    {
        // TODO: verify data in new context
        // TODO: make sure all fields in current context are valid.
    }

    //
    // bind vertex format
    //
    if( newFlags.vtxFmt )
    {
        if( newData.vtxFmt )
        {
            const D3DVtxDeclDesc * decl;
            decl = &mVtxFmts[newData.vtxFmt];
            GN_ASSERT( decl->decl );
            if( newData.vtxFmt != mContextData.vtxFmt || forceRebind )
            {
                GN_DX_CHECK( mDevice->SetVertexDeclaration( decl->decl ) );
            }
        }
    }

    //!
    //! bind vertex buffers
    //!
    if( newFlags.vtxBufs )
    {
        for( UINT i = 0; i < newData.numVtxBufs; ++i )
        {
            const ContextData::VtxBufDesc & vb = newData.vtxBufs[i];
            if( vb.buffer != mContextData.vtxBufs[i].buffer || forceRebind )
            {
                GN_ASSERT( vb.buffer );
                GN_DX_CHECK( mDevice->SetStreamSource(
                    i,
                    safeCast<const D3DVtxBuf*>(vb.buffer)->getD3DVb(),
                    0,
                    (UINT)vb.stride ) );
            }
        }
    }

    //
    // bind index buffer
    //
    if( newFlags.idxBuf &&
      ( newData.idxBuf != mContextData.idxBuf || forceRebind ) )
    {
        GN_DX_CHECK( mDevice->SetIndices( newData.idxBuf
            ? safeCast<const D3DIdxBuf*>(newData.idxBuf)->getD3DIb()
            : 0 ) );
    }

    //
    // bind textures
    //
    if( newFlags.textures )
    {
        UINT maxStages = getCaps(CAPS_MAX_TEXTURE_STAGES);
        UINT numTex = min<UINT>( (UINT)newData.numTextures, maxStages );
        UINT stage;
        for( stage = 0; stage < numTex; ++stage )
        {
            const Texture * tex = newData.textures[stage];
            if( tex != mContextData.textures[stage] ||
                stage > mContextData.numTextures ||
                !forceRebind )
            {
                if( tex )
                {
                    safeCast<const D3DTexture*>(tex)->bind( stage );
                }
                else
                {
                    mDevice->SetTexture( stage, NULL );
                }
            }
        }
        // clear unused stages
        numTex = min<UINT>( (UINT)mContextData.numTextures, maxStages );
        for( ; stage < numTex; ++stage )
        {
            mDevice->SetTexture( stage, 0 );
        }
    }
    
    GN_UNGUARD_SLOW;
}
