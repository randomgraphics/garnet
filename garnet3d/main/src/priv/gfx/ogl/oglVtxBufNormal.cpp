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
        GN_ERROR( "Vertex buffer size can't be zero!" );
        quit(); return selfOK();
    }

    // store properties
    setProperties( bytes, dynamic );

    mBuffer = (uint8_t*)memAlloc( bytes );

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
void * GN::gfx::OGLVtxBufNormal::lock( size_t offset, size_t bytes, LockFlag flag )
{
    GN_GUARD_SLOW;
    GN_ASSERT( selfOK() );
    if( !basicLock( offset, bytes, flag ) ) return false;
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
    basicUnlock();
    GN_UNGUARD_SLOW;
}
