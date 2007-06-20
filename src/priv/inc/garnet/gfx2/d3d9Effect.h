#ifndef __GN_GFX2_D3D9EFFECT_H__
#define __GN_GFX2_D3D9EFFECT_H__
// *****************************************************************************
//! \file    gfx2/d3d9Effect.h
//! \brief   d3d9 base effect interfaces
//! \author  chenli@@FAREAST (2007.6.15)
// *****************************************************************************

namespace GN { namespace gfx2
{
    class D3D9GraphicsSystem;
    class D3D9Effect;

    ///
    /// D3D9 effect port descriptor
    ///
    struct GN_GFX2_D3D9_PUBLIC D3D9EffectPortDesc : public EffectPortDesc
    {
        ///
        /// Surface type that this port expects.
        ///
        D3D9SurfaceType surfaceType;
    };

    ///
    /// D3D9 effect port class
    ///
    //@{
    class GN_GFX2_D3D9_PUBLIC D3D9EffectPort
    {
    protected:

        D3D9EffectPortDesc mDesc; ///< port descriptor

    public:

        ///
        /// ctor
        ///
        D3D9EffectPort() {}

        ///
        /// get descriptor
        ///
        const D3D9EffectPortDesc & getDesc() const { return mDesc; }

        ///
        /// check surface compatility
        ///
        virtual bool compatible( const Surface * surf ) const = 0;

        ///
        /// bind surface to device
        ///
        virtual void bind( const EffectPortBinding & ) = 0;
    };

    class GN_GFX2_D3D9_PUBLIC D3D9RenderTargetPort : public D3D9EffectPort
    {
    public:

        //@{
        virtual bool compatible( const Surface * surf ) const;
        virtual void bind( const EffectPortBinding & );
        //@}
    };

    class GN_GFX2_D3D9_PUBLIC D3D9DepthBufferPort : public D3D9EffectPort
    {
        //@{
        virtual bool compatible( const Surface * surf ) const;
        virtual void bind( const EffectPortBinding & );
        //@}
    };

    class GN_GFX2_D3D9_PUBLIC D3D9TexturePort : public D3D9EffectPort {};
    class GN_GFX2_D3D9_PUBLIC D3D9VtxBufPort : public D3D9EffectPort {};
    class GN_GFX2_D3D9_PUBLIC D3D9IdxBufPort : public D3D9EffectPort {};
    //@}

    ///
    /// base D3D9 effect binding
    ///
    class GN_GFX2_D3D9_PUBLIC D3D9EffectBinding
    {
        struct BindItem
        {
            D3D9EffectPort  * port;
            EffectPortBinding bind;
        };

        DynaArray<BindItem> mBindItems;

    public:

        ///
        /// ctor
        ///
        D3D9EffectBinding();

        ///
        /// dtor
        ///
        ~D3D9EffectBinding();

        ///
        /// binding setup
        ///
        bool setup( D3D9Effect & effect, const EffectBindingDesc & ebd );

        ///
        /// apply binding to rendering device
        ///
        void apply() const;
    };

    ///
    /// D3D9 effect descriptor
    ///
    struct GN_GFX2_D3D9_PUBLIC D3D9EffectDesc : public EffectDesc
    {
        //@{
        typedef std::map<StrA,EffectParameterDesc> ParameterDescContainer;
        //@}

        //@{
        D3D9Effect           * effect;
        ParameterDescContainer parameters;
        //@}

        //@{
        virtual const EffectPortDesc      * getPortDesc( const StrA & name ) const;
        virtual const EffectParameterDesc * getParameterDesc( const StrA & name ) const;
        //@}
    };

    ///
    /// base D3D9 effect
    ///
    class GN_GFX2_D3D9_PUBLIC D3D9Effect : public BaseEffect
    {
    public:

        ///
        /// ctor
        ///
        D3D9Effect( GraphicsSystem & gs ) : BaseEffect(gs) { mDesc.effect = this; }

        ///
        /// get D3D9 graphic system
        ///
        D3D9GraphicsSystem & d3d9gs() const { return (D3D9GraphicsSystem&)gs(); }

        /// \name from Effect
        //@{
        virtual const EffectDesc & getDesc() const { return mDesc; }
        virtual bool               compatible( const Surface * surf, const StrA & port );
        virtual EffectBinding      createBinding( const EffectBindingDesc & );
        virtual void               deleteBinding( EffectBinding );
        virtual void               bind( EffectBinding );
        //@}

        //@{
        const D3D9EffectPort * getPort( const StrA & name ) const;
        D3D9EffectPort * getPort( const StrA & name );
        //@}

    protected:

        //@{

        ///
        /// \note D3D9Effect class does _NOT_ hold the ownership of the port instance.
        ///
        void addPortRef( const StrA & name, D3D9EffectPort * port );

        ///
        /// \note D3D9Effect class will hold a copy of the input parmeter descriptor
        ///
        EffectParameterHandle addParameter( const StrA & name, const EffectParameterDesc & param );

        //@}

    private:

        typedef NamedHandleManager<D3D9EffectPort*,UInt32> PortContainer;
        typedef HandleManager<D3D9EffectBinding*,EffectBinding> EffectBindingContainer;

        D3D9EffectDesc         mDesc;
        PortContainer          mPorts;
        EffectBindingContainer mBindings;
    };
}}

// *****************************************************************************
//                           End of d3d9Effect.h
// *****************************************************************************
#endif // __GN_GFX2_D3D9EFFECT_H__
