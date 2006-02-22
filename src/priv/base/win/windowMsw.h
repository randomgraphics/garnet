#ifndef __GN_WIN_WINDOWMSW_H__
#define __GN_WIN_WINDOWMSW_H__
// *****************************************************************************
//! \file    win/windowMsw.h
//! \brief   Window class on MS Windows
//! \author  chenlee (2006.2.23)
// *****************************************************************************

namespace GN { namespace win
{
    //!
    //! Window class on MS Windows
    //!
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
        bool ok() const { return MyParent::ok(); }
    private:
        void clear() {}
        //@}

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
}}

// *****************************************************************************
//                           End of windowMsw.h
// *****************************************************************************
#endif // __GN_WIN_WINDOWMSW_H__
