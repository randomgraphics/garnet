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

///
/// Rest-in-peace macro
///
#define GN_RNDR_RIP GN::gfx::rip

namespace GN { namespace gfx
{
    ///
    /// reset in peace...
    ///
    void rip( const char * msg, ... );

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

        virtual void bindContext( const RendererContext & c );
        virtual void rebindContext();
        virtual inline const RendererContext & getContext() const { return mContext; }

    protected:

        virtual bool bindContextImpl( const RendererContext & context, bool forceBinding ) = 0;

    protected:

        RendererContext mContext;
        bool            mContextOk;

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

        virtual void         processRenderWindowMessages( bool blockWhileMinimized );
        virtual void         enableParameterCheck( bool enable ) { mParamCheckEnabled = enable; }
        virtual void         setUserData( const Guid & id, const void * data, size_t length );
        virtual const void * getUserData( const Guid & id, size_t * length ) const;
        virtual bool         hasUserData( const Guid & id ) const;

        bool                 paramCheckEnabled() const { return mParamCheckEnabled; }

    private:

        void miscClear()
        {
            mParamCheckEnabled = GN_BUILD_DEBUG;
            mUserData.clear();
        }

    private:

        typedef std::map<Guid,DynaArray<UInt8> > UserDataMap;

        bool mParamCheckEnabled;

        UserDataMap mUserData;

        //@}
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFXCOMMON_BASICRENDERER_H__
