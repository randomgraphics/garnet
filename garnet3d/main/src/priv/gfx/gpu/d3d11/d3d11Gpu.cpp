#include "pch.h"
#include "d3d11Gpu.h"

#if GN_MSVC

#pragma comment(lib, "d3d9.lib") // for PIX routines.
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#if GN_BUILD_DEBUG
//#pragma comment(lib, "d3dx10d.lib")
#else
//#pragma comment(lib, "d3dx10.lib")
#endif

#endif // GN_MSVC

static GN::Logger * sLogger = GN::getLogger("GN.gfx.gpu.D3D11");

// *****************************************************************************
// Global functions
// *****************************************************************************

bool gD3D11EnablePixPerf = true; // default is enabled

#if GN_BUILD_STATIC
GN::gfx::Gpu *
GNgfxCreateD3D11Gpu( const GN::gfx::GpuOptions & o )
#else
extern "C" GN_EXPORT GN::gfx::Gpu *
GNgfxCreateGpu( const GN::gfx::GpuOptions & o )
#endif
{
    GN_GUARD;

    GN::AutoObjPtr<GN::gfx::D3D11Gpu> p( new GN::gfx::D3D11Gpu );
    if( !p->init( o ) ) return 0;
    return p.detach();

    GN_UNGUARD;
}

// *****************************************************************************
// init/quit functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D11Gpu::init( const GN::gfx::GpuOptions & o )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::D3D11Gpu, ( o ) );

    // init sub-components
    if( !dispInit()     ) return failure();
    if( !capsInit()     ) return failure();
    if( !resourceInit() ) return failure();
    if( !contextInit()  ) return failure();
    if( !drawInit()     ) return failure();

    // successful
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D11Gpu::quit()
{
    GN_GUARD;

    drawQuit();
    contextQuit();
    resourceQuit();
    capsQuit();
    dispQuit();

    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D11Gpu::ReportLiveDeviceObjects()
{
    if( NULL == mD3D11Debug ) return;

    mD3D11Debug->ReportLiveDeviceObjects( D3D11_RLDO_DETAIL );
    mD3D11Debug->ReportLiveDeviceObjects( D3D11_RLDO_SUMMARY );
}
