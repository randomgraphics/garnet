#ifndef __GN_WIN_XWINDOW_H__
#define __GN_WIN_XWINDOW_H__
// *****************************************************************************
//! \file    xWindow.h
//! \brief   X window wrapper class
//! \author  chenlee (2005.10.31)
// *****************************************************************************

#if 0//GN_POSIX

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

        // ********************************
        // private variables
        // ********************************
    private:

        Display * mDisplay;
        Window    mWindow;

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
