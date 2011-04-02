#include "pch.h"
#include "myvtxbuf9.h"
#include "mydevice9.h"

static GN::Logger * sLogger = GN::getLogger("GN.tool.d3dwrapper.MyVtxBuf9");

//
//
// ----------------------------------------------------------------------------
HRESULT MyVtxBuf9::create(
    UINT Length,
    DWORD Usage,
    DWORD FVF,
    D3DPOOL Pool,
    HANDLE* pSharedHandle )
{
    GN_ASSERT( mDevice );

    mSysCopy.resize( Length );

    GN_TRACE(sLogger)( "Create D3D vertex buffer: Length(%d)", Length );

    return mDevice->realobj()->CreateVertexBuffer( Length, Usage, FVF, Pool, &mRealObject, pSharedHandle );
}

//
//
// ----------------------------------------------------------------------------
HRESULT MyVtxBuf9::Lock( UINT OffsetToLock, UINT SizeToLock, VOID ** ppbData, DWORD Flags )
{
    if( (OffsetToLock + SizeToLock) > mSysCopy.size() )
    {
        GN_ERROR(sLogger)( "Invalid lock range" );
        return D3DERR_INVALIDCALL;
    }

    if( 0 == ppbData )
    {
        GN_ERROR(sLogger)( "Null buffer pointer" );
        return D3DERR_INVALIDCALL;
    }

    mLockOffset = OffsetToLock;
    mLockBytes  = ( 0 == SizeToLock && 0 == OffsetToLock ) ? (UINT)mSysCopy.size() : SizeToLock;
    mLockFlags  = Flags;

    *ppbData = mSysCopy.rawptr() + mLockOffset;

    return D3D_OK;
}

//
//
// ----------------------------------------------------------------------------
HRESULT MyVtxBuf9::Unlock()
{
    // shortcut for readonly lock
    if( D3DLOCK_READONLY == mLockFlags ) return D3D_OK;

    uint8 * data;

    HRESULT hr = realobj()->Lock( mLockOffset, mLockBytes, (void**)&data, mLockFlags );
    if( FAILED(hr) ) return hr;

    memcpy( data + mLockOffset, mSysCopy.rawptr() + mLockOffset, mLockBytes );

    return realobj()->Unlock();
}
