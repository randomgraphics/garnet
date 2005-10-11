#ifndef __GN_BASE_WINDOW_H__
#define __GN_BASE_WINDOW_H__
// *****************************************************************************
//! \file    window.h
//! \brief   A simple window wrapper.
//! \author  chenlee (2005.10.11)
// *****************************************************************************

namespace GN
{
    //!
    //! Window wrapper
    //!
    class Window
    {
        class Impl; //!< Implementation class of Window wrapper.

        Impl * mImpl;

    public:

        //!
        //! Window creation parameters
        //!
        struct CreateParam
        {
            void *       parent;        //!< Parent window handle. Platform specific.
            void *       menu;          //!< Main menu handle. Platform specfic.
            const char * title;         //!< Window title.
            uint32_t     style;         //!< Window style. Platform specific. 0 means using default style.
            int32_t      left;          //!< Window position.
            int32_t      top;           //!< Window position.
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
#if GN_WINNT
                , left( (int32_t)0x80000000 )
                , top( (int32_t)0x80000000 )
#else
                , left(0)
                , top(0)
#endif
                , clientWidth(640)
                , clientHeight(480)
            {}
        };

        //@{
        Window();
        virtual ~Window();
        //@}

        //!
        //! Create or Recreate a window
        //!
        bool create( const CreateParam & );

        //!
        //! Get window handle
        //!
        void * getWindow();

        //!
        //! Get monitor handle
        //!
        void * getMonitor();

        //!
        //! Get client size
        //!
        bool getClientSize( uint32_t & width, uint32_t & height ) const;

#if GN_WINNT
        //!
        //! NT window procedure
        //!
        virtual size_t ntWinProc( void * hwnd, unsigned int msg, size_t wp, size_t lp );
#endif
    };
}

// *****************************************************************************
//                           End of window.h
// *****************************************************************************
#endif // __GN_BASE_WINDOW_H__
