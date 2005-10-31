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

#include "garnet/base/ntWindow.h"

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
#if !defined(D3DCOMPILE_USEVOIDS)
#define DX_CHECK_DO( func, something )              \
    if( true ) {                                    \
        HRESULT rr = func;                          \
        if( FAILED(rr) )                            \
        {                                           \
            GND3D_ERROR( DXGetErrorString9A(rr) );  \
            something                               \
        }                                           \
    } else void(0)
#else
#define DX_CHECK_DO( func, something ) func
#endif

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
//! Global D3D object. Must be instantiated and initialized before using d3d module.
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
            // ctor/dtor
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
                mD3D = 0;
                mDevice = 0;
            }
            //@}

            // ********************************
            //! \name device management signals
            //! - These signales provide standard way for device restoration and
            //!   and recreation.
            //! - During class initialization, sigDeviceCreate and sigDeviceRestore
            //!   will be triggered; during class destory sigDeviceDispose
            //!   and sigDeviceDestroyr will be triggerd.
            //! - signals may be triggered several times in life-time of application,
            //!   but must be in strict order:
            //!   <pre>
            //!                           +--------------+
            //!                           |              |
            //!                          \|/             |
            //!                           '              |
            //!   (start)-->create----->restore------->dispose------>destroy-->(end)
            //!               .                                          |
            //!              /|\                                         |
            //!               |                                          |
            //!               +------------------------------------------+
            //!   </pre>
            // ********************************
        public:

            //@{

            //!
            //! Device creation signal. This signal will be triggered
            //! after D3D device (re)creation.
            //!
            Signal0<bool> sigDeviceCreate;

            //!
            //! Device restore signal. This signal will be triggered
            //! after D3D device is reseted. You may recreate D3D resources in
            //! default pool here.
            //!
            Signal0<bool> sigDeviceRestore;

            //!
            //! Device dispose(lost) signal. This signal will be triggered
            //! before resetting lost device. You should release all D3D resources
            //! in default pool, to prepare for device reset.
            //!
            Signal0<void> sigDeviceDispose;

            //!
            //! Device destroy signal. This signal will be triggered right before
            //! destroy of D3D device. You map release all device-dependent
            //! resources here.
            //!
            Signal0<void> sigDeviceDestroy;

            //@}

            // ********************************
            // public functions
            // ********************************
        public:

            //!
            //! Get render window handle
            //!
            HWND getWindow() const
            {
#if GN_XENON
                return 0;
#else
                return mWindow.getWindow();
#endif
            }

            //!
            //! Get D3D monitor handle
            //!
            HMONITOR getMonitor() const
            {
#if GN_XENON
                return 0;
#else
                return mWindow.getMonitor();
#endif
            }

            //!
            //! Get D3D object
            //!
            IDirect3D9 * getD3D() const { GN_ASSERT(mD3D); return mD3D; }

            //!
            //! Get device object
            //!
            IDirect3DDevice9 * getDevice() const { GN_ASSERT(mDevice); return mDevice; }

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
#if !GN_XENON
            NTWindow                mWindow;
            HMONITOR                mOldMonitor;
#endif
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

            LRESULT winProc( HWND, UINT, WPARAM, LPARAM );
        };

        //!
        //! D3D application framework
        //!
        class App : public NoCopy
        {
            // ********************************
            //! \name public interface
            // ********************************

            //@{
        public:
            App() {}
            virtual ~App() { quit(); }

            void setInitParams( const D3DInitParams & ip ) { mInitParams = ip; }
            void setAppInitFunc( const Functor0<bool> & fp ) { mAppInitFunc = fp; }
            void setDevCreateFunc( const Functor0<bool> & fp ) { mDevCreateFunc = fp; }
            void setDevRestoreFunc( const Functor0<bool> & fp ) { mDevRestoreFunc = fp; }
            void setDevDisposeFunc( const Functor0<void> & fp ) { mDevDisposeFunc = fp; }
            void setDevDestroyFunc( const Functor0<void> & fp ) { mDevDestroyFunc = fp; }
            void setAppQuitFunc( const Functor0<void> & fp ) { mAppQuitFunc = fp; }
            void setUpdateFunc( const Functor0<void> & fp ) { mUpdateFunc = fp; }
            void setRenderFunc( const Functor0<void> & fp ) { mRenderFunc = fp; }
            
            int run();
            //@}

            // ********************************
            //! \name application framework
            // ********************************

            //@{
        protected:
            virtual bool appInit() { if( mAppInitFunc ) return mAppInitFunc(); else return true; }
            virtual bool devCreate() { if( mDevCreateFunc ) return mDevCreateFunc(); else return true; }
            virtual bool devRestore() { if( mDevRestoreFunc ) return mDevRestoreFunc(); else return true; }
            virtual void devDispose()  { if( mDevDisposeFunc ) mDevDisposeFunc(); }
            virtual void devDestroy()  { if( mDevDestroyFunc ) mDevDestroyFunc(); }
            virtual void appQuit() { if( mAppQuitFunc ) mAppQuitFunc(); }
            virtual void update() { if( mUpdateFunc ) mUpdateFunc(); }
            virtual void render()
            {
                LPDIRECT3DDEVICE9 dev = mD3D.getDevice();
                if( D3D_OK == dev->BeginScene() )
                {
                    if( mRenderFunc ) mRenderFunc();
                    DX_CHECK( dev->EndScene() );
                }
                mD3D.present();
            }
            //@}

            // ********************************
            // private variables
            // ********************************
        private:

            D3D mD3D;

            D3DInitParams mInitParams;

            Functor0<bool> mAppInitFunc, mDevCreateFunc, mDevRestoreFunc;
            Functor0<void> mDevDisposeFunc, mDevDestroyFunc, mAppQuitFunc, mUpdateFunc, mRenderFunc;

            // ********************************
            // private functions
            // ********************************
        private:
            bool init();
            void quit();
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
        LPDIRECT3DVERTEXSHADER9 compileVS( const char * code, size_t len = 0, uint32_t flags = 0, const char * entryFunc = "main" );

        //!
        //! Compile vertex shader from file
        //!
        LPDIRECT3DVERTEXSHADER9 compileVSFromFile( const char * file, uint32_t flags = 0, const char * entryFunc = "main" );

        //!
        //! Assemble vertex shader from string
        //!
        LPDIRECT3DVERTEXSHADER9 assembleVS( const char * code, size_t len = 0, uint32_t flags = 0 );

        //!
        //! Assemble vertex shader from file
        //!
        LPDIRECT3DVERTEXSHADER9 assembleVSFromFile( const char * file, uint32_t flags = 0 );

        //!
        //! Compile pixel shader from string
        //!
        LPDIRECT3DPIXELSHADER9 compilePS( const char * code, size_t len = 0, uint32_t flags = 0, const char * entryFunc = "main" );

        //!
        //! Compile pixel shader from file
        //!
        LPDIRECT3DPIXELSHADER9 compilePSFromFile( const char * file, uint32_t flags = 0, const char * entryFunc = "main" );

        //!
        //! Assemble pixel shader from string
        //!
        LPDIRECT3DPIXELSHADER9 assemblePS( const char * code, size_t len = 0, uint32_t flags = 0 );

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
        inline LPDIRECT3DVERTEXSHADER9 assembleXVS( const char * code, size_t len = 0, uint32_t flags = 0 )
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
        inline LPDIRECT3DPIXELSHADER9 assembleXPS( const char * code, size_t len = 0, uint32_t flags = 0 )
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
        //! Draw screen aligned quad on screen.
        //!
        //! \param left, top, right, bottom
        //!     Quad position.
        //!     - On PC platform, position (0,0,1,1) will draw quad that fulfill current viewport,
        //!       when there's no active vertex shader; or you may achieve same effect by using
        //!       a "pass-through" vertex shader.
        //!     - On Xenon, a valid vertex shader must be provided before calling this
        //!       function. Similar with PC case, position (0,0,1,1) will draw quad as large as
        //!       current viewport, if you pass-through vertex position in your shader.
        //!
        //! \param leftU, topV, rightU, bottomV
        //!     Texture coordinates of quad corners.
        //!
        void drawScreenAlignedQuad(
            double left = 0.0, double top = 0.0, double right = 1.0, double bottom = 1.0,
            double leftU = 0.0, double topV = 0.0, double rightU = 1.0, double bottomV = 1.0 );

        //@}
    }
}

// *****************************************************************************
//                           End of GNd3d.h
// *****************************************************************************
#endif // __GN_D3DAPP_GND3D_H__
