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
GN::gfx2::D3D9HlslEffect::D3D9HlslEffect( D3D9GraphicsSystem & gs )
    : D3D9Effect( gs )
    , mRenderTarget0( gs )
    , mRenderTarget1( gs )
    , mRenderTarget2( gs )
    , mRenderTarget3( gs )
    , mDepthBuffer( gs )
    , mTexture0( gs )
    , mTexture1( gs )
    , mTexture2( gs )
    , mTexture3( gs )
    , mTexture4( gs )
    , mTexture5( gs )
    , mTexture6( gs )
    , mTexture7( gs )
    , mVtxBuf0( gs, 0 )
    , mVtxBuf1( gs, 1 )
    , mVtxBuf2( gs, 2 )
    , mVtxBuf3( gs, 3 )
    , mVtxBuf4( gs, 4 )
    , mVtxBuf5( gs, 5 )
    , mVtxBuf6( gs, 6 )
    , mVtxBuf7( gs, 7 )
    , mIdxBuf( gs )
{
    // setup parameters
    EffectParameterDesc p;

    p.type  = EFFECT_PARAMETER_TYPE_STRING;
    p.count = 1;
    mVs = addParameter( "VS", p );

    p.type  = EFFECT_PARAMETER_TYPE_STRING;
    p.count = 1;
    mPs = addParameter( "PS", p );

    p.type = EFFECT_PARAMETER_TYPE_FLOAT4;
    p.count = 256;
    mVsConstants = addParameter( "VSC", p );

    p.type = EFFECT_PARAMETER_TYPE_FLOAT4;
    p.count = 256;
    mPsConstants = addParameter( "PSC", p );

    p.type = EFFECT_PARAMETER_TYPE_INT1;
    p.count = 1;
    mPrimType = addParameter( "PRIM_TYPE", p );

    p.type = EFFECT_PARAMETER_TYPE_INT1;
    p.count = 1;
    mPrimCount = addParameter( "PRIM_COUNT", p );

    p.type = EFFECT_PARAMETER_TYPE_INT1;
    p.count = 1;
    mBaseVertex = addParameter( "BASE_VERTEX", p );

    p.type = EFFECT_PARAMETER_TYPE_INT1;
    p.count = 1;
    mBaseIndex = addParameter( "BASE_INDEX", p );

    p.type = EFFECT_PARAMETER_TYPE_INT1;
    p.count = 1;
    mVertexCount = addParameter( "VERTEX_COUNT", p );

    // setup ports
    addPortRef( "TARGET0"  , &mRenderTarget0 );
    addPortRef( "TARGET1"  , &mRenderTarget1 );
    addPortRef( "TARGET2"  , &mRenderTarget2 );
    addPortRef( "TARGET3"  , &mRenderTarget3 );
    addPortRef( "DEPTH"    , &mDepthBuffer );
    addPortRef( "TEXTURE0" , &mTexture0 );
    addPortRef( "TEXTURE1" , &mTexture1 );
    addPortRef( "TEXTURE2" , &mTexture2 );
    addPortRef( "TEXTURE3" , &mTexture3 );
    addPortRef( "TEXTURE4" , &mTexture4 );
    addPortRef( "TEXTURE5" , &mTexture5 );
    addPortRef( "TEXTURE6" , &mTexture6 );
    addPortRef( "TEXTURE7" , &mTexture7 );
    addPortRef( "VTXBUF0"  , &mVtxBuf0 );
    addPortRef( "VTXBUF1"  , &mVtxBuf1 );
    addPortRef( "VTXBUF2"  , &mVtxBuf2 );
    addPortRef( "VTXBUF3"  , &mVtxBuf3 );
    addPortRef( "VTXBUF4"  , &mVtxBuf4 );
    addPortRef( "VTXBUF5"  , &mVtxBuf5 );
    addPortRef( "VTXBUF6"  , &mVtxBuf6 );
    addPortRef( "VTXBUF7"  , &mVtxBuf7 );
    addPortRef( "IDXBUF"   , &mIdxBuf );

    // setup properties
    setProperty( "RENDER_TARGET_COUNT", 4 );
    setProperty( "TEXTURE_COUNT", 4 );
    setProperty( "VTXBUF_COUNT", 4 );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx2::D3D9HlslEffect::render(
    const EffectParameterSet & param, EffectBinding binding )
{
    GN_GUARD_SLOW;

    D3D9EffectBinding & b = getPortBinding( binding );

    b.apply();

    const D3D9HlslEffectParameterSet & p = safeCast<const D3D9HlslEffectParameterSet &>(param);
 
    applyVS( p.vs() );
    applyPS( p.ps() );

    D3D9GraphicsSystem & gs = d3d9gs();
    IDirect3DDevice9  * dev = gs.d3ddev();

    const EffectParameter
        * pt = param.getParameter( mPrimType ),
        * pc = param.getParameter( mPrimCount ),
        * bv = param.getParameter( mBaseVertex );
    GN_ASSERT( pt && pc && bv );

    if( b.hasIdxBuf() )
    {
        const EffectParameter
            * bi = param.getParameter( mBaseIndex ),
            * vc = param.getParameter( mVertexCount );

        GN_DX9_CHECK( dev->DrawIndexedPrimitive(
            (D3DPRIMITIVETYPE)pt->toInt1(),
            bv->toInt1(),
            0, // min index
            vc->toUInt1(),
            bi->toUInt1(),
            pc->toUInt1() ) );
    }
    else
    {
        GN_DX9_CHECK( dev->DrawPrimitive(
            (D3DPRIMITIVETYPE)pt->toInt1(),
            bv->toUInt1(),
            pc->toUInt1() ) );
    }


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
