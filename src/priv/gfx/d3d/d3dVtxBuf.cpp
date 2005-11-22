#include "pch.h"
#include "d3dVtxBuf.h"
#include "d3DRenderer.h"

// *****************************************************************************
// Local functions
// *****************************************************************************

//!
//! convert garnet buffer usage flags to D3D flags
// ----------------------------------------------------------------------------
DWORD sBufUsage2D3D( GN::gfx::ResourceUsage usage )
{
    DWORD d3dUsage = D3DUSAGE_WRITEONLY;

    if( GN::gfx::USAGE_DYNAMIC == usage ) d3dUsage |= D3DUSAGE_DYNAMIC;

    return d3dUsage;
}

//!
//! convert garnet buffer lock flags to D3D flags
// ----------------------------------------------------------------------------
DWORD sLockFlags2D3D( GN::gfx::ResourceUsage usage, uint32_t lock )
{
    DWORD d3dFlag;

    if( GN::gfx::USAGE_STATIC == usage )
    {
        if( GN::gfx::LOCK_RO == lock )
            d3dFlag = D3DLOCK_READONLY;
        else
            d3dFlag = 0;
    }
    else if( GN::gfx::USAGE_DYNAMIC == usage )
    {
        if( GN::gfx::LOCK_RO == lock )
            d3dFlag = D3DLOCK_READONLY;
        else if( GN::gfx::LOCK_WO == lock || GN::gfx::LOCK_DISCARD == lock )
            d3dFlag = D3DLOCK_DISCARD;
        else if( GN::gfx::LOCK_NO_OVERWRITE == lock )
            d3dFlag = D3DLOCK_NOOVERWRITE;
        else
            d3dFlag = 0;
    }
    else
    {
        // Program should not reach here.
        GN_UNEXPECTED();
        return 0;
    }

#if GN_DEBUG
    d3dFlag |= D3DLOCK_NOSYSLOCK;
#endif

    return d3dFlag;
}

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3DVtxBuf::init( size_t bytes, ResourceUsage usage, bool sysCopy )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( D3DVtxBuf, () );

    if( 0 == bytes )
    {
        GNGFX_ERROR( "Vertex buffer size can't be zero!" );
        quit(); return selfOK();
    }
    if ( USAGE_STATIC != usage && USAGE_DYNAMIC != usage )
    {
        GNGFX_ERROR( "Vertex buffer usage can be only USAGE_STATIC or USAGE_DYNAMIC!" );
        quit(); return selfOK();
    }

    setProperties( bytes, usage );

    if( sysCopy ) mSysCopy.resize( bytes );

    if( !deviceCreate() || !deviceRestore() ) { quit(); return selfOK(); }

    // success
    return selfOK();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3DVtxBuf::quit()
{
    GN_GUARD;

    deviceDispose();
    deviceDestroy();

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// from D3DResource
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3DVtxBuf::deviceRestore()
{
    GN_GUARD;

    GN_ASSERT( !mLocked && 0 == mD3DVb );

    LPDIRECT3DDEVICE9 dev = getRenderer().getDevice();

    // evict managed resources
    GN_DX_CHECK_RV( dev->EvictManagedResources(), false );

    //
    // create d3d vertex buffer
    //
    GN_DX_CHECK_RV(
        dev->CreateVertexBuffer(
            (UINT)getSizeInBytes(),
            sBufUsage2D3D( getUsage() ),
            0,  // non-FVF vstream
            D3DPOOL_DEFAULT,
            &mD3DVb,
            0 ),
        false );

    // copy data from system copy
    if( !mSysCopy.empty() )
    {
        void * dst;
        GN_DX_CHECK_RV( mD3DVb->Lock( 0, 0, &dst, 0 ), false );
        ::memcpy( dst, GN::vec2ptr(mSysCopy), mSysCopy.size() );
        mD3DVb->Unlock();
    }

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3DVtxBuf::deviceDispose()
{
    GN_GUARD;

    if( mLocked )
    {
        unlock();
        GNGFX_ERROR( "call unlock() before u release the vertex buffer!" );
    }

    safeRelease( mD3DVb );

    GN_UNGUARD;
}

// *****************************************************************************
// from VtxBuf
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void * GN::gfx::D3DVtxBuf::lock( size_t offset, size_t bytes, uint32_t flag )
{
    GN_GUARD_SLOW;

    GN_ASSERT( selfOK() );

    if( mLocked )
    {
        GNGFX_ERROR( "Vertex buffer is already locked!" );
        return 0;
    }
    if( offset >= getSizeInBytes() )
    {
        GNGFX_ERROR( "offset is beyond the end of vertex buffer!" );
        return 0;
    }

    // adjust offset and bytes
    if( 0 == bytes ) bytes = getSizeInBytes();
    if( offset + bytes > getSizeInBytes() ) bytes = getSizeInBytes() - offset;

    if ( mSysCopy.empty() )
    {
        void * buf;
        GN_DX_CHECK_RV(
            mD3DVb->Lock(
                (UINT)offset,
                (UINT)bytes,
                &buf,
                sLockFlags2D3D(getUsage(),flag) ),
            0 );
        mLocked = true;
        return buf;
    }
    else
    {
        mLocked     = true;
        mLockOffset = offset;
        mLockBytes  = bytes;
        mLockFlag   = flag;
        return &mSysCopy[offset];
    }

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3DVtxBuf::unlock()
{
    GN_GUARD_SLOW;

    GN_ASSERT( selfOK() );

    if( !mLocked )
    {
        GNGFX_ERROR( "Can't unlock a vertex buffer that is not locked at all!" );
        return;
    }

    mLocked = false;

    if ( mSysCopy.empty() )
    {
        GN_DX_CHECK( mD3DVb->Unlock() );
    }
    else if ( LOCK_RO != mLockFlag )
    {
        GN_ASSERT(
            mLockOffset < getSizeInBytes() &&
            0 < mLockBytes &&
            (mLockOffset + mLockBytes) <= getSizeInBytes() );

        // update d3d buffer
        void * dst;
        GN_DX_CHECK_R( mD3DVb->Lock(
            (UINT)mLockOffset,
            (UINT)mLockBytes,
            &dst,
            sLockFlags2D3D(getUsage(),mLockFlag) ) );
        ::memcpy( dst, &mSysCopy[mLockOffset], mLockBytes );
        mD3DVb->Unlock();
    }

    GN_UNGUARD_SLOW;
}
