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
    //! Render window class on POSIX platform
    //!
    class XRenderWindow
    {
        bool mUseExternalDisplay;
        bool mUseExternalWindow;
        Display * mDisplay;
        Window    mWindow;
        Screen *  mScreen;
        
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
        //! (re)initialize render window to use external window
        //!
        bool initExternalRenderWindow( void * display, void * externalWindow );

        //!
        //! (re)initialize render window to use internal widow.
        //!
        bool initInternalRenderWindow( void * display, void * parentWindow, uint32_t width, uint32_t height );

        //!
        //! Delete render window
        //!
        void quit();

        //!
        //! Get display handle
        //!
        void * getDisplay() const { return mDisplay; }

        //!
        //! Get window handle
        //!
        void * getWindow() const { return (void*)mWindow; }

        //!
        //! Get monitor handle
        //!
        void * getMonitor() const { return mScreen; }

        //!
        //! Get client size
        //!
        bool getClientSize( uint32_t & width , uint32_t & height ) const;

        //!
        //! Get window size change flag.
        //!
        //! \param autoReset
        //!     If true, automatically clear the flag.
        //!
        bool getSizeChangeFlag( bool autoReset = true )
        { GN_UNUSED_PARAM(autoReset); return false; }

        // ********************************
        // private interface
        // ********************************
    private:

        bool initDisplay( void * display );
    };
}}

#endif // GN_POSIX

// *****************************************************************************
//                           End of xRenderWindow.h
// *****************************************************************************
#endif // __GN_GFX_XRENDERWINDOW_H__
