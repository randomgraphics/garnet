#include "pch.h"
#include "d3d9ScriptEffect.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx2.D3D9ScriptEffect");

// *****************************************************************************
// local functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------


// *****************************************************************************
// public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx2::D3D9ScriptEffect::render(
    const EffectParameterSet & param, EffectBinding binding )
{
    GN_GUARD_SLOW;

    applyBinding( binding );

    applyVS( param.getParameter( mVs ) );
    applyPS( param.getParameter( mPs ) );

    /*D3D9GraphicsSystem & gs = d3d9gs();
    IDirect3DDevice9  * dev = gs.d3ddev();

    const EffectParameter
        * pt = param.getParameter( mPrimType ),
        * pc = param.getParameter( mPrimCount ),
        * bi = param.getParameter( mBaseIndex ),
        * bv = param.getParameter( mBaseVertex ),
        * vc = param.getParameter( mVertexCount );*/

    GN_UNGUARD_SLOW;
}

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
inline void GN::gfx2::D3D9ScriptEffect::applyVS( const EffectParameter * )
{
    GN_UNIMPL_WARNING();
}

//
//
// -----------------------------------------------------------------------------
inline void GN::gfx2::D3D9ScriptEffect::applyPS( const EffectParameter * )
{
    GN_UNIMPL_WARNING();
}
