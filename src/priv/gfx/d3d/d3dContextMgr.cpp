#include "pch.h"
#include "d3dRenderer.h"
#if !GN_ENABLE_INLINE
#include "d3dContextMgr.inl"
#endif
#include "d3dShader.h"
#include "d3dRenderStateBlock.h"
#include "d3dTexture.h"
#include "d3dVertexDecl.h"
#include "d3dVtxBuf.h"
#include "d3dIdxBuf.h"

// *****************************************************************************
// local functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
static inline void sBindShaders(
    LPDIRECT3DDEVICE9 dev,
    const GN::gfx::RenderingContext & oldContext,
    const GN::gfx::RenderingContext & newContext,
    bool forceRebind )
{
    GN_GUARD_SLOW;

    const GN::gfx::Shader * o;
    const GN::gfx::Shader * n;

    for( int i = 0; i < GN::gfx::NUM_SHADER_TYPES; ++i )
    {
        o = oldContext.shaders[i].get();
        n = newContext.shaders[i].get();

        if( o != n || forceRebind )
        {
            if( n )
            {
                GN::safeCast<const GN::gfx::D3DBasicShader*>(n)->apply();
            }
            else if( 0 == i )
            {
                GN_DX_CHECK( dev->SetVertexShader( 0 ) );
            }
            else
            {
                GN_ASSERT( 1 == i );
                GN_DX_CHECK( dev->SetPixelShader( 0 ) );
            }
        }
        else if( n )
        {
            GN::safeCast<const GN::gfx::D3DBasicShader*>(n)->applyDirtyUniforms();
        }
    }

    GN_UNGUARD_SLOW;
}

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
bool GN::gfx::D3DRenderer::contextInit()
{
    _GNGFX_DEVICE_TRACE();
    return true;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3DRenderer::contextQuit()
{
    _GNGFX_DEVICE_TRACE();
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3DRenderer::contextClear()
{
    _GNGFX_DEVICE_TRACE();
    mContext.clear();
    mVtxPxlData.clear();
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3DRenderer::contextDeviceCreate()
{
    _GNGFX_DEVICE_TRACE();
    return true;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3DRenderer::contextDeviceRestore()
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    //
    // TODO: restore render/sampler/texture states
    //
    for( UINT i = 0; i < getCaps(CAPS_MAX_TEXTURE_STAGES); ++i )
    {
    }

    // rebind context and data
    bindContext( mContext, true );
    bindVtxPxlData( mVtxPxlData, true );

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3DRenderer::contextDeviceDispose()
{
    _GNGFX_DEVICE_TRACE();
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3DRenderer::contextDeviceDestroy()
{
    _GNGFX_DEVICE_TRACE();
}

// *****************************************************************************
// from Renderer
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3DRenderer::setContext( const RenderingContext & newContext )
{
    GN_GUARD_SLOW;
    bindContext( newContext, false );
    mContext = newContext;
    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3DRenderer::setVtxPxlData( const VtxPxlData & newData )
{
    GN_GUARD_SLOW;
    bindVtxPxlData( newData, false );
    mVtxPxlData = newData;
    GN_UNGUARD_SLOW;
}

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN_INLINE void GN::gfx::D3DRenderer::bindContext(
    const RenderingContext & newContext, bool forceRebind )
{
    GN_GUARD_SLOW;

    //
    // Parameter check
    //
    if( isParameterCheckEnabled() )
    {
        // TODO: verify data in new context
    }

    //
    // bind shaders
    //
    sBindShaders( mDevice, mContext, newContext, forceRebind );

    //
    // bind render states
    //
    if( 0 != newContext.rsb )
    {
        if( forceRebind || 0 == mContext.rsb )
        {
            applyRenderStateBlock( *this, getRsbFromHandle( newContext.rsb ) );
        }
        else if( mContext.rsb != newContext.rsb )
        {
            applyRenderStateBlock(
                *this,
                getRsbFromHandle( newContext.rsb ) - getRsbFromHandle( mContext.rsb ) );
        }
    }

    //
    // TODO: bind color and depth buffers
    //

    //
    // bind viewport
    //
    if( newContext.viewport != mContext.viewport || forceRebind )
    {
        // clamp viewport in valid range
        float l = newContext.viewport.x;
        float t = newContext.viewport.y;
        float r = l + newContext.viewport.w;
        float b = t + newContext.viewport.h;
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

    //
    // bind FFP parameters
    //
#if !GN_XENON
    if( newContext.world != mContext.world || forceRebind )
    {
        Matrix44f mat = Matrix44f::sTranspose( newContext.world );
        GN_DX_CHECK( mDevice->SetTransform( D3DTS_WORLD, (const D3DMATRIX*)&mat ) );
    }

    if( newContext.view != mContext.view || forceRebind )
    {
        Matrix44f mat = Matrix44f::sTranspose( newContext.view );
        GN_DX_CHECK( mDevice->SetTransform( D3DTS_VIEW, (const D3DMATRIX*)&mat ) );
    }

    if( newContext.proj != mContext.proj || forceRebind )
    {
        Matrix44f mat = Matrix44f::sTranspose( newContext.proj );
        GN_DX_CHECK( mDevice->SetTransform( D3DTS_PROJECTION, (const D3DMATRIX*)&mat ) );
    }
    if( newContext.light0Diffuse != mContext.light0Diffuse ||
        newContext.light0Pos != mContext.light0Pos ||
        forceRebind )
    {
        D3DLIGHT9 d3dlight;
        d3dlight.Type = D3DLIGHT_POINT;
        sSetColorValue( d3dlight.Diffuse, newContext.light0Diffuse );
        sSetColorValue( d3dlight.Specular, 0, 0, 0, 0 );
        sSetColorValue( d3dlight.Ambient, 0, 0, 0, 0 );
        sSetD3DVector( d3dlight.Position, newContext.light0Pos );
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

    if( newContext.materialDiffuse != mContext.materialDiffuse ||
        newContext.materialSpecular != mContext.materialSpecular ||
        forceRebind )
    {
        D3DMATERIAL9 mat;
        sSetColorValue( mat.Diffuse, newContext.materialDiffuse );
        sSetColorValue( mat.Specular, newContext.materialSpecular );
        sSetColorValue( mat.Ambient, 0, 0, 0, 0 );
        sSetColorValue( mat.Emissive, 0, 0, 0, 0 );
        mat.Power = 1.0f;
        GN_DX_CHECK( mDevice->SetMaterial( &mat ) );
    }

    if( newContext.textureStates != mContext.textureStates || forceRebind )
    {
        // apply all TSSs to API
        uint32_t i;
        const TextureStateBlockDesc & desc = newContext.textureStates;
        TextureStateValue tsv;
        DWORD d3dtsv;
        uint32_t numStages = GN::min<uint32_t>( MAX_TEXTURE_STAGES, getCaps( CAPS_MAX_TEXTURE_STAGES ) );
        for ( i = 0; i < numStages; ++i )
        {
            for ( uint32_t j = 0; j < NUM_TEXTURE_STATES; ++j )
            {
                tsv = desc.ts[i][j];
                if( TSV_INVALID != tsv )
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
#endif // !GN_XENON

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
GN_INLINE void GN::gfx::D3DRenderer::bindVtxPxlData(
    const VtxPxlData & newData, bool forceRebind )
{
    GN_GUARD_SLOW;

    //
    // Parameter check
    //
    if( isParameterCheckEnabled() )
    {
        // TODO: verify data in new context
    }

    //
    // bind vertex binding and vertx buffers
    //
    if( 0 != newData.vtxBinding )
    {
        const D3DVtxBindingDesc & desc = mVtxBindings[newData.vtxBinding];
        GN_ASSERT( desc.decl );

        if( newData.vtxBinding != mVtxPxlData.vtxBinding || forceRebind )
        {
            mDevice->SetVertexDeclaration( desc.decl );
        }

        for( UINT i = 0; i < desc.format.numStreams; ++i )
        {
            const VtxPxlData::VtxBufDesc & vb = newData.vtxBuffers[i];
            if( vb.buffer != mVtxPxlData.vtxBuffers[i].buffer || forceRebind )
            {
                GN_ASSERT( vb.buffer );
                GN_DX_CHECK( mDevice->SetStreamSource(
                    i,
                    safeCast<const D3DVtxBuf*>(vb.buffer.get())->getD3DVb(),
                    0,
                    //(UINT)( vb.stride ? vb.stride : desc.format.streams[i].stride ) ) );
                    (UINT)vb.stride ) );
            }
        }
    }

    //
    // bind index buffer
    //
    if( newData.idxBuffer != mVtxPxlData.idxBuffer || forceRebind )
    {
        GN_DX_CHECK( mDevice->SetIndices( newData.idxBuffer
            ? safeCast<const D3DIdxBuf*>(newData.idxBuffer.get())->getD3DIb()
            : 0 ) );
    }

    //
    // bind textures
    //
    UINT maxStages = getCaps(CAPS_MAX_TEXTURE_STAGES);
    UINT numTex = min<UINT>( (UINT)newData.numTextures, maxStages );
    UINT stage;
    for( stage = 0; stage < numTex; ++stage )
    {
        const Texture * tex = newData.textures[stage];
        if( tex != mVtxPxlData.textures[stage] ||
            stage > mVtxPxlData.numTextures ||
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
    for( ; stage < newData.numTextures; ++stage )
    {
        mDevice->SetTexture( stage, 0 );
    }

    GN_UNGUARD_SLOW;
}
