#include "pch.h"
#include "d3d10Buffer.h"
#include "d3d10Renderer.h"

GN::Logger * GN::gfx::D3D10IdxBuf::sLogger = GN::getLogger("GN.gfx.rndr.D3D10");

// *****************************************************************************
// Local functions
// *****************************************************************************

// *****************************************************************************
// init / quit functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D10IdxBuf::init( size_t numIdx, bool dynamic, bool syscopy )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::D3D10IdxBuf, () );

    // check parameter
    if( 0 == numIdx )
    {
        GN_ERROR(sLogger)( "invalid buffer length!" );
        return failure();
    }

    // store buffer parameters
    setProperties( numIdx, dynamic );

    // create D3D buffer
    if( !createBuffer() ) return failure();

    // create system copy
    mSysCopy = syscopy;
    if( syscopy ) mLockBuffer.resize( numIdx );

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D10IdxBuf::quit()
{
    GN_GUARD;

    if( isLocked() )
    {
        unlock();
        GN_ERROR(sLogger)( "call unlock() before u dispose the index buffer!" );
    }

    safeRelease( mD3DIdxBuf );

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// interface functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
UInt16 * GN::gfx::D3D10IdxBuf::lock( size_t startIdx, size_t numIdx, LockFlag flag )
{
    GN_GUARD_SLOW;

    GN_ASSERT( ok() );

    if( !basicLock( startIdx, numIdx, flag ) ) return 0;

    UInt16 * buf;
    if( mSysCopy )
    {
        GN_ASSERT( mLockBuffer.size() >= numIdx );
        buf = mLockBuffer.cptr() + startIdx;

        // TODO: copy data from d3d buffer
    }
    else
    {
        if( isDynamic() )
        {
            GN_DX10_CHECK_DO(
                mD3DIdxBuf->Map( lockFlags2D3D10( flag ), 0, (void**)&buf ),
                basicUnlock(); return 0; );

            buf += startIdx;
        }
        else
        {
            // create temporary lock ibuffer
            mLockBuffer.resize( numIdx * 2 );
            buf = mLockBuffer.cptr();
        }
    }

    // success
    mLockStartIdx = startIdx;
    mLockNumIdx   = numIdx;
    mLockFlag     = flag;
    return buf;

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D10IdxBuf::unlock()
{
    GN_GUARD_SLOW;

    GN_ASSERT( ok() );

    if( !basicUnlock() ) return;

    if( mSysCopy )
    {
        if( LOCK_RO != mLockFlag )
        {
            D3D10_BOX box = { mLockStartIdx, 0, 0, mLockStartIdx+mLockNumIdx, 0, 0 };

            // update d3d index buffer
            getDevice()->UpdateSubresource(
                mD3DIdxBuf,
                0,
                &box,
                mLockBuffer.cptr() + mLockStartIdx,
                0,   // row pitch
                0 ); // slice pitch
        }
    }
    else if( isDynamic() )
    {
        mD3DIdxBuf->Unmap();
    }
    else if( LOCK_RO != mLockFlag )
    {
        GN_ASSERT(
            mLockStartIdx < getNumIdx() &&
            0 < mLockNumIdx &&
            (mLockStartIdx + mLockNumIdx) <= getNumIdx() );

        mLockNumIdx <<= 1; // now, numIdx is in bytes.

        D3D10_BOX box = { mLockStartIdx, 0, 0, mLockStartIdx+mLockNumIdx, 0, 0 };

        // update d3d index buffer
        getDevice()->UpdateSubresource(
            mD3DIdxBuf,
            0,
            &box,
            mLockBuffer.cptr(),
            0,   // row pitch
            0 ); // slice pitch
    }

    GN_UNGUARD_SLOW;
}

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D10IdxBuf::createBuffer()
{
    GN_GUARD;

    GN_ASSERT( !isLocked() && !mD3DIdxBuf );

    ID3D10Device * dev = getDevice();

    D3D10_BUFFER_DESC desc;
    desc.ByteWidth = getNumIdx() * 2;
    desc.Usage = isDynamic() ? D3D10_USAGE_DYNAMIC : D3D10_USAGE_DEFAULT ;
    desc.BindFlags = D3D10_BIND_INDEX_BUFFER;
    desc.CPUAccessFlags = isDynamic() ? D3D10_CPU_ACCESS_WRITE : 0;
    desc.MiscFlags = 0;

    // create d3d ibuffer
    GN_DX10_CHECK_RV( dev->CreateBuffer( &desc, 0, &mD3DIdxBuf ), false );

    // success
    return true;

    GN_UNGUARD;
}
