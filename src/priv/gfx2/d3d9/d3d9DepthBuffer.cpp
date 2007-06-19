#include "pch.h"
#include "d3d9DepthBuffer.h"

//
//
// -----------------------------------------------------------------------------
GN::gfx2::D3D9DepthBuffer * GN::gfx2::D3D9DepthBuffer::sNewInstance(
    const SurfaceLayoutTemplate & templ,
    int                           access,
    const SurfaceCreationHints  & hints )
{
    GN_UNUSED_PARAM( templ );
    GN_UNUSED_PARAM( access );
    GN_UNUSED_PARAM( hints );
    GN_UNIMPL();
    return 0;
}
