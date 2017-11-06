#include "pch.h"
#include "basicGpuMsw.h"
#include "garnet/GNwin.h"

#if GN_WINPC

GN::gfx::BasicGpuMsw::WindowMap GN::gfx::BasicGpuMsw::msInstanceMap;

static GN::Logger * sLogger = GN::getLogger("GN.gfx.gpu.common");

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
    if( !dispInit() ) return failure();

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
//                         From BasicGpu
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
intptr_t GN::gfx::BasicGpuMsw::determineMonitor(const GpuOptions & go, intptr_t display)
{
    GN_UNUSED_PARAM(display);
    HMONITOR monitor;
    if( !::IsWindow( (HWND)go.parentWindow ) )
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
        monitor = ::MonitorFromWindow( (HWND)go.parentWindow, MONITOR_DEFAULTTONEAREST );
    }
    GN_ASSERT( monitor );
    return (intptr_t)monitor;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::BasicGpuMsw::getCurrentDisplayMode(DisplayMode & dm, intptr_t display, intptr_t monitor)
{
    GN_GUARD;

    GN_UNUSED_PARAM(display);

    MONITORINFOEXA mi;
    DEVMODEA windm;

    mi.cbSize = sizeof(mi);
    windm.dmSize = sizeof(windm);
    windm.dmDriverExtra = 0;

    GN_MSW_CHECK_RETURN( ::GetMonitorInfoA( (HMONITOR)monitor, &mi ), false );
    GN_MSW_CHECK_RETURN( ::EnumDisplaySettingsA( mi.szDevice, ENUM_CURRENT_SETTINGS, &windm ), false );

    dm.width = windm.dmPelsWidth;
    dm.height = windm.dmPelsHeight;
    dm.depth = windm.dmBitsPerPel;
    dm.refrate = windm.dmDisplayFrequency;

    // success
    return true;

    GN_UNGUARD;
}

// *****************************************************************************
//                         Private Methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::BasicGpuMsw::dispInit()
{
    msInstanceMap[getRenderWindow().getWindowHandle()] = this;

    // register a message hook to render window.
    mHook = ::SetWindowsHookEx( WH_CALLWNDPROC, &staticHookProc, 0, GetCurrentThreadId() );
    if( 0 == mHook )
    {
        GN_ERROR(sLogger)( "Fail to setup message hook : %s", getWin32LastErrorInfo() );
        return false;
    }

    return true;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::BasicGpuMsw::dispQuit()
{
    // delete hook
    if( mHook ) ::UnhookWindowsHookEx( mHook ), mHook = 0;

    msInstanceMap.remove(getRenderWindow().getWindowHandle());
}

//
//
// -----------------------------------------------------------------------------
LRESULT CALLBACK
GN::gfx::BasicGpuMsw::staticHookProc( int code, WPARAM wp, LPARAM lp )
{
    GN_GUARD;

    //GN_TRACE( "wnd=0x%X, msg=%s", wnd, win::msg2str(msg) );

    BasicGpuMsw ** pp = msInstanceMap.find( (intptr_t)((CWPSTRUCT*)lp)->hwnd );

    if( NULL != pp )
    {
        // trigger render window message signal.
        CWPSTRUCT * cwp = (CWPSTRUCT*)lp;
        GN_ASSERT( cwp && (*pp) );
        (*pp)->sigMessage( cwp->hwnd, cwp->message, cwp->wParam, cwp->lParam );
    }

    return ::CallNextHookEx( 0, code, wp, lp );

    GN_UNGUARD;
}

#endif
