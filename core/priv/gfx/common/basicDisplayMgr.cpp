#include "pch.h"
#include "basicRenderer.h"

// ****************************************************************************
// local function
// ****************************************************************************

//!
//! get current display depth. Return 0 if failed.
// ----------------------------------------------------------------------------
static uint32_t
sGetCurrentDisplayDepth( void * window )
{
    GN_GUARD;

#if GN_WINNT

    HDC hdc = ::GetDC( (HWND)window );
    if ( 0 == hdc )
    {
        GN_ERROR( "fail to get window DC!" );
        return 0;
    }
    uint32_t depth = GetDeviceCaps( hdc, BITSPIXEL );
    ::ReleaseDC( (HWND)window, hdc );
    return depth;

#else

    GN_WARN( "no implementation yet!" );
    return 32;

#endif

    GN_UNGUARD;
}

//!
//! get current display width and height
// ----------------------------------------------------------------------------
static bool
sGetCurrentDisplaySize( void * window, uint32_t & width, uint32_t & height )
{
    GN_GUARD;

#if GN_WINNT

    // get monitor information
    HMONITOR hmonitor = MonitorFromWindow( (HWND)window, MONITOR_DEFAULTTOPRIMARY );
    if( 0 == hmonitor )
    {
        GN_ERROR( "fail to get monitor handle!" );
        return false;
    }
    MONITORINFOEXA mi;
    mi.cbSize = sizeof(mi);
    GN_WIN_CHECK_RV( GetMonitorInfoA( hmonitor, &mi ), false );

    // get display mode
    width = (uint32_t) ( mi.rcMonitor.right - mi.rcMonitor.left );
    height = (uint32_t) (mi.rcMonitor.bottom - mi.rcMonitor.top );

#else

    GN_WARN( "no implementation yet!" );
    width = 640;
    height = 480;

#endif

    // success
    return true;

    GN_UNGUARD;
}

//!
//! get client size of specific window
// ----------------------------------------------------------------------------
static bool
sGetWindowClientSize( void * window, uint32_t & width, uint32_t & height )
{
    GN_GUARD;

#if GN_WINNT

    RECT rc;
    GN_WIN_CHECK_RV( GetClientRect( (HWND)window, &rc ), false );
    GN_ASSERT( rc.right >= rc.left && rc.bottom >= rc.left );
    width = (uint32_t)( rc.right - rc.left );
    height = (uint32_t)( rc.bottom - rc.top );
    return true;

#else

    GN_WARN( "not implemented!" );
    width = 640;
    height = 480;
    return true;

#endif

    GN_UNGUARD;
}

//
//
// ----------------------------------------------------------------------------
static bool
sSetupScreenDesc(
    GN::gfx::ScreenDesc & desc,
    const GN::gfx::DeviceSettings & ds )
{
    GN_GUARD;

    uint32_t w, h, d;

    // get current display depth
    d = sGetCurrentDisplayDepth( ds.renderWindow );
    if( 0 == d ) return false;

    // initiate screen descriptor
    desc.fullscreen = ds.fullscreen;
    if( ds.fullscreen )
    {
        if( !sGetCurrentDisplaySize(ds.renderWindow,w,h) ) return false;
        desc.width = (0==ds.fullscreenWidth) ? w : ds.fullscreenWidth;
        desc.height = (0==ds.fullscreenHeight) ? h : ds.fullscreenHeight;
        desc.depth = (0==ds.fullscreenDepth) ? d : ds.fullscreenDepth;
        desc.refrate = ds.fullscreenRefrate;
    }
    else
    {
        sGetWindowClientSize( ds.renderWindow, w, h );
        desc.width = (0==ds.windowedWidth) ? w : ds.windowedWidth;
        desc.height = (0==ds.windowedHeight) ? h : ds.windowedHeight;
        desc.depth = d;
        desc.refrate = 0;
    }

    // success
    return true;

    GN_UNGUARD;
}

// ****************************************************************************
// device data management
// ****************************************************************************

//
//
// ----------------------------------------------------------------------------
bool GN::gfx::BasicRenderer::dispDeviceCreate()
{
    GN_GUARD;

    _GN_RENDER_DEVICE_TRACE();

    GN_ASSERT( !isScreenDescOK() );

    ScreenDesc desc;
    if( !sSetupScreenDesc(desc,getDeviceSettings()) ) return false;
    setScreenDesc( desc );

    // successful
    return true;

    GN_UNGUARD;
}

//
//
// ----------------------------------------------------------------------------
bool GN::gfx::BasicRenderer::dispDeviceRestore()
{
    GN_GUARD;

    _GN_RENDER_DEVICE_TRACE();

    if( !isScreenDescOK() )
    {
        ScreenDesc desc;
        if( !sSetupScreenDesc(desc,getDeviceSettings()) ) return false;
        setScreenDesc( desc );
    }

    // successful
    return true;

    GN_UNGUARD;
}
