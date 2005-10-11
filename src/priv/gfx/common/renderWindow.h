#ifndef __GN_GFX_RENDERWINDOW_H__
#define __GN_GFX_RENDERWINDOW_H__
// *****************************************************************************
//! \file    renderWindow.h
//! \brief   Common render window interface
//! \author  chenlee (2005.10.10)
// *****************************************************************************

namespace GN { namespace gfx {

    class RenderWindow
    {
    public:

        //!
        //! Destructor
        //!
        virtual ~RenderWindow() {}

        //!
        //! initialize the render window based on current device setting.
        //!
        virtual bool init( const DeviceSettings & ) = 0;

        //!
        //! finalize the render window
        //!
        virtual void quit() = 0;

        //!
        //! Get window handle
        //!
        virtual void * getWindow() const = 0;

        //!
        //! Get monitor handle
        //!
        virtual void * getMonitor() const = 0;

        //!
        //! Get client size
        //!
        virtual bool getClientSize( uint32_t & width, uint32_t & height ) const = 0;
    };
}}

// *****************************************************************************
//                           End of renderWindow.h
// *****************************************************************************
#endif // __GN_GFX_RENDERWINDOW_H__
