#ifndef __GN_GFX_RENDERWINDOWX11_H__
#define __GN_GFX_RENDERWINDOWX11_H__
// *****************************************************************************
/// \file
/// \brief   Window class on X RenderWindowX11 platform
/// \author  chenlee (2005.10.4)
// *****************************************************************************

#if GN_POSIX

namespace GN { namespace win
{
    ///
    /// X11 window class
    ///
    class WindowX11 : public Window, public StdClass
    {
         GN_DECLARE_STDCLASS( WindowX11, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        WindowX11()          { clear(); }
        virtual ~WindowX11() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init( const WindowCreationParameters & );
        bool init( const WindowAttachingParameters & );
        void quit();
    private:
        void clear() { mUseExternalWindow = false; mDisplay = 0; mScreen = 0; mScreenNumber = 0; mWindow = 0; }
        //@}

        // ********************************
        // from Window
        // ********************************
    public:

        //@{
        intptr_t getDisplayHandle() const { return (intptr_t)mDisplay; }
        intptr_t getMonitorHandle() const { return (intptr_t)mScreenNumber; }
        intptr_t getWindowHandle() const { return (intptr_t)mWindow; }
        intptr_t getModuleHandle() const { return (intptr_t)1; }
        Vector2<uint32_t> getClientSize() const;
        void show();
        void hide();
        void minimize() { GN_UNIMPL_WARNING(); }
        void moveTo( int, int );
        void setClientSize( size_t, size_t );
        bool runUntilNoNewEvents(bool blockWhenMinimized);

        //@}

        // ********************************
        // private variables
        // ********************************
    private:

        bool            mUseExternalWindow;
        Display       * mDisplay;
        Screen        * mScreen;
        int             mScreenNumber;
        ::Window        mWindow;

        bool createExternalRenderWindow( const WindowAttachingParameters & );
        bool createInternalRenderWindow( const WindowCreationParameters & );
        bool initDisplay( Display * display );
    };
}}

#endif // GN_POSIX

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_RENDERWINDOWX11_H__
