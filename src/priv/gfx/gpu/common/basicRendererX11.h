#ifndef __GN_GFXCOMMON_BASICRENDERERX11_H__
#define __GN_GFXCOMMON_BASICRENDERERX11_H__
// *****************************************************************************
/// \file
/// \brief   Basic renderer class for X Windows
/// \author  chenlee (2009.4.25)
// *****************************************************************************

#if GN_POSIX

#include "basicRenderer.h"
#include "renderWindowX11.h"

namespace GN { namespace gfx
{
    ///
    /// basic renderer class
    ///
    class BasicRendererX11 : public BasicRenderer
    {
        GN_DECLARE_STDCLASS( BasicRendererX11, BasicRenderer );

        // ********************************
        // constructor/destructor
        // ********************************

        //@{
    public:
        BasicRendererX11() { clear(); }
        virtual ~BasicRendererX11() {}
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
        void dispClear() { mDefaultDisplay = 0; }

    protected:

        virtual void handleRenderWindowSizeMove();

        RenderWindowX11 & getRenderWindow() { return mWindow; }

    private:

        RendererOptions   mOptions;
        DispDesc          mDispDesc;

        Display         * mDefaultDisplay;
        RenderWindowX11   mWindow;  ///< Render window instance.

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
#endif // __GN_GFXCOMMON_BASICRENDERERX11_H__
