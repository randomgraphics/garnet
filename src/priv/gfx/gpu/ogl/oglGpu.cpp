#include "pch.h"
#include "oglGpu.h"

GN::Logger * GN::gfx::OGLGpu::sLogger = GN::GetLogger("GN.gfx.gpu.OGL");

// *****************************************************************************
// Global functions
// *****************************************************************************

#if GN_BUILD_STATIC
GN::gfx::Gpu * GNgfxCreateOGLGpu( const GN::gfx::GpuOptions & o )
#else
extern "C" GN_EXPORT GN::gfx::Gpu * GNgfxCreateGpu( const GN::gfx::GpuOptions & o )
#endif
{
    GN_GUARD;

    GN::AutoObjPtr<GN::gfx::OGLGpu> p( new GN::gfx::OGLGpu );
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
bool GN::gfx::OGLGpu::init( const GpuOptions & o )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::OGLGpu, (o) );

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
