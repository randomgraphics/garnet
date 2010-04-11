#include "pch.h"
#include "mydevice9.h"
#include "myvtxbuf9.h"

static GN::Logger * sLogger = GN::getLogger("GN.tool.d3d9wrapper.MyDevice9.VtxBuf");

using namespace GN;

// *****************************************************************************
// MyDevice9
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
HRESULT MyDevice9::CreateVertexBuffer(
    THIS_ UINT Length,
    DWORD Usage,
    DWORD FVF,
    D3DPOOL Pool,
    IDirect3DVertexBuffer9** ppVertexBuffer,
    HANDLE* pSharedHandle)
{
    if( 0 == ppVertexBuffer )
    {
        GN_ERROR(sLogger)( "NULL output pointer!" );
        return D3DERR_INVALIDCALL;
    }

    AutoComPtr<MyVtxBuf9> vb( new MyVtxBuf9(this) );

    HRESULT hr = vb->create( Length, Usage, FVF, Pool, pSharedHandle );
    if( FAILED(hr) ) return hr;

    // success
    *ppVertexBuffer = vb.detach();
    return D3D_OK;
}

//
//
// -----------------------------------------------------------------------------
HRESULT MyDevice9::SetStreamSource(
    THIS_ UINT StreamNumber,
    IDirect3DVertexBuffer9* pStreamData,
    UINT OffsetInBytes,
    UINT Stride)
{
    MyVtxBuf9 * vb = pStreamData ? safeCastPtr<MyVtxBuf9>( pStreamData ) : 0;

    HRESULT hr = realobj()->SetStreamSource( StreamNumber, vb ? vb->realobj() : 0, OffsetInBytes, Stride );
    if( FAILED(hr) ) return hr;

    VtxBufInfo & vbi = mVtxBufs[StreamNumber];

    if( vb ) vb->AddRef();
    if( vbi.stream ) vbi.stream->Release();
    vbi.stream = vb;
    vbi.offset = OffsetInBytes;
    vbi.stride = Stride;

    return D3D_OK;
}

//
//
// -----------------------------------------------------------------------------
HRESULT MyDevice9::GetStreamSource(
    THIS_ UINT StreamNumber,
    IDirect3DVertexBuffer9** ppStreamData,
    UINT* pOffsetInBytes,
    UINT* pStride)
{
    AutoComPtr<IDirect3DVertexBuffer9> vb;
    HRESULT hr = realobj()->GetStreamSource( StreamNumber, &vb, pOffsetInBytes, pStride );
    if( FAILED(hr) ) return hr;

    VtxBufInfo & vbi = mVtxBufs[StreamNumber];

    if( vbi.stream ) vbi.stream->AddRef();

    *ppStreamData = vbi.stream;
    *pOffsetInBytes = vbi.offset;
    *pStride = vbi.stride;

    return D3D_OK;
}

//
//
// -----------------------------------------------------------------------------
HRESULT MyDevice9::SetStreamSourceFreq(THIS_ UINT StreamNumber,UINT Setting)
{
    return realobj()->SetStreamSourceFreq( StreamNumber, Setting );
}

//
//
// -----------------------------------------------------------------------------
HRESULT MyDevice9::GetStreamSourceFreq(THIS_ UINT StreamNumber,UINT* pSetting)
{
    return realobj()->GetStreamSourceFreq( StreamNumber, pSetting );
}
