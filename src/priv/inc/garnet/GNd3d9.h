#ifndef __GN_D3D9_H__
#define __GN_D3D9_H__
// *****************************************************************************
/// \file
/// \brief   d3d9 utils
/// \author  chen@@CHENLI-HOMEPC (2007.4.16)
// *****************************************************************************

#include "GNbase.h"

#if GN_BUILD_DEBUG
#define D3D_DEBUG_INFO // Enable "Enhanced D3DDebugging"
#endif

#if GN_XENON
#include <xtl.h>
#include <xgraphics.h>
#include <xboxmath.h>
#elif GN_PC
#ifndef NOMINMAX
#define NOMINMAX ///< This is to disable windows min/max macros
#endif
#include <windows.h>
#endif

#include <d3d9.h>
#include <d3dx9.h>

// Check d3d version
#if DIRECT3D_VERSION < 0x0900
#error "directx 9.0 or higher is required!"
#endif

namespace GN { /* namespace for D3D9 utils */ namespace d3d9
{
    ///
    /// scoped PIX event
    ///
    struct PixPerfScopeEvent
    {
        //@{
        PixPerfScopeEvent( D3DCOLOR color, const char * name )
        {
#if GN_XENON
            PIXBeginNamedEvent( color, name );
#else
            D3DPERF_BeginEvent( color, mbs2wcs(name).cptr() );
#endif
        }
        ~PixPerfScopeEvent()
        {
#if GN_XENON
            PIXEndNamedEvent();
#else
            D3DPERF_EndEvent();
#endif
        }
        //@}
    };

    ///
    /// Compile vertex shader from string
    ///
    LPDIRECT3DVERTEXSHADER9 compileAndCreateVS( LPDIRECT3DDEVICE9 dev, const char * code, size_t len = 0, UInt32 flags = 0, const char * entryFunc = "main", const char * profile = 0, LPD3DXCONSTANTTABLE * constTable = 0 );

    ///
    /// Compile vertex shader from file
    ///
    LPDIRECT3DVERTEXSHADER9 compileAndCreateVSFromFile( LPDIRECT3DDEVICE9 dev, const char * file, UInt32 flags = 0, const char * entryFunc = "main", const char * profile = 0, LPD3DXCONSTANTTABLE * constTable = 0 );

    ///
    /// Assemble vertex shader from string
    ///
    LPDIRECT3DVERTEXSHADER9 assembleAndCreateVS( LPDIRECT3DDEVICE9 dev, const char * code, size_t len = 0, UInt32 flags = 0 );

    ///
    /// Assemble vertex shader from file
    ///
    LPDIRECT3DVERTEXSHADER9 assembleAndCreateVSFromFile( LPDIRECT3DDEVICE9 dev, const char * file, UInt32 flags = 0 );

    ///
    /// Compile pixel shader from string
    ///
    LPDIRECT3DPIXELSHADER9 compileAndCreatePS( LPDIRECT3DDEVICE9 dev, const char * code, size_t len = 0, UInt32 flags = 0, const char * entryFunc = "main", const char * profile = 0, LPD3DXCONSTANTTABLE * constTable = 0 );

    ///
    /// Compile pixel shader from file
    ///
    LPDIRECT3DPIXELSHADER9 compileAndCreatePSFromFile( LPDIRECT3DDEVICE9 dev, const char * file, UInt32 flags = 0, const char * entryFunc = "main", const char * profile = 0, LPD3DXCONSTANTTABLE * constTable = 0 );

    ///
    /// Assemble pixel shader from string
    ///
    LPDIRECT3DPIXELSHADER9 assembleAndCreatePS( LPDIRECT3DDEVICE9 dev, const char * code, size_t len = 0, UInt32 flags = 0 );

    ///
    /// Assemble pixel shader from file
    ///
    LPDIRECT3DPIXELSHADER9 assembleAndCreatePSFromFile( LPDIRECT3DDEVICE9 dev, const char * file, UInt32 flags = 0 );

    //@{

    ///
    /// D3D9 shader compiler template
    ///
    template<class SHADER_CLASS>
    struct ShaderCompiler
    {
    };

    ///
    /// D3D9 vertex shader compiler
    ///
    template<>
    struct ShaderCompiler<IDirect3DVertexShader9>
    {
        //@{

        static inline IDirect3DVertexShader9 *
        assembleAndCreate( IDirect3DDevice9 & dev, const char * code, size_t len = 0, UInt32 flags = 0 )
        {
            return assembleAndCreateVS( &dev, code, len, flags );
        }

        static inline IDirect3DVertexShader9 *
        compileAndCreate( IDirect3DDevice9 & dev, const char * code, size_t len = 0, UInt32 flags = 0, const char * entryFunc = "main", const char * profile = 0, LPD3DXCONSTANTTABLE * constTable = 0 )
        {
            return compileAndCreateVS( &dev, code, len, flags, entryFunc, profile, constTable );
        }

        static inline IDirect3DVertexShader9 *
        compileAndCreateFromFile( IDirect3DDevice9 & dev, const char * file, UInt32 flags = 0, const char * entryFunc = "main", const char * profile = 0, LPD3DXCONSTANTTABLE * constTable = 0 )
        {
            return compileAndCreateVSFromFile( &dev, file, flags, entryFunc, profile, constTable );
        }

        //@}
    };

    ///
    /// D3D9 pixel shader compiler
    ///
    template<>
    struct ShaderCompiler<IDirect3DPixelShader9>
    {
        //@{

        static inline IDirect3DPixelShader9 *
        assembleAndCreate( IDirect3DDevice9 & dev, const char * code, size_t len = 0, UInt32 flags = 0 )
        {
            return assembleAndCreatePS( &dev, code, len, flags );
        }

        static inline IDirect3DPixelShader9 *
        compileAndCreate( IDirect3DDevice9 & dev, const char * code, size_t len = 0, UInt32 flags = 0, const char * entryFunc = "main", const char * profile = 0, LPD3DXCONSTANTTABLE * constTable = 0 )
        {
            return compileAndCreatePS( &dev, code, len, flags, entryFunc, profile, constTable );
        }

        //@}
    };

    ///
    /// Compile effect from string
    ///
    LPD3DXEFFECT compileAndCreateEffect( LPDIRECT3DDEVICE9 dev, const char * code, size_t len = 0, UInt32 flags = 0, LPD3DXEFFECTPOOL pool = 0 );


    ///
    /// Get backbuffer descriptor
    ///
    inline bool getBackBufferDesc( LPDIRECT3DDEVICE9 dev, D3DSURFACE_DESC & desc )
    {
        GN_GUARD_SLOW;
        GN_ASSERT( dev );
        AutoComPtr<IDirect3DSurface9> surf;
        GN_DX_CHECK_RETURN( dev->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &surf ), false );
        GN_DX_CHECK_RETURN( surf->GetDesc( &desc ), false );
		return true;
        GN_UNGUARD_SLOW;
    }

    ///
    /// Get backbuffer size
    ///
    inline Vector2<UInt32> getBackBufferSize( LPDIRECT3DDEVICE9 dev )
    {
        GN_GUARD_SLOW;
        D3DSURFACE_DESC desc;
        return getBackBufferDesc( dev, desc )
            ? Vector2<UInt32>( desc.Width, desc.Height )
            : Vector2<UInt32>( 0, 0 );
        GN_UNGUARD_SLOW;
    }

    ///
    /// Get depth buffer descriptor
    ///
    inline bool getDepthStencilDesc( LPDIRECT3DDEVICE9 dev, D3DSURFACE_DESC & desc )
    {
        GN_GUARD_SLOW;
        GN_ASSERT( dev );
        AutoComPtr<IDirect3DSurface9> surf;
        GN_DX_CHECK_RETURN( dev->GetDepthStencilSurface( &surf ), false );
        GN_DX_CHECK_RETURN( surf->GetDesc( &desc ), false );
		return true;
        GN_UNGUARD_SLOW;
    }

    ///
    /// Get depth buffer size
    ///
    inline Vector2<UInt32> getDepthStencilSize( LPDIRECT3DDEVICE9 dev )
    {
        GN_GUARD_SLOW;
        D3DSURFACE_DESC desc;
        return getDepthStencilDesc( dev, desc )
            ? Vector2<UInt32>( desc.Width, desc.Height )
            : Vector2<UInt32>( 0, 0 );
        GN_UNGUARD_SLOW;
    }

// Note: D3DApplication is not available on Xenon platform yet.
#if !GN_XENON

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
            , refdev(false)
            , vsync(false)
            , fullscreen(false)
            , fsWidth(0)
            , fsHeight(0)
            , fsDepth(0)
            , fsRefrate(0)
            , windowedWidth(640)
            , windowedHeight(480)
        {
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
        /// use ref device. Default is false.
        ///
        bool refdev;

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
        /// full screen display mode
        ///
        //@{
        UInt32 fsWidth;   ///< Screen width.
        UInt32 fsHeight;  ///< Screen height.
        UInt32 fsDepth;   ///< Color depth. Ignored for windowed mode.
        UInt32 fsRefrate; ///< Referesh rate. Ignored for windowed mode.
        //@}

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

#endif // !GN_XENON

}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_D3D9_H__
