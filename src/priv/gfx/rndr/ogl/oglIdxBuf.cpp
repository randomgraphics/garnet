#include "pch.h"
#include "oglIdxBuf.h"

GN::Logger * GN::gfx::OGLIdxBuf::sLogger = GN::getLogger("GN.gfx.rndr.OGL");

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLIdxBuf::init( size_t numIdx, bool dynamic )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::OGLIdxBuf, () );

    // check parameter
    if( 0 == numIdx )
    {
        GN_ERROR(sLogger)( "invalid buffer length!" );
        return failure();
    }

    setProperties( numIdx, dynamic );

    mBuffer = (UInt16*) heapAlloc( numIdx * 2 );

    // call user-defined content loader
    if( !getLoader().empty() && !getLoader()(*this) ) return failure();

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

    safeHeapFree(mBuffer);

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
UInt16 * GN::gfx::OGLIdxBuf::lock( size_t startIdx, size_t numIdx, LockFlag flag )
{
    GN_GUARD_SLOW;
    GN_ASSERT( ok() );
    if( !basicLock( startIdx, numIdx, flag ) ) return 0;
    return mBuffer + startIdx;
    GN_UNGUARD_SLOW
}
