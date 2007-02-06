#include "pch.h"
#include "d3d9Renderer.h"
#include "d3d9RenderTargetMgr.h"
#include "d3d9Texture.h"
#include "garnet/GNd3d9.h"

#if GN_XENON

static GN::Logger * sLogger = GN::getLogger("GN.gfx.rndr.D3D9");

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D9RTMgrXenon::init()
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::D3D9RTMgrXenon, () );

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9RTMgrXenon::quit()
{
    GN_GUARD;

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
void GN::gfx::D3D9RTMgrXenon::bind(
    const RenderTargetDesc & oldDesc,
    const RenderTargetDesc & newDesc,
    bool forceRebind,
    bool & needRebindViewport )
{
    //GN_GUARD_SLOW;

    GN_UNUSED_PARAM( oldDesc );
    GN_UNUSED_PARAM( newDesc );
    GN_UNUSED_PARAM( forceRebind );

    needRebindViewport = true;

    //GN_UNGUARD_SLOW;
}

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------

#endif
