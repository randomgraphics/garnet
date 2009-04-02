#include "pch.h"
#include "xenonIdxBuf.h"
#include "xenonRenderer.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx.rndr.xenon");

// *****************************************************************************
// Local functions
// *****************************************************************************

///
/// convert garnet buffer usage flags to Xenon flags
// ----------------------------------------------------------------------------
static DWORD
sBufUsage2Xenon( const GN::gfx::IdxBufDesc & desc )
{
    if( desc.fastCpuRead || desc.fastCpuWrite )
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
bool GN::gfx::XenonIdxBuf::init( const IdxBufDesc & desc )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( XenonIdxBuf, () );

    if( 0 == desc.numidx )
    {
        GN_ERROR(sLogger)( "Index buffer size must be larger then zero." );
        return failure();
    }

    setDesc( desc );

    IDirect3DDevice9 & dev = getRenderer().getDeviceInlined();

    // get bytes per index
    UInt32 bpi = 2 << (UInt32)desc.bits32;

    //
    // create d3d vertex buffer
    //
    GN_DX9_CHECK_RV(
        dev.CreateIndexBuffer(
            desc.numidx * bpi,
            sBufUsage2Xenon( desc ),
            desc.bits32 ? D3DFMT_INDEX32 : D3DFMT_INDEX16,
            0, // POOL
            &mIb,
            0 ),
        failure() );

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::XenonIdxBuf::quit()
{
    GN_GUARD;

    safeRelease( mIb );

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// from IdxBuf
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void
GN::gfx::XenonIdxBuf::update(
    size_t            startidx,
    size_t            numidx,
    const void      * data,
    SurfaceUpdateFlag flag )
{
    GN_GUARD_SLOW;

    GN_ASSERT( ok() );

    // Note: this function may modify "numidx"
    if( !validateUpdateParameters( startidx, &numidx, data, flag ) ) return;

    size_t bpi = 2 << (size_t)getDesc().bits32;

    // Note: XDK does not support partial locking on index buffer
    UInt8 * buf;
    GN_DX9_CHECK_DO(
        mIb->Lock( 0, 0, (void**)&buf, sLockFlags2Xenon( flag ) ),
        return; );
    buf += startidx * bpi;

    ::memcpy( buf, data, numidx * bpi );

    GN_DX9_CHECK( mIb->Unlock() );

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::XenonIdxBuf::readback( std::vector<UInt8> & data )
{
    GN_GUARD_SLOW;

    GN_ASSERT( ok() );

    GN_UNIMPL();

    data.clear();

    GN_UNGUARD_SLOW;
}
