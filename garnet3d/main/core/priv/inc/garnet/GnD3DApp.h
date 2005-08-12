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

#define NOMINMAX
#include <windows.h>
#include <atlbase.h> // COM smart pointer
#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr9.h>
#define ATL_NO_LEAN_AND_MEAN

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
            bool refDevice;
            bool fullScreen;
            bool hwvp;
			bool showWindow;
            uint32_t width;
            uint32_t height;
            const char * winTitle;

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

        //!
        //! Resource manager
        //!
        template<typename RES>
        class ResourceManager : public Singleton< ResourceManager<RES> >
        {
            typedef ResourceManager<RES>       MyType;
            typedef Functor1<RES,const StrA &> Creator;
            typedef Functor1<void,RES&>        Deletor;

            struct ResDesc
            {
                Creator creator;
                Deletor deletor;
                RES     res;
            };

            typedef std::map<StrA,ResDesc> ResMap;

            static MyType msInstance;

            ResMap  mResPool;
            Creator mDefaultCreator;
            Creator mNullCreator; // Use to create "NULL" instance.
            Deletor mDefaultDeletor;

            void doDispose( std::pair<const StrA,ResDesc> & item )
            {
                ResDesc & desc = item.second;
                if( desc.deletor ) desc.deletor( desc.res );
                else if( mDefaultDeletor ) mDefaultDeletor( desc.res );
                desc.res = 0;
            }

        public:

            typedef RES ResType; //!< resource type

            //!
            //! Default constructor
            //!
            ResourceManager(
                const Creator & defaultCreator = Creator(),
                const Deletor & defaultDeletor = Deletor(),
                const Creator & nullCreator = Creator() )
                : mDefaultCreator(defaultCreator)
                , mDefaultDeletor(defaultDeletor)
                , mNullCreator(nullCreator)
            {
            }

            //!
            //! Default destructor
            //!
            ~ResourceManager() {}

            //!
            //! Set default creator
            //!
            void setDefaultCreator( const Creator & c ) { mDefaultCreator = c; }

            //!
            //! Set default deletor
            //!
            void setDefaultDeletor( const Deletor & d ) { mDefaultDeletor = d; }

            //!
            //! Set NULL instance creator
            //!
            void setNullCreator( const Creator & n ) { mNullCreator = n; }

            //!
            //! Clear all resources.
            //!
            void clear()
            {
                dispose();
                mResPool.clear();
            }

            //!
            //! Invalidate all resources. But keep all resources items.
            //!
            void dispose()
            {
                std::for_each(
                    mResPool.begin(),
                    mResPool.end(),
                    makeFunctor(this,&MyType::doDispose) );
            }

            //!
            //! Add new resource item to manager
            //!
            bool addResource(
                const StrA & name,
                const Creator & creator = Creator(),
                const Deletor & deletor = Deletor() )
            {
                ResMap::const_iterator ci = mResPool.find(name);
                if( mResPool.end() != ci )
                {
                    D3DAPP_ERROR( "resource '%s' already exist!", name.cstr() );
                    return false;
                }
                ResDesc & newItem = mResPool[name];
                newItem.creator = creator;
                newItem.deletor = deletor;
                newItem.res = 0;
                return true;
            }

            //!
            //! Get resource by name
            //!
            RES getResource( const StrA & name )
            {
                ResMap::iterator iter = mResPool.find( name );
                if( mResPool.end() == iter )
                {
                    D3DAPP_ERROR( "resource '%s' not found!", name.cstr() );
                    if( mNullCreator ) return mNullCreator(name);
                    else return RES(0);
                }
                if( !iter->second.res )
                {
                    if( iter->second.creator )
                    {
                        iter->second.res = iter->second.creator( name );
                    }
                    else if( mDefaultCreator )
                    {
                        iter->second.res = mDefaultCreator( name );
                    }
                    else if( mNullCreator )
                    {
                        D3DAPP_WARN( "Use Null instance for resource '%s'.", name.cstr() );
                        iter->second.res = mNullCreator(name);
                    }
                    else
                    {
                        D3DAPP_ERROR( "No creator found!" );
                    }
                }
                return iter->second.res;
            }
        };

        typedef ResourceManager<LPDIRECT3DBASETEXTURE9> TextureManager; //!< Texture manager
        typedef ResourceManager<LPD3DXEFFECT> EffectManager; //!< Texture manager
        typedef ResourceManager<LPD3DXMESH> MeshManager; //!< Texture manager
    }
}

// *****************************************************************************
//                           End of GnD3DApp.h
// *****************************************************************************
#endif // __GN_D3DAPP_GND3DAPP_H__
