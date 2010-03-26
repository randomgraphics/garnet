#include "pch.h"
#include "renderWindowX11.h"
#include "basicGpu.h"

#if GN_POSIX

#include "glew.h"
#include "glxew.h"

static GN::Logger * sLogger = GN::GetLogger("GN.gfx.gpu.common.renderWindow.X11");

// *****************************************************************************
// local functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
static int sXErrorHandler( Display * d, XErrorEvent * e )
{
    GN_GUARD;

    static char buf[4096];
    XGetErrorText( d, e->error_code, buf, 4095 );

    GN_ERROR(sLogger)(
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

//
//
// -----------------------------------------------------------------------------
static bool sIsWindow( Display * disp, Window win )
{
    // TODO: use real X function to check window handle.
    return 0 != disp && 0 != win;
}

//
//
// -----------------------------------------------------------------------------
static int sGetScreenNumber( Display * disp, Screen * screen )
{
    GN_ASSERT( disp && screen );

    int n = ScreenCount(disp);
    for( int i = 0; i < n; ++i )
    {
        if( screen == ScreenOfDisplay( disp, i ) ) return i;
    }

    GN_ERROR(sLogger)( "Fail to get screen number out of screen pointer." );
    return -1;
}

// *****************************************************************************
// public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::RenderWindowX11::initExternalRenderWindow(
    Gpu     * gpu,
    Display * display,
    Window    externalWindow )
{
    GN_GUARD;

    mGpu = gpu;

    if( !initDisplay( display ) ) return false;

    if( !sIsWindow( display, externalWindow ) )
    {
        GN_ERROR(sLogger)( "External render window is invalid!" );
        return false;
    }

    mWindow = externalWindow;
    mUseExternalWindow = true;

    // success
    GN_ASSERT( mDisplay && mWindow );
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::RenderWindowX11::initInternalRenderWindow(
    Gpu * gpu,
    Display  * display,
    Window     parentWindow,
    Screen   * monitor,
    UInt32     width,
    UInt32     height )
{
    GN_GUARD;

    GN_ASSERT( width > 0 && height > 0 && display && monitor );

    // remember renderer pointer
    mGpu = gpu;

    // remember screen/monitor pointer
    mScreen = monitor;

    // initialize display
    if( !initDisplay(display) ) return false;

    // get screen number
    mScreenNumber = sGetScreenNumber( display, monitor );
    if( mScreenNumber < 0 ) return false;

    // Choose an appropriate visual
    static int attributeList[] = { GLX_RGBA, GLX_DOUBLEBUFFER, None };
    AutoXPtr<XVisualInfo> vi( glXChooseVisual( display, mScreenNumber, attributeList ) );
    if( 0 == vi )
    {
        GN_ERROR(sLogger)( "Cannot find visual with desired attributes." );
        return false;
    }

    // determine parent window
    if( 0 == parentWindow )
    {
        parentWindow = XDefaultRootWindow( display );
        if( 0 == parentWindow )
        {
            GN_ERROR(sLogger)( "Fail to get default root window." );
            return false;
        }
    }

    // create a colormap
    Colormap cmap = XCreateColormap( display, parentWindow, vi->visual, AllocNone );
    if( 0 == cmap )
    {
        GN_ERROR(sLogger)( "Cannot allocate colormap." );
        return false;
    }

    // set window attributes
    XSetWindowAttributes swa;
    swa.colormap = cmap;
    swa.event_mask = ExposureMask | StructureNotifyMask;
    swa.border_pixel = BlackPixel( display, mScreenNumber );
    swa.background_pixel = BlackPixel( display, mScreenNumber );

    // create the render window.
    mWindow = XCreateWindow(
        display,
        parentWindow,
        0, 0, width, height, // position and size
        0, // border
        vi->depth,
        InputOutput,
        vi->visual,
        CWColormap | CWEventMask | CWBorderPixel | CWBackPixel,
        &swa ); // background
    if( 0 == mWindow )
    {
        GN_ERROR(sLogger)( "XCreateWindow() failed." );
        return false;
    }

    // map window
    GN_X_CHECK_RETURN( XSelectInput( display, mWindow, StructureNotifyMask ), false );
    XEvent e;
    XMapWindow( display, mWindow );
    for(;;)
    {
        XNextEvent( display, &e );
        if( e.type == MapNotify && e.xmap.window == mWindow ) break;
    }

    mUseExternalWindow = false;

    // success
    GN_ASSERT( mDisplay && mWindow );
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::RenderWindowX11::Quit()
{
    GN_GUARD;

    // Destroy window
    if( mWindow && !mUseExternalWindow )
    {
        GN_ASSERT( mDisplay );
        XDestroyWindow( mDisplay, mWindow );
        mWindow = 0;
    }

    // clear display
    mDisplay = 0;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::RenderWindowX11::GetClientSize( UInt32 & width, UInt32 & height ) const
{
    GN_GUARD;

    GN_ASSERT( sIsWindow( mDisplay, mWindow ) );

    XWindowAttributes attr;
    GN_X_CHECK_RETURN( XGetWindowAttributes( mDisplay, mWindow, &attr ), false );

    width = (UInt32)attr.width;
    height = (UInt32)attr.height;

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::RenderWindowX11::handleSizeMove()
{
    GN_UNIMPL_WARNING();
}

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::RenderWindowX11::initDisplay( Display * display )
{
    GN_GUARD;

    // store display pointer
    mDisplay = display;

#if GN_BUILD_DEBUG
    // Trun on synchronous behavior for debug build.
    XSynchronize( mDisplay, true );
#endif

    // update error handler
    XSetErrorHandler( &sXErrorHandler );

    // success
    return true;

    GN_UNGUARD;
}

#endif // GN_POSIX
