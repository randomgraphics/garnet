#ifndef __GN_GFXCOMMON_BASICRENDERERXENON_H__
#define __GN_GFXCOMMON_BASICRENDERERXENON_H__
// *****************************************************************************
/// \file
/// \brief   Basic renderer class
/// \author  chenlee (2005.10.1)
// *****************************************************************************

#ifdef GN_XENON

#include "basicRenderer.h"

namespace GN { namespace gfx
{
    ///
    /// basic renderer class on Xenon platform
    ///
    class BasicRendererXenon : public BasicRenderer
    {
        GN_DECLARE_STDCLASS( BasicRendererXenon, BasicRenderer );

        // ********************************
        // constructor/destructor
        // ********************************

        //@{
    public:
        BasicRendererXenon() { clear(); }
        virtual ~BasicRendererXenon() {}
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

        virtual void handleRenderWindowSizeMove() { /* do nothing */ }

    private:

        RendererOptions   mOptions;
        DispDesc          mDispDesc;

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

        virtual void processRenderWindowMessages( bool ) {} // do nothing on Xenon platform

        //@}
    };
}}

#endif

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFXCOMMON_BASICRENDERERXENON_H__
