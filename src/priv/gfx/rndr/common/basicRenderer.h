#ifndef __GN_GFXCOMMON_BASICRENDERER_H__
#define __GN_GFXCOMMON_BASICRENDERER_H__
// *****************************************************************************
/// \file
/// \brief   Basic renderer class
/// \author  chenlee (2005.10.1)
// *****************************************************************************

#include "renderWindowMsw.h"
#include "renderWindowX11.h"

///
/// trace the call sequence of device reset/recreate
///
#define _GNGFX_DEVICE_TRACE()  //GN_TRACE( GN_FUNCTION_NAME )

namespace GN { namespace gfx
{
    ///
    /// basic renderer class
    ///
    class BasicRenderer : public Renderer, public StdClass
    {
        GN_DECLARE_STDCLASS( BasicRenderer, StdClass );

        static Logger * sLogger;

        // ********************************
        // constructor/destructor
        // ********************************

        //@{
    public:
        BasicRenderer()
        {
             mWindow.setRenderer( this );
             clear();
        }
        virtual ~BasicRenderer() {}
        //@}

        // ********************************
        // standard init / quit
        // ********************************

        //@{
    public:
        bool init( const RendererOptions & );
        void quit();
    private :
        void clear()
        {
            dispClear();
            miscClear();
        }
        //@}

        // *****************************************************************************
        //
        /// \name                   Display Manager
        //
        // *****************************************************************************

        //@{

    public:

        virtual const RendererOptions & getOptions() const { return mOptions; }
        virtual const DispDesc        & getDispDesc() const { return mDispDesc; }

    private:
        bool dispInit( const RendererOptions & );
        void dispQuit();
        void dispClear()
        {
#if GN_POSIX
            mDefaultDisplay = 0;
#endif
        }

    protected:

        ///
        /// Called by sub class to respond to render window resizing/moving
        ///
        void handleRenderWindowSizeMove();

#if GN_MSWIN
        RenderWindowMsw & getRenderWindow() { return mWindow; }
#elif GN_POSIX
        RenderWindowX11 & getRenderWindow() { return mWindow; }
#endif

    private:

        RendererOptions   mOptions;
        DispDesc          mDispDesc;

#if GN_MSWIN
        RenderWindowMsw mWindow;  ///< Render window instance.
        WinProp         mWinProp; ///< Render window properites.
#elif GN_POSIX
        Display *       mDefaultDisplay;
        RenderWindowX11 mWindow;  ///< Render window instance
#endif

        //@}

        // *****************************************************************************
        //
        /// \name                   Caps Manager
        //
        // *****************************************************************************

        //@{

        //@}

        // *****************************************************************************
        //
        /// \name                   Resource Manager
        //
        // *****************************************************************************

        //@{

        //@}

        // *****************************************************************************
        //
        /// \name                   Context Manager
        //
        // *****************************************************************************

        //@{

    public:

        virtual void bindContext( const RendererContext & c ) { bindContextImpl( c, false ); mContext = c; }
        virtual void rebindContext() { bindContextImpl( mContext, true ); }
        virtual inline const RendererContext & getContext() const { return mContext; }

    protected:

        virtual void bindContextImpl( const RendererContext & context, bool forceBinding ) = 0;

    protected:

        RendererContext mContext;

        //@}

        // *****************************************************************************
        //
        /// \name                   Draw Manager
        //
        // *****************************************************************************

        //@{

        //@}

        // *****************************************************************************
        //
        /// \name                   Misc Manager
        //
        // *****************************************************************************

        //@{

    public:

        virtual void enableParameterCheck( bool enable ) { mParamCheckEnabled = enable; }

    protected:

        bool paramCheckEnabled() const { return mParamCheckEnabled; }

    private:

        void miscClear()
        {
            mParamCheckEnabled = GN_BUILD_DEBUG;
        }

    private:

        bool mParamCheckEnabled;

        //@}
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFXCOMMON_BASICRENDERER_H__
