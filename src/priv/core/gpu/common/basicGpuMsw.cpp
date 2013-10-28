#include "pch.h"
#include "basicGpuMsw.h"
#include "garnet/GNwin.h"

#if GN_WINPC

static GN::Logger * sLogger = GN::getLogger("GN.gfx.gpu.common");

//
//
// -----------------------------------------------------------------------------
static bool
sGetClientSize( HWND win, uint32 * width, uint32 * height )
{
    GN_GUARD;

    RECT rc;

    GN_MSW_CHECK_RETURN( ::GetClientRect( win, &rc ), false );

    if( width ) *width = (uint32)(rc.right - rc.left);

    if( height ) *height = (uint32)(rc.bottom - rc.top);

    return true;

    GN_UNGUARD;
}

///
/// Determine monitor handle that render window should stay in.
// ----------------------------------------------------------------------------
static intptr_t
sDetermineMonitorHandle( const GN::gfx::GpuOptions & ro )
{
    if( 0 == ro.monitorHandle )
    {
        HMONITOR monitor;
        if( !::IsWindow( (HWND)ro.parentWindow ) )
        {
            POINT pt = { LONG_MIN, LONG_MIN }; // Make sure primary monitor are returned.
            monitor = ::MonitorFromPoint( pt, MONITOR_DEFAULTTOPRIMARY );
            if( 0 == monitor )
            {
                GN_ERROR(sLogger)( "Fail to get primary monitor handle." );
                return 0;
            }
        }
        else
        {
            monitor = ::MonitorFromWindow( (HWND)ro.renderWindow, MONITOR_DEFAULTTONEAREST );
        }
        GN_ASSERT( monitor );
        return (intptr_t)monitor;
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
    const GN::gfx::GpuOptions & ro,
    GN::gfx::DisplayMode      & dm )
{
    GN_GUARD;

    // determine the monitor
    intptr_t monitor = sDetermineMonitorHandle( ro );
    if( 0 == monitor ) return false;

    MONITORINFOEXA mi;
    DEVMODEA windm;

    mi.cbSize = sizeof(mi);
    windm.dmSize = sizeof(windm);
    windm.dmDriverExtra = 0;

    GN_MSW_CHECK_RETURN( ::GetMonitorInfoA( (HMONITOR)monitor, &mi ), false );
    GN_MSW_CHECK_RETURN( ::EnumDisplaySettingsA( mi.szDevice, ENUM_CURRENT_SETTINGS, &windm ), false );

    GN_ASSERT( (uint32) ( mi.rcMonitor.right - mi.rcMonitor.left ) == windm.dmPelsWidth );
    GN_ASSERT( (uint32) (mi.rcMonitor.bottom - mi.rcMonitor.top ) == windm.dmPelsHeight );

    dm.width = windm.dmPelsWidth;
    dm.height = windm.dmPelsHeight;
    dm.depth = windm.dmBitsPerPel;
    dm.refrate = windm.dmDisplayFrequency;

    // success
    return true;

    GN_UNGUARD;
}

///
/// Determine render window size
// ----------------------------------------------------------------------------
static bool
sDetermineWindowSize(
    const GN::gfx::GpuOptions & ro,
    const GN::gfx::DisplayMode & currentDisplayMode,
    uint32 & width,
    uint32 & height )
{
    GN_GUARD;

    if( ro.useExternalWindow )
    {
        return sGetClientSize( (HWND)ro.renderWindow, &width, &height );
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

// *****************************************************************************
//                         BasicGpuMsw init / quit functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::BasicGpuMsw::init( const GpuOptions & o )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( BasicGpuMsw, (o) );

    // initialize sub-components one by one
    if( !dispInit(o) ) return failure();

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::BasicGpuMsw::quit()
{
    GN_GUARD;

    // shutdown sub-components in reverse sequence
    dispQuit();

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// from Gpu
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::BasicGpuMsw::processRenderWindowMessages( bool blockWhileMinimized )
{
    GN::win::processWindowMessages( mDispDesc.windowHandle, blockWhileMinimized );
}

// ****************************************************************************
// from BasicGpu
// ****************************************************************************

//
//
// ----------------------------------------------------------------------------
void
GN::gfx::BasicGpuMsw::handleRenderWindowSizeMove()
{
    mWindow.handleSizeMove();
}

// *****************************************************************************
// private function
// *****************************************************************************

//
//
// ----------------------------------------------------------------------------
bool GN::gfx::BasicGpuMsw::dispInit( const GpuOptions & ro )
{
    DispDesc desc;

    // determine monitor handle
    desc.monitorHandle = sDetermineMonitorHandle( ro );
    if( 0 == desc.monitorHandle ) return false;

    // setup display mode
    DisplayMode dm;
    if( !sGetCurrentDisplayMode( ro, dm ) ) return false;
    if( ro.fullscreen )
    {
        desc.width = (0==ro.displayMode.width) ? dm.width : ro.displayMode.width;
        desc.height = (0==ro.displayMode.height) ? dm.height : ro.displayMode.height;
        desc.depth = (0==ro.displayMode.depth) ? dm.depth : ro.displayMode.depth;
        desc.refrate = (0==ro.displayMode.refrate) ? 0 : ro.displayMode.refrate;
    }
    else
    {
        uint32 w, h;
        if( !sDetermineWindowSize( ro, dm, w, h ) ) return false;
        desc.width = ro.windowedWidth ? ro.windowedWidth : w;
        desc.height = ro.windowedHeight ? ro.windowedHeight : h;
        desc.depth = dm.depth;
        desc.refrate = 0;
    }
    GN_ASSERT( desc.width && desc.height && desc.depth );

    if( getOptions().fullscreen && !ro.fullscreen ) mWinProp.restore();
    if( ro.useExternalWindow )
    {
        if( !mWindow.initExternalWindow( this, ro.renderWindow ) ) return false;
    }
    else
    {
        if( !mWindow.initInternalWindow( this, ro.parentWindow, desc.monitorHandle, desc.width, desc.height ) ) return false;
    }
    if( !ro.fullscreen && !mWinProp.save( mWindow.getWindowHandle() ) ) return false;
    desc.displayHandle = 0;
    desc.windowHandle  = (intptr_t)mWindow.getWindowHandle();

    GN_ASSERT_EX(
        desc.windowHandle && desc.monitorHandle,
        str::format( "win(0x%X), monitor(0x%X)", desc.windowHandle, desc.monitorHandle ).rawptr() );

    // success
    mOptions = ro;
    mDispDesc = desc;
    return true;
}

//
//
// ----------------------------------------------------------------------------
void GN::gfx::BasicGpuMsw::dispQuit()
{
    mWindow.quit();
}

#endif
