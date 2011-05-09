#include "pch.h"
#include "d3d11Gpu.h"
#include "d3d11RenderTargetMgr.h"

// static primitive map
static D3D11_PRIMITIVE_TOPOLOGY sD3D11PrimMap[GN::gfx::PrimitiveType::NUM_PRIMITIVES] =
{
    D3D11_PRIMITIVE_TOPOLOGY_POINTLIST,
    D3D11_PRIMITIVE_TOPOLOGY_LINELIST,
    D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP,
    D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
    D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
    D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED, // no quad list
    D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED, // no rect list
};

//
//
// -----------------------------------------------------------------------------
static bool
sSetupUserD3D11Buffer(
    ID3D11Device         & dev,
    ID3D11DeviceContext  & context,
    ID3D11Buffer        ** buffer,
    uint32                 size,
    uint32                 bindFlags,
    const void           * data )
{
    GN_ASSERT( buffer );

    if( NULL == *buffer )
    {
        // create new buffer

        D3D11_BUFFER_DESC desc;
        desc.ByteWidth      = (uint32)size;
        desc.Usage          = D3D11_USAGE_DYNAMIC;
        desc.BindFlags      = bindFlags;
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        desc.MiscFlags      = 0;
        GN_DX_CHECK_RETURN( dev.CreateBuffer( &desc, NULL, buffer ), false );
    }
    else
    {
        // Create new buffer with new size,
        // if current buffer is not large enough

        D3D11_BUFFER_DESC desc;
        (*buffer)->GetDesc( &desc );

        GN_ASSERT( desc.BindFlags == bindFlags );

        if( desc.ByteWidth < size )
        {
            desc.ByteWidth = size;

            (*buffer)->Release();

            GN_DX_CHECK_RETURN( dev.CreateBuffer( &desc, NULL, buffer ), false );
        }
    }

    // update buffer content
    D3D11_MAPPED_SUBRESOURCE dst;
    GN_DX_CHECK_RETURN( context.Map( (*buffer), 0, D3D11_MAP_WRITE_DISCARD, 0, &dst ), false );
    memcpy( dst.pData, data, size );
    context.Unmap( (*buffer), 0 );

    // success
    return true;
}

///
/// class to automatically store/restore VB0 and IB
///
class D3D11RestoreVB0AndIB
{
    ID3D11DeviceContext & mDevcxt;
    ID3D11Buffer        * mOldVB;
    uint32                mOldStride;
    uint32                mOldVBOffset;
    ID3D11Buffer        * mOldIB;
    DXGI_FORMAT           mOldFormat;
    uint32                mOldIBOffset;

public:

    D3D11RestoreVB0AndIB( ID3D11DeviceContext & devcxt )
        : mDevcxt( devcxt )
        , mOldVB(0)
        , mOldIB(0)
    {
        mDevcxt.IAGetVertexBuffers( 0, 1, &mOldVB, &mOldStride, &mOldVBOffset );
        mDevcxt.IAGetIndexBuffer( &mOldIB, &mOldFormat, &mOldIBOffset );
    }

    ~D3D11RestoreVB0AndIB()
    {
        mDevcxt.IASetVertexBuffers( 0, 1, &mOldVB, &mOldStride, &mOldVBOffset );
        mDevcxt.IASetIndexBuffer( mOldIB, mOldFormat, mOldIBOffset );

        GN::safeRelease( mOldVB );
        GN::safeRelease( mOldIB );
    }
};

// *****************************************************************************
// init/quit
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D11Gpu::drawInit()
{
    return true;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D11Gpu::drawQuit()
{
    safeRelease( mUserVB );
    safeRelease( mUserIB );
}

// *****************************************************************************
// from Gpu
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D11Gpu::present()
{
    GN_DX_CHECK( mSwapChain->Present( getOptions().vsync ? 1 : 0, 0 ) );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D11Gpu::clearScreen(
    const GN::Vector4f & c, float z, uint8 s, uint32 flags )
{
    PIXPERF_FUNCTION_EVENT();

    if( CLEAR_C & flags )
    {
        for( uint32 i = 0; i < mRTMgr->getRenderTargetCount(); ++i )
        {
            mDeviceContext->ClearRenderTargetView( mRTMgr->getRenderTargetView( i ) , c );
        }
    }

    ID3D11DepthStencilView * dsview = mRTMgr->getDepthStencilView();
    uint32 d3dflag = 0;
    if( CLEAR_Z ) d3dflag |= D3D11_CLEAR_DEPTH;
    if( CLEAR_S ) d3dflag |= D3D11_CLEAR_STENCIL;
    if( d3dflag && dsview )
    {
        mDeviceContext->ClearDepthStencilView( dsview, d3dflag, z, s );
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D11Gpu::drawIndexed(
    PrimitiveType prim,
    uint32        numidx,
    uint32        basevtx,
    uint32        ,//startvtx,
    uint32        ,//numvtx,
    uint32        startidx )
{
    PIXPERF_FUNCTION_EVENT();

    mDeviceContext->IASetPrimitiveTopology( sD3D11PrimMap[prim] );
    mDeviceContext->DrawIndexed(
        numidx,
        startidx,
        (INT)basevtx );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D11Gpu::draw(
    PrimitiveType prim,
    uint32        numvtx,
    uint32        startvtx )
{
    PIXPERF_FUNCTION_EVENT();

    mDeviceContext->IASetPrimitiveTopology( sD3D11PrimMap[prim] );
    mDeviceContext->Draw( numvtx, startvtx );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D11Gpu::drawIndexedUp(
    PrimitiveType  prim,
    uint32         numidx,
    uint32         numvtx,
    const void   * vertexData,
    uint32         strideInBytes,
    const uint16 * indexData )
{
    PIXPERF_FUNCTION_EVENT();

    if( !sSetupUserD3D11Buffer(
        *mDevice,
        *mDeviceContext,
        &mUserVB,
        strideInBytes * numvtx,
        D3D11_BIND_VERTEX_BUFFER,
        vertexData ) )
    {
        return;
    }

    if( !sSetupUserD3D11Buffer(
        *mDevice,
        *mDeviceContext,
        &mUserIB,
        strideInBytes * numidx,
        D3D11_BIND_INDEX_BUFFER,
        indexData ) )
    {
        return;
    }

    // this will restore VB0 and IB when function exists.
    D3D11RestoreVB0AndIB autoRestore( *mDeviceContext );

    // setup vertex buffer and index buffer
    uint32 stride = (uint32)strideInBytes;
    uint32 offset = 0;
    mDeviceContext->IASetVertexBuffers( 0, 1, &mUserVB, &stride, &offset );
    mDeviceContext->IASetIndexBuffer( mUserIB, DXGI_FORMAT_R16_UINT, 0 );

    // do rendering
    mDeviceContext->IASetPrimitiveTopology( sD3D11PrimMap[prim] );
    mDeviceContext->DrawIndexed( numidx, 0, 0 );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D11Gpu::drawUp(
    PrimitiveType prim,
    uint32        numvtx,
    const void *  vertexData,
    uint32        strideInBytes )
{
    PIXPERF_FUNCTION_EVENT();

    if( !sSetupUserD3D11Buffer(
        *mDevice,
        *mDeviceContext,
        &mUserVB,
        strideInBytes * numvtx,
        D3D11_BIND_VERTEX_BUFFER,
        vertexData ) )
    {
        return;
    }

    // this will restore VB0 and IB when function exists.
    D3D11RestoreVB0AndIB autoRestore( *mDeviceContext );

    // setup vertex buffer and index buffer
    uint32 stride = (uint32)strideInBytes;
    uint32 offset = 0;
    mDeviceContext->IASetVertexBuffers( 0, 1, &mUserVB, &stride, &offset );

    // do rendering
    mDeviceContext->IASetPrimitiveTopology( sD3D11PrimMap[prim] );
    mDeviceContext->Draw( numvtx, 0 );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D11Gpu::drawLines(
    uint32            /*options*/,
    const void      * /*positions*/,
    uint32            /*stride*/,
    uint32            /*numPoints*/,
    uint32            /*rgba*/,
    const Matrix44f & /*model*/,
    const Matrix44f & /*view*/,
    const Matrix44f & /*proj*/ )
{
    PIXPERF_FUNCTION_EVENT();

    GN_UNIMPL_WARNING();
}
