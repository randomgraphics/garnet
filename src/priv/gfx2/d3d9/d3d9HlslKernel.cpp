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
    IDirect3DDevice9                * dev,
    const GN::gfx::KernelParameter & param,
    ID3DXConstantTable             ** consts )
{
    using namespace GN::gfx;
    using namespace GN::gfx::d3d9;

    if( KERNEL_PARAMETER_TYPE_STRING != param.type )
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
    const GN::gfx::KernelParameter & param,
    ID3DXConstantTable             ** consts )
{
    using namespace GN::gfx;
    using namespace GN::gfx::d3d9;

    if( KERNEL_PARAMETER_TYPE_STRING != param.type )
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
// D3D9HlslKernelParameterSet
// *****************************************************************************

GN::gfx::D3D9HlslKernelParameterSet::D3D9HlslKernelParameterSet( IDirect3DDevice9 * dev, Kernel & e )
    : BaseKernelParameterSet( e )
    , mDev( dev )
    , mVsHandle( e.getParameterHandle( "VS" ) )
    , mPsHandle( e.getParameterHandle( "PS" ) )
    , mVscfHandle( e.getParameterHandle( "VSCF" ) )
    , mPscfHandle( e.getParameterHandle( "PSCF" ) )
{
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D9HlslKernelParameterSet::init()
{
    BaseKernelParameter * p = getParameter( mVsHandle );
    p->sigValueSet.connect( this, &D3D9HlslKernelParameterSet::onVsSet );
    p->sigValueUnset.connect( this, &D3D9HlslKernelParameterSet::onVsUnset );

    p = getParameter( mPsHandle );
    p->sigValueSet.connect( this, &D3D9HlslKernelParameterSet::onPsSet );
    p->sigValueUnset.connect( this, &D3D9HlslKernelParameterSet::onPsUnset );

    p = getParameter( mVscfHandle );
    p->sigValueSet.connect( this, &D3D9HlslKernelParameterSet::onVscfSet );

    p = getParameter( mPscfHandle );
    p->sigValueSet.connect( this, &D3D9HlslKernelParameterSet::onPscfSet );

    return true;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9HlslKernelParameterSet::onVsSet( size_t, size_t )
{
    BaseKernelParameter * p = GN_SAFE_CAST<BaseKernelParameter*>( getParameter( mVsHandle ) );
    GN_ASSERT( p );

    mVsConstBuffer.clear();
    mVs.attach( sCreateVs( mDev, p->toString()[0].cptr(), &mVsConstBuffer ) );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9HlslKernelParameterSet::onVsUnset()
{
    mVs.clear();
    mVsConstBuffer.clear();
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9HlslKernelParameterSet::onPsSet( size_t, size_t )
{
    BaseKernelParameter * p = GN_SAFE_CAST<BaseKernelParameter*>( getParameter( mPsHandle ) );
    GN_ASSERT( p );

    mPsConstBuffer.clear();
    mPs.attach( sCreatePs( mDev, p->toString()[0].cptr(), &mPsConstBuffer ) );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9HlslKernelParameterSet::onPsUnset()
{
    mPs.clear();
    mPsConstBuffer.clear();
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9HlslKernelParameterSet::onVscfSet( size_t offset, size_t count )
{
    mVscfUpdate.append( ConstUpdate( (UInt32)offset, (UInt32)count );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9HlslKernelParameterSet::onPscfSet( size_t offset, size_t count )
{
    mPscfUpdate.append( ConstUpdate( (UInt32)offset, (UInt32)count );
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
    KernelParameterDesc p;

    p.type  = KERNEL_PARAMETER_TYPE_STRING;
    p.count = 1;
    mVs = addParameter( "VS", p );

    p.type  = KERNEL_PARAMETER_TYPE_STRING;
    p.count = 1;
    mPs = addParameter( "PS", p );

    p.type = KERNEL_PARAMETER_TYPE_FLOAT4;
    p.count = 256;
    mVsFloatConstants = addParameter( "VSCF", p );

    p.type = KERNEL_PARAMETER_TYPE_FLOAT4;
    p.count = 256;
    mPsFloatConstants = addParameter( "PSCF", p );

    p.type = KERNEL_PARAMETER_TYPE_INT1;
    p.count = 1;
    mPrimType = addParameter( "PRIM_TYPE", p );

    p.type = KERNEL_PARAMETER_TYPE_INT1;
    p.count = 1;
    mPrimCount = addParameter( "PRIM_COUNT", p );

    p.type = KERNEL_PARAMETER_TYPE_INT1;
    p.count = 1;
    mBaseVertex = addParameter( "BASE_VERTEX", p );

    p.type = KERNEL_PARAMETER_TYPE_INT1;
    p.count = 1;
    mBaseIndex = addParameter( "BASE_INDEX", p );

    p.type = KERNEL_PARAMETER_TYPE_INT1;
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
void GN::gfx::D3D9HlslKernel::render(
    const KernelParameterSet & param, KernelBinding binding )
{
    GN_GUARD_SLOW;

    D3D9KernelBinding & b = getPortBinding( binding );

    b.apply();

    const D3D9HlslKernelParameterSet & p = safeCast<const D3D9HlslKernelParameterSet &>(param);
 
    applyShader( p );

    D3D9GraphicsSystem & gs = d3d9gs();
    IDirect3DDevice9  * dev = gs.d3ddev();

    const KernelParameter
        * pt = param.getParameter( mPrimType ),
        * pc = param.getParameter( mPrimCount ),
        * bv = param.getParameter( mBaseVertex );
    GN_ASSERT( pt && pc && bv );

    if( b.hasIdxBuf() )
    {
        const KernelParameter
            * bi = param.getParameter( mBaseIndex ),
            * vc = param.getParameter( mVertexCount );
        GN_ASSERT( bi && vc );

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
inline void GN::gfx::D3D9HlslKernel::applyShader( const D3D9HlslKernelParameterSet & param )
{
    D3D9GraphicsSystem & gs = d3d9gs();
    IDirect3DDevice9  * dev = gs.d3ddev();

    dev->SetVertexShader( param.vs() );
    dev->SetPixelShader( param.ps() );

    const KernelParameter * vscf = param.getParameter( mVsFloatConstants );
    if( vscf )
    {
        dev->SetVertexShaderConstantF( 0, (const float*)vscf->toRaw(), (UINT)(vscf->raw.bytes / sizeof(Vector4f)) );
    }

    const KernelParameter * pscf = param.getParameter( mPsFloatConstants );
    if( pscf )
    {
        dev->SetPixelShaderConstantF( 0, (const float*)pscf->toRaw(), (UINT)(pscf->raw.bytes / sizeof(Vector4f)) );
    }
}
