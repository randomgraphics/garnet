#include "pch.h"
#include "d3d9BackBuffer.h"

//
//
// -----------------------------------------------------------------------------
GN::gfx2::D3D9BackBuffer * GN::gfx2::D3D9BackBuffer::sNewInstance(
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
