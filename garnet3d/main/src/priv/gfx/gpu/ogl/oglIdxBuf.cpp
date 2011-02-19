#include "pch.h"
#include "oglIdxBuf.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx.gpu.OGL.IdxBuf");

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLIdxBuf::init( const IdxBufDesc & desc )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::OGLIdxBuf, () );

    // check parameter
    if( 0 == desc.numidx )
    {
        GN_ERROR(sLogger)( "invalid buffer length!" );
        return failure();
    }

    setDesc( desc );

    mBytesPerIndex = desc.bits32 ? 4 : 2;
    mBuffer = (uint8*)HeapMemory::alloc( desc.numidx * mBytesPerIndex );

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLIdxBuf::quit()
{
    GN_GUARD;

    safeHeapDealloc(mBuffer);

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// From IdxBuf
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLIdxBuf::update( size_t startidx, size_t numidx, const void * data, SurfaceUpdateFlag flag )
{
    GN_GUARD_SLOW;

    GN_ASSERT( ok() );

    if( !validateUpdateParameters( startidx, &numidx, data, flag ) ) return;

    if( 0 == numidx ) return;

    memcpy( mBuffer + startidx * mBytesPerIndex, data, numidx * mBytesPerIndex );

    GN_UNGUARD_SLOW
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLIdxBuf::readback( DynaArray<uint8> & data )
{
    const IdxBufDesc & ibdesc = getDesc();

    size_t lengthInBytes = ibdesc.numidx * mBytesPerIndex;

    data.resize( lengthInBytes );

    memcpy( &data[0], mBuffer, lengthInBytes );
}