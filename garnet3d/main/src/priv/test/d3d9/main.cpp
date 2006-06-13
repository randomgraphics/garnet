#include "pch.h"

#if GN_MSVC

#if GN_XENON
#if GN_DEBUG_BUILD
#pragma comment(lib, "d3d9d.lib")
#pragma comment( lib, "xgraphicsd.lib" )
#pragma comment(lib, "xapilibd.lib")
#elif GN_PROFILE_BUILD
#pragma comment(lib, "d3d9i.lib")
#pragma comment( lib, "xgraphics.lib" )
#pragma comment(lib, "xapilibi.lib")
#else
#pragma comment(lib, "d3d9.lib")
#pragma comment( lib, "xgraphics.lib" )
#pragma comment(lib, "xapilib.lib")
#endif
#else
#if GN_DEBUG_BUILD
#pragma comment( lib, "d3dx9d.lib" )
#else
#pragma comment( lib, "d3dx9.lib" )
#endif
#endif

#endif

using namespace GN;
using namespace GN::gfx;
using namespace GN::app;

class MainWindow
{
    HANDLE mCreationOk;
    HANDLE mThread;
    HWND mWindow;

    static LRESULT CALLBACK
    sWindowProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
    {
        return DefWindowProc( hWnd, message, wParam, lParam );
    }

    static DWORD WINAPI
    sThreadProc( LPVOID inParam )
    {
        MainWindow * pHostWindow = static_cast<MainWindow*>(inParam);

        // get instance handle
        HINSTANCE hInst = (HINSTANCE)GetCurrentProcess();

        // register window class
        WNDCLASSEXA wcex;
        wcex.cbSize         = sizeof(WNDCLASSEX); 
        wcex.style			= CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc	= (WNDPROC)&sWindowProc;
        wcex.cbClsExtra		= 0;
        wcex.cbWndExtra		= 0;
        wcex.hInstance		= hInst;
        wcex.hIcon			= NULL;
        wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
        wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
        wcex.lpszMenuName	= NULL;
        wcex.lpszClassName	= "GNtestD3D9";
        wcex.hIconSm		= NULL;
        if( 0 == ::RegisterClassExA(&wcex) )
        {
            GN_ERROR( "fail to register window class, %s!", getOSErrorInfo() );
            return false;
        }

        // adjust window size
        RECT rc = { 0, 0, 640, 480 };
        AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE );

        // create window
        GN_MSW_CHECK_RV( pHostWindow->mWindow = CreateWindowA(
            "GNtestD3D9",
            "GNtestD3D9",
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            rc.right - rc.left,
            rc.bottom - rc.top,
            NULL,
            NULL,
            hInst,
            NULL ), 1 );

        // Show the window
        ShowWindow( pHostWindow->mWindow, SW_SHOW );
        GN_MSW_CHECK_RV( UpdateWindow( pHostWindow->mWindow ), 1 );

        // trigger window creation event
        SetEvent( pHostWindow->mCreationOk );

        // message loop
        MSG message;
        while( GetMessage( &message, pHostWindow->mWindow, 0, 0 ) )
        {
            TranslateMessage( &message );
            DispatchMessage( &message );
        }

        // destroy the window
        GN_MSW_CHECK( DestroyWindow( pHostWindow->mWindow ) );
        pHostWindow->mWindow = 0;

        // success
        return 0;
    }

public:

    MainWindow()
        : mCreationOk(0)
        , mThread(0)
        , mWindow(0)
    {}

    HWND getWindow() const { return mWindow; }

    bool create()
    {
        // create event
        GN_MSW_CHECK_RV( mCreationOk = CreateEventA( NULL, true, false, NULL ), false );

        // create thread
        GN_MSW_CHECK_RV( mThread = ::CreateThread(
            NULL,
            0,
            sThreadProc,
            this,
            0, // no suspend
            NULL ), false );

        // wait for window creation completion
        return WAIT_OBJECT_0 == WaitForMultipleObjects( 2, &mCreationOk, FALSE, INFINITE );
    }

    void destroy()
    {
        if( mThread )
        {
            if( IsWindow(mWindow) ) PostMessageA( mWindow, WM_QUIT, 0, 0 );
            WaitForSingleObject( mThread, INFINITE );
            CloseHandle( mThread );
            mThread = NULL;
        }

        if( mCreationOk )
        {
            CloseHandle( mCreationOk );
            mCreationOk = NULL;
        }
    }
};

class MyApp : public SampleApp
{
    MainWindow mWindow;
    
public:
    bool onAppInit()
    {
        if( !mWindow.create() ) return false;

        return true;
    }

    void onAppQuit()
    {
        mWindow.destroy();
    }

    void onDetermineInitParam( InitParam & ip )
    {
        ip.rapi = API_D3D9;
        ip.ro.useExternalWindow = true;
        ip.ro.renderWindow = mWindow.getWindow();
    }

    bool onRendererRestore()
    {
        return true;
    }

    void onRendererDispose()
    {
    }

    void onUpdate()
    {
    }

    void onRender()
    {
        LPDIRECT3DDEVICE9 dev = (LPDIRECT3DDEVICE9)gRenderer.getD3DDevice();
        dev->Clear( 0, 0, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL, 0, 1.0f, 0 );
    }
};

int main( int argc, const char * argv[] )
{
    MyApp app;
    return app.run( argc, argv );
}
