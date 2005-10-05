#include "pch.h"
#include "basicRenderer.h"
#include <limits.h>

#if GN_WINNT

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

    HDC hdc = ::GetDC( (HWND)window );
    if ( 0 == hdc )
    {
        GN_ERROR( "fail to get window DC!" );
        return 0;
    }
    uint32_t depth = GetDeviceCaps( hdc, BITSPIXEL );
    ::ReleaseDC( (HWND)window, hdc );
    return depth;

    GN_UNGUARD;
}

//!
//! get current display width and height
// ----------------------------------------------------------------------------
static bool
sGetMonitorSize( void * monitor, uint32_t & width, uint32_t & height )
{
    GN_GUARD;

    // get monitor information
    MONITORINFOEXA mi;
    mi.cbSize = sizeof(mi);
    GN_WIN_CHECK_RV( GetMonitorInfoA( (HMONITOR)monitor, &mi ), false );

    // get display mode
    width = (uint32_t) ( mi.rcMonitor.right - mi.rcMonitor.left );
    height = (uint32_t) (mi.rcMonitor.bottom - mi.rcMonitor.top );

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

    RECT rc;
    GN_WIN_CHECK_RV( GetClientRect( (HWND)window, &rc ), false );
    GN_ASSERT( rc.right >= rc.left && rc.bottom >= rc.left );
    width = (uint32_t)( rc.right - rc.left );
    height = (uint32_t)( rc.bottom - rc.top );
    return true;

    GN_UNGUARD;
}

// ****************************************************************************
// private functions
// ****************************************************************************

//
//
// ----------------------------------------------------------------------------
bool
GN::gfx::BasicRenderer::setupDispDesc( const DeviceSettings & ds )
{
    GN_GUARD;

    uint32_t w, h;

    DispDesc desc;

    // Note: ds.renderWindow and ds.parentWindow are actually same variable,
    // so here we can use one check for both of them.
    bool windowValid = !!::IsWindow((HWND)ds.renderWindow);

    // setup monitor
    if( 0 == ds.monitorHandle )
    {
        if( !windowValid )
        {
            // make sure pt is outside of any possible display monitor.
            POINT pt = { LONG_MIN, LONG_MIN };
            desc.monitorHandle = ::MonitorFromPoint( pt, MONITOR_DEFAULTTOPRIMARY );
            if( 0 == desc.monitorHandle )
            {
                GN_ERROR( "Fail to get primary monitor handle." );
                return false;
            }
        }
        else
        {
            desc.monitorHandle = ::MonitorFromWindow( (HWND)desc.windowHandle, MONITOR_DEFAULTTONEAREST );
            if( 0 == desc.monitorHandle )
            {
                GN_ERROR( "Fail to get monitor handle from render window handle." );
                return false;
            }
        }
    }
    else
    {
        desc.monitorHandle = ds.monitorHandle;
    }
    GN_ASSERT( desc.monitorHandle );

    // setup render window
    mWindow.quit(); // release old window
    if( ds.useExternalWindow )
    {
        if( !windowValid )
        {
            GNGFX_ERROR( "External render window handle must be valid." );
            return false;
        }
        desc.windowHandle = ds.renderWindow;
    }
    else
    {
        if( 0 == ds.width || 0 == ds.height )
        {
            if( ds.fullscreen )
            {
                if( !::sGetMonitorSize( desc.monitorHandle, w, h ) ) return false;
            }
            else if ( windowValid )
            {
                if( !::sGetWindowClientSize( ds.parentWindow, w, h ) ) return false;
            }
            else
            {
                // Both fullsreen and windowValid are false. So we have to use
                // default width and height
                w = 640;
                h = 480;
            }
            w = ds.width ? ds.width : w;
            h = ds.height ? ds.height : h;
        }
        else
        {
            w = ds.width;
            h = ds.height;
        }
        GN_ASSERT( w > 0 && h > 0 );
        if( !mWindow.init( ds.parentWindow, w, h ) ) return false;
        desc.windowHandle = mWindow.getHandle();
    }
    GN_ASSERT( desc.windowHandle );
    
    // setup screen size
    if( ds.width && ds.height )
    {
        desc.width = ds.width;
        desc.height = ds.height;
    }
    else if( ds.fullscreen )
    {
        if( !sGetMonitorSize( desc.windowHandle, w, h ) ) return false;
        desc.width = (0==ds.width) ? w : ds.width;
        desc.height = (0==ds.height) ? h : ds.height;
    }
    else
    {
        if( !sGetWindowClientSize( desc.windowHandle, w, h ) ) return false;
        desc.width = (0==ds.width) ? w : ds.width;
        desc.height = (0==ds.height) ? h : ds.height;
    }
    GN_ASSERT( desc.width > 0 && desc.height > 0 );

    // setup screen depth
    if( ds.depth && ds.fullscreen )
    {
        desc.depth = ds.depth;
    }
    else
    {
        desc.depth = sGetCurrentDisplayDepth( desc.windowHandle );
        if( 0 == desc.depth ) return false;
    }
    GN_ASSERT( desc.depth > 0 );

    // setup misc. parameters
    desc.fullscreen = ds.fullscreen;
    desc.refrate = ds.fullscreen ? ds.refrate : 0;
    desc.vsync = ds.vsync;
    desc.reference = ds.reference;
    desc.software = ds.software;
    desc.autoRestore = ds.autoRestore;

    // success
    setDispDesc( desc );
    return true;

    GN_UNGUARD;
}

#endif // GN_WINNT
