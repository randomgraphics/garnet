#include "pch.h"
#include "d3dIdxBuf.h"
#include "d3dRenderer.h"

// *****************************************************************************
// Local functions
// *****************************************************************************

// Note: these two functions are implemented in d3dVtxBuf.cpp
extern DWORD sBufUsage2D3D( bool dynamic );
extern DWORD sLockFlags2D3D( bool dynamic, uint32_t lock );

// *****************************************************************************
// init / quit functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3DIdxBuf::init(
    size_t numIdx, bool dynamic, bool sysCopy, const IdxBufLoader & loader )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::D3DIdxBuf, () );

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
void GN::gfx::D3DIdxBuf::quit()
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
bool GN::gfx::D3DIdxBuf::deviceRestore()
{
    GN_GUARD;

    GN_ASSERT( !mLocked && !mD3DIb );

    LPDIRECT3DDEVICE9 dev = getRenderer().getDevice();

#if !GN_XENON
    // evict managed resources
    GN_DX_CHECK_RV( dev->EvictManagedResources(), 0 );
#endif

    // create d3d ibuffer
    GN_DX_CHECK_RV(
        dev->CreateIndexBuffer(
            (UINT)( getNumIdx() * 2 ),
            sBufUsage2D3D( isDynamic() ),
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
        GN_DX_CHECK_RV( mD3DIb->Lock( 0, 0, &dst, 0 ), false );
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
void GN::gfx::D3DIdxBuf::deviceDispose()
{
    GN_GUARD;

    if( mLocked )
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
uint16_t * GN::gfx::D3DIdxBuf::lock( size_t startIdx, size_t numIdx, uint32_t flag )
{
    GN_GUARD_SLOW;

    GN_ASSERT( selfOK() );

    if( mLocked )
    {
        GN_ERROR( "This buffer is already locked!" );
        return 0;
    }
    if( startIdx >= getNumIdx() )
    {
        GN_ERROR( "startIdx is beyond the end of index buffer!" );
        return 0;
    }

    // adjust startIdx and numIdx
    if( 0 == numIdx ) numIdx = getNumIdx();
    if( startIdx + numIdx > getNumIdx() ) numIdx = getNumIdx() - startIdx;

    if( mSysCopy.empty() )
    {
        void * buf;
        GN_DX_CHECK_RV(
            mD3DIb->Lock(
                (UINT)( startIdx<<1 ),
                (UINT)( numIdx<<1 ),
                &buf,
                sLockFlags2D3D( isDynamic(), flag ) ),
            0 );
        mLocked = true;
        return (uint16_t*)buf;
    }
    else
    {
        mLocked       = true;
        mLockStartIdx = startIdx;
        mLockNumIdx   = numIdx;
        mLockFlag     = flag;
        return &mSysCopy[startIdx];
    }

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3DIdxBuf::unlock()
{
    GN_GUARD_SLOW;

    GN_ASSERT( selfOK() );

    if( !mLocked )
    {
        GN_ERROR( "Can't unlock a index buffer that is not locked at all!" );
        return;
    }

    mLocked = false;

    if( mSysCopy.empty() )
    {
        GN_DX_CHECK( mD3DIb->Unlock() );
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
        GN_DX_CHECK_R( mD3DIb->Lock(
            (UINT)( mLockStartIdx<<1 ),
            (UINT)( mLockNumIdx ),
            &dst,
            sLockFlags2D3D( isDynamic(), mLockFlag ) ) );
        ::memcpy( dst, &mSysCopy[mLockStartIdx], mLockNumIdx );
        mD3DIb->Unlock();
    }

    GN_UNGUARD_SLOW;
}
