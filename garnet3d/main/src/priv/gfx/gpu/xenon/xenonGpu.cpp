#include "pch.h"
#include "xenonGpu.h"
#include "xenonResource.h"
#include <mtgpu.h>

static GN::Logger * sLogger = GN::getLogger("GN.gfx.gpu.xenon");

// *****************************************************************************
// Global functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
static GN::gfx::Gpu * sCreateXenonGpuPrivate( const GN::gfx::GpuOptions & o, void * )
{
    GN_GUARD;

    GN::AutoObjPtr<GN::gfx::XenonGpu> p( new GN::gfx::XenonGpu );
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
        return createMultiThreadGpu( lo, sCreateXenonGpuPrivate, 0 );
    }
    else
    {
        return sCreateXenonGpuPrivate( lo, 0 );
    }
}
#endif

//
//
// -----------------------------------------------------------------------------
#if !GN_BUILD_STATIC
extern "C" GN_EXPORT GN::gfx::Gpu * GNgfxCreateGpu( const GN::gfx::GpuOptions & o )
{
    return sCreateXenonGpuPrivate( o, 0 );
}
#endif

// *****************************************************************************
// init/quit functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::XenonGpu::init( const GN::gfx::GpuOptions & o )
{
    GN_GUARD;

    PIXPERF_FUNCTION_EVENT();

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::XenonGpu, (o) );

    // get current thread
    mThread = Thread::sGetCurrentThread();
    if( 0 == mThread ) return failure();

    // init sub-components
    if( !dispInit()         ) return failure();
    if( !capsInit()         ) return failure();
    if( !resourceInit()     ) return failure();
    if( !contextInit()      ) return failure();
    if( !drawInit()         ) return failure();

    // successful
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::XenonGpu::quit()
{
    GN_GUARD;

    GN_ASSERT( isGpuThread() );

    PIXPERF_FUNCTION_EVENT();

    drawQuit();
    contextQuit();
    resourceQuit();
    capsQuit();
    dispQuit();

    mThread = 0;

    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// from Gpu
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::XenonGpu::debugDumpNextFrame( size_t startBatchIndex, size_t numBatches )
{
    GN_UNUSED_PARAM( startBatchIndex );
    GN_UNUSED_PARAM( numBatches );
    GN_WARN(sLogger)( "Xenon frame dump is not implemented. Use PIX please." );
}
