#include "pch.h"
#include "windowMsw.h"

namespace GN { namespace win
{
    static GN::Logger * sLogger = GN::getLogger("GN.win");

    // *************************************************************************
    // Fake Window
    // *************************************************************************

    //!
    //! Fake window class
    //!
    class FakeWindow : public Window
    {
        // ********************************
        // from Window
        // ********************************
    public:

        //@{

        DisplayHandle getDisplayHandle() const { return (DisplayHandle)1; }
        MonitorHandle getMonitorHandle() const { return (MonitorHandle)1; }
        WindowHandle getWindowHandle() const { return (WindowHandle)1; }
        Vector2<size_t> getClientSize() const { return Vector2<size_t>(640,480); }
        void show() {}
        void hide() {}
        void minimize() {}
        void moveTo( int, int ) {}
        void resize( size_t, size_t ) {}
        void repaint() {}
        void run() {}
        void runWhileEvents() {}
        void stepOneEvent() {}
        void attachEventHandler( const StrA &, const WindowEventHandler & ) {}
        void removeEventHandler( const StrA &, const WindowEventHandler & ) {}

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

    const WindowCreationParams WCP_WINDOWED_RENDER_WINDOW =
    {
        StrA("Garnet Application"),
        0, // no parent
        640, 480, // size
        true, // border
        true, // titlebar
        false, // topmost
    };

    const WindowCreationParams WCP_FULLSCREEN_RENDER_WINDOW =
    {
        StrA(""),
        0, // no parent
        640, 480, // size
        false, // border
        false, // titlebar
        true, // topmost
    };

    //
    //
    // -------------------------------------------------------------------------
    Window * createWindow( const WindowCreationParams & wcp )
    {
        GN_GUARD;

#if GN_XENON

        GN_UNUSED_PARAM( wcp );
        return new FakeWindow;

#elif GN_MSWIN

        AutoObjPtr<WindowMsw> p( new WindowMsw );
        if( !p->init( wcp ) ) return 0;
        return p.detach();

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
    MonitorHandle getMonitorByIndex( size_t ) { return (MonitorHandle)1; }
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
    MonitorHandle getMonitorByIndex( size_t i )
    {
        MonitorEnumInfo mei = { 0, i, 0 };
        ::EnumDisplayMonitors( 0, 0, &sMonitorEnumProc, (LPARAM)&mei );
        return mei.handle;
    }
#else
    MonitorHandle getMonitorByIndex( size_t )
    {
        GN_UNIMPL();
        return 0;
    }
#endif

    //
    //
    // -------------------------------------------------------------------------
#if GN_MSWIN && !GN_XENON
    void processWindowMessages( WindowHandle window, bool blockWhileMinized )
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
                GN_INFO(sLogger)( "Wait for window messages..." );
                ::WaitMessage();
            }
            else return; // Idle time
        }

        GN_UNGUARD_SLOW;
    }
#else
    void processWindowMessages( WindowHandle, bool ) {}
#endif
}}
