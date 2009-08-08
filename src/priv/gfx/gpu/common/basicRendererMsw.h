#ifndef __GN_GFXCOMMON_BASICRENDERERMSW_H__
#define __GN_GFXCOMMON_BASICRENDERERMSW_H__
// *****************************************************************************
/// \file
/// \brief   Basic renderer class
/// \author  chenlee (2005.10.1)
// *****************************************************************************

#if GN_MSWIN && !GN_XENON

#include "basicRenderer.h"
#include "renderWindowMsw.h"

namespace GN { namespace gfx
{
    ///
    /// basic renderer class
    ///
    class BasicRendererMsw : public BasicRenderer
    {
        GN_DECLARE_STDCLASS( BasicRendererMsw, BasicRenderer );

        // ********************************
        // constructor/destructor
        // ********************************

        //@{
    public:
        BasicRendererMsw() { clear(); }
        virtual ~BasicRendererMsw() {}
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
        void dispClear() {}

    protected:

        virtual void handleRenderWindowSizeMove();

        RenderWindowMsw & getRenderWindow() { return mWindow; }

    private:

        RendererOptions   mOptions;
        DispDesc          mDispDesc;
        RenderWindowMsw   mWindow;  ///< Render window instance.
        WinProp           mWinProp; ///< Render window properites.

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

        virtual void processRenderWindowMessages( bool blockWhileMinimized );

        //@}
    };
}}

#endif

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFXCOMMON_BASICRENDERERMSW_H__
