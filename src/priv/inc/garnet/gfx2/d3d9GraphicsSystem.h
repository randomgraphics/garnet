#ifndef __GN_GFX2_D3D9GRAPHICSSYSTEM_H__
#define __GN_GFX2_D3D9GRAPHICSSYSTEM_H__
// *****************************************************************************
/// \file
/// \brief   d3d9 graphics system interface
/// \author  chenli@@FAREAST (2007.6.15)
// *****************************************************************************

namespace GN { namespace gfx
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
        UINT                  behavior;
        D3DPRESENT_PARAMETERS pp;
        D3DCAPS9              caps;

        //@}
    };

    class D3D9GraphicsSystem;

    ///
    /// D3D9 resource that can not suvive device lost.
    ///
    class D3D9UnstableResource
    {
        D3D9GraphicsSystem & mGraphicsSystem;

    protected:

        //@{

        D3D9UnstableResource( D3D9GraphicsSystem & gs );
        virtual ~D3D9UnstableResource();

        //@}

    public:

        //@{

        D3D9GraphicsSystem & gfxsys() const { return mGraphicsSystem; }

        virtual bool onRestore() = 0; ///< restore device data after device reset
        virtual void onDispose() = 0; ///< dispose device data after device lost

        //@}
    };

    class GN_GFX2_D3D9_PUBLIC D3D9RenderStateBlock
    {
        struct StateValue
        {
            DWORD                        value;
            bool                         valid;

            StateValue() : valid(false) {}
        };

        struct StageState
        {
            DWORD               stage;
            D3DSAMPLERSTATETYPE type;

            bool operator==( const StageState & rhs ) const
            {
                return stage == rhs.stage && type == rhs.type;
            }
        };

        D3D9GraphicsSystem               & mGfxSys;
        IDirect3DDevice9                 * mDevice;

        StackArray<D3DRENDERSTATETYPE,210> mRsTypes;
        StateValue                         mRsValues[210];

        StackArray<StageState,21*14>       mSsTypes;
        StateValue                         mSsValues[21][14];

        //StackArray<StateValue,21*33>          mTextureStates;
        //StateValue                            mTsMasks[21][33];

        inline void applyRenderStates() const;
        inline void applyRenderStates( const D3D9RenderStateBlock & last ) const;

        inline void applySamplerStates() const;
        inline void applySamplerStates( const D3D9RenderStateBlock & last ) const;

    public:

        //@{

        static void sSetupDefaultDeviceStates( D3D9GraphicsSystem & gs );

        D3D9RenderStateBlock( D3D9GraphicsSystem & gs );

        ~D3D9RenderStateBlock() {}

        void setRenderState( D3DRENDERSTATETYPE type, DWORD value );
        void unsetRenderState( D3DRENDERSTATETYPE type );

        void setSamplerState( size_t stage, D3DSAMPLERSTATETYPE type, DWORD value );

        void setTextureState( size_t stage, D3DTEXTURESTAGESTATETYPE type, DWORD value );

        void apply( const D3D9RenderStateBlock * last ) const;
        void apply() const;

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

        void setRenderStateBlock( const D3D9RenderStateBlock & rsb )
        {
            rsb.apply( mCurrentRsb );
            mCurrentRsb = &rsb;
        }

        void setTexture( UINT stage, IDirect3DBaseTexture9 * tex )
        {
            if( stage < 16 )
            {
                if( mCurrentTextures[stage] == tex ) return;
                mDesc.device->SetTexture( stage, tex );
                mCurrentTextures[stage] = tex;
            }
            else
            {
                GN_UNIMPL();
            }
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

        D3D9GraphicsSystemDesc       mDesc;
        bool                         mSceneBegun;
        const D3D9RenderStateBlock * mCurrentRsb;
        IDirect3DBaseTexture9      * mCurrentTextures[21];

        // ********************************
        // private functions
        // ********************************
    private:

        bool restoreDevice();

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
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX2_D3D9GRAPHICSSYSTEM_H__
