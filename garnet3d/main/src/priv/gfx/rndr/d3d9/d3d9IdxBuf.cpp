#include "pch.h"
#include "d3d9IdxBuf.h"
#include "d3d9Renderer.h"

GN::Logger * GN::gfx::D3D9IdxBuf::sLogger = GN::getLogger("GN.gfx.rndr.D3D9");

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
bool GN::gfx::D3D9IdxBuf::init( const IdxBufDesc & desc )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::D3D9IdxBuf, () );

    // check parameter
    if( 0 == desc.numidx )
    {
        GN_ERROR(sLogger)( "invalid buffer length!" );
        return failure();
    }

    // store buffer parameters
    setDesc( desc );
    if( !desc.dynamic && desc.readback ) mSysCopy.resize( desc.numidx );

    if( !deviceRestore() ) return failure();

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9IdxBuf::quit()
{
    GN_GUARD;

    deviceDispose();

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

    const IdxBufDesc & desc = getDesc();

    // create d3d ibuffer
    GN_DX9_CHECK_RV(
        dev->CreateIndexBuffer(
            desc.numidx * 2,
            sBufUsage2D3D9( desc.dynamic ),
            D3DFMT_INDEX16,
            D3DPOOL_DEFAULT,
            &mD3DIb,
            0 ),
        false );

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
        GN_ERROR(sLogger)( "call unlock() before u dispose the index buffer!" );
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
UInt16 * GN::gfx::D3D9IdxBuf::lock( size_t startIdx, size_t numidx, LockFlag flag )
{
    GN_GUARD_SLOW;

    GN_ASSERT( ok() );

    if( !basicLock( startIdx, numidx, flag ) ) return 0;

    UInt16 * buf;
    if( mSysCopy.empty() )
    {
#if GN_XENON
        // Xenon does not support range locking on index buffer
        GN_DX9_CHECK_DO(
            mD3DIb->Lock( 0, 0, (void**)&buf, sLockFlags2D3D9( getDesc().dynamic, flag ) ),
            basicUnlock(); return 0; );
        buf += startIdx;
#else
        GN_DX9_CHECK_DO(
            mD3DIb->Lock(
                (UINT)( startIdx<<1 ),
                (UINT)( numidx<<1 ),
                (void**)&buf,
                sLockFlags2D3D9( getDesc().dynamic, flag ) ),
            basicUnlock(); return 0; );
#endif
    }
    else
    {
        mLockStartIdx = startIdx;
        mLockNumIdx   = numidx;
        mLockFlag     = flag;
        buf = &mSysCopy[startIdx];
    }

    // success
    return (UInt16*)buf;

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9IdxBuf::unlock()
{
    GN_GUARD_SLOW;

    GN_ASSERT( ok() );

    if( !basicUnlock() ) return;

    if( mSysCopy.empty() )
    {
        GN_DX9_CHECK( mD3DIb->Unlock() );
    }
    else if( LOCK_RO != mLockFlag )
    {
        const IdxBufDesc & desc = getDesc();
        
        GN_ASSERT(
            mLockStartIdx < desc.numidx &&
            0 < mLockNumIdx &&
            (mLockStartIdx + mLockNumIdx) <= desc.numidx );

        mLockNumIdx <<= 1; // now, numidx is in bytes.

        // update d3d index buffer
#if GN_XENON
        // Xenon does not support range locking on index buffer
        UInt16 * dst;
        GN_DX9_CHECK_R( mD3DIb->Lock( 0, 0, (void**)&dst, sLockFlags2D3D9( desc.dynamic, mLockFlag ) ) );
        dst += mLockStartIdx;
#else
        void * dst;
        GN_DX9_CHECK_R( mD3DIb->Lock(
            (UINT)( mLockStartIdx<<1 ),
            (UINT)( mLockNumIdx ),
            &dst,
            sLockFlags2D3D9( desc.dynamic, mLockFlag ) ) );
#endif
        ::memcpy( dst, &mSysCopy[mLockStartIdx], mLockNumIdx );
        mD3DIb->Unlock();
    }

    GN_UNGUARD_SLOW;
}
