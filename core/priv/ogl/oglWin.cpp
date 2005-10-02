#include "pch.h"
#include "garnet/GNogl.h"

#if GN_WINNT

#include "../core/core.cpp"

#if GN_MSVC
#pragma comment( lib, "opengl32.lib" )
#pragma comment( lib, "glu32.lib" )
#endif

//
//
// ----------------------------------------------------------------------------
static int sChoosePixelFormat( HDC hdc )
{
    GN_GUARD;

    PIXELFORMATDESCRIPTOR pfd;

    DWORD requiredFlags =
        PFD_DRAW_TO_WINDOW |
        PFD_SUPPORT_OPENGL |
        PFD_DOUBLEBUFFER   ;

    // flags that can not exist
    DWORD xxx_flags = PFD_NEED_PALETTE; // we're aiming for a RGB device

    // Get number of available pixel formats.
    int num;
    GN_WIN_CHECK_RV( num=DescribePixelFormat(hdc, 1, 0, 0), 0 );
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
        GN_WIN_CHECK_RV( DescribePixelFormat(hdc, i, sizeof(pfd), &pfd), 0 );

        // check pfd flags;
        if( (pfd.dwFlags & requiredFlags) != requiredFlags ) continue;
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
        GNOGL_INFO( "select ixelformat #%d (ICD).", candidates[0] );
        return candidates[0];
    }
    else if( candidates[1] > 0 )
    {
        GNOGL_INFO( "select pixelformat #%d (MCD).", candidates[1] );
        return candidates[1];
    }
    else if( candidates[2] > 0 )
    {
        GNOGL_INFO( "select pixelformat #%d(unknown).", candidates[2] );
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
// ----------------------------------------------------------------------------
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
    GN_WIN_CHECK_RV( SetPixelFormat(hdc, n, &pfd), false );

    // success
    return true;

    GN_UNGUARD;
}

GN_IMPLEMENT_SINGLETON( GN::ogl::OGL );

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::ogl::OGL::init( const OGLInitParams & params )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( OGL, () );

    mInitParams = params;

    if( !createWindow() ) { quit(); return selfOK(); }
    if( !createOGL() ) { quit(); return selfOK(); }

    // success
    return selfOK();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::ogl::OGL::quit()
{
    GN_GUARD;

    if( mRC )
    {
        wglMakeCurrent(0, 0);
        wglDeleteContext(mRC);
        mRC = 0;
    }

    if( mDC )
    {
        GN_ASSERT( IsWindow(mWindow) );
        ReleaseDC( mWindow, mDC );
        mDC = 0;
    }

    restoreDisplayMode();

    if( mWindow )
    {
        DestroyWindow( mWindow );
        mWindow = 0;
    }

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// Public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::ogl::OGL::present()
{
    GN_GUARD_SLOW;

    GN_ASSERT( selfOK() );

    processWindowMessages();

    // respond to window size-move
    GN_ASSERT( !mMinimized );

    if( mSizeChanged )
    {
        // TODO: handle size changing.
    }

    // swap buffer
    GNOGL_CHECK( wglSwapLayerBuffers( mDC, WGL_SWAP_MAIN_PLANE ) );

    // success
    return true;

    GN_UNGUARD_SLOW;
}

// *****************************************************************************
// Private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::ogl::OGL::createWindow()
{
    GN_GUARD;

#if GN_XENON
	mWindow = 0;
#else
    // register window class
    WNDCLASSEXA wcex;
    wcex.cbSize         = sizeof(WNDCLASSEX);
    wcex.style          = 0;//CS_NOCLOSE;
    wcex.lpfnWndProc    = (WNDPROC)&staticProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = (HINSTANCE)GetModuleHandle(0);
    wcex.hIcon          = LoadIcon (0, IDI_APPLICATION);
    wcex.hCursor        = LoadCursor (0,IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = 0;
    wcex.lpszClassName  = "GarnetOGLAppWindowClass";
    wcex.hIconSm        = LoadIcon(0, IDI_APPLICATION);
    if( 0 == RegisterClassExA(&wcex) )
    {
        GNOGL_ERROR( "fail to register window class, %s!", getOSErrorInfo() );
        return false;
    }

    // calculate window size
    DWORD style = WS_OVERLAPPEDWINDOW;
    RECT rc = { 0, 0, mInitParams.width, mInitParams.height };
    AdjustWindowRect( &rc, style, 0 );

    // create window
    mWindow = CreateWindowA(
        "GarnetOGLAppWindowClass",
        mInitParams.winTitle,
        style,
        CW_USEDEFAULT, CW_USEDEFAULT,
        rc.right - rc.left, rc.bottom - rc.top,
        0, // no parent
        0, // no menu
        (HINSTANCE)GetModuleHandle(0), NULL );
    if( 0 == mWindow )
    {
        GNOGL_ERROR( "fail to create window, %s!", getOSErrorInfo() );
        return false;
    }

    // show the window
    if( mInitParams.showWindow )
    {
        ShowWindow( mWindow, SW_NORMAL );
        UpdateWindow( mWindow );
    }
#endif

    mMinimized = false;
    mClosed = false;
    mInsideSizeMove = false;
    mSizeChanged = false;

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::ogl::OGL::createOGL()
{
    GN_GUARD;

    if( !setupDisplayMode() ) return false;

    GN_ASSERT( mWindow );

    GN_WIN_CHECK_RV( mDC = GetDC(mWindow), false );

    if( !sSetupPixelFormat(mDC) ) return false;

    GN_WIN_CHECK_RV( mRC = wglCreateContext(mDC), false );

    GN_WIN_CHECK_RV( wglMakeCurrent(mDC, mRC), false );

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::ogl::OGL::setupDisplayMode()
{
    GN_GUARD;

    if( mFullscreenModeInitialized ) return true;

    // only change display mode if we are in fullscreen mode
    if( mInitParams.fullScreen )
    {
        GN_ASSERT( IsWindow(mWindow) );

        // get monitor information
        HMONITOR hmonitor = MonitorFromWindow( mWindow, MONITOR_DEFAULTTOPRIMARY );
        if( 0 == hmonitor )
        {
            GNOGL_ERROR( "fail to get monitor handle!" );
            return false;
        }
        MONITORINFOEXA mi;
        mi.cbSize = sizeof(mi);
        GN_WIN_CHECK_RV( GetMonitorInfoA( hmonitor, &mi ), false );

        // change display mode
        DEVMODEA dm;
        memset(&dm, 0, sizeof(dm));
        dm.dmSize             = sizeof(dm);
        dm.dmPelsWidth        = mInitParams.width;
        dm.dmPelsHeight       = mInitParams.height;
        dm.dmBitsPerPel       = 32;
        dm.dmFields           = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;

        if( DISP_CHANGE_SUCCESSFUL != ChangeDisplaySettingsExA(
            mi.szDevice,
            &dm,
            NULL,
            CDS_FULLSCREEN,
            NULL ) )
        {
            GNOGL_ERROR( "failed to change to specified fullscr mode!" );
            return false;
        }
        mFullscreenModeInitialized = true;
        GNOGL_INFO( "Fullscreen mode activated." );

        // update monitor information
        mi.cbSize = sizeof(mi);
        if( !GetMonitorInfoA(
            MonitorFromWindow( mWindow,
                               MONITOR_DEFAULTTOPRIMARY ),
            &mi ) )
        {
            GNOGL_ERROR( "fail to update monitor information!" );
            return false;
        }

        // modify window style
        GN_WIN_CHECK( SetParent( mWindow, 0 ) );
        GN_WIN_CHECK( SetMenu( mWindow, 0 ) );
        GN_WIN_CHECK( SetWindowLong( mWindow, GWL_STYLE, WS_POPUP|WS_VISIBLE ) );
        if( IsIconic(mWindow) )
        {
            GN_WIN_CHECK( ShowWindow( mWindow, SW_SHOWNORMAL ) );
        }
        GNOGL_INFO( "move fullscreen window to %d, %d", mi.rcWork.left, mi.rcWork.top );
        GN_WIN_CHECK( SetWindowPos(
            mWindow, HWND_TOPMOST,
            mi.rcWork.left, mi.rcWork.top,
            mInitParams.width, mInitParams.height,
            SWP_FRAMECHANGED | SWP_SHOWWINDOW ) );

        // trigger a redraw operation
        GN_WIN_CHECK( UpdateWindow( mWindow ) );
    }

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::ogl::OGL::restoreDisplayMode()
{
    GN_GUARD;

    if( mFullscreenModeInitialized )
    {
        mFullscreenModeInitialized = false;

        // restore display mode
        if( DISP_CHANGE_SUCCESSFUL != ChangeDisplaySettings(0, 0) )
        {
            GNOGL_ERROR( "Failed to restore display mode, %s!", GN::getOSErrorInfo() );
        }
        GNOGL_INFO( "Display mode restored." );
    }

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::ogl::OGL::processWindowMessages()
{
    GN_GUARD_SLOW;

#if GN_WINNT & GN_PC
    MSG msg;
    while( true )
    {
        if( ::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) )
        {
            if( WM_QUIT == msg.message )
            {
                mClosed = true;
                return;
            }
            ::TranslateMessage( &msg );
            ::DispatchMessage(&msg);
        }
        else if( mMinimized )
        {
            ::WaitMessage();
        }
        else return; // Idle time!
    }
#endif

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
LRESULT GN::ogl::OGL::windowProc( HWND wnd, UINT msg, WPARAM wp, LPARAM lp )
{
    GN_GUARD;

    if( selfOK() ) switch (msg)
    {
        case WM_CLOSE :
            ::PostQuitMessage(0);
            return 0;

        case WM_ENTERSIZEMOVE :
            mInsideSizeMove = true;
            break;

        case WM_EXITSIZEMOVE :
            mInsideSizeMove = false;
            mSizeChanged = true;
            break;

        case WM_SIZE :
            //GNOGL_INFO( "window resize to %dx%d", LOWORD(lParam), HIWORD(lParam) ) );
            mMinimized = ( SIZE_MINIMIZED == wp );
            if( !mMinimized && !mInsideSizeMove ) mSizeChanged = true;
            break;

        // 防止不必要的清除背景的操作
        case WM_ERASEBKGND :
            return 1;
    }

    return ::DefWindowProc( wnd, msg, wp, lp );

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
LRESULT GN::ogl::OGL::staticProc( HWND wnd, UINT msg, WPARAM wp, LPARAM lp )
{
    return OGL::getInstance().windowProc( wnd, msg, wp, lp );
}

#endif // GN_WINNT
