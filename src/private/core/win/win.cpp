#include "pch.h"
#include "windowMsw.h"

namespace GN { namespace win
{
    static GN::Logger * sLogger = GN::getLogger("GN.win");

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

        intptr_t getDisplayHandle() const { return (intptr_t)1; }
        intptr_t getMonitorHandle() const { return (intptr_t)1; }
        intptr_t getWindowHandle() const { return (intptr_t)1; }
        intptr_t getModuleHandle() const { return (intptr_t)1; }
        Vector2<size_t> getClientSize() const { return Vector2<size_t>(640,480); }
        void show() {}
        void hide() {}
        void minimize() {}
        void moveTo( int, int ) {}
        void setClientSize( size_t, size_t ) {}
        void repaint() {}
        bool runUntilNoNewEvents(bool) { return false; }

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

    GN_API const WindowCreationParameters WCP_APPLICATION_WINDOW =
    {
        StrA("Garnet Application"),
        0,     // no parent
        0, 0,  // default size
        true,  // border
        true,  // titlebar
        false, // topmost
        true,  // has close box
    };

    GN_API const WindowCreationParameters WCP_WINDOWED_RENDER_WINDOW =
    {
        StrA("Garnet Application"),
        0, // no parent
        640, 480, // size
        true, // border
        true, // titlebar
        false, // topmost
        false, // no close box
    };

    GN_API const WindowCreationParameters WCP_FULLSCREEN_RENDER_WINDOW =
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
    GN_API Window * createWindow( const WindowCreationParameters & wcp )
    {
        GN_GUARD;

#if GN_XBOX2

        GN_UNUSED_PARAM( wcp );
        return new FakeWindow;

#elif GN_WINPC

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
    GN_API Window * attachToExistingWindow( const WindowAttachingParameters & wap )
    {
        GN_GUARD;

#if GN_XBOX2

        GN_UNUSED_PARAM( wap );
        return new FakeWindow;

#elif GN_WINPC

        AutoObjPtr<WindowMsw> p( new WindowMsw );
        if( !p->init( wap ) ) return 0;
        return p.detach();

#else

        GN_UNUSED_PARAM( wap );
        GN_WARN(sLogger)( "No window class implementation on current platform." );
        return new FakeWindow;

#endif

        GN_UNGUARD;
    }

    //
    //
    // -------------------------------------------------------------------------
#if GN_XBOX2
    GN_API intptr_t getMonitorByIndex( size_t ) { return (intptr_t)1; }
#elif GN_WINPC
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
    GN_API intptr_t getMonitorByIndex( size_t i )
    {
        MonitorEnumInfo mei = { 0, i, 0 };
        ::EnumDisplayMonitors( 0, 0, &sMonitorEnumProc, (LPARAM)&mei );
        return (intptr_t)mei.handle;
    }
#else
    GN_API intptr_t getMonitorByIndex( size_t )
    {
        GN_UNIMPL();
        return 0;
    }
#endif

}}
