#include "pch.h"
#include "xenonGpu.h"
#include "xenonResource.h"

#if GN_MSVC

#pragma comment( lib, "dxerr9.lib" )

#if GN_BUILD_DEBUG
#pragma comment(lib, "d3d9d.lib")
#pragma comment(lib, "d3dx9d.lib")
#elif GN_BUILD_PROFILE
#pragma comment(lib, "d3d9i.lib")
#pragma comment(lib, "d3dx9i.lib")
#else
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#endif

#endif

static GN::Logger * sLogger = GN::getLogger("GN.gfx.gpu.xenon");

// *****************************************************************************
// Global functions
// *****************************************************************************

#if GN_BUILD_STATIC
GN::gfx::Gpu *
GNgfxCreateXenonGpu( const GN::gfx::GpuOptions & o )
#else
extern "C" GN_EXPORT GN::gfx::Gpu *
GNgfxCreateGpu( const GN::gfx::GpuOptions & o )
#endif
{
    GN_GUARD;

    GN::AutoObjPtr<GN::gfx::XenonGpu> p( new GN::gfx::XenonGpu );
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
bool GN::gfx::XenonGpu::init( const GN::gfx::GpuOptions & o )
{
    GN_GUARD;

    GN_ASSERT( getCurrentThreadId() == mThreadId );

    PIXPERF_FUNCTION_EVENT();

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::XenonGpu, (o) );

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

    GN_ASSERT( getCurrentThreadId() == mThreadId );

    PIXPERF_FUNCTION_EVENT();

    drawQuit();
    contextQuit();
    resourceQuit();
    capsQuit();
    dispQuit();

    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// from Gpu
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::XenonGpu::dumpNextFrame( size_t startBatchIndex, size_t numBatches )
{
    GN_UNUSED_PARAM( startBatchIndex );
    GN_UNUSED_PARAM( numBatches );
    GN_WARN(sLogger)( "Xenon frame dump is not implemented. Use PIX please." );
}
