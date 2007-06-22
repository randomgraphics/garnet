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
    /// base D3D9 effect port class
    ///
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

    ///
    /// D3D9 render target port (accept render target texture)
    ///
    class GN_GFX2_D3D9_PUBLIC D3D9RenderTargetPort : public D3D9EffectPort
    {
    public:

        //@{
        virtual bool compatible( const Surface * surf ) const;
        virtual void bind( const EffectPortBinding & );
        //@}
    };

    ///
    /// D3D9 render target port (accept depth surface)
    ///
    class GN_GFX2_D3D9_PUBLIC D3D9DepthBufferPort : public D3D9EffectPort
    {
    public:

        //@{
        virtual bool compatible( const Surface * surf ) const;
        virtual void bind( const EffectPortBinding & );
        //@}
    };

    ///
    /// D3D9 render target port (accepts texture)
    ///
    class GN_GFX2_D3D9_PUBLIC D3D9TexturePort : public D3D9EffectPort
    {
    public:

        //@{
        virtual bool compatible( const Surface * surf ) const;
        virtual void bind( const EffectPortBinding & );
        //@}
    };

    ///
    /// D3D9 render target port (accepts vertex buffer)
    ///
    class GN_GFX2_D3D9_PUBLIC D3D9VtxBufPort : public D3D9EffectPort
    {
    public:

        ///
        /// ctor
        ///
        D3D9VtxBufPort();

        //@{
        void setStride( UInt32 );      // call this only if this port requires fixed stride.
        void setVertexCount( UInt32 ); // call this only if this port requires a fixed vertex count.
        void addRequiredAttribute( const SurfaceAttributeTemplate & );
        void addRequiredAttribute( const char * s, UInt32 offset = -1 );
        void addOptionalAttribute( const SurfaceAttributeTemplate & );
        //@}

        //@{
        virtual bool compatible( const Surface * surf ) const;
        virtual void bind( const EffectPortBinding & );
        //@}
    };

    ///
    /// D3D9 render target port (accept index buffer)
    ///
    class GN_GFX2_D3D9_PUBLIC D3D9IdxBufPort : public D3D9EffectPort
    {
    public:

        //@{
        virtual bool compatible( const Surface * surf ) const;
        virtual void bind( const EffectPortBinding & );
        //@}
    };

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
    /// base D3D9 effect
    ///
    class GN_GFX2_D3D9_PUBLIC D3D9Effect : public BaseEffect
    {
    public:

        //@{

        ///
        /// ctor
        ///
        D3D9Effect( GraphicsSystem & gs ) : BaseEffect(gs) {}

        ///
        /// get D3D9 graphic system
        ///
        D3D9GraphicsSystem & d3d9gs() const { return (D3D9GraphicsSystem&)gs(); }

        const D3D9EffectPort * getPort( const StrA & name ) const;
        D3D9EffectPort * getPort( const StrA & name );

        //@}

        /// \name from Effect
        //@{
        virtual const EffectPortDesc * getPortDesc( const StrA & name ) const;
        virtual bool                   compatible( const Surface * surf, const StrA & port );
        virtual EffectBinding          createBinding( const EffectBindingDesc & );
        virtual void                   deleteBinding( EffectBinding );
        //@}

    protected:

        //@{

        ///
        /// \note D3D9Effect class does _NOT_ hold the ownership of the port instance.
        ///
        void addPortRef( const StrA & name, D3D9EffectPort * port );

        ///
        /// apply port binding
        ///
        void applyBinding( EffectBinding b )
        {
            GN_GUARD_SLOW;

            GN_ASSERT( mBindings.validHandle( b ) );
            GN_ASSERT( mBindings[b] );

            mBindings[b]->apply();

            GN_UNGUARD_SLOW;
        }

        //@}

    private:

        typedef NamedHandleManager<D3D9EffectPort*,UInt32>      PortContainer;
        typedef HandleManager<D3D9EffectBinding*,EffectBinding> EffectBindingContainer;

        PortContainer          mPorts;
        EffectBindingContainer mBindings;
    };
}}

// *****************************************************************************
//                           End of d3d9Effect.h
// *****************************************************************************
#endif // __GN_GFX2_D3D9EFFECT_H__
