#include "pch.h"
#include "mtgpu.h"

using namespace GN;
using namespace GN::gfx;

// *************************************************************************
// local functions
// *************************************************************************

static GN::Logger * sLogger = GN::getLogger("GN.gfx.util.gpu");

#define D3D_GPU_API     GpuAPI::D3D11
#define D3D_GPU_NAME    "D3D11"
#define DEFAULT_GPU_API GpuAPI::OGL;

// ***********************************************************************
// Public functions
// ***********************************************************************

//
//
// -------------------------------------------------------------------------
#if !GN_BUILD_HAS_OGL
GN_API Gpu * GN::gfx::createOGLGpu( const GpuOptions &, uint32 )
{
    GN_ERROR(sLogger)( "OpenGL renderer is not available." );
    return 0;
}
#endif

//
//
// -------------------------------------------------------------------------
#if !GN_BUILD_HAS_D3D11 && !GN_XBOX2
GN_API Gpu * GN::gfx::createD3DGpu( const GpuOptions &, uint32 )
{
    GN_ERROR(sLogger)( D3D_GPU_NAME " renderer is not available." );
    return 0;
}
#endif

//
//
// -------------------------------------------------------------------------
GN_API Gpu * GN::gfx::createGpu( const GpuOptions & inputOptions, uint32 creationFlags )
{
    GpuOptions ro = inputOptions;

    if( GpuAPI::AUTO == ro.api ) ro.api = DEFAULT_GPU_API;

    switch( ro.api )
    {
        case GpuAPI::OGL   : return createOGLGpu( ro, creationFlags );
        case GpuAPI::D3D11 : return createD3DGpu( ro, creationFlags );
        case GpuAPI::FAKE  : GN_UNIMPL(); return 0;
        default : GN_ERROR(sLogger)( "Invalid API(%d)", ro.api.toRawEnum() ); return 0;
    }
}
