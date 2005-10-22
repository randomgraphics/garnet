#include "pch.h"
#include "oglRenderer.h"

#if GN_WINNT

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

    GNOGL_INFO( "Enumerating pixelformats..." );
    int num = DescribePixelFormat(hdc, 1, 0, 0);
    GNOGL_INFO( "%d pixelformats in total.", num );

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
            GNOGL_ERROR( "can't get the describtion of the %dth pixelformat!", i );
            return 0;
        }

        // check pfd flags;
        if( (pfd.dwFlags & required_flags) != required_flags ) continue;
        if( (pfd.dwFlags & xxx_flags) != 0 ) continue;

        // check pixel type
        if( PFD_TYPE_RGBA != pfd.iPixelType ) continue;

        // check z-buffer
        if( 0 == pfd.cDepthBits ) continue;

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
        GNOGL_INFO( "select pixelformat #%d (ICD).", candidates[0] );
        return candidates[0];
    }
    else if( candidates[1] > 0 )
    {
        GNOGL_INFO( "select pixelformat #%d (MCD).", candidates[1] );
        return candidates[1];
    }
    else if( candidates[2] > 0 )
    {
        GNOGL_INFO( "select pixelformat #%d(what's this?).", candidates[2] );
        return candidates[2];
    }
    else if( candidates[3] > 0 )
    {
        GNOGL_INFO( "select pixelformat %d(Software).", candidates[3] );
        return candidates[3];
    }

    // error
    GNOGL_ERROR( "no appropriate pixelformat!" );
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

    // Set the pixel format for the device context
    if (!SetPixelFormat(hdc, n, &pfd))
    {
        GNOGL_ERROR( "SetPixelFormat failed!" );
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
bool GN::gfx::OGLRenderer::dispDeviceCreate()
{
    GN_GUARD;

    _GN_RENDER_DEVICE_TRACE();

    GN_ASSERT( !mDispOK && !mRenderContext && !mDeviceContext );

    HWND hwnd = (HWND)getDispDesc().windowHandle;
    if( !::IsWindow(hwnd) )
    {
        GNOGL_ERROR( "Invalid render window handle!" );
        return false;
    }

    GN_WIN_CHECK_RV( mDeviceContext = ::GetDC(hwnd), false );

    if( !sSetupPixelFormat(mDeviceContext) ) return false;

    GN_WIN_CHECK_RV(
        mRenderContext = ::wglCreateContext( mDeviceContext ),
        false );

    GN_WIN_CHECK_RV( ::wglMakeCurrent(mDeviceContext, mRenderContext), false );

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLRenderer::dispDeviceRestore()
{
    GN_GUARD;

    _GN_RENDER_DEVICE_TRACE();

    GN_ASSERT( !mDispOK && mRenderContext && mDeviceContext );

    // activate displaymode
    if( !activateDisplayMode() ) return false;

    // setup message hook
    if( getUserOptions().autoRestore )
    {
        mWindow.sigMessage.connect( this, &OGLRenderer::msgHook );
    }

    // successful
    mDispOK = true;
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLRenderer::dispDeviceDispose()
{
    _GN_RENDER_DEVICE_TRACE();
    mDispOK = false;

    // remove message hook
    mWindow.sigMessage.disconnect( this );

    // restore display mode
    restoreDisplayMode();
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLRenderer::dispDeviceDestroy()
{
    GN_GUARD;

    _GN_RENDER_DEVICE_TRACE();

    GN_ASSERT( !mDispOK );

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

    // check for redundent activation
    if( mDisplayModeActivated ) return true;

    // only change display mode if we are in fullscreen mode
    if( !getUserOptions().fullscreen ) return true;

    const DispDesc & dd = getDispDesc();

    // ignore message hook during this function call
    ScopeBool ignoreHook(mIgnoreMsgHook);

    HWND hwnd = (HWND)dd.windowHandle;
    HMONITOR hmonitor = (HMONITOR)dd.monitorHandle;
    GN_ASSERT( hwnd && hmonitor );

    // get monitor information
    MONITORINFOEXA mi;
    mi.cbSize = sizeof(mi);
    GN_WIN_CHECK_RV( ::GetMonitorInfoA( hmonitor, &mi ), false );

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
        GNOGL_ERROR( "failed to change to specified fullscr mode!" );
        return false;
    }
    mDisplayModeActivated = true;
    GNOGL_INFO( "Fullscreen mode activated." );

    // update monitor information
    GN_WIN_CHECK_RV( ::GetMonitorInfoA( hmonitor, &mi ), false );

    // modify window style
    GN_WIN_CHECK( ::SetParent( hwnd, 0 ) );
    GN_WIN_CHECK( ::SetMenu( hwnd, 0 ) );
    GN_WIN_CHECK( ::SetWindowLong( hwnd, GWL_STYLE, WS_POPUP|WS_VISIBLE ) );
    if( ::IsIconic(hwnd) )
    {
        GN_WIN_CHECK( ::ShowWindow( hwnd, SW_SHOWNORMAL ) );
    }
    GNOGL_INFO( "Move window to %d, %d", mi.rcWork.left,mi.rcWork.top );
    GN_WIN_CHECK( ::SetWindowPos(
        hwnd, HWND_TOPMOST,
        mi.rcWork.left, mi.rcWork.top,
        dd.width, dd.height,
        SWP_FRAMECHANGED | SWP_SHOWWINDOW ) );

    // trigger a redraw operation
    GN_WIN_CHECK( ::UpdateWindow( hwnd ) );

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
            GNOGL_ERROR( "Failed to restore display mode: %s!", getOSErrorInfo() );
        }

        GNOGL_INFO( "Display mode restored." );
    }

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLRenderer::msgHook( HWND, UINT msg, WPARAM wp, LPARAM )
{
    GN_GUARD;

    if( WM_ACTIVATEAPP == msg && !mIgnoreMsgHook )
    {
        if( wp )
        {
            activateDisplayMode();
        }
        else
        {
            restoreDisplayMode();
        }
    }

    GN_UNGUARD;
}

#endif // GN_WINNT
