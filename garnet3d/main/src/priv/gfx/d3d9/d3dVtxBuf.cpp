#include "pch.h"
#include "d3dVtxBuf.h"
#include "d3DRenderer.h"

// *****************************************************************************
// Local functions
// *****************************************************************************

//!
//! convert garnet buffer usage flags to D3D flags
// ----------------------------------------------------------------------------
DWORD sBufUsage2D3D( bool dynamic )
{
    DWORD d3dUsage = D3DUSAGE_WRITEONLY;

    if( dynamic ) d3dUsage |= D3DUSAGE_DYNAMIC;

    return d3dUsage;
}

//!
//! convert garnet buffer lock flags to D3D flags
// ----------------------------------------------------------------------------
DWORD sLockFlags2D3D( bool dynamic, GN::gfx::LockFlag flag )
{
    using namespace GN::gfx;

    static DWORD staticFlags[] =
    {
        0,
        D3DLOCK_READONLY,
        0,
        0,
        0
    };
    GN_CASSERT( sizeof(staticFlags)/sizeof(staticFlags[0]) == NUM_LOCK_FLAGS );

    static DWORD dynamicFlags[] =
    {
        0,
        D3DLOCK_READONLY,
        0,
        D3DLOCK_DISCARD,
        D3DLOCK_NOOVERWRITE
    };
    GN_CASSERT( sizeof(dynamicFlags)/sizeof(dynamicFlags[0]) == NUM_LOCK_FLAGS );

    GN_ASSERT( flag < NUM_LOCK_FLAGS );

    DWORD d3dFlag = dynamic ? dynamicFlags[flag] : staticFlags[flag];

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
bool GN::gfx::D3DVtxBuf::init(
    size_t bytes, bool dynamic, bool sysCopy, const VtxBufLoader & loader )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( D3DVtxBuf, () );

    if( 0 == bytes )
    {
        GN_ERROR( "Vertex buffer size can't be zero!" );
        quit(); return selfOK();
    }

    setProperties( bytes, dynamic );
    setLoader( loader );
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

    GN_ASSERT( !isLocked() && !mD3DVb );

    LPDIRECT3DDEVICE9 dev = getRenderer().getDevice();

#if !GN_XENON
    // evict managed resources
    GN_DX_CHECK_RV( dev->EvictManagedResources(), false );
#endif

    //
    // create d3d vertex buffer
    //
    GN_DX_CHECK_RV(
        dev->CreateVertexBuffer(
            (UINT)getSizeInBytes(),
            sBufUsage2D3D( isDynamic() ),
            0,  // non-FVF vstream
            D3DPOOL_DEFAULT,
            &mD3DVb,
            0 ),
        false );

    if( !getLoader().empty() )
    {
        // call user-defined loader
        if( !getLoader()( *this ) ) return false;
    }
    else if( !mSysCopy.empty() )
    {
        // copy data from system copy
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

    if( isLocked() )
    {
        unlock();
        GN_ERROR( "call unlock() before u release the vertex buffer!" );
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
void * GN::gfx::D3DVtxBuf::lock( size_t offset, size_t bytes, LockFlag flag )
{
    GN_GUARD_SLOW;

    GN_ASSERT( selfOK() );

    if( !basicLock( offset, bytes, flag ) ) return false;
    AutoScope< Functor0<bool> > basicUnlocker( makeFunctor(this,&D3DVtxBuf::basicUnlock) );

    void * buf;
    if ( mSysCopy.empty() )
    {
        GN_DX_CHECK_RV(
            mD3DVb->Lock(
                (UINT)offset,
                (UINT)bytes,
                &buf,
                sLockFlags2D3D(isDynamic(),flag) ),
            0 );
    }
    else
    {
        mLockOffset = offset;
        mLockBytes  = bytes;
        mLockFlag   = flag;
        buf = &mSysCopy[offset];
    }

    // success
    basicUnlocker.dismiss();
    return buf;

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3DVtxBuf::unlock()
{
    GN_GUARD_SLOW;

    GN_ASSERT( selfOK() );

    if( !basicUnlock() ) return;

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
            sLockFlags2D3D(isDynamic(),mLockFlag) ) );
        ::memcpy( dst, &mSysCopy[mLockOffset], mLockBytes );
        mD3DVb->Unlock();
    }

    GN_UNGUARD_SLOW;
}
