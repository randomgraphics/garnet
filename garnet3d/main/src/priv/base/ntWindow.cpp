#include "pch.h"

#if GN_WINNT && !GN_XENON

// *****************************************************************************
// Implementation of GN::Window::Impl
// *****************************************************************************

//!
//! Implementation class of window wrapper on NT platform
//!
class GN::Window::Impl
{
    GN::Window & mOwner;

    HWND mWindow;

    void destroy()
    {
        GN_GUARD;

        if( ::IsWindow( mWindow ) )
        {
            ::DestroyWindow( mWindow );
        }
        mWindow = 0;

        GN_UNGUARD;
    }

public:

    Impl( GN::Window & owner ) : mOwner(owner) {}

    ~Impl() { destroy(); }

    bool create( const CreateParam & cp )
    {
        GN_GUARD;

        destroy(); // destroy old widow

        // check parent
        HWND parent = ::IsWindow((HWND)cp.parent) ? (HWND)cp.parent : 0;

        static const char * sClassName = "GNbaseWindowWrapper";

        HINSTANCE moduleHandle = (HINSTANCE)::GetModuleHandle(0);

        WNDCLASSEXA wcex;

        // find the window class
        if( !::GetClassInfoEx( moduleHandle, sClassName, &wcex ) )
        {
            // register window class
            wcex.cbSize         = sizeof(WNDCLASSEX);
            wcex.style          = 0;//CS_NOCLOSE;
            wcex.lpfnWndProc    = (WNDPROC)&sMsgRouter;
            wcex.cbClsExtra     = 0;
            wcex.cbWndExtra     = 0;
            wcex.hInstance      = moduleHandle;
            wcex.hIcon          = LoadIcon (0, IDI_APPLICATION);
            wcex.hCursor        = LoadCursor (0,IDC_ARROW);
            wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
            wcex.lpszMenuName   = 0;
            wcex.lpszClassName  = sClassName;
            wcex.hIconSm        = LoadIcon(0, IDI_APPLICATION);
            if( 0 == ::RegisterClassExA(&wcex) )
            {
                GN_ERROR( "fail to register window class, %s!", getOSErrorInfo() );
                return false;
            }
        }

        // setup window style
        DWORD style;
        DWORD exStyle;
        if( parent )
        {
            if( cp.style )
                style = cp.style;
            else
                style = WS_POPUP | WS_BORDER | WS_CAPTION | WS_SIZEBOX;
            exStyle = WS_EX_TOOLWINDOW;
        }
        else
        {
            if( cp.style )
                style = cp.style;
            else
                style = WS_OVERLAPPEDWINDOW;
            exStyle = 0;
        }

        // calculate window size
        RECT rc = { 0, 0, cp.clientWidth, cp.clientHeight };
        ::AdjustWindowRectEx( &rc, style, 0, exStyle );

        // create window
        mWindow = ::CreateWindowExA(
            exStyle,
            sClassName,
            cp.title,
            style,
            cp.left, cp.top,
            rc.right - rc.left, rc.bottom - rc.top,
            parent,
            (HMENU)cp.menu,
            moduleHandle,
            this );
        if( 0 == mWindow )
        {
            GN_ERROR( "fail to create window, %s!", getOSErrorInfo() );
            return false;
        }

        // show the window
        ::ShowWindow( mWindow, SW_NORMAL );
        ::UpdateWindow( mWindow );

        // success
        return true;

        GN_UNGUARD;
    }

    void * getWindow() { return mWindow; }

    void * getMonitor()
    {
        GN_GUARD;

        if( !::IsWindow(mWindow) )
        {
            GN_ERROR( "Window class is yet to initialized!" );
            return 0;
        }
        return ::MonitorFromWindow( mWindow, MONITOR_DEFAULTTONEAREST );

        GN_UNGUARD;
    }

    bool getClientSize( uint32_t & width, uint32_t & height ) const
    {
        GN_GUARD;

        if( !::IsWindow(mWindow) )
        {
            GN_ERROR( "Impl class is yet to initialized!" );
            return false;
        }

        RECT rc;
        GN_WIN_CHECK_RV( ::GetClientRect( mWindow, &rc ), false );

        width = (UINT)(rc.right - rc.left);
        height = (UINT)(rc.bottom - rc.top);

        return true;

        GN_UNGUARD;
    }

    LRESULT winProc( HWND hwnd, UINT msg, WPARAM wp, LPARAM lp )
    {
        return mOwner.ntWinProc( hwnd, msg, wp, lp );
    }

    static LRESULT
    sMsgRouter( HWND wnd, UINT msg, WPARAM wp, LPARAM lp )
    {
        GN_GUARD;

        Impl * ptr;

        //GN_INFO( "wnd=0x%X, msg=%s", wnd, GN::winMsg2Str(msg) );

        // handle WM_NCCREATE
        if( WM_NCCREATE == msg )
        {
            GN_ASSERT( lp );
            ptr = (Impl*)((CREATESTRUCT*)lp)->lpCreateParams;
            ::SetWindowLongA( wnd, GWL_USERDATA, (LONG)ptr );
        }
        else
        {
            ptr = (Impl*)::GetWindowLong( wnd, GWL_USERDATA );
        }

        // call Window specific procedure
        return ptr
            ? ptr->winProc( wnd, msg, wp, lp )
            : ::DefWindowProc( wnd, msg, wp, lp );

        GN_UNGUARD;
    }
};

// *****************************************************************************
// public function of GN::Window
// *****************************************************************************

GN::Window::Window() { mImpl = new Impl(*this); }
GN::Window::~Window() { delete mImpl; }
bool GN::Window::create( const CreateParam & cp ) { return mImpl->create(cp); }
void * GN::Window::getWindow() { return mImpl->getWindow(); }
void * GN::Window::getMonitor() { return mImpl->getMonitor(); }
bool GN::Window::getClientSize( uint32_t & width, uint32_t & height ) const { return mImpl->getClientSize(width,height); }
size_t GN::Window::ntWinProc( void * hwnd, unsigned int msg, size_t wp, size_t lp )
{
    GN_GUARD;

    return ::DefWindowProc( (HWND)hwnd, msg, wp, lp );

    GN_UNGUARD;
}

#endif // GN_WINNT && !GN_XENON
