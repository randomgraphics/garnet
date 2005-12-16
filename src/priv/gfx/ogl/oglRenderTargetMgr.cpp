#include "pch.h"
#include "oglRenderer.h"

#if GN_MSVC
#pragma warning(disable:4100) // unused parameters
#endif

// *****************************************************************************
// device management
// *****************************************************************************

// *****************************************************************************
// from Renderer
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLRenderer::setRenderTarget(
    size_t index, const Texture * texture, TexFace face )
{
    GN_GUARD_SLOW;

    GN_UNIMPL_WARNING();

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLRenderer::setRenderDepth(
    const Texture * texture, TexFace face )
{
    GN_GUARD_SLOW;

    GN_UNIMPL_WARNING();

    GN_UNGUARD_SLOW;
}
