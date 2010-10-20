#include "pch.h"
#include "d3d11Gpu.h"
#include <mtgpu.h>

#if GN_MSVC

#pragma comment(lib, "d3d9.lib") // for PIX routines.
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#if GN_ENABLE_DEBUG
//#pragma comment(lib, "d3dx10d.lib")
#else
//#pragma comment(lib, "d3dx10.lib")
#endif

#endif // GN_MSVC

static GN::Logger * sLogger = GN::getLogger("GN.gfx.gpu.D3D11");

// *****************************************************************************
// GPU creator
// *****************************************************************************

bool gD3D11EnablePixPerf = true; // default is enabled

//
//
// -----------------------------------------------------------------------------
static GN::gfx::Gpu * sCreateD3DGpuPrivate( const GN::gfx::GpuOptions & o, void * )
{
    GN_GUARD;

    GN::AutoObjPtr<GN::gfx::D3D11Gpu> p( new GN::gfx::D3D11Gpu );
    if( !p->init( o ) ) return 0;
    return p.detach();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
#if GN_BUILD_STATIC
GN::gfx::Gpu * GN::gfx::createD3DGpu( const GN::gfx::GpuOptions & o, uint32 creationFlags )
{
    GpuOptions lo = o;
    lo.api = GpuAPI::D3D11;
    if( 0 != (creationFlags & GPU_CREATION_MULTIPLE_THREADS) )
    {
        return createMultiThreadGpu( lo, sCreateD3DGpuPrivate, 0 );
    }
    else
    {
        return sCreateD3DGpuPrivate( lo, 0 );
    }
}
#endif

//
//
// -----------------------------------------------------------------------------
#if !GN_BUILD_STATIC
extern "C" GN_EXPORT GN::gfx::Gpu * GNgfxCreateGpu( const GN::gfx::GpuOptions & o )
{
    return sCreateD3DGpuPrivate( o, 0 );
}
#endif

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
