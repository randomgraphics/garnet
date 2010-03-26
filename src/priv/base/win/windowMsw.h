#ifndef __GN_WIN_WINDOWMSW_H__
#define __GN_WIN_WINDOWMSW_H__
// *****************************************************************************
/// \file
/// \brief   Window class on MS Windows
/// \author  chenlee (2006.2.23)
// *****************************************************************************

#if GN_MSWIN && !GN_XENON

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
        WindowMsw()          { Clear(); }
        virtual ~WindowMsw() { Quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool Init( const WindowCreationParams & );
        void Quit();
    private:
        void Clear() { mClassName.Clear(); mModuleInstance = 0; mWindow = 0; }
        //@}

        // ********************************
        // from Window
        // ********************************
    public:

        //@{

        DisplayHandle GetDisplayHandle() const { return (DisplayHandle)1; }
        MonitorHandle GetMonitorHandle() const;
        WindowHandle GetWindowHandle() const { return mWindow; }
        Vector2<size_t> GetClientSize() const;
        void Show();
        void Hide();
        void Minimize() { GN_UNIMPL_WARNING(); }
        void MoveTo( int, int );
        void SetClientSize( size_t, size_t );
        void Repaint();
        void Run();
        void RunUntilNoNewEvents() { ProcessWindowMessages(mWindow,false); }
        void StepOneEvent() { GN_UNIMPL_WARNING(); }
        void AttachEventHandler( const StrA &, const WindowEventHandler & ) { GN_UNIMPL_WARNING(); }
        void RemoveEventHandler( const StrA &, const WindowEventHandler & ) { GN_UNIMPL_WARNING(); }

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
        bool NewWindow( const WindowCreationParams & wcp );

        LRESULT windowProc( HWND wnd, UINT msg, WPARAM wp, LPARAM lp );
        static LRESULT CALLBACK staticWindowProc( HWND wnd, UINT msg, WPARAM wp, LPARAM lp );
    };
}}

#endif

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_WIN_WINDOWMSW_H__
