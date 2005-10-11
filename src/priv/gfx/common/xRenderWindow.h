#ifndef __GN_GFX_XRENDERWINDOW_H__
#define __GN_GFX_XRENDERWINDOW_H__
// *****************************************************************************
//! \file    xRenderWindow.h
//! \brief   Window class on X XRenderWindow platform
//! \author  chenlee (2005.10.4)
// *****************************************************************************

#if GN_POSIX

#include "renderWindow.h"

namespace GN { namespace gfx {
    //!
    //! Window class on POSIX platform (Unimplemented)
    //!
    class XRenderWindow : public RenderWindow
    {
        //@{
    public:
        XRenderWindow()  {}
        ~XRenderWindow() {}
        //@}

        // ********************************
        // from Renderer
        // ********************************
    public:

        virtual bool init( const DeviceSettings & ) { return true; }
        virtual void quit() {}
        virtual void * getWindow() const { return (void*)1; }
        virtual void * getMonitor() const { return (void*)1; }
        virtual bool getClientSize( uint32_t & width , uint32_t & height ) { width = 640; height = 480; return true; }
    };
}}

#endif // GN_POSIX

// *****************************************************************************
//                           End of xRenderWindow.h
// *****************************************************************************
#endif // __GN_GFX_XRENDERWINDOW_H__
