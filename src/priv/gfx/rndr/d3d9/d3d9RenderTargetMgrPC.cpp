#include "pch.h"
#include "d3d9Renderer.h"
#include "d3d9RenderTargetMgr.h"
#include "d3d9Texture.h"
#include "garnet/GNd3d9.h"

#if GN_PC

static GN::Logger * sLogger = GN::getLogger("GN.gfx.rndr.D3D9");

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D9RTMgrPC::init()
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::D3D9RTMgrPC, () );

    IDirect3DDevice9 & dev = mRenderer.getDeviceRef();

    // get default color and depth buffer
    GN_DX9_CHECK_RV( dev.GetRenderTarget( 0, &mAutoColor0 ), false );
    GN_ASSERT( mAutoColor0 );
    dev.GetDepthStencilSurface( &mAutoDepth ); // note taht auto depth buffer might not be avaliable.

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9RTMgrPC::quit()
{
    GN_GUARD;

    if( mRenderer.getDevice() )
    {
        mRenderer.getDevice()->SetDepthStencilSurface( mAutoDepth );
    }

    mAutoColor0.clear();
    mAutoDepth.clear();

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9RTMgrPC::bind(
    const RenderTargetDesc & oldDesc,
    const RenderTargetDesc & newDesc,
    bool forceRebind,
    bool & needRebindViewport )
{
    GN_GUARD_SLOW;

    needRebindViewport = false;

    const RenderTargetTexture * newSurf;

    IDirect3DDevice9 & dev = mRenderer.getDeviceRef();

    // special case for color buffer 0
    if( 0 == newDesc.count )
    {
        if( oldDesc.count > 0 || forceRebind )
        {
            GN_DX9_CHECK( dev.SetRenderTarget( 0, mAutoColor0 ) );
        }
        needRebindViewport = true;
    }
    else
    {
        newSurf = &newDesc.cbuffers[0];
        GN_ASSERT( newSurf->texture );
        
        if( 0 == oldDesc.count || oldDesc.cbuffers[0] != *newSurf || forceRebind )
        {
            AutoComPtr<IDirect3DSurface9> surf;
            const D3D9Texture * tex = safeCast<const D3D9Texture*>( newSurf->texture );
            surf.attach( tex->getSurface( newSurf->face, newSurf->level ) );
            GN_DX9_CHECK( dev.SetRenderTarget( 0, surf ) );
        }
        needRebindViewport = true;
    }

    // bind remained color buffers
    UInt32 maxCount = mRenderer.getCaps( CAPS_MAX_RENDER_TARGETS );
    UInt32 count = min( (UInt32)newDesc.count, maxCount );
    for( UInt32 i = 1; i < count; ++i )
    {
        newSurf = &newDesc.cbuffers[i];
        GN_ASSERT( newSurf->texture );

        if( i >= oldDesc.count || *newSurf != oldDesc.cbuffers[i] || forceRebind )
        {
            const D3D9Texture * tex = safeCast<const D3D9Texture*>(newSurf->texture);
            AutoComPtr<IDirect3DSurface9> surf;
            surf.attach( tex->getSurface( newSurf->face, newSurf->level ) );
            GN_ASSERT( surf );
            GN_DX9_CHECK( dev.SetRenderTarget( 0, surf ) );
        }
    }

    // disable unused color buffers
    for( UInt32 i = count?count:1; i < maxCount; ++i )
    {
        GN_DX9_CHECK( dev.SetRenderTarget( i, 0 ) );
    }

    // bind depth texture
    newSurf = &newDesc.zbuffer;
    if( 0 == newSurf->texture )
    {
        AutoComPtr<IDirect3DSurface9> rt0;
        GN_DX9_CHECK( dev.GetRenderTarget( 0, &rt0 ) );
        GN_ASSERT( rt0 );
        D3DSURFACE_DESC rt0Desc;
        GN_DX9_CHECK( rt0->GetDesc( &rt0Desc ) );
        if( mAutoDepth )
        {
            // create new depth buffer, if current one is not compatible with the color buffer.
            // TODO: find appropriate depth format using IDirect3DDevice9::CheckDepthStencilMatch(...)
            D3DSURFACE_DESC depthDesc;
            GN_DX9_CHECK( mAutoDepth->GetDesc( &depthDesc ) );
            if( depthDesc.Width < rt0Desc.Width ||
                depthDesc.Height < rt0Desc.Height ||
                depthDesc.MultiSampleType != rt0Desc.MultiSampleType ||
                depthDesc.MultiSampleQuality != rt0Desc.MultiSampleQuality )
            {
                UInt32 w = max(depthDesc.Width, rt0Desc.Width);
                UInt32 h = max(depthDesc.Height, rt0Desc.Height);
                mAutoDepth.clear();
                GN_DX9_CHECK_R( dev.CreateDepthStencilSurface(
                    w,
                    h,
                    depthDesc.Format,
                    rt0Desc.MultiSampleType,
                    rt0Desc.MultiSampleQuality,
                    TRUE, // discardable depth buffer
                    &mAutoDepth, 0 ) );
                GN_DX9_CHECK( dev.SetDepthStencilSurface( mAutoDepth ) );
                GN_TRACE(sLogger)( "Recreate depth surface: width(%d), height(%d), format(%s), msaa(%d), quality(%d)",
                    w, h, d3d9::d3dFormat2Str(depthDesc.Format),
                    rt0Desc.MultiSampleType, rt0Desc.MultiSampleQuality );
            }
            else if( *newSurf != oldDesc.zbuffer || forceRebind )
            {
                GN_DX9_CHECK( dev.SetDepthStencilSurface( mAutoDepth ) );
            }
        }
        else
        {
            GN_DX9_CHECK_R( dev.CreateDepthStencilSurface(
                rt0Desc.Width,
                rt0Desc.Height,
                DEFAULT_DEPTH_FORMAT, // TODO: enumerate appropriate depth buffer format.
                rt0Desc.MultiSampleType,
                rt0Desc.MultiSampleQuality,
                TRUE,
                &mAutoDepth, 0 ) );
            GN_DX9_CHECK( dev.SetDepthStencilSurface( mAutoDepth ) );
            GN_TRACE(sLogger)( "Create depth surface: width(%d), height(%d), format(%s), msaa(%d), quality(%d)",
                rt0Desc.Width, rt0Desc.Height,
                d3d9::d3dFormat2Str(DEFAULT_DEPTH_FORMAT),
                rt0Desc.MultiSampleType,
                rt0Desc.MultiSampleQuality );
        }
    }
    else if( *newSurf != oldDesc.zbuffer || forceRebind )
    {
        AutoComPtr<IDirect3DSurface9> surf;
        const D3D9Texture * tex = safeCast<const D3D9Texture*>(newSurf->texture);
        surf.attach( tex->getSurface( newSurf->face, newSurf->level ) );
        GN_DX9_CHECK( dev.SetDepthStencilSurface( surf ) );
    }

    GN_UNGUARD_SLOW;
}

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------

#endif
