#ifndef __GN_D3DAPP_GND3DAPP_H__
#define __GN_D3DAPP_GND3DAPP_H__
// *****************************************************************************
//! \file    GnD3DApp.h
//! \brief   Public interface for d3dapp module
//! \author  chenlee (2005.8.11)
// *****************************************************************************

#include "garnet/GnBase.h"

#ifdef GN_DEBUG
#define D3D_DEBUG_INFO // Enable "Enhanced D3DDebugging"
#endif

#if GN_XENON
#include <xtl.h>
#elif GN_WINPC
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
//! d3dapp module error log macro
//!
#define D3DAPP_ERROR GN_ERROR

//!
//! d3dapp module warning log macro
//!
#define D3DAPP_WARN GN_WARN

//!
//! d3dapp module informational log macro
//!
#define D3DAPP_INFO GN_INFO

//!
//! DX error check routine
//!
#define DX_CHECK_DO( func, something )              \
    if( true ) {                                    \
        HRESULT rr = func;                          \
        if (D3D_OK != rr)                           \
        {                                           \
            D3DAPP_ERROR( DXGetErrorString9A(rr) ); \
            something                               \
        }                                           \
    } else void(0)

//!
//! DX error check routine
//!
#ifdef GN_DEBUG
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

//! \name Alias for singleton objects
//@{
#define gD3D       (::GN::d3dapp::D3D::getInstance()) //!< global D3D object
#define gTexMgr    (::GN::d3dapp::TextureManager::getInstance()) //!< Global texture manager
#define gEffectMgr (::GN::d3dapp::EffectManager::getInstance()) //!< Global effect manager
#define gMeshMgr   (::GN::d3dapp::MeshManager::getInstance()) //!< Global mesh manager
//@}

namespace GN
{
    //!
    //! Namespace for d3dapp module
    //!
    namespace d3dapp
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
            //! Get D3D present parameters
            //!
            const D3DPRESENT_PARAMETERS & getPresentParams() const { return mPresentParams; }

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
            void present();

            // ********************************
            // private variables
            // ********************************
        private:

            HWND                    mWindow;
            LPDIRECT3D9             mD3D;
            LPDIRECT3DDEVICE9       mDevice;
            D3DPRESENT_PARAMETERS   mPresentParams;

            bool mClosed;
            bool mMinimized;

            // ********************************
            // private functions
            // ********************************
        private:

            bool createWindow( const D3DInitParams & );
            bool createD3D( const D3DInitParams & );

            void processWindowMessages();

            LRESULT windowProc( HWND, UINT, WPARAM, LPARAM );
            static LRESULT staticProc( HWND, UINT, WPARAM, LPARAM );
        };

        typedef ResourceManager<LPDIRECT3DBASETEXTURE9,true>  TextureManager; //!< Texture manager
        typedef ResourceManager<LPDIRECT3DVERTEXSHADER9,true> VShaderManager; //!< Vertex shader manager
        typedef ResourceManager<LPDIRECT3DPIXELSHADER9,true>  PShaderManager; //!< Pixel shader manager
        typedef ResourceManager<LPD3DXEFFECT,true> EffectManager; //!< Effect manager
        typedef ResourceManager<LPD3DXEFFECT,true> EffectManager; //!< Effect manager
        typedef ResourceManager<LPD3DXMESH,true> MeshManager; //!< Mesh manager

        // Util functions

        //!
        //! Compile vertex shader from string
        //!
        LPDIRECT3DVERTEXSHADER9 compileVS( const char * code, size_t len, uint32_t flags = 0, const char * entryFunc = "main" );

        //!
        //! Assemble vertex shader from string
        //!
        LPDIRECT3DVERTEXSHADER9 assembleVS( const char * code, size_t len, uint32_t flags = 0 );

        //!
        //! Compile pixel shader from string
        //!
        LPDIRECT3DPIXELSHADER9 compilePS( const char * code, size_t len, uint32_t flags = 0, const char * entryFunc = "main" );

        //!
        //! Assemble pixel shader from string
        //!
        LPDIRECT3DPIXELSHADER9 assemblePS( const char * code, size_t len, uint32_t flags = 0 );

        //!
        //! Draw screen aligned quad on screen
        //!
        void drawScreenAlignedQuad(
            double fLeft = 0.0, double fTop = 0.0, double fRight = 1.0, double fBottom = 1.0,
            float fLeftU = 0.0f, float fTopV = 0.0f, float fRightU = 1.0f, float fBottomV = 1.0f );
    }
}

// *****************************************************************************
//                           End of GnD3DApp.h
// *****************************************************************************
#endif // __GN_D3DAPP_GND3DAPP_H__
