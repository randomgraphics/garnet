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

    mBuffer = (uint16_t*) memAlloc( numIdx * 2 );

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

    safeMemFree(mBuffer);

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
uint16_t * GN::gfx::OGLIdxBuf::lock( size_t startIdx, size_t /*numIdx*/, uint32_t /*flag*/ )
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

    // success
    mLocked = true;
    return mBuffer + startIdx;

    GN_UNGUARD_SLOW
}
