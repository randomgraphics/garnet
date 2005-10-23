#include "pch.h"
#include "basicRenderer.h"
#include <limits.h>
#include "ntRenderWindow.h"
#include "xRenderWindow.h"


// ****************************************************************************
// local function
// ****************************************************************************

//!
//! get current display mode. Return 0 if failed.
// ----------------------------------------------------------------------------
static bool
sGetCurrentDisplayMode( void * window, GN::gfx::DisplayMode & dm )
{
#if GN_WINNT

    GN_GUARD;

    HDC hdc = ::GetDC( (HWND)window );
    if ( 0 == hdc )
    {
        GN_ERROR( "fail to get window DC!" );
        return false;
    }

    dm.width = GetDeviceCaps( hdc, HORZRES );
    dm.height = GetDeviceCaps( hdc, VERTRES );
    dm.depth = GetDeviceCaps( hdc, BITSPIXEL );
    dm.refrate = GetDeviceCaps( hdc, VREFRESH );

    ::ReleaseDC( (HWND)window, hdc );

    // success
    return true;

    GN_UNGUARD;

#else

    GN_WARN( "unimplemented" );
    GN_UNUSED_PARAM( window );
    dm.width = 640;
    dm.height = 480;
    dm.depth = 32;
    dm.refrate = 0;
    return true;

#endif
}

// ****************************************************************************
// protected functions
// ****************************************************************************

//
//
// ----------------------------------------------------------------------------
bool
GN::gfx::BasicRenderer::processUserOptions( const UserOptions & uo )
{
    GN_GUARD;

#if GN_WINNT
    // Restore render window properties, if switching back from fullscreen mode
    if( getUserOptions().fullscreen && !uo.fullscreen ) mWinProp.restore();
#endif

    DispDesc desc;

    // (re)initialize render window.
    if( !mWindow.init(uo) ) return false;

#if GN_WINNT
    // Save new render window properties, for windowed mode only
    if( !uo.fullscreen && !mWinProp.save( mWindow.getWindow() ) ) return false;
#endif

    desc.windowHandle = mWindow.getWindow();
    desc.monitorHandle = mWindow.getMonitor();
    GN_ASSERT( desc.windowHandle && desc.monitorHandle );

    // get current display properties
    DisplayMode dm;
    if( !sGetCurrentDisplayMode( desc.windowHandle, dm ) ) return false;
    GN_ASSERT( dm.width && dm.height && dm.depth );

    // setup display mode
    if( uo.fullscreen )
    {
        desc.width = (0==uo.displayMode.width) ? dm.width : uo.displayMode.width;
        desc.height = (0==uo.displayMode.height) ? dm.height : uo.displayMode.height;
        desc.depth = (0==uo.displayMode.depth) ? dm.depth : uo.displayMode.depth;
        desc.refrate = (0==uo.displayMode.refrate) ? dm.refrate : uo.displayMode.refrate;
    }
    else
    {
        uint32_t w, h;
        if( !mWindow.getClientSize( w, h ) ) return false;
        desc.width = (0==uo.windowedWidth) ? w : uo.windowedWidth;
        desc.height = (0==uo.windowedHeight) ? h : uo.windowedHeight;
        desc.depth = dm.depth;
        desc.refrate = 0;
    }
    GN_ASSERT( desc.width && desc.height && desc.depth );

    // success
    setUserOptions( uo );
    setDispDesc( desc );
    return true;

    GN_UNGUARD;
}

//
//
// ----------------------------------------------------------------------------
bool
GN::gfx::BasicRenderer::handleRenderWindowSizeMove()
{
    GN_GUARD;

    // handle render window size move
    const UserOptions & uo = getUserOptions();
    if( !uo.fullscreen && // only when we're in windowed mode
        uo.autoBackbufferResizing &&
        mWindow.getSizeChangeFlag() )
    {
        UserOptions newOptions = uo;
        newOptions.monitorHandle = mWindow.getMonitor();
        mWindow.getClientSize( newOptions.windowedWidth, newOptions.windowedHeight );
        if( !changeDevice( newOptions, false ) )
        {
            GNGFX_FATAL( "Fail to respond to render window size and position change!" );
            return false;
        }
    }

    // success
    return true;

    GN_UNGUARD;
}
