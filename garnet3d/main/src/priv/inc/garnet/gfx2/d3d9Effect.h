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

    ///
    /// d3d9 effect port type
    ///
    enum D3D9SurfaceType
    {
        //@{
        SURFACE_TYPE_VB,
        SURFACE_TYPE_IB,
        SURFACE_TYPE_TEX_2D,
        SURFACE_TYPE_TEX_3D,
        SURFACE_TYPE_TEX_CUBE,
        SURFACE_TYPE_RTT_2D,    ///< 2D render target texture
        SURFACE_TYPE_RTT_CUBE,  ///< 2D cube texture
        SURFACE_TYPE_RTS_COLOR, ///< render target surface
        SURFACE_TYPE_RTS_DEPTH, ///< depth stencil surface
        SURFACE_TYPE_BACKBUF,   ///< backbuffer
        SURFACE_TYPE_ANY = -1,  ///< indicate any kind of surfaces.
        //@}
    };

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
    /// base D3D9 effect binding
    ///
    class GN_GFX2_D3D9_PUBLIC D3D9EffectBinding
    {
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
        bool setup( const EffectBindingDesc & ebd );

        ///
        /// apply binding to rendering device
        ///
        void apply();
    };

    ///
    /// D3D9 effect descriptor
    ///
    struct GN_GFX2_D3D9_PUBLIC D3D9EffectDesc : public EffectDesc
    {
        //@{
        typedef std::map<StrA,D3D9EffectPortDesc>  PortDescContainer;
        typedef std::map<StrA,EffectParameterDesc> ParameterValueDescContainer;
        //@}

        //@{
        PortDescContainer           ports;
        ParameterValueDescContainer parameters;
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
        D3D9Effect( GraphicsSystem & gs ) : BaseEffect(gs) {}

        /// \name from Effect
        //@{
        virtual const EffectDesc & getDesc() const { return mDesc; }
        virtual bool               compatible( const Surface * surf, const StrA & port );
        virtual EffectBinding      createBinding( const EffectBindingDesc & );
        virtual void               deleteBinding( EffectBinding );
        virtual void               bind( EffectBinding );
        //@}

    protected:

        D3D9EffectDesc mDesc;     ///< effect descriptor

    private:

        typedef HandleManager<D3D9EffectBinding*,EffectBinding> EffectBindingManager;

        EffectBindingManager mBindings; ///< bindings
    };
}}

// *****************************************************************************
//                           End of d3d9Effect.h
// *****************************************************************************
#endif // __GN_GFX2_D3D9EFFECT_H__
