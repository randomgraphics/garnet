#ifndef __GN_BASE_NTWINDOW_H__
#define __GN_BASE_NTWINDOW_H__
// *****************************************************************************
//! \file    ntWindow.h
//! \brief   A simple window wrapper.
//! \author  chenlee (2005.10.11)
// *****************************************************************************

#if GN_WINNT && !GN_XENON

#include <windows.h>

namespace GN
{
    //!
    //! Window wrapper on NT platform.
    //!
    class NTWindow
    {
    public:

        //!
        //! NTWindow creation parameters
        //!
        struct CreateParam
        {
            HWND         parent;        //!< Parent window handle. Platform specific.
            HMENU        menu;          //!< Main menu handle. Platform specfic.
            const char * title;         //!< NTWindow title.
            uint32_t     style;         //!< NTWindow style. Platform specific. 0 means using default style.
            int32_t      left;          //!< NTWindow position.
            int32_t      top;           //!< NTWindow position.
            uint32_t     clientWidth;   //!< Client width of the window.
            uint32_t     clientHeight;  //!< Client height of the window.

            //!
            //! Setup default parameters
            //!
            CreateParam()
                : parent(0)
                , menu(0)
                , title("")
                , style(0)
                , left( (int32_t)0x80000000 )
                , top( (int32_t)0x80000000 )
                , clientWidth(640)
                , clientHeight(480)
            {}
        };

        //@{
        NTWindow() {}
        virtual ~NTWindow() { destroy(); }
        //@}

        //!
        //! Create or Recreate a window
        //!
        bool create( const CreateParam & );

        //!
        //! Destroy the window.
        //!
        void destroy();

        //!
        //! Show/Hide the window
        //!
        void showWindow( bool show ) const;

        //!
        //! Get window handle
        //!
        HWND getWindow() const { return mWindow; }

        //!
        //! Get monitor handle
        //!
        HMONITOR getMonitor() const;

        //!
        //! Get client size
        //!
        bool getClientSize( uint32_t & width, uint32_t & height ) const;

        //!
        //! Windows message handler.
        //!
        typedef Functor4< LRESULT, HWND, UINT, WPARAM, LPARAM > WindowProcedure;

        //!
        //! Set NT window procedure
        //!
        void setWindowProcedure( const WindowProcedure & handler ) { mWinProc = handler; };

    private:

        HINSTANCE mInstanceHandle;
        StrA mClassName;
        HWND mWindow;
        WindowProcedure mWinProc;

    private:

        LRESULT winProc( HWND hwnd, UINT msg, WPARAM wp, LPARAM lp );

        static LRESULT CALLBACK sMsgRouter( HWND wnd, UINT msg, WPARAM wp, LPARAM lp );
    };
}

#endif // GN_WINNT && !GN_XENON

// *****************************************************************************
//                           End of ntWindow.h
// *****************************************************************************
#endif // __GN_BASE_NTWINDOW_H__
