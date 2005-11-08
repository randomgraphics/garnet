#include "pch.h"

#if GN_POSIX

// *****************************************************************************
// local functions
// *****************************************************************************

enum
{
    //!
    //! Mask for all possible X events.
    //!
    XEVENT_MASK_ALL = (1L<<25)-1
};

//
//
// -----------------------------------------------------------------------------
static int sXErrorHandler( Display * d, XErrorEvent * e )
{
    GN_GUARD;

    static char buf[4096];
    XGetErrorText( d, e->error_code, buf, 4095 );

    GN_ERROR(
        "X error : %s"
        "   Major opcode of failed request:  %d\n"
        "   Minor opcode of failed request:  %d\n"
        "   Serial number of failed request:  %d\n"
        "   Resource ID:  0x%X",
        buf,
        e->request_code,
        e->minor_code,
        e->serial,
        e->resourceid );

    return 0;

    GN_UNGUARD;
}

// *****************************************************************************
// XWindow class
// *****************************************************************************

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

#if GN_DEBUG
    // Trun on synchronous behavior for debug build.
    XSynchronize( mDisplay, true );
#endif

    // update error handler
    XSetErrorHandler( &sXErrorHandler );

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
    GN_X_CHECK_RV( XSelectInput( mDisplay, mWindow, XEVENT_MASK_ALL ), false );

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
        XWindowEvent( mDisplay, mWindow, StructureNotifyMask, &e );
        if( type == e.type ) break;
    }

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::win::XWindow::processXEvents( bool /*blockOnMinimized*/ ) const
{
    GN_GUARD_SLOW;

    if( mEventHandler.empty() ) return;

    XEvent e;
    for(;;)
    {
        if( XCheckWindowEvent( mDisplay, mWindow, XEVENT_MASK_ALL, &e ) )
        {
            // call event handler
            mEventHandler( e );
        }
        else return; // Idle time
    }

    GN_UNGUARD_SLOW;
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
