#include "pch.h"
#include "basicRenderer.h"
#include "renderWindowX11.h"
#include <limits.h>

#if GN_POSIX

static GN::Logger * sLogger = GN::getLogger("GN.gfx.rndr.common.BasicRenderer");

//
//
// -----------------------------------------------------------------------------
static bool
sGetClientSize( GN::HandleType disp, GN::HandleType win, UInt32 * width, UInt32 * height )
{
    GN_GUARD;

    XWindowAttributes attr;
    GN_X_CHECK_RV( XGetWindowAttributes( (Display*)disp, (Window)win, &attr ), false );
    if( width ) *width = (UInt32)attr.width;
    if( height ) *height = (UInt32)attr.height;
    return true;

    GN_UNGUARD;
}

// ****************************************************************************
// local function
// ****************************************************************************

///
/// Determine monitor handle that render window should stay in.
// ----------------------------------------------------------------------------
static GN::HandleType
sDetermineMonitorHandle( Display * defaultDisplay, const GN::gfx::RendererOptions & ro )
{
    if( 0 == ro.monitorHandle )
    {
        Display * disp = ro.displayHandle ? (Display*)ro.displayHandle : defaultDisplay;
        GN_ASSERT( disp );
        Screen * scr = DefaultScreenOfDisplay( disp );
        GN_ASSERT( scr );
        return (GN::HandleType)scr;
    }
    else
    {
        return ro.monitorHandle;
    }
}

///
/// get current display mode
// ----------------------------------------------------------------------------
static bool
sGetCurrentDisplayMode(
    Display * defaultDisplay,
    const GN::gfx::RendererOptions & ro,
    GN::gfx::DisplayMode & dm )
{
    GN_GUARD;

    // determine the monitor
    GN::HandleType monitor = sDetermineMonitorHandle( defaultDisplay, ro );
    if( 0 == monitor ) return false;

    if( (void*)1 == monitor )
    {
        GN_WARN(sLogger)( "No valid screen found. Use hard-coded display mode:: 640x480 32bits" );
        dm.width = 640;
        dm.height = 480;
        dm.depth = 32;
        dm.refrate = 0;
    }
    else
    {
        const Screen * scr = (Screen*)monitor;
        dm.width = WidthOfScreen( scr );
        dm.height = HeightOfScreen( scr );
        dm.depth = DefaultDepthOfScreen( scr );
        dm.refrate = 0;
    }

    // success
    return true;

    GN_UNGUARD;
}

///
/// Determine render window size
// ----------------------------------------------------------------------------
static bool
sDetermineWindowSize(
    const GN::gfx::RendererOptions & ro,
    const GN::gfx::DisplayMode & currentDisplayMode,
    UInt32 & width,
    UInt32 & height )
{
    GN_GUARD;

    if( ro.useExternalWindow )
    {
        return sGetClientSize( ro.displayHandle, ro.renderWindow, &width, &height );
    }
    else
    {
        if( ro.fullscreen )
        {
            // In fullscreen mode, default window size is determined by current display mode.
            const GN::gfx::DisplayMode & dm = ro.displayMode;
            width = dm.width ? dm.width : currentDisplayMode.width;
            height = dm.height ? dm.height : currentDisplayMode.height;
        }
        else
        {
            // In windowed mode, default window size is 640x480
            width = ro.windowedWidth ? ro.windowedWidth : 640;
            height = ro.windowedHeight ? ro.windowedHeight : 480;
        }

        // success
        return true;
    }

    GN_UNGUARD;
}

// ****************************************************************************
// init / quit
// ****************************************************************************

//
//
// ----------------------------------------------------------------------------
bool GN::gfx::BasicRenderer::dispInit( const RendererOptions & ro )
{
    // open default display
    StrA dispStr = getEnv("DISPLAY");
    mDefaultDisplay = XOpenDisplay( dispStr.cptr() );
    if( 0 == mDefaultDisplay )
    {
        GN_ERROR(sLogger)( "Fail to open display '%s'.", dispStr.cptr() );
        return false;
    }

    DispDesc desc;

    // determine monitor handle
    desc.monitorHandle = sDetermineMonitorHandle( mDefaultDisplay, ro );
    if( 0 == desc.monitorHandle ) return false;

    // setup display mode
    DisplayMode dm;
    if( !sGetCurrentDisplayMode( mDefaultDisplay, ro, dm ) ) return false;
    if( ro.fullscreen )
    {
        desc.width = (0==ro.displayMode.width) ? dm.width : ro.displayMode.width;
        desc.height = (0==ro.displayMode.height) ? dm.height : ro.displayMode.height;
        desc.depth = (0==ro.displayMode.depth) ? dm.depth : ro.displayMode.depth;
        desc.refrate = (0==ro.displayMode.refrate) ? 0 : ro.displayMode.refrate;
    }
    else
    {
        UInt32 w, h;
        if( !sDetermineWindowSize( ro, dm, w, h ) ) return false;
        desc.width = ro.windowedWidth ? ro.windowedWidth : w;
        desc.height = ro.windowedHeight ? ro.windowedHeight : h;
        desc.depth = dm.depth;
        desc.refrate = 0;
    }
    GN_ASSERT( desc.width && desc.height && desc.depth );

    HandleType disp = ( 0 == ro.displayHandle ) ? mDefaultDisplay : ro.displayHandle;
    GN_ASSERT( disp );

    if( ro.useExternalWindow )
    {
        if( !mWindow.initExternalRenderWindow( disp, ro.renderWindow ) ) return false;
    }
    else
    {
        if( !mWindow.initInternalRenderWindow( disp, ro.parentWindow, desc.monitorHandle, desc.width, desc.height ) ) return false;
    }
    desc.displayHandle = disp;
    desc.windowHandle = mWindow.getWindow();
    GN_ASSERT( desc.displayHandle );

    GN_ASSERT_EX(
        desc.windowHandle && desc.monitorHandle,
        strFormat( "win(0x%X), monitor(0x%X)", desc.windowHandle, desc.monitorHandle ).cptr() );

    // success
    mOptions = ro;
    mDispDesc = desc;
    return true;
}

//
//
// ----------------------------------------------------------------------------
void GN::gfx::BasicRenderer::dispQuit()
{
    mWindow.quit();

    // close default display
    if( mDefaultDisplay )
    {
        XCloseDisplay( mDefaultDisplay );
        mDefaultDisplay = 0;
    }
}

// ****************************************************************************
// protected functions
// ****************************************************************************

//
//
// ----------------------------------------------------------------------------
void
GN::gfx::BasicRenderer::handleRenderWindowSizeMove()
{
    GN_GUARD;

    // handle render window size move
    const RendererOptions & ro = getOptions();
    if( !ro.fullscreen && // only when we're in windowed mode
        mWindow.getSizeChangeFlag() )
    {
        HandleType monitor = mWindow.getMonitor();
        UInt32 width, height;
        mWindow.getClientSize( width, height );

        const DispDesc & dd = getDispDesc();

        if( dd.width != width || dd.height != height || dd.monitorHandle != monitor )
        {
            sigRendererWindowSizeMove( monitor, width, height );
        }
    }

    GN_UNGUARD;
}

#endif
