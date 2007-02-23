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
bool GN::gfx::D3D10IdxBuf::init( const IdxBufDesc & desc )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::D3D10IdxBuf, () );

    // check parameter
    if( 0 == desc.numidx )
    {
        GN_ERROR(sLogger)( "invalid buffer length!" );
        return failure();
    }

    // store buffer parameters
    setDesc( desc );

    // create D3D buffer
    if( !createBuffer() ) return failure();

    // create system copy
    mSysCopy = !desc.dynamic && desc.readback;
    if( mSysCopy ) mLockBuffer.resize( desc.numidx );

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
UInt16 * GN::gfx::D3D10IdxBuf::lock( size_t startIdx, size_t numidx, LockFlag flag )
{
    GN_GUARD_SLOW;

    GN_ASSERT( ok() );

    if( !basicLock( startIdx, numidx, flag ) ) return 0;

    UInt16 * buf;
    if( mSysCopy )
    {
        GN_ASSERT( mLockBuffer.size() >= numidx );
        buf = mLockBuffer.cptr() + startIdx;

        // TODO: copy data from d3d buffer
    }
    else if( getDesc().dynamic )
    {
        GN_DX10_CHECK_DO(
            mD3DIdxBuf->Map( lockFlags2D3D10( flag ), 0, (void**)&buf ),
            basicUnlock(); return 0; );

        buf += startIdx;
    }
    else
    {
        // create temporary lock ibuffer
        mLockBuffer.resize( numidx * 2 );
        buf = mLockBuffer.cptr();
    }

    // success
    mLockStartIdx = startIdx;
    mLockNumIdx   = numidx;
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
    else if( getDesc().dynamic )
    {
        mD3DIdxBuf->Unmap();
    }
    else if( LOCK_RO != mLockFlag )
    {
        GN_ASSERT(
            mLockStartIdx < getDesc().numidx &&
            0 < mLockNumIdx &&
            (mLockStartIdx + mLockNumIdx) <= getDesc().numidx );

        mLockNumIdx <<= 1; // now, numidx is in bytes.

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

    const IdxBufDesc & desc = getDesc();

    D3D10_BUFFER_DESC d3ddesc;
    d3ddesc.ByteWidth = desc.numidx * 2;
    d3ddesc.Usage = desc.dynamic ? D3D10_USAGE_DYNAMIC : D3D10_USAGE_DEFAULT ;
    d3ddesc.BindFlags = D3D10_BIND_INDEX_BUFFER;
    d3ddesc.CPUAccessFlags = desc.dynamic ? D3D10_CPU_ACCESS_WRITE : 0;
    d3ddesc.MiscFlags = 0;

    // create d3d ibuffer
    GN_DX10_CHECK_RV( dev->CreateBuffer( &d3ddesc, 0, &mD3DIdxBuf ), false );

    // success
    return true;

    GN_UNGUARD;
}
