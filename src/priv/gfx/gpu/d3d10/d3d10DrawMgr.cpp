#include "pch.h"
#include "d3d10Gpu.h"
#include "d3d10RenderTargetMgr.h"

// static primitive map

//
//
// -----------------------------------------------------------------------------
static inline D3D10_PRIMITIVE_TOPOLOGY
sPrimType2D3D10( GN::gfx::PrimitiveType prim )
{
    static D3D10_PRIMITIVE_TOPOLOGY sD3D10PrimMap[GN::gfx::PrimitiveType::NUM_PRIMITIVES] =
    {
        D3D10_PRIMITIVE_TOPOLOGY_POINTLIST,
        D3D10_PRIMITIVE_TOPOLOGY_LINELIST,
        D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP,
        D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
        D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
        D3D10_PRIMITIVE_TOPOLOGY_UNDEFINED, // no quad list
        D3D10_PRIMITIVE_TOPOLOGY_UNDEFINED, // no rect list
    };

    GN_ASSERT( prim <= GN_ARRAY_COUNT(sD3D10PrimMap) );
    return sD3D10PrimMap[prim];
}

//
//
// -----------------------------------------------------------------------------
static bool
sSetupUserD3D10Buffer(
    ID3D10Device  & dev,
    ID3D10Buffer ** buffer,
    size_t          size,
    UInt32          bindFlags,
    const void    * data )
{
    GN_ASSERT( buffer );

    if( NULL == *buffer )
    {
        // create new buffer

        D3D10_BUFFER_DESC desc;
        desc.ByteWidth      = (UInt32)size;
        desc.Usage          = D3D10_USAGE_DYNAMIC;
        desc.BindFlags      = bindFlags;
        desc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
        desc.MiscFlags      = 0;
        GN_DX_CHECK_RETURN( dev.CreateBuffer( &desc, NULL, buffer ), false );
    }
    else
    {
        // Create new buffer with new size,
        // if current buffer is not large enough

        D3D10_BUFFER_DESC desc;
        (*buffer)->GetDesc( &desc );

        GN_ASSERT( desc.BindFlags == bindFlags );

        if( desc.ByteWidth < size )
        {
            desc.ByteWidth = (UInt32)size;

            (*buffer)->Release();

            GN_DX_CHECK_RETURN( dev.CreateBuffer( &desc, NULL, buffer ), false );
        }
    }

    // update buffer content
    void * dstptr;
    GN_DX_CHECK_RETURN( (*buffer)->Map( D3D10_MAP_WRITE_DISCARD, 0, &dstptr ), false );
    memcpy( dstptr, data, size );
    (*buffer)->Unmap();

    // success
    return true;
}

///
/// class to automatically store/restore VB0 and IB
///
class D3D10RestoreVB0AndIB
{
    ID3D10Device & mDevice;
    ID3D10Buffer * mOldVB;
    UInt32         mOldStride;
    UInt32         mOldVBOffset;
    ID3D10Buffer * mOldIB;
    DXGI_FORMAT    mOldFormat;
    UInt32         mOldIBOffset;

public:

    D3D10RestoreVB0AndIB( ID3D10Device & dev )
        : mDevice( dev )
        , mOldVB(0)
        , mOldIB(0)
    {
        mDevice.IAGetVertexBuffers( 0, 1, &mOldVB, &mOldStride, &mOldVBOffset );
        mDevice.IAGetIndexBuffer( &mOldIB, &mOldFormat, &mOldIBOffset );
    }

    ~D3D10RestoreVB0AndIB()
    {
        mDevice.IASetVertexBuffers( 0, 1, &mOldVB, &mOldStride, &mOldVBOffset );
        mDevice.IASetIndexBuffer( mOldIB, mOldFormat, mOldIBOffset );

        GN::SafeRelease( mOldVB );
        GN::SafeRelease( mOldIB );
    }
};

// *****************************************************************************
// init/quit
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D10Gpu::drawInit()
{
    return true;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D10Gpu::drawQuit()
{
    SafeRelease( mUserVB );
    SafeRelease( mUserIB );
}

// *****************************************************************************
// from Gpu
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D10Gpu::present()
{
    GN_DX_CHECK( mSwapChain->Present( getOptions().vsync ? 1 : 0, 0 ) );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D10Gpu::clearScreen(
    const GN::Vector4f & c, float z, UInt8 s, BitFields flags )
{
    PIXPERF_FUNCTION_EVENT();

    if( CLEAR_C & flags )
    {
        for( UInt32 i = 0; i < mRTMgr->getRenderTargetCount(); ++i )
        {
            mDevice->ClearRenderTargetView( mRTMgr->getRenderTargetView( i ) , c );
        }
    }

    ID3D10DepthStencilView * dsview = mRTMgr->getDepthStencilView();
    UInt32 d3dflag = 0;
    if( CLEAR_Z ) d3dflag |= D3D10_CLEAR_DEPTH;
    if( CLEAR_S ) d3dflag |= D3D10_CLEAR_STENCIL;
    if( d3dflag && dsview )
    {
        mDevice->ClearDepthStencilView( dsview, d3dflag, z, s );
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D10Gpu::drawIndexed(
    PrimitiveType prim,
    size_t        numidx,
    size_t        basevtx,
    size_t        ,//startvtx,
    size_t        ,//numvtx,
    size_t        startidx )
{
    PIXPERF_FUNCTION_EVENT();

    mDevice->IASetPrimitiveTopology( sPrimType2D3D10( prim ) );
    mDevice->DrawIndexed(
        (UINT)numidx,
        (UINT)startidx,
        (INT)basevtx );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D10Gpu::Draw(
    PrimitiveType prim,
    size_t        numvtx,
    size_t        startvtx )
{
    PIXPERF_FUNCTION_EVENT();

    mDevice->IASetPrimitiveTopology( sPrimType2D3D10( prim ) );
    mDevice->Draw( (UINT)numvtx, (UINT)startvtx );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D10Gpu::drawIndexedUp(
    PrimitiveType  prim,
    size_t         numidx,
    size_t         numvtx,
    const void   * vertexData,
    size_t         strideInBytes,
    const UInt16 * indexData )
{
    PIXPERF_FUNCTION_EVENT();

    if( !sSetupUserD3D10Buffer(
        *mDevice,
        &mUserVB,
        strideInBytes * numvtx,
        D3D10_BIND_VERTEX_BUFFER,
        vertexData ) )
    {
        return;
    }

    if( !sSetupUserD3D10Buffer(
        *mDevice,
        &mUserIB,
        strideInBytes * numidx,
        D3D10_BIND_INDEX_BUFFER,
        indexData ) )
    {
        return;
    }

    // this will restore VB0 and IB when function exists.
    D3D10RestoreVB0AndIB autoRestore( *mDevice );

    // setup vertex buffer and index buffer
    UInt32 stride = (UInt32)strideInBytes;
    UInt32 offset = 0;
    mDevice->IASetVertexBuffers( 0, 1, &mUserVB, &stride, &offset );
    mDevice->IASetIndexBuffer( mUserIB, DXGI_FORMAT_R16_UINT, 0 );

    // do rendering
    mDevice->IASetPrimitiveTopology( sPrimType2D3D10( prim ) );
    mDevice->DrawIndexed( (UINT)numidx, 0, 0 );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D10Gpu::drawUp(
    PrimitiveType prim,
    size_t        numvtx,
    const void *  vertexData,
    size_t        strideInBytes )
{
    PIXPERF_FUNCTION_EVENT();

    if( !sSetupUserD3D10Buffer(
        *mDevice,
        &mUserVB,
        strideInBytes * numvtx,
        D3D10_BIND_VERTEX_BUFFER,
        vertexData ) )
    {
        return;
    }

    // this will restore VB0 and IB when function exists.
    D3D10RestoreVB0AndIB autoRestore( *mDevice );

    // setup vertex buffer and index buffer
    UInt32 stride = (UInt32)strideInBytes;
    UInt32 offset = 0;
    mDevice->IASetVertexBuffers( 0, 1, &mUserVB, &stride, &offset );

    // do rendering
    mDevice->IASetPrimitiveTopology( sPrimType2D3D10( prim ) );
    mDevice->Draw( (UINT)numvtx, 0 );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D10Gpu::drawLines(
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
