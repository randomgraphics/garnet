#include "pch.h"
#include "basicRenderer.h"
#include <limits.h>
#include "ntRenderWindow.h"
#include "xRenderWindow.h"


// ****************************************************************************
// local function
// ****************************************************************************

//!
//! get current display depth. Return 0 if failed.
// ----------------------------------------------------------------------------
static uint32_t
sGetCurrentDisplayDepth( void * window )
{
#if GN_WINNT

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

#else

    GN_WARN( "unimplemented" );
    GN_UNUSED_PARAM( window );
    return 32;

#endif
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

    DispDesc desc;

    // (re)initialize render window.
    if( !mWindow.init(ds) ) return false;

    desc.monitorHandle = mWindow.getMonitor();
    desc.windowHandle = mWindow.getWindow();
    GN_ASSERT( desc.monitorHandle && desc.windowHandle );

    // get back buffer size
    if( ds.width && ds.height )
    {
        desc.width = ds.width;
        desc.height = ds.height;
    }
    else
    {
        uint32_t w, h;
        if( !mWindow.getClientSize( w, h ) ) return false;
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
        desc.depth = sGetCurrentDisplayDepth( mWindow.getWindow() );
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
