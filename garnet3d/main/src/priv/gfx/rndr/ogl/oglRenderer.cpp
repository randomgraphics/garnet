#include "pch.h"
#include "oglRenderer.h"

GN::Logger * GN::gfx::OGLRenderer::sLogger = GN::getLogger("GN.gfx.rndr.OGL");

// *****************************************************************************
// Global functions
// *****************************************************************************

#if GN_BUILD_STATIC
GN::gfx::Renderer * createOGLRenderer()
#else
extern "C" GN_EXPORT GN::gfx::Renderer * GNgfxCreateRenderer( const GN::gfx::RendererOptions & o )
#endif
{
    GN_GUARD;

    GN::AutoObjPtr<GN::gfx::OGLRenderer> p( new GN::gfx::OGLRenderer );
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
bool GN::gfx::OGLRenderer::init( const RendererOptions & o )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::OGLRenderer, (o) );

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
void GN::gfx::OGLRenderer::quit()
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
