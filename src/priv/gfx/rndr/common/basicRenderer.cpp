#include "pch.h"
#include "basicRenderer.h"

GN::Logger * GN::gfx::BasicRenderer::sLogger = GN::getLogger("GN.gfx.rndr.common");

void GN::gfx::rip( const char * msg, ... )
{
    GN_UNUSED_PARAM(msg);
}

// *****************************************************************************
//                         BasicRenderer init / quit functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::BasicRenderer::init( const RendererOptions & o )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( BasicRenderer, () );

    // initialize sub-components one by one
    if( !dispInit(o) ) return failure();

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::BasicRenderer::quit()
{
    GN_GUARD;

    // shutdown sub-components in reverse sequence
    dispQuit();

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::BasicRenderer::bindContext( const RendererContext & c )
{
    if( !bindContextImpl( c, false ) )
    {
        if( !bindContextImpl( mContext, true ) )
        {
            GN_RNDR_RIP( "fail to recover from context binding failure." );
        }
        return false;
    }
    mContext = c;
    return true;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::BasicRenderer::rebindContext()
{
    if( !bindContextImpl( mContext, true ) )
    {
        GN_RNDR_RIP( "fail to rebind current context." );
    }
}
