#include "pch.h"
#include "xenonGpu.h"
#include "xenonRenderTargetMgr.h"
#include "xenonTexture.h"

static GN::Logger * sLogger = GN::GetLogger("GN.gfx.gpu.xenon");

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::XenonRenderTargetManager::Init()
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::XenonRenderTargetManager, () );

    // success
    return Success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::XenonRenderTargetManager::Quit()
{
    GN_GUARD;

    // standard Quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::XenonRenderTargetManager::bind(
    const XenonRenderTargetDesc & oldDesc,
    const XenonRenderTargetDesc & newDesc,
    bool                          skipDirtyCheck,
    bool                        & needRebindViewport )
{
    GN_UNUSED_PARAM( oldDesc );
    GN_UNUSED_PARAM( newDesc );
    GN_UNUSED_PARAM( skipDirtyCheck );
    GN_UNIMPL_WARNING();

    needRebindViewport = true;
}

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
