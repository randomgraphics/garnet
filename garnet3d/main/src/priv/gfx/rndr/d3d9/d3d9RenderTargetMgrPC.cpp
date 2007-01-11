#include "pch.h"
#include "d3d9Renderer.h"
#include "d3d9RenderTargetMgr.h"

#if GN_PC

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
    GN_UNUSED_PARAM( oldDesc );
    GN_UNUSED_PARAM( newDesc );
    GN_UNUSED_PARAM( forceRebind );
    GN_UNUSED_PARAM( needRebindViewport );
}

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------

#endif
