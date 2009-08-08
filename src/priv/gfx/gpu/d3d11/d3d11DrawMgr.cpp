#include "pch.h"
#include "d3d11Renderer.h"
//#include "d3d11RenderTargetMgr.h"

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
    size_t                 size,
    UInt32                 bindFlags,
    const void           * data )
{
    GN_ASSERT( buffer );

    if( NULL == *buffer )
    {
        // create new buffer

        D3D11_BUFFER_DESC desc;
        desc.ByteWidth      = (UInt32)size;
        desc.Usage          = D3D11_USAGE_DYNAMIC;
        desc.BindFlags      = bindFlags;
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        desc.MiscFlags      = 0;
        GN_DX10_CHECK_RV( dev.CreateBuffer( &desc, NULL, buffer ), false );
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
            desc.ByteWidth = (UInt32)size;

            (*buffer)->Release();

            GN_DX10_CHECK_RV( dev.CreateBuffer( &desc, NULL, buffer ), false );
        }
    }

    // update buffer content
    D3D11_MAPPED_SUBRESOURCE dst;
    GN_DX10_CHECK_RV( context.Map( (*buffer), 0, D3D11_MAP_WRITE_DISCARD, 0, &dst ), false );
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
    ID3D11DeviceContext & mDevice;
    ID3D11Buffer        * mOldVB;
    UInt32                mOldStride;
    UInt32                mOldVBOffset;
    ID3D11Buffer        * mOldIB;
    DXGI_FORMAT           mOldFormat;
    UInt32                mOldIBOffset;

public:

    D3D11RestoreVB0AndIB( ID3D11DeviceContext & dev ) : mDevice( dev )
    {
        mDevice.IAGetVertexBuffers( 0, 1, &mOldVB, &mOldStride, &mOldVBOffset );
        mDevice.IAGetIndexBuffer( &mOldIB, &mOldFormat, &mOldIBOffset );
    }

    ~D3D11RestoreVB0AndIB()
    {
        mDevice.IASetVertexBuffers( 0, 1, &mOldVB, &mOldStride, &mOldVBOffset );
        mDevice.IASetIndexBuffer( mOldIB, mOldFormat, mOldIBOffset );
    }
};

// *****************************************************************************
// init/quit
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D11Renderer::drawInit()
{
    return true;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D11Renderer::drawQuit()
{
    //safeRelease( mUserVB );
    //safeRelease( mUserIB );
}

// *****************************************************************************
// from Renderer
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D11Renderer::present()
{
    GN_DX10_CHECK( mSwapChain->Present( getOptions().vsync ? 1 : 0, 0 ) );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D11Renderer::clearScreen(
    const GN::Vector4f & c, float z, UInt8 s, BitFields flags )
{
    PIXPERF_FUNCTION_EVENT();

    /*if( CLEAR_C & flags )
    {
        for( UInt32 i = 0; i < mRTMgr->getRenderTargetCount(); ++i )
        {
            mDevice->ClearRenderTargetView( mRTMgr->getRenderTargetView( i ) , c );
        }
    }

    ID3D11DepthStencilView * dsview = mRTMgr->getDepthStencilView();
    UInt32 d3dflag = 0;
    if( CLEAR_Z ) d3dflag |= D3D11_CLEAR_DEPTH;
    if( CLEAR_S ) d3dflag |= D3D11_CLEAR_STENCIL;
    if( d3dflag && dsview )
    {
        mDevice->ClearDepthStencilView( dsview, d3dflag, z, s );
    }*/
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D11Renderer::drawIndexed(
    PrimitiveType prim,
    size_t        numidx,
    size_t        basevtx,
    size_t        ,//startvtx,
    size_t        ,//numvtx,
    size_t        startidx )
{
    PIXPERF_FUNCTION_EVENT();

    mDeviceContext->IASetPrimitiveTopology( sD3D11PrimMap[prim] );
    mDeviceContext->DrawIndexed(
        (UINT)numidx,
        (UINT)startidx,
        (INT)basevtx );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D11Renderer::draw(
    PrimitiveType prim,
    size_t        numvtx,
    size_t        startvtx )
{
    PIXPERF_FUNCTION_EVENT();

    mDeviceContext->IASetPrimitiveTopology( sD3D11PrimMap[prim] );
    mDeviceContext->Draw( (UINT)numvtx, (UINT)startvtx );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D11Renderer::drawIndexedUp(
    PrimitiveType  prim,
    size_t         numidx,
    size_t         numvtx,
    const void   * vertexData,
    size_t         strideInBytes,
    const UInt16 * indexData )
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
    UInt32 stride = (UInt32)strideInBytes;
    UInt32 offset = 0;
    mDeviceContext->IASetVertexBuffers( 0, 1, &mUserVB, &stride, &offset );
    mDeviceContext->IASetIndexBuffer( mUserIB, DXGI_FORMAT_R16_UINT, 0 );

    // do rendering
    mDeviceContext->IASetPrimitiveTopology( sD3D11PrimMap[prim] );
    mDeviceContext->DrawIndexed( (UINT)numidx, 0, 0 );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D11Renderer::drawUp(
    PrimitiveType prim,
    size_t        numvtx,
    const void *  vertexData,
    size_t        strideInBytes )
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
    UInt32 stride = (UInt32)strideInBytes;
    UInt32 offset = 0;
    mDeviceContext->IASetVertexBuffers( 0, 1, &mUserVB, &stride, &offset );

    // do rendering
    mDeviceContext->IASetPrimitiveTopology( sD3D11PrimMap[prim] );
    mDeviceContext->Draw( (UINT)numvtx, 0 );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D11Renderer::drawLines(
    BitFields         /*options*/,
    const void      * /*positions*/,
    size_t            /*stride*/,
    size_t            /*numPoints*/,
    UInt32            /*rgba*/,
    const Matrix44f & /*model*/,
    const Matrix44f & /*view*/,
    const Matrix44f & /*proj*/ )
{
    PIXPERF_FUNCTION_EVENT();

    GN_UNIMPL_WARNING();
}
