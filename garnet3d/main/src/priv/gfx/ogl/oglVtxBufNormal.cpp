#include "pch.h"
#include "oglVtxBuf.h"

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLVtxBufNormal::init( size_t bytes, bool dynamic )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( OGLVtxBufNormal, () );

    if( 0 == bytes )
    {
        GNGFX_ERROR( "Vertex buffer size can't be zero!" );
        quit(); return selfOK();
    }

    // store properties
    setProperties( bytes, dynamic );

    mBuffer = (uint8_t*)memAlloc( bytes );

    // success
    return selfOK();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLVtxBufNormal::quit()
{
    GN_GUARD;

    safeMemFree( mBuffer );

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// from VtxBuf
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void * GN::gfx::OGLVtxBufNormal::lock( size_t offset, size_t /*numVtx*/, uint32_t /*flag*/ )
{
    GN_GUARD_SLOW;

    GN_ASSERT( selfOK() );

    if( mLocked )
    {
        GNGFX_ERROR( "Vertex buffer is already locked!" );
        return 0;
    }
    if( offset >= getSizeInBytes() )
    {
        GNGFX_ERROR( "offset is beyond the end of vertex buffer!" );
        return 0;
    }

    // success
    mLocked = true;
    return &mBuffer[offset];

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLVtxBufNormal::unlock()
{
    GN_GUARD_SLOW;

    GN_ASSERT( selfOK() );

    if( !mLocked )
    {
        GNGFX_ERROR( "Can't unlock a vertex buffer that is not locked at all!" );
        return;
    }

    mLocked = false;

    GN_UNGUARD_SLOW;
}

