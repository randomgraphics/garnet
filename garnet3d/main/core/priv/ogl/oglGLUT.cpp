#include "pch.h"
#include "garnet/GNogl.h"

#if !GN_WINNT

#include "../core/core.cpp"

GN_IMPLEMENT_SINGLETON( GN::ogl::OGL );

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::ogl::OGL::init( const OGLInitParams & params )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( OGL, () );

    mInitParams = params;
    mClosed = true;

    // success
    return selfOK();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::ogl::OGL::quit()
{
    GN_GUARD;

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// Public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::ogl::OGL::present()
{
    GN_GUARD_SLOW;

    GN_ASSERT( selfOK() );

    // success
    return true;

    GN_UNGUARD_SLOW;
}

#endif // !GN_WINNT
