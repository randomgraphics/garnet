#ifndef __GN_GFXCOMMON_BASICRENDERER_H__
#define __GN_GFXCOMMON_BASICRENDERER_H__
// *****************************************************************************
//! \file    BasicRenderer.h
//! \brief   Basic renderer class
//! \author  chenlee (2005.10.1)
// *****************************************************************************

//!
//! Global renderer instance
//!
#define gBasicRenderer (::GN::gfx::BasicRenderer::getInstance())

//!
//! trace the call sequence of device reset/recreate
//!
#define _GN_RENDER_DEVICE_TRACE()  //GN_TRACE( GN_FUNCTION_NAME )

namespace GN { namespace gfx
{
    //!
    //! basic renderer class
    //!
    class BasicRenderer : public Renderer,
                          public StdClass,
                          public LocalSingleton<BasicRenderer>,
                          public SlotBase
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
        //! \name standard init / quit
        // ********************************

        //@{
    public:
        bool init();
        void quit();
        bool ok() const
        {
            return MyParent::ok()
                && dispOk();
        }
    private :
        void clear()
        {
            dispClear();
        }
        //@}

    // ****************************************************************************
    //
    //! \name                   Device manager
    //
    // ****************************************************************************

        //@{

    protected:
        bool deviceCreate( const DeviceSettings & ds );
        bool deviceRestore( const DeviceSettings & ds );
        void deviceDispose();
        void deviceDestroy();

        //@}

    // ****************************************************************************
    //
    //! \name                   Display Manager
    //
    // ****************************************************************************

        //@{

    public:

    private :
        bool dispInit() { return true; }
        void dispQuit() { /* do nothing */ }
        bool dispOk() const { return true; }
        void dispClear()    { invalidateScreenDesc(); }

    private:
        bool dispDeviceCreate();
        bool dispDeviceRestore();
        void dispDeviceDispose() { invalidateScreenDesc(); }
        void dispDeviceDestroy() {}

        //@}
    };
}}

// *****************************************************************************
//                           End of BasicRenderer.h
// *****************************************************************************
#endif // __GN_GFXCOMMON_BASICRENDERER_H__
