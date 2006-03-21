#include "pch.h"
#include "d3d9IdxBuf.h"
#include "d3d9Renderer.h"

// *****************************************************************************
// Local functions
// *****************************************************************************

// Note: these two functions are implemented in d3dVtxBuf.cpp
extern DWORD sBufUsage2D3D9( bool dynamic );
extern DWORD sLockFlags2D3D9( bool dynamic, GN::gfx::LockFlag lock );

// *****************************************************************************
// init / quit functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D9IdxBuf::init(
    size_t numIdx, bool dynamic, bool sysCopy, const IdxBufLoader & loader )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::D3D9IdxBuf, () );

    // check parameter
    if( 0 == numIdx )
    {
        GN_ERROR( "invalid buffer length!" );
        quit(); return selfOK();
    }

    // store buffer parameters
    setProperties( numIdx, dynamic );
    setLoader( loader );
    if( sysCopy ) mSysCopy.resize( numIdx );

    if( !deviceCreate() || !deviceRestore() ) { quit(); return selfOK(); }

    // success
    return selfOK();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9IdxBuf::quit()
{
    GN_GUARD;

    deviceDispose();
    deviceDestroy();

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// resource management
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D9IdxBuf::deviceRestore()
{
    GN_GUARD;

    GN_ASSERT( !isLocked() && !mD3DIb );

    LPDIRECT3DDEVICE9 dev = getRenderer().getDevice();

#if !GN_XENON
    // evict managed resources
    GN_DX9_CHECK_RV( dev->EvictManagedResources(), 0 );
#endif

    // create d3d ibuffer
    GN_DX9_CHECK_RV(
        dev->CreateIndexBuffer(
            (UINT)( getNumIdx() * 2 ),
            sBufUsage2D3D9( isDynamic() ),
            D3DFMT_INDEX16,
            D3DPOOL_DEFAULT,
            &mD3DIb,
            0 ),
        false );

    if( !getLoader().empty() )
    {
        // call user-defined loader
        if( !getLoader()( *this ) ) return false;
    }
    else if( !mSysCopy.empty() )
    {
        // copy data from system copy to D3D buffer
        void * dst;
        GN_DX9_CHECK_RV( mD3DIb->Lock( 0, 0, &dst, 0 ), false );
        ::memcpy( dst, &mSysCopy[0], mSysCopy.size()*2 );
        mD3DIb->Unlock();
    }

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9IdxBuf::deviceDispose()
{
    GN_GUARD;

    if( isLocked() )
    {
        unlock();
        GN_ERROR( "call unlock() before u dispose the index buffer!" );
    }

    safeRelease( mD3DIb );

    GN_UNGUARD;
}

// *****************************************************************************
// interface functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
uint16_t * GN::gfx::D3D9IdxBuf::lock( size_t startIdx, size_t numIdx, LockFlag flag )
{
    GN_GUARD_SLOW;

    GN_ASSERT( selfOK() );

    if( !basicLock( startIdx, numIdx, flag ) ) return false;
    AutoScope< Functor0<bool> > basicUnlocker( makeFunctor(this,&D3D9IdxBuf::basicUnlock) );

    uint16_t * buf;
    if( mSysCopy.empty() )
    {
        GN_DX9_CHECK_RV(
            mD3DIb->Lock(
                (UINT)( startIdx<<1 ),
                (UINT)( numIdx<<1 ),
                (void**)&buf,
                sLockFlags2D3D9( isDynamic(), flag ) ),
            0 );
    }
    else
    {
        mLockStartIdx = startIdx;
        mLockNumIdx   = numIdx;
        mLockFlag     = flag;
        buf = &mSysCopy[startIdx];
    }

    // success
    basicUnlocker.dismiss();
    return (uint16_t*)buf;

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9IdxBuf::unlock()
{
    GN_GUARD_SLOW;

    GN_ASSERT( selfOK() );

    if( !basicUnlock() ) return;

    if( mSysCopy.empty() )
    {
        GN_DX9_CHECK( mD3DIb->Unlock() );
    }
    else if( LOCK_RO != mLockFlag )
    {
        GN_ASSERT(
            mLockStartIdx < getNumIdx() &&
            0 < mLockNumIdx &&
            (mLockStartIdx + mLockNumIdx) <= getNumIdx() );

        mLockNumIdx <<= 1; // now, numIdx is in bytes.

        // update d3d index buffer
        void * dst;
        GN_DX9_CHECK_R( mD3DIb->Lock(
            (UINT)( mLockStartIdx<<1 ),
            (UINT)( mLockNumIdx ),
            &dst,
            sLockFlags2D3D9( isDynamic(), mLockFlag ) ) );
        ::memcpy( dst, &mSysCopy[mLockStartIdx], mLockNumIdx );
        mD3DIb->Unlock();
    }

    GN_UNGUARD_SLOW;
}
