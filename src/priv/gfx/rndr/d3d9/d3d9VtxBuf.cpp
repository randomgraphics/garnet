#include "pch.h"
#include "d3d9VtxBuf.h"
#include "d3d9Renderer.h"

GN::Logger * GN::gfx::D3D9VtxBuf::sLogger = GN::getLogger("GN.gfx.rndr.D3D9");

// *****************************************************************************
// Local functions
// *****************************************************************************

///
/// convert garnet buffer usage flags to D3D9 flags
// ----------------------------------------------------------------------------
DWORD sBufUsage2D3D9( bool dynamic )
{
    DWORD d3dUsage = D3DUSAGE_WRITEONLY;

    if( dynamic ) d3dUsage |= D3DUSAGE_DYNAMIC;

    return d3dUsage;
}

///
/// convert garnet buffer lock flags to D3D9 flags
// ----------------------------------------------------------------------------
DWORD sLockFlags2D3D9( bool dynamic, GN::gfx::LockFlag flag )
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

#if GN_DEBUG_BUILD
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
bool GN::gfx::D3D9VtxBuf::init( const VtxBufDesc & desc )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( D3D9VtxBuf, () );

    if( 0 == desc.bytes )
    {
        GN_ERROR(sLogger)( "Vertex buffer size can't be zero!" );
        return failure();
    }

    setDesc( desc );
    if( !desc.dynamic && desc.readback ) mSysCopy.resize( desc.bytes );

    if( !deviceRestore() ) return failure();

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9VtxBuf::quit()
{
    GN_GUARD;

    deviceDispose();

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// from D3D9Resource
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D9VtxBuf::deviceRestore()
{
    GN_GUARD;

    GN_ASSERT( !isLocked() && !mD3DVb );

    LPDIRECT3DDEVICE9 dev = getRenderer().getDevice();

#if !GN_XENON
    // evict managed resources
    GN_DX9_CHECK_RV( dev->EvictManagedResources(), false );
#endif

    const VtxBufDesc & desc = getDesc();

    //
    // create d3d vertex buffer
    //
    GN_DX9_CHECK_RV(
        dev->CreateVertexBuffer(
            desc.bytes,
            sBufUsage2D3D9( desc.dynamic ),
            0,  // non-FVF vstream
            D3DPOOL_DEFAULT,
            &mD3DVb,
            0 ),
        false );

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9VtxBuf::deviceDispose()
{
    GN_GUARD;

    if( isLocked() )
    {
        unlock();
        GN_ERROR(sLogger)( "call unlock() before u release the vertex buffer!" );
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
void * GN::gfx::D3D9VtxBuf::lock( size_t offset, size_t bytes, LockFlag flag )
{
    GN_GUARD_SLOW;

    GN_ASSERT( ok() );

    if( !basicLock( offset, bytes, flag ) ) return false;

    void * buf;
    if ( mSysCopy.empty() )
    {
#if GN_XENON
        // Note: XDK does not support range locking on vertex buffer
        UInt8 * u8buf;
        GN_DX9_CHECK_DO(
            mD3DVb->Lock( 0, 0, (void**)&u8buf, sLockFlags2D3D9(getDesc().dynamic,flag) ),
            basicUnlock(); return 0; );
        buf = u8buf + offset;
#else
        GN_DX9_CHECK_DO(
            mD3DVb->Lock(
                (UINT)offset,
                (UINT)bytes,
                &buf,
                sLockFlags2D3D9(getDesc().dynamic,flag) ),
            basicUnlock(); return 0; );
#endif
    }
    else
    {
        mLockOffset = offset;
        mLockBytes  = bytes;
        mLockFlag   = flag;
        buf = &mSysCopy[offset];
    }

    // success
    return buf;

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9VtxBuf::unlock()
{
    GN_GUARD_SLOW;

    GN_ASSERT( ok() );

    if( !basicUnlock() ) return;

    if ( mSysCopy.empty() )
    {
        GN_DX9_CHECK( mD3DVb->Unlock() );
    }
    else if ( LOCK_RO != mLockFlag )
    {
        const VtxBufDesc & desc = getDesc();
        
        GN_ASSERT(
            mLockOffset < desc.bytes &&
            0 < mLockBytes &&
            (mLockOffset + mLockBytes) <= desc.bytes );

        // update d3d buffer
#if GN_XENON
        // Note: XDK does not support range locking on vertex buffer
        UInt8 * dst;
        GN_DX9_CHECK_R( mD3DVb->Lock( 0, 0, (void**)&dst, sLockFlags2D3D9(desc.dynamic,mLockFlag) ) );
        dst += mLockOffset;
#else
        void * dst;
        GN_DX9_CHECK_R( mD3DVb->Lock(
            (UINT)mLockOffset,
            (UINT)mLockBytes,
            &dst,
            sLockFlags2D3D9(desc.dynamic,mLockFlag) ) );
#endif
        ::memcpy( dst, &mSysCopy[mLockOffset], mLockBytes );
        mD3DVb->Unlock();
    }

    GN_UNGUARD_SLOW;
}
