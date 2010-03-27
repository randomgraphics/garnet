#include "pch.h"
#include "xenonGpu.h"
#include "xenonResource.h"

#if GN_MSVC

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

static GN::Logger * sLogger = GN::GetLogger("GN.gfx.gpu.xenon");

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
    if( !p->Init( o ) ) return 0;
    return p.Detach();

    GN_UNGUARD;
}

// *****************************************************************************
// init/quit functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::XenonGpu::Init( const GN::gfx::GpuOptions & o )
{
    GN_GUARD;

    GN_ASSERT( GetCurrentThreadIdentifier() == mThreadId );

    PIXPERF_FUNCTION_EVENT();

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::XenonGpu, (o) );

    // init sub-components
    if( !dispInit()         ) return Failure();
    if( !capsInit()         ) return Failure();
    if( !resourceInit()     ) return Failure();
    if( !contextInit()      ) return Failure();
    if( !drawInit()         ) return Failure();

    // successful
    return Success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::XenonGpu::Quit()
{
    GN_GUARD;

    GN_ASSERT( GetCurrentThreadIdentifier() == mThreadId );

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
void GN::gfx::XenonGpu::DumpNextFrame( size_t startBatchIndex, size_t numBatches )
{
    GN_UNUSED_PARAM( startBatchIndex );
    GN_UNUSED_PARAM( numBatches );
    GN_WARN(sLogger)( "Xenon frame dump is not implemented. Use PIX please." );
}
