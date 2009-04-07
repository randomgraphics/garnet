#include "pch.h"
#include "xenonVtxBuf.h"
#include "xenonRenderer.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx.rndr.xenon");

// *****************************************************************************
// Local functions
// *****************************************************************************

///
/// convert garnet buffer usage flags to Xenon flags
// ----------------------------------------------------------------------------
static DWORD
sBufUsage2Xenon( const GN::gfx::VtxBufDesc & desc )
{
    if( desc.fastCpuWrite )
    {
        return D3DUSAGE_CPU_CACHED_MEMORY;
    }
    else
    {
        return 0;
    }
}

///
/// convert garnet buffer lock flags to Xenon flags
// ----------------------------------------------------------------------------
static inline DWORD
sLockFlags2Xenon( GN::gfx::SurfaceUpdateFlag flag )
{
    using namespace GN::gfx;

    static DWORD CONVERT_TABLE[] =
    {
        0, // SURFACE_UPDATE_DEFAULT
        0, // SURFACE_UPDATE_DISCARD
        D3DLOCK_NOOVERWRITE, // SURFACE_UPDATE_NO_OVERWRITE
    };
    GN_CASSERT( GN_ARRAY_COUNT(CONVERT_TABLE) == NUM_SURFACE_UPDATE_FLAGS );

    GN_ASSERT( flag < NUM_SURFACE_UPDATE_FLAGS );

    return CONVERT_TABLE[flag];
}

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::XenonVtxBuf::init( const VtxBufDesc & desc )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( XenonVtxBuf, () );

    if( 0 == desc.length || desc.length > (64*1024*1024) )
    {
        GN_ERROR(sLogger)( "Vertex buffer size must be larger then zero and not greater than 64MB." );
        return failure();
    }

    setDesc( desc );

    IDirect3DDevice9 & dev = getRenderer().getDeviceInlined();

    //
    // create d3d vertex buffer
    //
    GN_DX9_CHECK_RV(
        dev.CreateVertexBuffer(
            desc.length,
            sBufUsage2Xenon( desc ),
            0, // FVF
            0, // POOL
            &mVb,
            0 ),
        failure() );

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::XenonVtxBuf::quit()
{
    GN_GUARD;

    safeRelease( mVb );

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
void
GN::gfx::XenonVtxBuf::update(
    size_t            offset,
    size_t            length,
    const void      * data,
    SurfaceUpdateFlag flag )
{
    GN_GUARD_SLOW;

    GN_ASSERT( ok() );

    if( !validateUpdateParameters( offset, &length, data, flag ) ) return;

    // Note: XDK does not support range locking on vertex buffer
    UInt8 * buf;
    GN_DX9_CHECK_DO(
        mVb->Lock( 0, 0, (void**)&buf, sLockFlags2Xenon( flag ) ),
        return; );
    buf += offset;

    ::memcpy( buf, data, length );

    GN_DX9_CHECK( mVb->Unlock() );

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::XenonVtxBuf::readback( std::vector<UInt8> & data )
{
    GN_GUARD_SLOW;

    GN_ASSERT( ok() );

    GN_UNIMPL();

    data.clear();

    GN_UNGUARD_SLOW;
}
