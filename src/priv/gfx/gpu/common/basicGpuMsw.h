#ifndef __GN_GPUCOMMON_BASICGPUMSW_H__
#define __GN_GPUCOMMON_BASICGPUMSW_H__
// *****************************************************************************
/// \file
/// \brief   Basic GPU class
/// \author  chenlee (2005.10.1)
// *****************************************************************************

#if GN_MSWIN && !GN_XENON

#include "basicGpu.h"
#include "renderWindowMsw.h"

namespace GN { namespace gfx
{
    ///
    /// basic GPU class
    ///
    class BasicGpuMsw : public BasicGpu
    {
        GN_DECLARE_STDCLASS( BasicGpuMsw, BasicGpu );

        // ********************************
        // constructor/destructor
        // ********************************

        //@{
    public:
        BasicGpuMsw() { Clear(); }
        virtual ~BasicGpuMsw() {}
        //@}

        // ********************************
        // standard init / quit
        // ********************************

        //@{
    public:
        bool Init( const GpuOptions & );
        void Quit();
    private :
        void Clear()
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

        virtual const GpuOptions & GetOptions() const { return mOptions; }
        virtual const DispDesc        & GetDispDesc() const { return mDispDesc; }

    private:
        bool dispInit( const GpuOptions & );
        void dispQuit();
        void dispClear() {}

    protected:

        virtual void handleRenderWindowSizeMove();

        RenderWindowMsw & getRenderWindow() { return mWindow; }

    private:

        GpuOptions   mOptions;
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

        virtual void ProcessRenderWindowMessages( bool blockWhileMinimized );

        //@}
    };
}}

#endif

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GPUCOMMON_BASICGPUMSW_H__
