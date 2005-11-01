#ifndef __GN_WIN_XWINDOW_H__
#define __GN_WIN_XWINDOW_H__
// *****************************************************************************
//! \file    xWindow.h
//! \brief   X window wrapper class
//! \author  chenlee (2005.10.31)
// *****************************************************************************

#if GN_POSIX

#include <X11/XLib.h>

namespace GN { namespace win {
    //!
    //! X window wrapper class
    //!
    class XWindow
    {
    public:
        //!
        //! Creation parameters
        //!
        struct CreateParam
        {
            const char * display;       //!< Display name. Default is empty.
            Window       parent;        //!< Parent window handle.
            const char * title;         //!< MswWindow title.
            int32_t      left;          //!< MswWindow position.
            int32_t      top;           //!< MswWindow position.
            uint32_t     clientWidth;   //!< Client width of the window.
            uint32_t     clientHeight;  //!< Client height of the window.

            //!
            //! Setup default parameters
            //!
            CreateParam()
                : display("")
                , parent(0)
                , title("")
                , left(0)
                , top(0)
                , clientWidth(640)
                , clientHeight(480)
            {}
        };

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        XWindow() : mDisplay(0), mWindow(0) {}
        virtual ~XWindow() { destroy(); }
        //@}

        // ********************************
        // public functions
        // ********************************
    public:

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
        //! Get display
        //!
        Display * getDisplay() const { return mDisplay; }

        //!
        //! Get window handle
        //!
        Window getWindow() const { return mWindow; }

        //!
        //! Get client size
        //!
        bool getClientSize( uint32_t & width, uint32_t & height ) const;

        //!
        //! Event handler
        //!
        typedef Functor1<void,const XEvent&> EventHandler;

        //!
        //! Set event handler
        //!
        void setEventHandler( const EventHandler & handler ) { mEventHandler = handler; };

        // ********************************
        // private variables
        // ********************************
    private:

        Display * mDisplay;
        Window    mWindow;
        EventHandler mEventHandler;

        // ********************************
        // private functions
        // ********************************
    private:
    };
}}

#endif // GN_POSIX

// *****************************************************************************
//                           End of xWindow.h
// *****************************************************************************
#endif // __GN_WIN_XWINDOW_H__
