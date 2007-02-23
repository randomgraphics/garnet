#include "pch.h"
#include "oglVtxBuf.h"

GN::Logger * GN::gfx::OGLVtxBufNormal::sLogger = GN::getLogger("GN.gfx.rndr.OGL");

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLVtxBufNormal::init( const VtxBufDesc & desc )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( OGLVtxBufNormal, () );

    if( 0 == desc.bytes )
    {
        GN_ERROR(sLogger)( "Vertex buffer size can't be zero!" );
        return failure();
    }

    // store descriptor
    setDesc( desc );

    mBuffer = (UInt8*)heapAlloc( desc.bytes );

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLVtxBufNormal::quit()
{
    GN_GUARD;

    safeHeapFree( mBuffer );

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
    GN_ASSERT( ok() );
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
    GN_ASSERT( ok() );
    basicUnlock();
    GN_UNGUARD_SLOW;
}
