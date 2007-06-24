#include "pch.h"
#include "d3d9HlslEffect.h"
#include "garnet/GNgfx.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx2.D3D9HlslEffect");

// *****************************************************************************
// local functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
static IDirect3DVertexShader9 * sCreateVs(
    IDirect3DDevice9                * dev,
    const GN::gfx2::EffectParameter & param,
    ID3DXConstantTable             ** consts )
{
    using namespace GN::gfx2;
    using namespace GN::gfx::d3d9;

    if( EFFECT_PARAMETER_TYPE_STRING != param.type )
    {
        GN_ERROR(sLogger)( "Parameter 'VS' accepts only string value." );
        return false;
    }
    if( 0 == param.str )
    {
        GN_ERROR(sLogger)( "Null VS string." );
        return false;
    }

    return compileVS( dev, param.str, 0, 0, "main", 0, consts );
}

//
//
// -----------------------------------------------------------------------------
static IDirect3DPixelShader9 * sCreatePs(
    IDirect3DDevice9                * dev,
    const GN::gfx2::EffectParameter & param,
    ID3DXConstantTable             ** consts )
{
    using namespace GN::gfx2;
    using namespace GN::gfx::d3d9;

    if( EFFECT_PARAMETER_TYPE_STRING != param.type )
    {
        GN_ERROR(sLogger)( "Parameter 'PS' accepts only string value." );
        return 0;
    }
    if( 0 == param.str )
    {
        GN_ERROR(sLogger)( "Null PS string." );
        return 0;
    }

    return compilePS( dev, param.str, 0, 0, "main", 0, consts );
}

// *****************************************************************************
// D3D9HlslEffectParameterSet
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx2::D3D9HlslEffectParameterSet::setParameter(
    EffectParameterHandle   handle,
    const EffectParameter & value )
{
    BaseEffectParameterSet::setParameter( handle, value );

    IDirect3DDevice9  * dev = mGs.d3ddev();

    if( handle == mVsHandle )
    {
        mVsConsts.clear();
        mVs.attach( sCreateVs( dev, value, &mVsConsts ) );
    }
    else if( handle == mPsHandle )
    {
        mPsConsts.clear();
        mPs.attach( sCreatePs( dev, value, &mPsConsts ) );
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx2::D3D9HlslEffectParameterSet::unsetParameter( EffectParameterHandle handle )
{
    BaseEffectParameterSet::unsetParameter( handle );

    if( handle == mVsHandle )
    {
        mVs.clear();
        mVsConsts.clear();
    }
    else if( handle == mPsHandle )
    {
        mPs.clear();
        mPsConsts.clear();
    }
}

// *****************************************************************************
// public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx2::D3D9HlslEffect::render(
    const EffectParameterSet & param, EffectBinding binding )
{
    GN_GUARD_SLOW;

    applyBinding( binding );

    const D3D9HlslEffectParameterSet & p = safeCast<const D3D9HlslEffectParameterSet &>(param);
 
    applyVS( p.vs() );
    applyPS( p.ps() );

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
inline void GN::gfx2::D3D9HlslEffect::applyVS( IDirect3DVertexShader9 * vs )
{
    D3D9GraphicsSystem & gs = d3d9gs();
    IDirect3DDevice9  * dev = gs.d3ddev();
    dev->SetVertexShader( vs );
}

//
//
// -----------------------------------------------------------------------------
inline void GN::gfx2::D3D9HlslEffect::applyPS( IDirect3DPixelShader9 * ps )
{
    D3D9GraphicsSystem & gs = d3d9gs();
    IDirect3DDevice9  * dev = gs.d3ddev();
    dev->SetPixelShader( ps );
}
