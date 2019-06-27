#include "pch.h"
#include "d3d11Gpu.h"
#include "../util/mtgpu.h"

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
GN_API GN::gfx::Gpu * GN::gfx::createD3DGpu( const GN::gfx::GpuOptions & o, uint32 creationFlags )
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
    GN_STDCLASS_INIT( o );

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

// *****************************************************************************
// Debug methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D11Gpu::debugDumpNextFrame( uint32 startBatchIndex, uint32 numBatches )
{
    GN_UNUSED_PARAM( startBatchIndex );
    GN_UNUSED_PARAM( numBatches );
    GN_TODO( "D3D11 frame dump is not implemented." );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D11Gpu::debugMarkBegin( const char * markerName )
{
    if( NULL != markerName && gD3D11EnablePixPerf )
    {
        size_t len = str::length( markerName );
        wchar_t * wcs = (wchar_t*)alloca( sizeof(wchar_t) * (len+1) );
        mbs2wcs( wcs, len+1, markerName, len );
        PIXPERF_BEGIN_EVENT_EX(mDeviceContext, 0xFFFFFFFF, wcs);
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D11Gpu::debugMarkEnd()
{
    if( gD3D11EnablePixPerf )
    {
        PIXPERF_END_EVENT(mDeviceContext);
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D11Gpu::debugMarkSet( const char * markerName )
{
    if( NULL != markerName && gD3D11EnablePixPerf )
    {
        size_t len = str::length( markerName );
        wchar_t * wcs = (wchar_t*)alloca( sizeof(wchar_t) * (len+1) );
        mbs2wcs( wcs, len+1, markerName, len );
        PIXPERF_SET_MARKER_EX(mDeviceContext, 0xFFFFFFFF, wcs);
    }
}
