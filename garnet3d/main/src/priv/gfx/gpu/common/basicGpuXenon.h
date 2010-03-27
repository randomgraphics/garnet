#ifndef __GN_GPUCOMMON_BASICGPUXENON_H__
#define __GN_GPUCOMMON_BASICGPUXENON_H__
// *****************************************************************************
/// \file
/// \brief   Basic GPU class
/// \author  chenlee (2005.10.1)
// *****************************************************************************

#ifdef GN_XENON

#include "basicGpu.h"

namespace GN { namespace gfx
{
    ///
    /// basic GPU class on Xenon platform
    ///
    class BasicGpuXenon : public BasicGpu
    {
        GN_DECLARE_STDCLASS( BasicGpuXenon, BasicGpu );

        // ********************************
        // constructor/destructor
        // ********************************

        //@{
    public:
        BasicGpuXenon() { Clear(); }
        virtual ~BasicGpuXenon() {}
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

        virtual void handleRenderWindowSizeMove() { /* do nothing */ }

    private:

        GpuOptions   mOptions;
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

        virtual void ProcessRenderWindowMessages( bool ) {} // do nothing on Xenon platform

        //@}
    };
}}

#endif

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GPUCOMMON_BASICGPUXENON_H__
