#ifndef __GN_GFX2_D3D9EFFECT_H__
#define __GN_GFX2_D3D9EFFECT_H__
// *****************************************************************************
//! \file    gfx2/d3d9Effect.h
//! \brief   d3d9 effect interface
//! \author  chenli@@FAREAST (2007.6.15)
// *****************************************************************************

namespace GN { namespace gfx2
{
    class D3D9GraphicsSystem;

    ///
    /// D3D9 effect descriptor
    ///
    struct D3D9EffectDesc : public EffectDesc
    {
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
        virtual bool               compatible( Surface * surf, const StrA & port ) = 0;
        virtual EffectBinding      createBinding( const EffectBindingDesc & ) = 0;
        virtual void               deleteBinding( EffectBinding ) = 0;
        virtual void               bind( EffectBinding ) = 0;
        //@}

    protected:

        ///
        /// effect descriptor
        ///
        D3D9EffectDesc mDesc;
    };

    ///
    /// build-in clear effect
    ///
    class D3D9ClearEffect : public D3D9Effect
    {
    public:

        ///
        /// ctor
        ///
        D3D9ClearEffect( GraphicsSystem & gs ) : D3D9Effect(gs)
        {
            // setup effect descriptor
            EffectPortDesc & c0 = mDesc.ports["color0"];
            c0.input  = true;
            c0.output = true;
            c0.layout.flags.u32 = 0;
            GN_UNIMPL();
        }
    };

    ///
    /// build-in present effect
    ///
    class D3D9PresentEffect : public D3D9Effect
    {
    public:

        ///
        /// ctor
        ///
        D3D9PresentEffect( GraphicsSystem & gs ) : D3D9Effect(gs)
        {
            // setup effect descriptor
            GN_UNIMPL();
        }
    };
}}

// *****************************************************************************
//                           End of d3d9Effect.h
// *****************************************************************************
#endif // __GN_GFX2_D3D9EFFECT_H__
