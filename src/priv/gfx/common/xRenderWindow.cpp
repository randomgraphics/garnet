#include "pch.h"
#include "xRenderWindow.h"

#if GN_POSIX

// *****************************************************************************
// local functions
// *****************************************************************************

static bool sIsWindow( void * disp, void * win )
{
    // TODO: use real X function to check window handle.
    return 0 != disp && 0 != win;
}

// *****************************************************************************
// public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::XRenderWindow::initExternalRenderWindow(
    void * display, void * externalWindow )
{
    GN_GUARD;

    quit();

    if( !initDisplay( display ) ) return false;

    if( !sIsWindow( display, externalWindow ) )
    {
        GNGFX_ERROR( "External render window is invalid!" );
        return false;
    }

    mWindow = (Window)externalWindow;
    mUseExternalWindow = true;

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::XRenderWindow::initInternalRenderWindow(
    void * display, void * parentWindow, uint32_t width, uint32_t height )
{
    GN_GUARD;

    GN_ASSERT( width > 0 && height > 0 );

    if( !initDisplay( display ) ) return false;

    GN_UNUSED_PARAM( parentWindow );

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::XRenderWindow::quit()
{
    GN_GUARD;

    // Destroy window
    if( mWindow && !mUseExternalWindow )
    {
        GN_ASSERT( mDisplay );
        XDestroyWindow( mDisplay, mWindow );
        mWindow = 0;
    }

    // close display
    if( mDisplay && !mUseExternalDisplay )
    {
        XCloseDisplay( mDisplay );
        mDisplay = 0;
    }

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::XRenderWindow::getClientSize( uint32_t & width , uint32_t & height ) const
{
    GN_GUARD;

    if( !sIsWindow( mDisplay, (void*)mWindow ) )
    {
        GNGFX_ERROR( "XRenderWindow is not initialized!" );
        return false;
    }

    XWindowAttributes attr;
    GN_X_CHECK_RV( XGetWindowAttributes( mDisplay, mWindow, &attr ), false );

    width = (uint32_t)attr.width;
    height = (uint32_t)attr.height;

    // success
    return true;

    GN_UNGUARD;
}

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::XRenderWindow::initDisplay( void * display )
{
    GN_GUARD;

    GN_ASSERT( 0 == mDisplay );

    if( 0 != display )
    {
        mUseExternalDisplay = true;
        mDisplay = (Display*)display;
    }
    else
    {
        // TODO: get display name from environment variable.
        mUseExternalDisplay = false;
        mDisplay = XOpenDisplay( "" );
        if( 0 == mDisplay )
        {
            GNGFX_ERROR( "Fail to open display" );
            return false;
        }
    }

    // success
    return true;

    GN_UNGUARD;
}

#endif // GN_POSIX
