#include "pch.h"
#include "garnet/win/xWindow.h"

#if GN_POSIX

//
//
// -----------------------------------------------------------------------------
bool GN::win::XWindow::create( const CreateParam & cp )
{
    GN_GUARD;

    destroy();

    // open display
    mDisplay = XOpenDisplay( cp.display );
    if( 0 == mDisplay )
    {
        GNWIN_ERROR( "Fail to open display '%s'!", StrA(cp.display).cstr() );
        return false;
    }

    // Get some colors
    int blackColor = BlackPixel( mDisplay, DefaultScreen(mDisplay) );

    // create window
    Window parent = cp.parent ? cp.parent : DefaultRootWindow(mDisplay);
    mWindow = XCreateSimpleWindow(
        mDisplay, parent,
        cp.left, cp.top, cp.clientWidth, cp.clientHeight, // size
        0, // border
        blackColor, blackColor );
    if( 0 == mWindow )
    {
        GNWIN_ERROR( "XCreateSimpleWindow() failed!" );
        return false;
    }

    // Select inputs
    long mask = 0xFFFFFFFF; // Receive all kinds of events.
    GN_X_CHECK_RV( XSelectInput( mDisplay, mWindow, mask ), false );

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::win::XWindow::destroy()
{
    GN_GUARD;

    // close display
    if( mDisplay ) XCloseDisplay( mDisplay ), mDisplay = 0;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::win::XWindow::showWindow( bool show ) const
{
    GN_GUARD;

    if( !mDisplay || !mWindow )
    {
        GNWIN_ERROR( "Not initialized!" );
        return;
    }

    int type;
    if( show )
    {
        XMapWindow( mDisplay, mWindow );
        type = MapNotify;
    }
    else
    {
        XUnmapWindow( mDisplay, mWindow );
        type = UnmapNotify;
    }

    // Wait for map/unmap notification
    XEvent e;
    for(;;)
    {
        XNextEvent( mDisplay, &e );
        if( type == e.type ) break;
        sleep(0);
    }

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::win::XWindow::getClientSize( uint32_t & width, uint32_t & height ) const
{
    GN_GUARD;

    XWindowAttributes attr;
    GN_X_CHECK_RV( XGetWindowAttributes( mDisplay, mWindow, &attr ), false );

    width = attr.width;
    height = attr.height;

    // success
    return true;

    GN_UNGUARD;
}

#endif // GN_POSIX
