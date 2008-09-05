#include "pch.h"
#include "renderWindowX11.h"
#include "basicRenderer.h"

#if GN_POSIX

#include "glew.h"
#include "glxew.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx.rndr.common.renderWindow.X11");

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
static bool sIsWindow( GN::HandleType disp, GN::HandleType win )
{
    // TODO: use real X function to check window handle.
    return 0 != disp && 0 != win;
}

//
//
// -----------------------------------------------------------------------------
Screen * sGetScreenStructureOfWindow( Display * disp, Window win )
{
    GN_GUARD;

    if( !disp || !win )
    {
        GN_ERROR(sLogger)( "invalid display or invalid window!" );
        return NULL;
    }

    XWindowAttributes attr;
    GN_X_CHECK_RV( XGetWindowAttributes( disp, win, &attr ), NULL );
    GN_ASSERT( attr.screen );
    return attr.screen;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
static Window sGetParentWindow( Display * disp, Window win )
{
    GN_GUARD;

    GN_ASSERT( disp && win );
    Window r, p, *c;
    unsigned int n;
    GN_X_CHECK_RV( XQueryTree( disp, win, &r, &p, &c, &n ), 0 );
    GN_ASSERT( p );
    return p;

    GN_UNGUARD;
}

// *****************************************************************************
// public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
int GN::gfx::getScreenNumberOfWindow( Display * disp, Window win )
{
    GN_GUARD;

    Screen * scr = sGetScreenStructureOfWindow( disp, win );
    if( 0 == scr ) return -1;

    int n = ScreenCount(disp);
    for( int i = 0; i < n; ++i )
    {
        if( scr == ScreenOfDisplay( disp, i ) ) return i;
    }

    return -1;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::RenderWindowX11::initExternalRenderWindow(
    HandleType display, HandleType externalWindow )
{
    GN_GUARD;

    if( !initDisplay( display ) ) return false;

    if( !sIsWindow( display, externalWindow ) )
    {
        GN_ERROR(sLogger)( "External render window is invalid!" );
        return false;
    }

    mWindow = (Window)externalWindow;
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
    HandleType display, HandleType parentWindow, HandleType monitor, UInt32 width, UInt32 height )
{
    GN_GUARD;

    GN_ASSERT( width > 0 && height > 0 );

    Display * oldDisplay = mDisplay;

    if( !initDisplay(display) ) return false;
    GN_ASSERT( mDisplay );

    if( 0 == parentWindow ) parentWindow = (void*)XDefaultRootWindow( mDisplay );

    // resize old window, if:
    // - mWindow is valid
    // - currently using internal window
    // - same display
    // - same parent
    if( mWindow &&
        !mUseExternalWindow &&
        mDisplay == oldDisplay &&
        parentWindow == (void*)sGetParentWindow( mDisplay, mWindow ) )
    {
        GN_X_CHECK( XResizeWindow( mDisplay, mWindow, width, height ) );
    }
    else
    {
        GN_ASSERT( mDisplay && (parentWindow || monitor) );

        int scr = -1;
        if( parentWindow )
        {
            scr = getScreenNumberOfWindow( mDisplay, (Window)parentWindow );
        }
        else if( monitor )
        {
            scr = (int)(intptr_t)monitor;
        }
        else
        {
            GN_UNEXPECTED();
        }
        if( scr < 0 ) return false;

        // Choose an appropriate visual
        static int attributeList[] = { GLX_RGBA, GLX_DOUBLEBUFFER, None };
        AutoXPtr<XVisualInfo> vi( glXChooseVisual( mDisplay, scr, attributeList ) );
        if( 0 == vi )
        {
            GN_ERROR(sLogger)( "Cannot find visual with desired attributes." );
            return false;
        }

        // create a colormap
        Colormap cmap = XCreateColormap( mDisplay, (Window)parentWindow, vi->visual, AllocNone );
        if( 0 == cmap )
        {
            GN_ERROR(sLogger)( "Cannot allocate colormap." );
            return false;
        }

        // set window attributes
        XSetWindowAttributes swa;
        swa.colormap = cmap;
        swa.event_mask = ExposureMask | StructureNotifyMask;
        swa.border_pixel = BlackPixel( mDisplay, scr );
        swa.background_pixel = BlackPixel( mDisplay, scr );

        // create the render window.
        mWindow = XCreateWindow(
            mDisplay, (Window)parentWindow,
            0, 0, width, height, // position and size
            0, // border
            vi->depth,
            InputOutput,
            vi->visual,
            CWColormap | CWEventMask | CWBorderPixel | CWBackPixel,
            &swa ); // background
        if( 0 == mWindow )
        {
            GN_ERROR(sLogger)( "XCreateSimpleWindow() failed." );
            return false;
        }

        GN_ASSERT( (Window)parentWindow == sGetParentWindow( mDisplay, mWindow ) );

        // map window
        GN_X_CHECK_RV( XSelectInput( mDisplay, mWindow, StructureNotifyMask ), false );
        XEvent e;
        XMapWindow( mDisplay, mWindow );
        for(;;)
        {
            XNextEvent( mDisplay, &e );
            if( e.type == MapNotify && e.xmap.window == mWindow ) break;
        }
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
void GN::gfx::RenderWindowX11::quit()
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
GN::HandleType GN::gfx::RenderWindowX11::getMonitor() const
{
    GN_GUARD;

    return sGetScreenStructureOfWindow( mDisplay, mWindow );

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::RenderWindowX11::getClientSize( UInt32 & width, UInt32 & height ) const
{
    GN_GUARD;

    if( !sIsWindow( mDisplay, (void*)mWindow ) )
    {
        GN_ERROR(sLogger)( "RenderWindowX11 is not initialized!" );
        return false;
    }

    XWindowAttributes attr;
    GN_X_CHECK_RV( XGetWindowAttributes( mDisplay, mWindow, &attr ), false );

    width = (UInt32)attr.width;
    height = (UInt32)attr.height;

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
bool GN::gfx::RenderWindowX11::initDisplay( HandleType display )
{
    GN_GUARD;

    GN_ASSERT( display );

    // reuse previous display if possible
    if( display == mDisplay )
    {
        return true;
    }

    // release old X resources.
    quit();

    GN_ASSERT( 0 == mDisplay );

    mDisplay = (Display*)display;

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
