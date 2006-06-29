#include "pch.h"
#include "oglIdxBuf.h"

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
        GN_ERROR( "invalid buffer length!" );
        quit(); return selfOK();
    }

    setProperties( numIdx, dynamic );

    mBuffer = (uint16_t*) heapAlloc( numIdx * 2 );

    // call user-defined content loader
    if( !getLoader().empty() )
    {
        if( !getLoader()( *this ) ) { quit(); return selfOK(); }
    }

    // success
    return selfOK();

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
uint16_t * GN::gfx::OGLIdxBuf::lock( size_t startIdx, size_t numIdx, LockFlag flag )
{
    GN_GUARD_SLOW;
    GN_ASSERT( selfOK() );
    if( !basicLock( startIdx, numIdx, flag ) ) return 0;
    return mBuffer + startIdx;
    GN_UNGUARD_SLOW
}
