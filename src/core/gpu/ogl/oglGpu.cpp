#include "pch.h"
#include "oglGpu.h"
#include "../util/mtgpu.h"

GN::Logger * GN::gfx::OGLGpu::sLogger = GN::getLogger("GN.gfx.gpu.OGL");

// *****************************************************************************
// GPU creator
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
static GN::gfx::Gpu * sCreateOGLGpuPrivate( const GN::gfx::GpuOptions & o, void * )
{
    GN_GUARD;

    GN::AutoObjPtr<GN::gfx::OGLGpu> p( new GN::gfx::OGLGpu );
    if( !p->init( o ) ) return 0;
    return p.detach();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN_API GN::gfx::Gpu * GN::gfx::createOGLGpu( const GN::gfx::GpuOptions & o, uint32 creationFlags )
{
    GpuOptions localOptions = o;
    localOptions.api = GpuAPI::OGL;

    if( 0 != (creationFlags & GPU_CREATION_MULTIPLE_THREADS) )
    {
        return createMultiThreadGpu( o, sCreateOGLGpuPrivate, 0 );
    }
    else
    {
        return sCreateOGLGpuPrivate( localOptions, 0 );
    }
}

// *****************************************************************************
// init/quit functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLGpu::init( const GpuOptions & o )
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
void GN::gfx::OGLGpu::quit()
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
