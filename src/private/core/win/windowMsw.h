#ifndef __GN_WIN_WINDOWMSW_H__
#define __GN_WIN_WINDOWMSW_H__
// *****************************************************************************
/// \file
/// \brief   Window class on MS Windows
/// \author  chenlee (2006.2.23)
// *****************************************************************************

#if GN_WINPC

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
        bool init( const WindowCreationParameters & );
        bool init( const WindowAttachingParameters & );
        void quit();
    private:
        void clear() { mClassName.clear(); mModuleInstance = 0; mWindow = 0; mHook = 0; }
        //@}

        // ********************************
        // from Window
        // ********************************
    public:

        //@{
        intptr_t getDisplayHandle() const { return (intptr_t)1; }
        intptr_t getMonitorHandle() const;
        intptr_t getWindowHandle() const { return (intptr_t)mWindow; }
        intptr_t getModuleHandle() const { return (intptr_t)mModuleInstance; }
        Vector2<size_t> getClientSize() const;
        void show();
        void hide();
        void minimize() { GN_UNIMPL_WARNING(); }
        void moveTo( int, int );
        void setClientSize( size_t, size_t );
        void repaint();
        void run();
        bool runUntilNoNewEvents(bool blockWhenMinimized);

        //@}

        // ********************************
        // private variables
        // ********************************
    private:

        StrW      mClassName;
        HINSTANCE mModuleInstance;
        HWND      mWindow;
        HHOOK     mHook;
        bool      mIsExternal;
        bool      mInsideSizeMove;

        static Dictionary<void*,WindowMsw*> msInstanceMap;

        // ********************************
        // private functions
        // ********************************
    private:
        bool createWindow( const WindowCreationParameters & wcp );
        void handleMessage( HWND wnd, UINT msg, WPARAM wp, LPARAM lp );
        LRESULT windowProc( HWND wnd, UINT msg, WPARAM wp, LPARAM lp );
        static LRESULT CALLBACK staticWindowProc( HWND wnd, UINT msg, WPARAM wp, LPARAM lp );
        static LRESULT CALLBACK staticHookProc( int code, WPARAM wp, LPARAM lp );
    };
}}

#endif

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_WIN_WINDOWMSW_H__
