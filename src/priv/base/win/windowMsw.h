#ifndef __GN_WIN_WINDOWMSW_H__
#define __GN_WIN_WINDOWMSW_H__
// *****************************************************************************
/// \file
/// \brief   Window class on MS Windows
/// \author  chenlee (2006.2.23)
// *****************************************************************************

#if GN_MSWIN

namespace GN { namespace win
{
    ///
    /// Window class on MS Windows
    ///
    class WindowMsw : public Window, public StdClass
    {
         GN_DECLARE_STDCLASS( WindowMsw, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        WindowMsw()          { clear(); }
        virtual ~WindowMsw() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init( const WindowCreationParams & );
        void quit();
    private:
        void clear() { mClassName.clear(); mModuleInstance = 0; mWindow = 0; }
        //@}

        // ********************************
        // from Window
        // ********************************
    public:

        //@{

        DisplayHandle getDisplayHandle() const { return (DisplayHandle)1; }
        MonitorHandle getMonitorHandle() const;
        WindowHandle getWindowHandle() const { return mWindow; }
        Vector2<size_t> getClientSize() const;
        void show();
        void hide();
        void minimize() { GN_UNIMPL_WARNING(); }
        void moveTo( int, int );
        void setClientSize( size_t, size_t );
        void repaint();
        void run();
        void runUntilNoNewEvents() { processWindowMessages(mWindow,false); }
        void stepOneEvent() { GN_UNIMPL_WARNING(); }
        void attachEventHandler( const StrA &, const WindowEventHandler & ) { GN_UNIMPL_WARNING(); }
        void removeEventHandler( const StrA &, const WindowEventHandler & ) { GN_UNIMPL_WARNING(); }

        //@}

        // ********************************
        // private variables
        // ********************************
    private:

        StrW mClassName;
        HINSTANCE mModuleInstance;
        HWND mWindow;

        static HashMap<void*,WindowMsw*> msInstanceMap;

        // ********************************
        // private functions
        // ********************************
    private:
        bool createWindow( const WindowCreationParams & wcp );

        LRESULT windowProc( HWND wnd, UINT msg, WPARAM wp, LPARAM lp );
        static LRESULT CALLBACK staticWindowProc( HWND wnd, UINT msg, WPARAM wp, LPARAM lp );
    };
}}

#endif

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_WIN_WINDOWMSW_H__
