#include "pch.h"
#include "oglIdxBuf.h"

static GN::Logger * sLogger = GN::GetLogger("GN.gfx.gpu.OGL.IdxBuf");

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLIdxBuf::Init( const IdxBufDesc & desc )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::OGLIdxBuf, () );

    // check parameter
    if( 0 == desc.numidx )
    {
        GN_ERROR(sLogger)( "invalid buffer length!" );
        return Failure();
    }

    SetDesc( desc );

    mBytesPerIndex = desc.bits32 ? 4 : 2;
    mBuffer = (UInt8*)HeapMemory::Alloc( desc.numidx * mBytesPerIndex );

    // success
    return Success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLIdxBuf::Quit()
{
    GN_GUARD;

    SafeHeapFree(mBuffer);

    // standard Quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// From IdxBuf
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLIdxBuf::Update( size_t startidx, size_t numidx, const void * data, SurfaceUpdateFlag flag )
{
    GN_GUARD_SLOW;

    GN_ASSERT( Ok() );

    if( !validateUpdateParameters( startidx, &numidx, data, flag ) ) return;

    if( 0 == numidx ) return;

    memcpy( mBuffer + startidx * mBytesPerIndex, data, numidx * mBytesPerIndex );

    GN_UNGUARD_SLOW
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLIdxBuf::Readback( DynaArray<UInt8> & data )
{
    const IdxBufDesc & desc = GetDesc();

    size_t lengthInBytes = desc.numidx * mBytesPerIndex;

    data.Resize( lengthInBytes );

    memcpy( &data[0], mBuffer, lengthInBytes );
}