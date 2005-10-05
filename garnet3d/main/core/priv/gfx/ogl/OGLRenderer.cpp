#include "pch.h"
#include "OGLRenderer.h"

extern "C" GN_EXPORT GN::gfx::Renderer *
GNgfxCreateRenderer( const GN::gfx::DeviceSettings & )
{
    return 0;
}
