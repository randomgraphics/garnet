#ifndef __GN_GFX_XRENDERWINDOW_H__
#define __GN_GFX_XRENDERWINDOW_H__
// *****************************************************************************
//! \file    xRenderWindow.h
//! \brief   Window class on X XRenderWindow platform
//! \author  chenlee (2005.10.4)
// *****************************************************************************

#if GN_POSIX

namespace GN { namespace gfx
{
    //!
    //! Render window class on POSIX platform (Unimplemented)
    //!
    class XRenderWindow
    {
        uint32_t mWidth, mHeight;
        
        //@{
    public:
        XRenderWindow()  {}
        ~XRenderWindow() {}
        //@}

        // ********************************
        // public interface
        // ********************************
    public:

        //!
        //! initialize or reinitialize the render window based on current device setting.
        //!
        bool init( const UserOptions & uo )
        {
            if( uo.fullscreen )
            {
                mWidth = uo.displayMode.width;
                mHeight = uo.displayMode.height;
            }
            else
            {
                mWidth = uo.windowedWidth;
                mHeight = uo.windowedHeight;
            }
            return true;
        }

        //!
        //! Delete render window
        //!
        void quit() {}

        //!
        //! Get window handle
        //!
        void * getWindow() const { return (void*)1; }

        //!
        //! Get monitor handle
        //!
        void * getMonitor() const { return (void*)1; }

        //!
        //! Get client size
        //!
        bool getClientSize( uint32_t & width , uint32_t & height ) const { width = mWidth; height = mHeight; return true; }

        //!
        //! Get window size change flag.
        //!
        //! \param autoReset
        //!     If true, automatically clear the flag.
        //!
        bool getSizeChangeFlag( bool autoReset = true ) { GN_UNUSED_PARAM(autoReset); return false; }
    };
}}

#endif // GN_POSIX

// *****************************************************************************
//                           End of xRenderWindow.h
// *****************************************************************************
#endif // __GN_GFX_XRENDERWINDOW_H__
