#include "pch.h"
#include "d3d10Buffer.h"
#include "d3d10Renderer.h"

GN::Logger * sLogger = GN::getLogger("GN.gfx.rndr.D3D10.VtxBuf");

// *****************************************************************************
// Local functions
// *****************************************************************************

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D10VtxBuf::init( const VtxBufDesc & desc )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( D3D10VtxBuf, (desc.bytes,desc.dynamic,desc.readback,D3D10_BIND_VERTEX_BUFFER) );

    setDesc( desc );

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D10VtxBuf::quit()
{
    GN_GUARD;

    if( isLocked() )
    {
        unlock();
        GN_ERROR(sLogger)( "call unlock() before u dispose the index buffer!" );
    }

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
void * GN::gfx::D3D10VtxBuf::lock( size_t offset, size_t bytes, LockFlag flag )
{
    GN_GUARD_SLOW;

    GN_ASSERT( ok() );

    if( !basicLock( offset, bytes, flag ) ) return false;

    GN_ASSERT( ok() );

    return d3dlock( offset, bytes, flag );

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D10VtxBuf::unlock()
{
    GN_GUARD_SLOW;

    GN_ASSERT( ok() );

    if( !basicUnlock() ) return;

    d3dunlock();

    GN_UNGUARD_SLOW;
}
