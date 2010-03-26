#include "pch.h"
#include "windowMsw.h"

namespace GN { namespace win
{
    static GN::Logger * sLogger = GN::GetLogger("GN.win");

    // *************************************************************************
    // Fake Window
    // *************************************************************************

    ///
    /// Fake window class
    ///
    class FakeWindow : public Window
    {
        // ********************************
        // from Window
        // ********************************
    public:

        //@{

        DisplayHandle GetDisplayHandle() const { return (DisplayHandle)1; }
        MonitorHandle GetMonitorHandle() const { return (MonitorHandle)1; }
        WindowHandle GetWindowHandle() const { return (WindowHandle)1; }
        Vector2<size_t> GetClientSize() const { return Vector2<size_t>(640,480); }
        void Show() {}
        void Hide() {}
        void Minimize() {}
        void MoveTo( int, int ) {}
        void SetClientSize( size_t, size_t ) {}
        void Repaint() {}
        void Run() {}
        void RunUntilNoNewEvents() {}
        void StepOneEvent() {}
        void AttachEventHandler( const StrA &, const WindowEventHandler & ) {}
        void RemoveEventHandler( const StrA &, const WindowEventHandler & ) {}

        //@}

        // ********************************
        // private variables
        // ********************************
    private:

        // ********************************
        // private functions
        // ********************************
    private:
    };

    // *************************************************************************
    // Public functions
    // *************************************************************************

    const WindowCreationParams WCP_APPLICATION_WINDOW =
    {
        StrA("Garnet Application"),
        0,     // no parent
        0, 0,  // default size
        true,  // border
        true,  // titlebar
        false, // topmost
        true,  // has close box
    };

    const WindowCreationParams WCP_WINDOWED_RENDER_WINDOW =
    {
        StrA("Garnet Application"),
        0, // no parent
        640, 480, // size
        true, // border
        true, // titlebar
        false, // topmost
        false, // no close box
    };

    const WindowCreationParams WCP_FULLSCREEN_RENDER_WINDOW =
    {
        StrA(""),
        0, // no parent
        640, 480, // size
        false, // border
        false, // titlebar
        true, // topmost
        false, // no close box
    };

    //
    //
    // -------------------------------------------------------------------------
    Window * NewWindow( const WindowCreationParams & wcp )
    {
        GN_GUARD;

#if GN_XENON

        GN_UNUSED_PARAM( wcp );
        return new FakeWindow;

#elif GN_MSWIN

        AutoObjPtr<WindowMsw> p( new WindowMsw );
        if( !p->Init( wcp ) ) return 0;
        return p.Detach();

#else

        GN_UNUSED_PARAM( wcp );
        GN_WARN(sLogger)( "No window class implementation on current platform." );
        return new FakeWindow;

#endif

        GN_UNGUARD;
    }

    //
    //
    // -------------------------------------------------------------------------
#if GN_XENON
    MonitorHandle GetMonitorByIndex( size_t ) { return (MonitorHandle)1; }
#elif GN_MSWIN
    struct MonitorEnumInfo
    {
        HMONITOR handle;
        size_t   targetIndex;
        size_t   currentIndex;
    };
    static BOOL CALLBACK sMonitorEnumProc( HMONITOR hMonitor, HDC, LPRECT, LPARAM dwData )
    {
        MonitorEnumInfo * mei = (MonitorEnumInfo*)dwData;
        if( mei->currentIndex == mei->targetIndex )
        {
            mei->handle = hMonitor;
            return FALSE;
        }
        else
        {
            ++mei->currentIndex;
            return TRUE;
        }
    }
    MonitorHandle GetMonitorByIndex( size_t i )
    {
        MonitorEnumInfo mei = { 0, i, 0 };
        ::EnumDisplayMonitors( 0, 0, &sMonitorEnumProc, (LPARAM)&mei );
        return mei.handle;
    }
#else
    MonitorHandle GetMonitorByIndex( size_t )
    {
        GN_UNIMPL();
        return 0;
    }
#endif

    //
    //
    // -------------------------------------------------------------------------
#if GN_MSWIN && !GN_XENON
    void ProcessWindowMessages( WindowHandle window, bool blockWhileMinized )
    {
        GN_GUARD_SLOW;

        GN_ASSERT( ::IsWindow( (HWND)window ) );

        MSG msg;
        while( true )
        {
            if( ::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) )
            {
                if( WM_QUIT == msg.message )
                {
                    return;
                }
                ::TranslateMessage( &msg );
                ::DispatchMessage(&msg);
            }
            else if( ::IsIconic( (HWND)window ) && blockWhileMinized )
            {
                GN_TRACE(sLogger)( "Wait for window messages..." );
                ::WaitMessage();
            }
            else return; // Idle time
        }

        GN_UNGUARD_SLOW;
    }
#else
    void ProcessWindowMessages( WindowHandle, bool ) {}
#endif
}}
