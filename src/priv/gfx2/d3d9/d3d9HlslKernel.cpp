#include "pch.h"
#include "d3d9HlslKernel.h"
#include "garnet/GNgfx.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx2.D3D9HlslKernel");

// *****************************************************************************
// local functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
static IDirect3DVertexShader9 * sCreateVs(
    IDirect3DDevice9                   * dev,
    const GN::gfx::BaseKernelParameter & param,
    ID3DXConstantTable                ** consts )
{
    using namespace GN;
    using namespace GN::gfx;
    using namespace GN::gfx::d3d9;

    if( KERNEL_PARAMETER_TYPE_STRING != param.getDesc().type )
    {
        GN_ERROR(sLogger)( "Parameter 'VS' accepts only string value." );
        return false;
    }
    const StrA * s = param.toString();
    if( 0 == s || s->empty() )
    {
        GN_ERROR(sLogger)( "Null VS string." );
        return false;
    }

    return compileVS( dev, s->cptr(), 0, 0, "main", 0, consts );
}

//
//
// -----------------------------------------------------------------------------
static IDirect3DPixelShader9 * sCreatePs(
    IDirect3DDevice9                   * dev,
    const GN::gfx::BaseKernelParameter & param,
    ID3DXConstantTable                ** consts )
{
    using namespace GN;
    using namespace GN::gfx;
    using namespace GN::gfx::d3d9;

    if( KERNEL_PARAMETER_TYPE_STRING != param.getDesc().type )
    {
        GN_ERROR(sLogger)( "Parameter 'PS' accepts only string value." );
        return 0;
    }
    const StrA * s = param.toString();
    if( 0 == s || s->empty() )
    {
        GN_ERROR(sLogger)( "Null PS string." );
        return 0;
    }

    return compilePS( dev, s->cptr(), 0, 0, "main", 0, consts );
}

// *****************************************************************************
// D3D9HlslKernelParameterSet
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::gfx::D3D9HlslKernelParameterSet::D3D9HlslKernelParameterSet( D3D9Kernel & k )
    : BaseKernelParameterSet( k )
    , mDev( k.d3d9gs().d3ddev() )
    , mRsb( k.d3d9gs() )
    , mVsHandle( k.getParameterIndex( "VS" ) )
    , mPsHandle( k.getParameterIndex( "PS" ) )
    , mVscfHandle( k.getParameterIndex( "VSCF" ) )
    , mPscfHandle( k.getParameterIndex( "PSCF" ) )
    , mBlending( mRsb, *getBaseParameterByName( "BLENDING" ), D3DRS_ALPHABLENDENABLE )
{
    mVscfUpdate.clear();
    mPscfUpdate.clear();

    BaseKernelParameter * p = getBaseParameterByIndex( mVsHandle );
    p->sigValueSet.connect( this, &D3D9HlslKernelParameterSet::onVsSet );
    p->sigValueUnset.connect( this, &D3D9HlslKernelParameterSet::onVsUnset );

    p = getBaseParameterByIndex( mPsHandle );
    p->sigValueSet.connect( this, &D3D9HlslKernelParameterSet::onPsSet );
    p->sigValueUnset.connect( this, &D3D9HlslKernelParameterSet::onPsUnset );

    p = getBaseParameterByIndex( mVscfHandle );
    p->sigValueSet.connect( this, &D3D9HlslKernelParameterSet::onVscfSet );

    p = getBaseParameterByIndex( mPscfHandle );
    p->sigValueSet.connect( this, &D3D9HlslKernelParameterSet::onPscfSet );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9HlslKernelParameterSet::apply() const
{
    GN_GUARD_SLOW;

    // apply shader
    mDev->SetVertexShader( mVs );
    mDev->SetPixelShader( mPs );

    // apply shader constants
    BaseKernelParameter * p;
    const float * v;
    if( mVscfUpdate.registerCount > 0 )
    {
        p = GN_SAFE_CAST<BaseKernelParameter*>( getParameter( mVscfHandle ) );
        v = p->toFloat();
        mDev->SetVertexShaderConstantF(
            mVscfUpdate.firstRegister,
            v + mVscfUpdate.firstRegister * 4,
            mVscfUpdate.registerCount );
    }
    if( mPscfUpdate.registerCount > 0 )
    {
        p = GN_SAFE_CAST<BaseKernelParameter*>( getParameter( mPscfHandle ) );
        v = p->toFloat();
        mDev->SetPixelShaderConstantF(
            mPscfUpdate.firstRegister,
            v + mPscfUpdate.firstRegister * 4,
            mPscfUpdate.registerCount );
    }

    // apply render states
    GN_SAFE_CAST<D3D9Kernel&>(getKernel()).d3d9gs().setRenderStateBlock( mRsb );

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9HlslKernelParameterSet::onVsSet( size_t, size_t, size_t )
{
    BaseKernelParameter * p = GN_SAFE_CAST<BaseKernelParameter*>( getParameter( mVsHandle ) );
    GN_ASSERT( p );

    mVsConstBuffer.clear();
    mVs.attach( sCreateVs( mDev, *p, &mVsConstBuffer ) );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9HlslKernelParameterSet::onVsUnset( size_t )
{
    mVs.clear();
    mVsConstBuffer.clear();
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9HlslKernelParameterSet::onPsSet( size_t, size_t, size_t )
{
    BaseKernelParameter * p = GN_SAFE_CAST<BaseKernelParameter*>( getParameter( mPsHandle ) );
    GN_ASSERT( p );

    mPsConstBuffer.clear();
    mPs.attach( sCreatePs( mDev, *p, &mPsConstBuffer ) );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9HlslKernelParameterSet::onPsUnset( size_t )
{
    mPs.clear();
    mPsConstBuffer.clear();
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9HlslKernelParameterSet::onVscfSet( size_t, size_t offset, size_t count )
{
    mVscfUpdate.merge( (UInt32)offset/4, (UInt32)count/4 );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9HlslKernelParameterSet::onPscfSet( size_t, size_t offset, size_t count )
{
    mPscfUpdate.merge( (UInt32)offset/4, (UInt32)count/4 );
}

// *****************************************************************************
// public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::gfx::D3D9HlslKernel::D3D9HlslKernel( D3D9GraphicsSystem & gs )
    : D3D9Kernel( gs )
    , mRenderTarget0( gs )
    , mRenderTarget1( gs )
    , mRenderTarget2( gs )
    , mRenderTarget3( gs )
    , mDepthBuffer( gs )
    , mTexture0( gs, 0 )
    , mTexture1( gs, 1 )
    , mTexture2( gs, 2 )
    , mTexture3( gs, 3 )
    , mTexture4( gs, 4 )
    , mTexture5( gs, 5 )
    , mTexture6( gs, 6 )
    , mTexture7( gs, 7 )
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
    addParameter( "VS", KERNEL_PARAMETER_TYPE_STRING, 1 );
    addParameter( "PS", KERNEL_PARAMETER_TYPE_STRING, 1 );

    addParameter( "VSCF", KERNEL_PARAMETER_TYPE_FLOAT, 256 * 4 );
    addParameter( "PSCF", KERNEL_PARAMETER_TYPE_FLOAT, 256 * 4 );

    mPrimType = addParameter( "PRIM_TYPE", KERNEL_PARAMETER_TYPE_INT, 1 );
    mPrimCount = addParameter( "PRIM_COUNT", KERNEL_PARAMETER_TYPE_INT, 1 );
    mBaseVertex = addParameter( "BASE_VERTEX", KERNEL_PARAMETER_TYPE_INT, 1 );
    mBaseIndex = addParameter( "BASE_INDEX", KERNEL_PARAMETER_TYPE_INT, 1 );
    mVertexCount = addParameter( "VERTEX_COUNT", KERNEL_PARAMETER_TYPE_INT, 1 );

    // render states
    addParameter( "BLENDING", KERNEL_PARAMETER_TYPE_INT, 1 );

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
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::KernelParameterSet *
GN::gfx::D3D9HlslKernel::createParameterSet()
{
    GN_GUARD;

    return new D3D9HlslKernelParameterSet( *this );

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9HlslKernel::render(
    const KernelParameterSet & param, KernelBinding binding )
{
    GN_GUARD_SLOW;

    PIXPERF_FUNCTION_EVENT();

    D3D9KernelBinding & b = getPortBinding( binding );

    b.apply();

    const D3D9HlslKernelParameterSet & p = safeCast<const D3D9HlslKernelParameterSet &>(param);

    p.apply();

    D3D9GraphicsSystem & gs = d3d9gs();
    IDirect3DDevice9  * dev = gs.d3ddev();

    const BaseKernelParameter
        * pt = GN_SAFE_CAST<BaseKernelParameter*>( param.getParameter( mPrimType ) ),
        * pc = GN_SAFE_CAST<BaseKernelParameter*>( param.getParameter( mPrimCount ) ),
        * bv = GN_SAFE_CAST<BaseKernelParameter*>( param.getParameter( mBaseVertex ) );
    GN_ASSERT( pt && !pt->empty() );
    GN_ASSERT( pc && !pc->empty() );
    GN_ASSERT( bv && !bv->empty() );

    if( b.hasIdxBuf() )
    {
        const BaseKernelParameter
            * bi = GN_SAFE_CAST<BaseKernelParameter*>( param.getParameter( mBaseIndex ) ),
            * vc = GN_SAFE_CAST<BaseKernelParameter*>( param.getParameter( mVertexCount ) );
        GN_ASSERT( bi && !bi->empty() );
        GN_ASSERT( vc && !vc->empty() );

        GN_DX9_CHECK( dev->DrawIndexedPrimitive(
            (D3DPRIMITIVETYPE)*pt->toInt(),
            *bv->toInt(),
            0, // min index
            *vc->toUInt(),
            *bi->toUInt(),
            *pc->toUInt() ) );
    }
    else
    {
        GN_DX9_CHECK( dev->DrawPrimitive(
            (D3DPRIMITIVETYPE)*pt->toInt(),
            *bv->toUInt(),
            *pc->toUInt() ) );
    }


    GN_UNGUARD_SLOW;
}

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
