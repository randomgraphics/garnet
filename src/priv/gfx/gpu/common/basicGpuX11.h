#ifndef __GN_GPUCOMMON_BASICGPUX11_H__
#define __GN_GPUCOMMON_BASICGPUX11_H__
// *****************************************************************************
/// \file
/// \brief   Basic GPU class for X Windows
/// \author  chenlee (2009.4.25)
// *****************************************************************************

#if GN_POSIX

#include "basicGpu.h"
#include "renderWindowX11.h"

namespace GN { namespace gfx
{
    ///
    /// basic GPU class
    ///
    class BasicGpuX11 : public BasicGpu
    {
        GN_DECLARE_STDCLASS( BasicGpuX11, BasicGpu );

        // ********************************
        // constructor/destructor
        // ********************************

        //@{
    public:
        BasicGpuX11() { clear(); }
        virtual ~BasicGpuX11() {}
        //@}

        // ********************************
        // standard init / quit
        // ********************************

        //@{
    public:
        bool init( const GpuOptions & );
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

        virtual const GpuOptions & getOptions() const { return mOptions; }
        virtual const DispDesc        & getDispDesc() const { return mDispDesc; }

    private:
        bool dispInit( const GpuOptions & );
        void dispQuit();
        void dispClear() { mDefaultDisplay = 0; }

    protected:

        virtual void handleRenderWindowSizeMove();

        RenderWindowX11 & getRenderWindow() { return mWindow; }

    private:

        GpuOptions   mOptions;
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
#endif // __GN_GPUCOMMON_BASICGPUX11_H__
