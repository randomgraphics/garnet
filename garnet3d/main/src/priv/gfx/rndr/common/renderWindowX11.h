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
        Renderer      * mRenderer;
        bool            mUseExternalWindow;
        Display       * mDisplay;
        Screen        * mScreen;
        int             mScreenNumber;
        Window          mWindow;

        //@{
    public:
        RenderWindowX11() : mRenderer(0), mDisplay(0), mScreen(0), mWindow(0) {}
        ~RenderWindowX11() {}
        //@}

        // ********************************
        // public interface
        // ********************************
    public:

        ///
        /// associate the window with a renderer
        ///
        void setRenderer( Renderer * r ) { GN_ASSERT(r); mRenderer = r; }

        ///
        /// (re)initialize render window to use external window
        ///
        bool initExternalRenderWindow( Renderer * r, Display * display, Window externalWindow );

        ///
        /// (re)initialize render window to use internal widow.
        ///
        bool initInternalRenderWindow( Renderer * r, Display * display, Window parentWindow, Screen * monitor, UInt32 width, UInt32 height );

        ///
        /// Delete render window
        ///
        void quit();

        ///
        /// Get display handle
        ///
        Display *  getDisplay() const { return mDisplay; }

        ///
        /// Get window handle
        ///
        Window getWindow() const { return mWindow; }

        ///
        /// Get screen number
        ///
        int getScreenNumber() const { return mScreenNumber; }

        ///
        /// Get client size
        ///
        bool getClientSize( UInt32 & width , UInt32 & height ) const;

        ///
        /// handle render window size move, trigger renderer signal as apropriate.
        ///
        void handleSizeMove();

        // ********************************
        // private interface
        // ********************************
    private:

        bool initDisplay( Display * display );
    };
}}

#endif // GN_POSIX

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_RENDERWINDOWX11_H__
