#ifndef __GN_GFX2_D3D9GRAPHICSSYSTEM_H__
#define __GN_GFX2_D3D9GRAPHICSSYSTEM_H__
// *****************************************************************************
//! \file    gfx2/d3d9GraphicsSystem.h
//! \brief   d3d9 graphics system interface
//! \author  chenli@@FAREAST (2007.6.15)
// *****************************************************************************

namespace GN { namespace gfx2
{
    ///
    /// D3D9 graphics system descriptor
    ///
    struct D3D9GraphicsSystemDesc : public GraphicsSystemDesc
    {
        //@{

        IDirect3D9          * d3d;
        IDirect3DDevice9    * device;

        UINT                  adapter;
        D3DDEVTYPE            devtype;
        D3DPRESENT_PARAMETERS pp;
        D3DCAPS9              caps;

        bool                  zbuffer; ///< has zbuffer or not

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
        D3D9GraphicsSystem();
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

        virtual const GraphicsSystemDesc & getDesc() const { return mDesc; }
        virtual void present();
        virtual Surface * createSurface( const SurfaceCreationParameter & scp );

        //@}

        // ********************************
        // public methods
        // ********************************
    public:

        //@{

        const D3D9GraphicsSystemDesc & getD3D9Desc() const { return mDesc; }

        IDirect3DDevice9 * d3ddev() const { GN_ASSERT(mDesc.device); return mDesc.device; }

        void setZBuffer( IDirect3DSurface9 * surf ) { mDesc.device->SetDepthStencilSurface( surf ); mDesc.zbuffer = !!surf; }

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

        const D3D9EffectDesc * getEffectDesc( const StrA & name )
        {
            Effect * e = getEffect( name );
            if( 0 == e ) return 0;
            return (const D3D9EffectDesc*)&e->getDesc();
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
