#include "pch.h"
#include "xenonRenderer.h"
#include "xenonResource.h"

#if GN_MSVC

#pragma comment( lib, "dxerr9.lib" )

#if GN_BUILD_DEBUG
#pragma comment(lib, "xenond.lib")
#pragma comment(lib, "d3dx9d.lib")
#else
#pragma comment(lib, "xenon.lib")
#pragma comment(lib, "d3dx9.lib")
#endif

#endif

static GN::Logger * sLogger = GN::getLogger("GN.gfx.rndr.Xenon");

// *****************************************************************************
// Global functions
// *****************************************************************************

#if GN_BUILD_STATIC
GN::gfx::Renderer *
GNgfxCreateXenonRenderer( const GN::gfx::RendererOptions & o )
#else
extern "C" GN_EXPORT GN::gfx::Renderer *
GNgfxCreateRenderer( const GN::gfx::RendererOptions & o )
#endif
{
    GN_GUARD;

    GN::AutoObjPtr<GN::gfx::XenonRenderer> p( new GN::gfx::XenonRenderer );
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
bool GN::gfx::XenonRenderer::init( const GN::gfx::RendererOptions & o )
{
    GN_GUARD;

    GN_ASSERT( getCurrentThreadId() == mThreadId );

    PIXPERF_FUNCTION_EVENT();

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::XenonRenderer, (o) );

    // init sub-components
    if( !dispInit()         ) return failure();
    if( !capsInit()         ) return failure();
    if( !resourceInit()     ) return failure();
    //if( !contextInit()      ) return failure();
    //if( !drawInit()         ) return failure();

    // successful
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::XenonRenderer::quit()
{
    GN_GUARD;

    GN_ASSERT( getCurrentThreadId() == mThreadId );

    PIXPERF_FUNCTION_EVENT();

    //drawQuit();
    //contextQuit();
    resourceQuit();
    capsQuit();
    dispQuit();

    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// from Renderer
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::XenonRenderer::dumpNextFrame( size_t startBatchIndex, size_t numBatches )
{
    GN_UNUSED_PARAM( startBatchIndex );
    GN_UNUSED_PARAM( numBatches );
    GN_WARN(sLogger)( "Xenon frame dump is not implemented. Use PIX please." );
}
