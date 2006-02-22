#include "pch.h"
#include "windowMsw.h"

namespace GN { namespace win
{
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
        WindowHandle getWindowHandle() const { return (WindowHandle)1; }
        Vector2<size_t> getClientSize() const { return Vector2<size_t>(640,480); }
        void show() {}
        void hide() {}
        void redraw() {}
        void moveTo( int, int ) {}
        void resize( size_t, size_t ) {}
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
        GN_WARN( "No window class implementation on current platform." );
        return new FakeWindow;

#endif

        GN_UNGUARD;
    }
}}
