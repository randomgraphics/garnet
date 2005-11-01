#ifndef __GN_WIN_MSWWINDOW_H__
#define __GN_WIN_MSWWINDOW_H__
// *****************************************************************************
//! \file    mswWindow.h
//! \brief   A simple window wrapper.
//! \author  chenlee (2005.10.11)
// *****************************************************************************

#if GN_MSWIN && !GN_XENON

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>

namespace GN { namespace win
{
    //!
    //! Window wrapper on Microsoft Windows.
    //!
    class MswWindow
    {
    public:

        //!
        //! MswWindow creation parameters
        //!
        struct CreateParam
        {
            HWND         parent;        //!< Parent window handle. Platform specific.
            HMENU        menu;          //!< Main menu handle. Platform specfic.
            const char * title;         //!< MswWindow title.
            uint32_t     style;         //!< MswWindow style. Platform specific. 0 means using default style.
            int32_t      left;          //!< MswWindow position.
            int32_t      top;           //!< MswWindow position.
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
        MswWindow() : mWindow(0) {}
        virtual ~MswWindow() { destroy(); }
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
        void showWindow( bool show = true ) const;

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
        //! Set window procedure
        //!
        void setWindowProcedure( const WindowProcedure & handler ) { mWinProc = handler; };

    private:

        HWND mWindow;
        StrA mClassName;
        HINSTANCE mInstanceHandle;
        WindowProcedure mWinProc;

    private:

        LRESULT winProc( HWND hwnd, UINT msg, WPARAM wp, LPARAM lp );

        static LRESULT CALLBACK sMsgRouter( HWND wnd, UINT msg, WPARAM wp, LPARAM lp );
    };
}}

#endif // GN_MSWIN && !GN_XENON

// *****************************************************************************
//                           End of mswWindow.h
// *****************************************************************************
#endif // __GN_WIN_MSWWINDOW_H__
