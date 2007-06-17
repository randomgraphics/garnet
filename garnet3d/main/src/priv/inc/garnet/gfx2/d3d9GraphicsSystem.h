#ifndef __GN_GFX2_D3D9GRAPHICSSYSTEM_H__
#define __GN_GFX2_D3D9GRAPHICSSYSTEM_H__
// *****************************************************************************
//! \file    gfx2/d3d9GraphicsSystem.h
//! \brief   d3d9 graphics system interface
//! \author  chenli@@FAREAST (2007.6.15)
// *****************************************************************************

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>

#define D3D_DEBUG_INFO // Enable "Enhanced D3DDebugging"
#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr9.h>

#ifdef HAS_CG_D3D9
#include <Cg/cg.h>
#include <Cg/cgD3D9.h>
#endif

namespace GN { namespace gfx2
{
    ///
    /// D3D9 graphics system descriptor
    ///
    struct D3D9GraphicsSystemDesc : public GraphicSystemDesc
    {
        //@{
        HWND                  window;
        IDirect3D9          * d3d;
        IDirect3DDevice9    * device;
        UINT                  adapter;
        D3DDEVTYPE            devtype;
        D3DPRESENT_PARAMETERS pp;
        D3DCAPS9              caps;
        //@}
    };

    ///
    /// D3D9 graphics system
    ///
    class GN_GFX2_D3D9_PUBLIC D3D9GraphicsSystem : public BaseGraphicsSystem
    {
        GN_DECLARE_STDCLASS( D3D9GraphicsSystem, BaseGraphicsSystem );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        D3D9GraphicsSystem()          { clear(); }
        virtual ~D3D9GraphicsSystem() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init( const GraphicsSystemCreationParameter & );
        void quit();
    private:
        void clear();
        //@}

        // ********************************
        // from GraphicsSystem
        // ********************************
    public:

        //@{

        virtual const GraphicSystemDesc & getDesc() const { return mDesc; }
        virtual void onFrame();
        virtual Surface * createSurface( const SurfaceCreationParameter & scp );

        //@}

        // ********************************
        // public methods
        // ********************************
    public:

        //@{

        void present()
        {
            endScene();
            mDesc.device->Present( 0, 0, 0, 0 );
            beginScene();
        }

        //@}

        // ********************************
        // public signals
        // ********************************
    public:

        //@{

        Signal0<bool> sigDeviceRestore;
        Signal0<void> sigDeviceDispose;

        //@}

        // ********************************
        // private variables
        // ********************************
    private:

        D3D9GraphicsSystemDesc mDesc;
        bool                   mSceneBegun;

        // ********************************
        // private functions
        // ********************************
    private:

        bool beginScene()
        {
            if( mSceneBegun ) return true;
            if( !handleDeviceLost() ) return false;
            GN_DX9_CHECK_RV( mDesc.device->BeginScene(), false );
            mSceneBegun = true;
            return true;
        }

        void endScene()
        {
            if( mSceneBegun )
            {
                mSceneBegun = false;
                GN_DX9_CHECK( mDesc.device->EndScene() );
            }
        }

        bool handleDeviceLost();
    };

    ///
    /// create D3D9 graphics system
    ///
    extern "C" GN_GFX2_D3D9_PUBLIC D3D9GraphicsSystem * createD3D9GraphicsSystem( const GraphicsSystemCreationParameter & );
}}

// *****************************************************************************
//                           End of d3d9GraphicsSystem.h
// *****************************************************************************
#endif // __GN_GFX2_D3D9GRAPHICSSYSTEM_H__
