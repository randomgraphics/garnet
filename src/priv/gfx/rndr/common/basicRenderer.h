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
        bool init( const RendererOptions & );
        void quit();
    private :
        void clear()
        {
            contextClear();
            miscClear();
        }
        //@}

        // *****************************************************************************
        //
        /// \name                   Display Manager
        //
        // *****************************************************************************

        //@{

     protected:

        ///
        /// Called by sub class to respond to render window resizing/moving
        ///
        virtual void handleRenderWindowSizeMove() = 0;

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

        virtual bool bindContextImpl( const RendererContext & context, bool skipDirtyCheck ) = 0;

    private:

        void contextClear() { mContextOk = false; }

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
