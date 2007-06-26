#ifndef __GN_GFX_RENDERWINDOWX11_H__
#define __GN_GFX_RENDERWINDOWX11_H__
// *****************************************************************************
/// \file
/// \brief   Window class on X RenderWindowX11 platform
/// \author  chenlee (2005.10.4)
// *****************************************************************************

#if GN_POSIX

namespace GN { namespace gfx
{
    ///
    /// Render window class on POSIX platform
    ///
    class RenderWindowX11
    {
        bool            mUseExternalWindow;
        Display       * mDisplay;
        Window          mWindow;

        //@{
    public:
        RenderWindowX11() : mDisplay(0), mWindow(0) {}
        ~RenderWindowX11() {}
        //@}

        // ********************************
        // public interface
        // ********************************
    public:

        ///
        /// (re)initialize render window to use external window
        ///
        bool initExternalRenderWindow( HandleType display, HandleType externalWindow );

        ///
        /// (re)initialize render window to use internal widow.
        ///
        bool initInternalRenderWindow( HandleType display, HandleType parentWindow, HandleType monitor, UInt32 width, UInt32 height );

        ///
        /// Delete render window
        ///
        void quit();

        ///
        /// Get display handle
        ///
        HandleType getDisplay() const { return mDisplay; }

        ///
        /// Get window handle
        ///
        HandleType getWindow() const { return (void*)mWindow; }

        ///
        /// Get monitor handle (pointer to screen structure).
        ///
        HandleType getMonitor() const;

        ///
        /// Get client size
        ///
        bool getClientSize( UInt32 & width , UInt32 & height ) const;

        ///
        /// Get window size change flag.
        ///
        /// \param autoReset
        ///     If true, automatically clear the flag.
        ///
        bool getSizeChangeFlag( bool autoReset = true )
        { GN_UNUSED_PARAM(autoReset); return false; }

        // ********************************
        // private interface
        // ********************************
    private:

        bool initDisplay( HandleType display );
    };

    ///
    /// Get screen number of a window. Return -1 if failed.
    ///
    int getScreenNumberOfWindow( Display * disp, Window win );
}}

#endif // GN_POSIX

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_RENDERWINDOWX11_H__
