#ifndef __GN_GPUCOMMON_BASICGPUMSW_H__
#define __GN_GPUCOMMON_BASICGPUMSW_H__
// *****************************************************************************
/// \file
/// \brief   Basic GPU class
/// \author  chenlee (2005.10.1)
// *****************************************************************************

#if GN_WINPC

#include "basicGpu.h"

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
        BasicGpuMsw() { clear(); }
        virtual ~BasicGpuMsw() {}
        //@}

        // ********************************
        // standard init / quit
        // ********************************

        //@{
    public:
        bool init( const GpuOptions & );
        void quit();
    private:
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

    protected:
        virtual intptr_t getDefaultDisplay() { return 1; }
        virtual intptr_t determineMonitor(const GpuOptions & go, intptr_t display);
        virtual bool getCurrentDisplayMode(DisplayMode & dm, intptr_t display, intptr_t monitor);

        ///
        /// This signal will be triggered, whenever the render window receive a message.
        ///
        Signal4<void,HWND,UINT,WPARAM,LPARAM> sigMessage;

    private:
        bool dispInit();
        void dispQuit();
        void dispClear() { mHook = 0; }

        static  LRESULT CALLBACK staticHookProc( int code, WPARAM wp, LPARAM lp );

    private:

        HHOOK mHook;

        typedef Dictionary<intptr_t,BasicGpuMsw*> WindowMap;
        static WindowMap msInstanceMap;

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

        //@}
    };
}}

#endif

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GPUCOMMON_BASICGPUMSW_H__
