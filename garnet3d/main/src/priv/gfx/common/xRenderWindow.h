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

        //@{
    public:
        XRenderWindow() : mDisplay(0), mWindow(0) {}
        ~XRenderWindow() {}
        //@}

        // ********************************
        // public interface
        // ********************************
    public:

        //!
        //! (re)initialize render window to use external window
        //!
        bool initExternalRenderWindow( HandleType display, HandleType externalWindow );

        //!
        //! (re)initialize render window to use internal widow.
        //!
        bool initInternalRenderWindow( HandleType display, HandleType parentWindow, uint32_t width, uint32_t height );

        //!
        //! Delete render window
        //!
        void quit();

        //!
        //! Get display handle
        //!
        HandleType getDisplay() const { return mDisplay; }

        //!
        //! Get window handle
        //!
        HandleType getWindow() const { return (void*)mWindow; }

        //!
        //! Get monitor handle (pointer to screen structure).
        //!
        HandleType getMonitor() const;

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

        bool initDisplay( HandleType display );
    };

    //!
    //! Get screen number of a window. Return -1 if failed.
    //!
    int getScreenNumberOfWindow( Display * disp, Window win );
}}

#endif // GN_POSIX

// *****************************************************************************
//                           End of xRenderWindow.h
// *****************************************************************************
#endif // __GN_GFX_XRENDERWINDOW_H__
