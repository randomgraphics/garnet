#ifndef __GN_D3DAPP_GND3D_H__
#define __GN_D3DAPP_GND3D_H__
// *****************************************************************************
//! \file    GNd3d.h
//! \brief   Public interface for d3d module
//! \author  chenlee (2005.8.11)
// *****************************************************************************

#include "garnet/GNbase.h"

#if GN_DEBUG
#define D3D_DEBUG_INFO // Enable "Enhanced D3DDebugging"
#endif

#if GN_XENON
#include <xtl.h>
#include <xgraphics.h>
#include <xboxmath.h>
#elif GN_PC
#define NOMINMAX //!< This is to disable windows min/max macros
#include <windows.h>
#endif

#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr9.h>

// Check dx version
#if DIRECT3D_VERSION < 0x0900
#error "directx 9.0 or higher is required!"
#endif

//!
//! d3d module error log macro
//!
#define GND3D_ERROR GN_ERROR

//!
//! d3d module warning log macro
//!
#define GND3D_WARN GN_WARN

//!
//! d3d module informational log macro
//!
#define GND3D_INFO GN_INFO

//!
//! DX error check routine
//!
#define DX_CHECK_DO( func, something )              \
    if( true ) {                                    \
        HRESULT rr = func;                          \
        if (D3D_OK != rr)                           \
        {                                           \
            GND3D_ERROR( DXGetErrorString9A(rr) );  \
            something                               \
        }                                           \
    } else void(0)

//!
//! DX error check routine
//!
#if GN_DEBUG
#define DX_CHECK( func )         DX_CHECK_DO( func, )
#else
#define DX_CHECK( func )         func
#endif

//!
//! DX error check routine
//!
#define DX_CHECK_R( func )        DX_CHECK_DO( func, return; )

//!
//! DX error check routine
//!
#define DX_CHECK_RV( func, rval ) DX_CHECK_DO( func, return rval; )

//!
//! Global D3D object. Must instantiate and initialize it before using d3d module.
//!
#define gD3D (::GN::d3d::D3D::getInstance())

namespace GN
{
    //!
    //! Namespace for d3d module
    //!
    namespace d3d
    {
        //!
        //! D3D initialization parameters
        //!
        struct D3DInitParams
        {
            bool refDevice;  //!< use reference device.
            bool fullScreen; //!< use fullscreen mode.
            bool hwvp;       //!< use hardware vertex processing
            bool showWindow; //!< show render window auotmatically while starting up.
            uint32_t width;  //!< render window width.
            uint32_t height; //!< render window height.
            const char * winTitle; //!< render window title.

            //!
            //! Default constructor
            //!
            D3DInitParams()
                : refDevice(false)
                , fullScreen(false)
                , hwvp(true)
                , showWindow(true)
                , width(640)
                , height(480)
                , winTitle("D3DApp")
            {}
        };
        
        //!
        //! Main d3d class
        //!
        class D3D : public StdClass, public Singleton<D3D>
        {
             GN_DECLARE_STDCLASS( D3D, StdClass );

            // ********************************
            //! \name  ctor/dtor
            // ********************************

            //@{
        public:
            D3D()          { clear(); }
            virtual ~D3D() { quit(); }
            //@}

            // ********************************
            //! \name standard init/quit
            // ********************************

            //@{
        public:
            bool init( const D3DInitParams & param );
            void quit();
            bool ok() const { return MyParent::ok(); }
        private:
            void clear()
            {
                mWindow = 0;
                mD3D = 0;
                mDevice = 0;
            }
            //@}

            // ********************************
            // public signals
            // ********************************
        public:

            Signal0<bool> sigDeviceCreate;
            Signal0<bool> sigDeviceRestore;
            Signal0<void> sigDeviceInvalidate;
            Signal0<void> sigDeviceDestroy;

#define GN_D3DSIG_CONNECT_TO(PTR,CLASS) \
    gD3D.sigDeviceCreate.connect( PTR, &CLASS::onDeviceCreate ); \
    gD3D.sigDeviceRestore.connect( PTR, &CLASS::onDeviceRestore ); \
    gD3D.sigDeviceInvalidate.connect( PTR, &CLASS::onDeviceInvalidate ); \
    gD3D.sigDeviceDestroy.connect( PTR, &CLASS::onDeviceDestroy );

            // ********************************
            // public functions
            // ********************************
        public:

            //!
            //! Get render window handle
            //!
            HWND getWindow() const { return mWindow; }

            //!
            //! Get D3D object
            //!
            IDirect3D9 * getD3D() const { GN_ASSERT(mD3D); return mD3D; }

            //!
            //! Get device object
            //!
            IDirect3DDevice9 * getDevice() const { GN_ASSERT(mDevice); return mDevice; }

            //!
            //! Get D3D monitor handle
            //!
            HMONITOR getMonitor() const { return mMonitor; }

            //!
            //! Get D3D adapter ID
            //!
            uint32_t getAdapter() const { return mAdapter; }

            //!
            //! Get D3D device behavior flags
            uint32_t getBehaviorFlags() const { return mBehaviorFlags; }

            //!
            //! Get D3D present parameters
            //!
            const D3DPRESENT_PARAMETERS & getPresentParams() const { return mPresentParams; }

            //!
            //! Get D3D device caps
            //!
            const D3DCAPS9 & getDevCaps() const { return mDevCaps; }

            //!
            //! Is render window closed?
            //!
            bool windowClosed() const { return mClosed; }

            //!
            //! Do present. Please always call this function,
            //! instead of IDirect3DDevice::Present().
            //!
            //! This function handles device-lost, windows messages.
            //!
            //! \return Return false, if device restore/recreation is failed.
            //!
            bool present();

            // ********************************
            // private variables
            // ********************************
        private:

            D3DInitParams           mInitParams;
            HWND                    mWindow;
            HMONITOR                mMonitor;
            uint32_t                mAdapter;
            D3DDEVTYPE              mDevType;
            uint32_t                mBehaviorFlags;
            D3DPRESENT_PARAMETERS   mPresentParams;
            D3DCAPS9                mDevCaps;
            LPDIRECT3D9             mD3D;
            LPDIRECT3DDEVICE9       mDevice;

            bool mClosed;
            bool mMinimized;
            bool mInsideSizeMove;
            bool mSizeChanged;

            // ********************************
            // private functions
            // ********************************
        private:

            bool createWindow();
            bool createD3D();
            void destroyD3D();

            void setupPresentParameters();
            bool restoreDevice();
            bool recreateDevice();

            void processWindowMessages();

            LRESULT windowProc( HWND, UINT, WPARAM, LPARAM );
            static LRESULT staticProc( HWND, UINT, WPARAM, LPARAM );
        };

        typedef ResourceManager<LPDIRECT3DBASETEXTURE9>  TextureManager; //!< Texture manager
        typedef ResourceManager<LPDIRECT3DVERTEXSHADER9> VShaderManager; //!< Vertex shader manager
        typedef ResourceManager<LPDIRECT3DPIXELSHADER9>  PShaderManager; //!< Pixel shader manager
        typedef ResourceManager<LPD3DXEFFECT> EffectManager; //!< Effect manager
        typedef ResourceManager<LPD3DXEFFECT> EffectManager; //!< Effect manager
        typedef ResourceManager<LPD3DXMESH> MeshManager; //!< Mesh manager

        //! \name Global variables
        //@{
        extern TextureManager gTexMgr; //!< Global texture manager
        extern VShaderManager gVSMgr; //!< Global vertex shader manager
        extern PShaderManager gPSMgr; //!< Global pixel shader manager
        extern EffectManager gEffectMgr; //!< Global effect manager
        extern MeshManager gMeshMgr; //!< Global mesh manager
        //@}

        //! \name Util functions
        //@{

        //!
        //! Compile vertex shader from string
        //!
        LPDIRECT3DVERTEXSHADER9 compileVS( const char * code, size_t len, uint32_t flags = 0, const char * entryFunc = "main" );

        //!
        //! Compile vertex shader from file
        //!
        LPDIRECT3DVERTEXSHADER9 compileVSFromFile( const char * file, uint32_t flags = 0, const char * entryFunc = "main" );

        //!
        //! Assemble vertex shader from string
        //!
        LPDIRECT3DVERTEXSHADER9 assembleVS( const char * code, size_t len, uint32_t flags = 0 );

        //!
        //! Assemble vertex shader from file
        //!
        LPDIRECT3DVERTEXSHADER9 assembleVSFromFile( const char * file, uint32_t flags = 0 );

        //!
        //! Compile pixel shader from string
        //!
        LPDIRECT3DPIXELSHADER9 compilePS( const char * code, size_t len, uint32_t flags = 0, const char * entryFunc = "main" );

        //!
        //! Compile pixel shader from file
        //!
        LPDIRECT3DPIXELSHADER9 compilePSFromFile( const char * file, uint32_t flags = 0, const char * entryFunc = "main" );

        //!
        //! Assemble pixel shader from string
        //!
        LPDIRECT3DPIXELSHADER9 assemblePS( const char * code, size_t len, uint32_t flags = 0 );

        //!
        //! Assemble pixel shader from file
        //!
        LPDIRECT3DPIXELSHADER9 assemblePSFromFile( const char * file, uint32_t flags = 0 );

#if GN_XENON

#ifndef D3DXSHADER_MICROCODE_TARGET_FINAL
#define D3DXSHADER_MICROCODE_TARGET_FINAL 0
#endif

        //!
        //! Assemble XVS shader from string
        //!
        inline LPDIRECT3DVERTEXSHADER9 assembleXVS( const char * code, size_t len, uint32_t flags = 0 )
        {
	        GN_GUARD;
	        return assembleVS( code, len, flags | D3DXSHADER_MICROCODE_TARGET_FINAL );
	        GN_UNGUARD;
        }

        //!
        //! Assemble XVS shader from file
        //!
        inline LPDIRECT3DVERTEXSHADER9 assembleXVSFromFile( const char * file, uint32_t flags = 0 )
        {
	        GN_GUARD;
	        return assembleVSFromFile( file, flags | D3DXSHADER_MICROCODE_TARGET_FINAL );
	        GN_UNGUARD;
        }

        //!
        //! Assemble XPS shader from string
        //!
        inline LPDIRECT3DPIXELSHADER9 assembleXPS( const char * code, size_t len, uint32_t flags = 0 )
        {
	        GN_GUARD;
	        return assemblePS( code, len, flags | D3DXSHADER_MICROCODE_TARGET_FINAL );
	        GN_UNGUARD;
        }

        //!
        //! Assemble XPS shader from file
        //!
        inline LPDIRECT3DPIXELSHADER9 assembleXPSFromFile( const char * file, uint32_t flags = 0 )
        {
	        GN_GUARD;
	        return assemblePSFromFile( file, flags | D3DXSHADER_MICROCODE_TARGET_FINAL );
	        GN_UNGUARD;
        }
#endif

        //!
        //! Draw screen aligned quad on screen
        //!
        void drawScreenAlignedQuad(
            double fLeft = 0.0, double fTop = 0.0, double fRight = 1.0, double fBottom = 1.0,
            double fLeftU = 0.0, double fTopV = 0.0, double fRightU = 1.0, double fBottomV = 1.0 );

        //@}
    }
}

// *****************************************************************************
//                           End of GNd3d.h
// *****************************************************************************
#endif // __GN_D3DAPP_GND3D_H__
