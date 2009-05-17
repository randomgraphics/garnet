#include "pch.h"
#include "oglRenderer.h"

#if GN_MSWIN

static GN::Logger * sLogger = GN::getLogger("GN.gfx.rndr.OGL");

// ****************************************************************************
// local functions
// ****************************************************************************

//
//
// ------------------------------------------------------------------------
static int sChoosePixelFormat( HDC hdc )
{
    GN_GUARD;

    PIXELFORMATDESCRIPTOR pfd;

    DWORD required_flags = PFD_DRAW_TO_WINDOW |
        PFD_SUPPORT_OPENGL |
        PFD_DOUBLEBUFFER   ;

    // flags that can not exist
    DWORD xxx_flags = PFD_NEED_PALETTE; // we're aiming for a RGB device

    GN_VERBOSE(sLogger)( "Enumerating pixelformats..." );
    int num = DescribePixelFormat(hdc, 1, 0, 0);
    GN_VERBOSE(sLogger)( "%d pixelformats in total.", num );

    int candidates[4] =
    {
        0, // ICD
        0, // MCD
        0, // ???
        0, // software
    };

    for ( int i = 1; i <= num; i++ )
    {
        if (!DescribePixelFormat(hdc, i, sizeof(pfd), &pfd))
        {
            GN_ERROR(sLogger)( "can't get the description of the %dth pixelformat!", i );
            return 0;
        }

        // check pfd flags;
        if( (pfd.dwFlags & required_flags) != required_flags ) continue;
        if( (pfd.dwFlags & xxx_flags) != 0 ) continue;

        // check pixel type
        if( PFD_TYPE_RGBA != pfd.iPixelType ) continue;

        // check z-buffer
        if( 0 == pfd.cDepthBits ) continue;

        // check stencil buffer
        if( 0 == pfd.cStencilBits ) continue;

        // check acceleration flag
        if( PFD_GENERIC_ACCELERATED & pfd.dwFlags )
        {
            if( PFD_GENERIC_FORMAT & pfd.dwFlags )
            {
                // mixed device
                if( 0 == candidates[2] ) candidates[2] = i;
            }
            else
            {
                // MCD device
                if( 0 == candidates[1] ) candidates[1] = i;
            }
        }
        else if( PFD_GENERIC_FORMAT & pfd.dwFlags )
        {
            // software device
            if( 0 == candidates[3] ) candidates[3] = i;
        }
        else
        {
            // might be ICD device
            if( 0 == candidates[0] ) candidates[0] = i;
        }

        if( candidates[0] > 0 && candidates[1] > 0 && candidates[2] > 0 )
        {
            // no need to iterate more formats
            break;
        }
    }

    // prefer hardware than mixed, than software
    if( candidates[0] > 0 )
    {
        return candidates[0];
    }
    else if( candidates[1] > 0 )
    {
        return candidates[1];
    }
    else if( candidates[2] > 0 )
    {
        return candidates[2];
    }
    else if( candidates[3] > 0 )
    {
        return candidates[3];
    }

    // error
    GN_ERROR(sLogger)( "no appropriate pixelformat!" );
    return 0;

    GN_UNGUARD;
}



//
//
// ------------------------------------------------------------------------
static bool sSetupPixelFormat( HDC hdc )
{
    GN_GUARD;

    // 注：事实上pfd这个变量完全没有必要初始化，因为在程序中它只是用来接收
    //     SetupPixelFormat(..)函数所返回的信息用的。
    //     这段代码的主要目的是为了表明pfd中各个项的含义，以及它们通常情况
    //     下的取值
    PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR),  //  size of this pfd
            1,                     // version number
            PFD_GENERIC_ACCELERATED |
            PFD_DRAW_TO_WINDOW |   // support window
            PFD_SUPPORT_OPENGL |   // support OpenGL
            PFD_DOUBLEBUFFER,      // double buffered
            PFD_TYPE_RGBA,         // RGBA type
            32,                    // 24-bit color depth
            0, 0, 0, 0, 0, 0,      // color bits ignored
            0,                     // no alpha buffer
            0,                     // shift bit ignored
            0,                     // no accumulation buffer
            0, 0, 0, 0,            // accum bits ignored
            32,                    // 32-bit z-buffer
            0,                     // no stencil buffer
            0,                     // no auxiliary buffer
            PFD_MAIN_PLANE,        // main layer
            0,                     // reserved
            0, 0, 0                // layer masks ignored
    };

    // choose pixel format
    int n = sChoosePixelFormat( hdc );
    if( 0 == n ) return false;
    GN_VERBOSE(sLogger)( "select pixelformat #%d.", n );

    // Set the pixel format for the device context
    if (!SetPixelFormat(hdc, n, &pfd))
    {
        GN_ERROR(sLogger)( "SetPixelFormat failed!" );
        return false;
    }

    // success
    return true;

    GN_UNGUARD;
}

// *****************************************************************************
// device management
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLRenderer::dispInit()
{
    GN_GUARD;

    GN_ASSERT( !mRenderContext && !mDeviceContext );

    HWND hwnd = (HWND)getDispDesc().windowHandle;
    if( !::IsWindow(hwnd) )
    {
        GN_ERROR(sLogger)( "Invalid render window handle!" );
        return false;
    }

    GN_MSW_CHECK_RV( mDeviceContext = ::GetDC(hwnd), false );

    if( !sSetupPixelFormat(mDeviceContext) ) return false;

    GN_MSW_CHECK_RV(
        mRenderContext = ::wglCreateContext( mDeviceContext ),
        false );

    GN_MSW_CHECK_RV( ::wglMakeCurrent(mDeviceContext, mRenderContext), false );

    // init GLEW
    glewInit();

    const RendererOptions & ro = getOptions();

    // modify fullscreen render window properties
    if( ro.fullscreen )
    {
        // activate display mode
        if( !activateDisplayMode() ) return false;

        const DispDesc & dd = getDispDesc();

        HWND hwnd = (HWND)dd.windowHandle;
        HMONITOR hmonitor = (HMONITOR)dd.monitorHandle;

        // modify window style
        GN_MSW_CHECK( ::SetParent( hwnd, 0 ) );
        GN_MSW_CHECK( ::SetMenu( hwnd, 0 ) );
        GN_MSW_CHECK( ::SetWindowLong( hwnd, GWL_STYLE, WS_POPUP|WS_VISIBLE ) );
        if( ::IsIconic(hwnd) )
        {
            GN_MSW_CHECK( ::ShowWindow( hwnd, SW_SHOWNORMAL ) );
        }

        // get monitor information
        MONITORINFOEXA mi;
        mi.cbSize = sizeof(mi);
        GN_MSW_CHECK_RV( ::GetMonitorInfoA( hmonitor, &mi ), false );

        // move window to left-top of the monitor, and set it as TOPMOST window.
        GN_TRACE(sLogger)( "Move window to %d, %d", mi.rcWork.left,mi.rcWork.top );
        GN_MSW_CHECK( ::SetWindowPos(
            hwnd, HWND_TOPMOST,
            mi.rcWork.left, mi.rcWork.top,
            dd.width, dd.height,
            SWP_FRAMECHANGED | SWP_SHOWWINDOW ) );

        // trigger a redraw operation
        GN_MSW_CHECK( ::UpdateWindow( hwnd ) );
    }

    // setup message hook
    if( ro.autoRestore )
    {
        getRenderWindow().sigMessage.connect( this, &OGLRenderer::msgHook );
    }

    // set swap interval
    if( WGLEW_EXT_swap_control )
    {
        if( !wglSwapIntervalEXT( ro.vsync ) )
        {
            GN_WARN(sLogger)( "Fail to adjust SGI swap control" );
        }
    }

    // successful
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLRenderer::dispQuit()
{
    GN_GUARD;

    // remove message hook
    getRenderWindow().sigMessage.disconnect( this );

    // restore display mode
    restoreDisplayMode();

    if( mRenderContext )
    {
        ::wglMakeCurrent(0, 0);
        ::wglDeleteContext(mRenderContext);
        mRenderContext = 0;
    }

    if( mDeviceContext )
    {
        HWND hwnd = (HWND)getDispDesc().windowHandle;
        GN_ASSERT( ::IsWindow(hwnd) );
        ::ReleaseDC( hwnd, mDeviceContext );
        mDeviceContext = 0;
    }

    GN_UNGUARD;
}

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLRenderer::activateDisplayMode()
{
    GN_GUARD;

    // check for redundant activation
    if( mDisplayModeActivated ) return true;

    // only change display mode if we are in fullscreen mode
    if( !getOptions().fullscreen ) return true;

    const DispDesc & dd = getDispDesc();

    // ignore message hook during this function call
    ScopeBool ignoreHook(mIgnoreMsgHook);

    // get monitor information
    MONITORINFOEXA mi;
    mi.cbSize = sizeof(mi);
    GN_MSW_CHECK_RV( ::GetMonitorInfoA( (HMONITOR)dd.monitorHandle, &mi ), false );

    // change display mode
    DEVMODEA dm;
    memset(&dm, 0, sizeof(dm));
    dm.dmSize             = sizeof(dm);
    dm.dmPelsWidth        = dd.width;
    dm.dmPelsHeight       = dd.height;
    dm.dmBitsPerPel       = dd.depth;
    dm.dmFields           = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;
    if( dd.refrate != 0 )
    {
        dm.dmFields |= DM_DISPLAYFREQUENCY;
        dm.dmDisplayFrequency = dd.refrate;
    }

    if( DISP_CHANGE_SUCCESSFUL != ::ChangeDisplaySettingsExA(
        mi.szDevice,
        &dm,
        NULL,
        CDS_FULLSCREEN,
        NULL ) )
    {
        GN_ERROR(sLogger)( "Failed to change to specified full screen mode!" );
        return false;
    }
    mDisplayModeActivated = true;
    GN_INFO(sLogger)(
        "Fullscreen mode activated: width(%d), height(%d), depth(%d), refrate(%d).",
        dd.width, dd.height, dd.depth, dd.refrate );

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLRenderer::restoreDisplayMode()
{
    GN_GUARD;

    if( mDisplayModeActivated )
    {
        mDisplayModeActivated = false;

        // ignore message hook during this function call
        ScopeBool ignoreHook(mIgnoreMsgHook);

        // restore display mode
        if( DISP_CHANGE_SUCCESSFUL != ::ChangeDisplaySettings(0, 0) )
        {
            GN_ERROR(sLogger)( "Failed to restore display mode: %s!", getOSErrorInfo() );
        }

        GN_INFO(sLogger)( "Display mode restored." );
    }

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLRenderer::msgHook( HWND, UINT msg, WPARAM wp, LPARAM )
{
    GN_GUARD;

    //GN_TRACE( "Message(%s), wp(0x%X)", win::msg2str(msg), wp );

    if( !getOptions().fullscreen ) return;

    if( WM_ACTIVATEAPP == msg && !mIgnoreMsgHook )
    {
        if( wp )
        {
            activateDisplayMode();
            ::ShowWindowAsync( (HWND)getDispDesc().windowHandle, SW_NORMAL );
        }
        else
        {
            ::ShowWindowAsync( (HWND)getDispDesc().windowHandle, SW_MINIMIZE );
            restoreDisplayMode();
        }
    }

    GN_UNGUARD;
}

#endif // GN_MSWIN
