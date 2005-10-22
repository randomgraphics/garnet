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
GN::gfx::BasicRenderer::setupDispDesc( const UserOptions & uo )
{
    GN_GUARD;

    DispDesc desc;

    // (re)initialize render window.
    if( !mWindow.init(uo) ) return false;

    desc.windowHandle = mWindow.getWindow();
    desc.monitorHandle = mWindow.getMonitor();
    GN_ASSERT( desc.windowHandle && desc.monitorHandle );

    // get back buffer size
    if( uo.width && uo.height )
    {
        desc.width = uo.width;
        desc.height = uo.height;
    }
    else
    {
        uint32_t w, h;
        if( !mWindow.getClientSize( w, h ) ) return false;
        desc.width = (0==uo.width) ? w : uo.width;
        desc.height = (0==uo.height) ? h : uo.height;
    }
    GN_ASSERT( desc.width > 0 && desc.height > 0 );

    // setup screen depth
    if( uo.depth && uo.fullscreen )
    {
        desc.depth = uo.depth;
    }
    else
    {
        desc.depth = sGetCurrentDisplayDepth( mWindow.getWindow() );
        if( 0 == desc.depth ) return false;
    }
    GN_ASSERT( desc.depth > 0 );

    // setup misc. parameters
    desc.autoBackbufferResizing = uo.autoBackbufferResizing;
    desc.fullscreen = uo.fullscreen;
    desc.refrate = uo.fullscreen ? uo.refrate : 0;
    desc.vsync = uo.vsync;
    desc.reference = uo.reference;
    desc.software = uo.software;
    desc.autoRestore = uo.autoRestore;

    // success
    setDispDesc( desc );
    return true;

    GN_UNGUARD;
}
