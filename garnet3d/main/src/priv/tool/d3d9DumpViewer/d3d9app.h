#ifndef __GN_GFX_D3D9APP_H__
#define __GN_GFX_D3D9APP_H__
// *****************************************************************************
//! \file    d3d9DumpViewer/d3d9app.h
//! \brief   interface of d3d9 application framework
//! \author  chen@@CHENLI-HOMEPC (2007.4.16)
// *****************************************************************************

#include "garnet/GNgfx.h"

namespace GN { namespace gfx { namespace d3d9
{
    ///
    /// scoped PIX event
    ///
    struct PixPerfScopeEvent
    {
        //@{
        PixPerfScopeEvent( D3DCOLOR color, const wchar_t * name )
        {
            D3DPERF_BeginEvent( color, name );
        }
        ~PixPerfScopeEvent()
        {
            D3DPERF_EndEvent();
        }
        //@}
    };

    ///
    /// D3D9 application framework
    ///
    struct D3D9AppOption
    {
        ///
        /// ctor
        ///
        D3D9AppOption()
            : parent(0)
            , monitor(0)
            , vsync(false)
            , fullscreen(false)
            , windowedWidth(640)
            , windowedHeight(480)
        {
            dm.set(0,0,0,0);
        }

        ///
        /// Handle of parent window. Default is zero, means a top-level window.
        ///
        HWND parent;

        ///
        /// Monitor handle.
        ///
        /// - 0 means using the monitor where parent window stays in.
        ///   If monitor and parent are both zero, primary monitor will be used.
        /// - Default is 0.
        ///
        HMONITOR monitor;

        ///
        /// V-SYNC enable or not. Default is false.
        ///
        bool vsync;

        ///
        /// fullscreen or windowed mode.
        /// Default is false.
        ///
        bool fullscreen;

        ///
        /// Display mode for fullscreen mode. Ignored in windowed mode.
        ///
        /// \note For field equals zero, current display setting will be used.
        ///
        DisplayMode dm;

        ///
        /// Backbuffer width for windowed mode. Ignored in fullscreen mode.
        /// Zero means using client width of render window. If render window
        /// is also not avaiable, default width 640 will be used.
        /// Default is 0.
        ///
        UInt32 windowedWidth;

        ///
        /// Backbuffer height for windowed mode. Ignored in fullscreen mode.
        /// Zero means using client height of render window. If render window
        /// is also not avaiable, default height 480 will be used.
        /// Default is 0.
        ///
        UInt32 windowedHeight;
    };

    ///
    /// D3D9 application framework
    ///
    class D3D9Application
    {
    public:

        //@{

        D3D9Application();
        ~D3D9Application();

        IDirect3DDevice9 & d3d9dev() const { GN_ASSERT( mDevice ); return *mDevice; }

        int run( const D3D9AppOption * = 0 );

        bool changeOption( const D3D9AppOption & );

        //@}

    protected:

        //@{

        virtual bool onInit( D3D9AppOption & ) { return true; }
        virtual bool onCreate() { return true; }
        virtual bool onRestore() { return true; }
        virtual void onDispose() {}
        virtual void onDestroy() {}
        virtual void onQuit() {}

        virtual void onDraw() {}
        virtual void onUpdate() {}

        //@}

    private:

        bool init();
        void quit();

        bool createDevice();
        bool restoreDevice();
        void disposeDevice();
        void destroyDevice();

    private:

        D3D9AppOption         mOption;
        HWND                  mWindow;
        UINT                  mAdapter;
        D3DDEVTYPE            mDeviceType;
        UINT                  mBehavior;
        D3DPRESENT_PARAMETERS mPresentParameters;
        IDirect3D9          * mD3D;
        IDirect3DDevice9    * mDevice;
        bool                  mRunning;
    };
}}}

// *****************************************************************************
//                           End of d3d9app.h
// *****************************************************************************
#endif // __GN_GFX_D3D9APP_H__
