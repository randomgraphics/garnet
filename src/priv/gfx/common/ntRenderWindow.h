#ifndef __GN_GFX_NTRENDERWINDOW_H__
#define __GN_GFX_NTRENDERWINDOW_H__
// *****************************************************************************
//! \file    ntRenderWindow.h
//! \brief   Render window class on NT platform
//! \author  chenlee (2005.10.4)
// *****************************************************************************

#if GN_WINNT

#include "renderWindow.h"

namespace GN { namespace gfx {
    //!
    //! Render window class on NT platform
    //!
    class NTRenderWindow : public RenderWindow
    {
        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        NTRenderWindow() : mWindow(0), mMonitor(0) {}
        ~NTRenderWindow() { quit(); }
        //@}

        // ********************************
        // from RenderWindow
        // ********************************

    public:

        virtual bool init( const DeviceSettings & );
        virtual void quit();
        virtual void * getWindow() const { return mWindow; }
        virtual void * getMonitor() const { return mMonitor; }
        virtual bool getClientSize( uint32_t & width, uint32_t & height ) const;

        // ********************************
        // private variables
        // ********************************
    private:

        HWND mWindow;
        HMONITOR mMonitor;
        bool mUseExternalWindow;

        bool mInsideSizeMove;
        bool mSizeChanged;

        // ********************************
        // private functions
        // ********************************
    private:

        bool createWindow( HWND parent, uint32_t width, uint32_t height, bool fullscreen );
        LRESULT windowProc( HWND wnd, UINT msg, WPARAM wp, LPARAM lp );
        static LRESULT staticWindowProc( HWND wnd, UINT msg, WPARAM wp, LPARAM lp );
    };
}}

#endif

// *****************************************************************************
//                           End of ntRenderWindow.h
// *****************************************************************************
#endif // __GN_GFX_NTRENDERWINDOW_H__
