#ifndef __GN_GFX2_D3D9BUILDINEFFECTS_H__
#define __GN_GFX2_D3D9BUILDINEFFECTS_H__
// *****************************************************************************
//! \file    d3d9/d3d9BuildInEffects.h
//! \brief   D3D9 build-in effects
//! \author  chenli@@FAREAST (2007.6.19)
// *****************************************************************************

namespace GN { namespace gfx2
{
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
            // setup ports
            D3D9EffectPortDesc & c0 = mDesc.ports["COLOR0"];
            c0.input  = true;
            c0.output = true;
            c0.surfaceType = SURFACE_TYPE_BACKBUF;

            D3D9EffectPortDesc & d0 = mDesc.ports["DEPTH"];
            d0.input  = true;
            d0.output = true;
            d0.surfaceType = SURFACE_TYPE_RTS_DEPTH;

            // setup parameters
            EffectParameterDesc & c = mDesc.parameters["COLOR_VALUE"];
            c.type  = EFFECT_PARAMETER_TYPE_FLOAT4;
            c.count = 1;

            EffectParameterDesc & z = mDesc.parameters["DEPTH_VALUE"];
            z.type  = EFFECT_PARAMETER_TYPE_FLOAT1;
            z.count = 1;

            EffectParameterDesc & s = mDesc.parameters["STENCIL_VALUE"];
            s.type  = EFFECT_PARAMETER_TYPE_INT1;
            s.count = 1;
         }

        // from Effect
        virtual void render();
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
            // setup ports
            D3D9EffectPortDesc & c0 = mDesc.ports["COLOR0"];
            c0.input  = true;
            c0.output = true;
            c0.surfaceType = SURFACE_TYPE_BACKBUF;

            // no parameters
        }

        // from Effect
        virtual void render();
    };
}}

// *****************************************************************************
//                           End of d3d9BuildInEffects.h
// *****************************************************************************
#endif // __GN_GFX2_D3D9BUILDINEFFECTS_H__

