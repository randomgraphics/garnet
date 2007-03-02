#include "pch.h"
#include "d3d10Buffer.h"
#include "d3d10Renderer.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx.rndr.D3D10.Buffer");

// *****************************************************************************
// Local functions
// *****************************************************************************

// *****************************************************************************
// init / quit functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D10Buffer::init( UInt32 bytes, bool dynamic, bool readback, UInt32 bindFlags )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::D3D10Buffer, () );

    // check parameter
    if( 0 == bytes )
    {
        GN_ERROR(sLogger)( "invalid buffer length!" );
        return failure();
    }

    mBytes = bytes;
    mDynamic = dynamic;
    mReadback = readback;

    // create D3D buffer
    if( !createBuffer( bindFlags ) ) return failure();

    // create system copy
    mSysCopy = !dynamic && readback;
    if( mSysCopy ) mLockBuffer.resize( bytes );

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D10Buffer::quit()
{
    GN_GUARD;

    safeRelease( mD3DBuffer );

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
void * GN::gfx::D3D10Buffer::d3dlock( size_t offset, size_t bytes, LockFlag flag )
{
    GN_GUARD_SLOW;

    GN_ASSERT( ok() );

    UInt8 * buf;
    if( mSysCopy )
    {
        GN_ASSERT( mLockBuffer.size() >= (offset+bytes) );
        buf = mLockBuffer.cptr() + offset;

        // TODO: copy data from d3d buffer
    }
    else if( mDynamic )
    {
        GN_DX10_CHECK_RV( mD3DBuffer->Map( lockFlags2D3D10( flag ), 0, (void**)&buf ), 0 );

        buf += offset;
    }
    else
    {
        // create temporary lock ibuffer
        mLockBuffer.resize( bytes );
        buf = mLockBuffer.cptr();
    }

    // success
    mLockOffset = (UInt32)offset;
    mLockBytes  = (UInt32)bytes;
    mLockFlag   = flag;
    return buf;

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D10Buffer::d3dunlock()
{
    GN_GUARD_SLOW;

    GN_ASSERT( ok() );

    if( mSysCopy )
    {
        if( LOCK_RO != mLockFlag )
        {
            D3D10_BOX box = { mLockOffset, 0, 0, mLockOffset+mLockBytes, 1, 1 };

            // update d3d index buffer
            getDevice()->UpdateSubresource(
                mD3DBuffer,
                0,
                &box,
                mLockBuffer.cptr() + mLockOffset,
                0,   // row pitch
                0 ); // slice pitch
        }
    }
    else if( mDynamic )
    {
        mD3DBuffer->Unmap();
    }
    else if( LOCK_RO != mLockFlag )
    {
        GN_ASSERT(
            mLockOffset < mBytes &&
            0 < mLockBytes &&
            (mLockOffset + mLockBytes) <= mBytes );

        D3D10_BOX box = { mLockOffset, 0, 0, mLockOffset+mLockBytes, 1, 1 };

        // update d3d index buffer
        getDevice()->UpdateSubresource(
            mD3DBuffer,
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
bool GN::gfx::D3D10Buffer::createBuffer( UInt32 bindFlags )
{
    GN_GUARD;

    GN_ASSERT( !mD3DBuffer );

    ID3D10Device * dev = getDevice();

    D3D10_BUFFER_DESC d3ddesc;
    d3ddesc.ByteWidth = mBytes;
    d3ddesc.Usage = mDynamic ? D3D10_USAGE_DYNAMIC : D3D10_USAGE_DEFAULT ;
    d3ddesc.BindFlags = bindFlags;
    d3ddesc.CPUAccessFlags = mDynamic ? D3D10_CPU_ACCESS_WRITE : 0;
    d3ddesc.MiscFlags = 0;

    // create d3d ibuffer
    GN_DX10_CHECK_RV( dev->CreateBuffer( &d3ddesc, 0, &mD3DBuffer ), false );

    // success
    return true;

    GN_UNGUARD;
}
