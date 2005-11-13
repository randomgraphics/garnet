#ifndef __GN_GFXCOMMON_BASICRENDERER_H__
#define __GN_GFXCOMMON_BASICRENDERER_H__
// *****************************************************************************
//! \file    basicRenderer.h
//! \brief   Basic renderer class
//! \author  chenlee (2005.10.1)
// *****************************************************************************

#include "ntRenderWindow.h"
#include "xRenderWindow.h"

//!
//! trace the call sequence of device reset/recreate
//!
#define _GNGFX_DEVICE_TRACE()  //GNGFX_TRACE( GN_FUNCTION_NAME )

namespace GN { namespace gfx
{
    //!
    //! basic renderer class
    //!
    class BasicRenderer : public Renderer, public StdClass
    {
        GN_DECLARE_STDCLASS( BasicRenderer, StdClass );

        // ********************************
        // constructor/destructor
        // ********************************

        //@{
    public:
        BasicRenderer() { clear(); }
        virtual ~BasicRenderer() {}
        //@}

        // ********************************
        // standard init / quit
        // ********************************

        //@{
    public:
        bool init();
        void quit();
        bool ok() const
        {
            return MyParent::ok()
                && dispOk()
                && drawOk();
        }
    private :
        void clear()
        {
            drawClear();
            dispClear();
        }
        //@}

        // *****************************************************************************
        //
        //! \name                   Device manager
        //
        // *****************************************************************************

        //@{

    protected:
        bool deviceCreate() { return true; }
        bool deviceRestore() { return true; }
        void deviceDispose() {}
        void deviceDestroy() {}

        //@}

        // *****************************************************************************
        //
        //! \name                   Display Manager
        //
        // *****************************************************************************

        //@{

    private :
        bool dispInit() { return true; }
        void dispQuit() { mWindow.quit(); }
        bool dispOk() const { return true; }
        void dispClear() {}

    protected:

        //!
        //! Called by sub-classes to initialize display descriptor
        //! based on device settings.
        //!
        bool processUserOptions( const RendererOptions & );

        //!
        //! Called by sub class to respond to render window resizing/moving
        //!
        bool handleRenderWindowSizeMove();

#if GN_MSWIN
    protected:
        NTRenderWindow mWindow;  //!< Render window instance.
    private:
        WinProp        mWinProp; //!< Render window properites.
#elif GN_POSIX
    private:
        XRenderWindow mWindow;  //!< Render window instance
#endif

        //@}

        // *****************************************************************************
        //
        //! \name                   Draw Manager
        //
        // *****************************************************************************

        //@{
    public:
        virtual void drawTextA( const char * text, int x, int y, const Vector4f & color );

    private:
        bool drawInit()     { return true; }
        void drawQuit()     {}
        bool drawOk() const { return true; }
        void drawClear()    { mNumPrims = 0; mNumDraws = 0; }

    protected:

        //!
        //! Render target texture descriptor
        //!
        struct RenderTargetTextureDesc
        {
            const Texture * tex;  //!< Render target texture.
            TexFace         face; //!< Effective only when tex is cubemap.

            //!
            //! Ctor
            //!
            RenderTargetTextureDesc() : tex(0) {}

            //!
            //! Equality check
            //!
            bool equal( const Texture * t, TexFace f ) const
            {
                return tex == t && ( 0 == tex || face == f );
            }
        };

        //@}

    };
}}

// *****************************************************************************
//                           End of basicRenderer.h
// *****************************************************************************
#endif // __GN_GFXCOMMON_BASICRENDERER_H__
