#include "pch.h"
#include "windowMsw.h"

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::win::WindowMsw::init( const WindowCreationParams & )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::win::WindowMsw, () );

    // Do custom init here

    // success
    return selfOK();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::win::WindowMsw::quit()
{
    GN_GUARD;

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}