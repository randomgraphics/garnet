#include "pch.h"
#include "xenonVtxBuf.h"
#include "xenonGpu.h"

static GN::Logger * sLogger = GN::GetLogger("GN.gfx.gpu.xenon");

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
        0, // SurfaceUpdateFlag::DEFAULT
        0, // SurfaceUpdateFlag::DISCARD
        D3DLOCK_NOOVERWRITE, // SurfaceUpdateFlag::NO_OVERWRITE
    };
    GN_CASSERT( GN_ARRAY_COUNT(CONVERT_TABLE) == SurfaceUpdateFlag::NUM_FLAGS );

    GN_ASSERT( flag < SurfaceUpdateFlag::NUM_FLAGS );

    return CONVERT_TABLE[flag];
}

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::XenonVtxBuf::Init( const VtxBufDesc & desc )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( XenonVtxBuf, () );

    if( 0 == desc.length || desc.length > (64*1024*1024) )
    {
        GN_ERROR(sLogger)( "Vertex buffer size must be larger then zero and not greater than 64MB." );
        return Failure();
    }

    SetDesc( desc );

    IDirect3DDevice9 & dev = GetGpu().getDeviceInlined();

    //
    // create d3d vertex buffer
    //
    GN_DX_CHECK_RETURN(
        dev.CreateVertexBuffer(
            desc.length,
            sBufUsage2Xenon( desc ),
            0, // FVF
            0, // POOL
            &mVb,
            0 ),
        Failure() );

    // success
    return Success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::XenonVtxBuf::Quit()
{
    GN_GUARD;

    SafeRelease( mVb );

    // standard Quit procedure
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
GN::gfx::XenonVtxBuf::Update(
    size_t            offset,
    size_t            length,
    const void      * data,
    SurfaceUpdateFlag flag )
{
    GN_GUARD_SLOW;

    GN_ASSERT( Ok() );

    if( !validateUpdateParameters( offset, &length, data, flag ) ) return;

    // check if the vertex buffer is binding to the device.
    IDirect3DDevice9 * dev = (IDirect3DDevice9 *)GetGpu().getD3DDevice();
    const DWORD NUM_VB = 16;
    bool bindingFlags[NUM_VB];
    UINT offsets[NUM_VB];
    UINT strides[NUM_VB];
    for( DWORD i = 0; i < NUM_VB; ++i )
    {
        AutoComPtr<IDirect3DVertexBuffer9> vb;
        dev->GetStreamSource( i, &vb, &offsets[i], &strides[i] );

        bindingFlags[i] = vb == mVb;

        if( bindingFlags[i] )
        {
            dev->SetStreamSource( i, 0, 0, 0 );
        }
    }

    // Note: XDK does not support range locking on vertex buffer
    UInt8 * buf;
    GN_DX_CHECK_DO(
        mVb->Lock( 0, 0, (void**)&buf, sLockFlags2Xenon( flag ) ),
        return; );
    buf += offset;

    ::memcpy( buf, data, length );

    GN_DX_CHECK( mVb->Unlock() );

    // restore vertex buffer bindings
    for( DWORD i = 0; i < NUM_VB; ++i )
    {
        if( bindingFlags[i] )
        {
            dev->SetStreamSource( i, mVb, offsets[i], strides[i] );
        }
    }

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::XenonVtxBuf::Readback( DynaArray<UInt8> & data )
{
    GN_GUARD_SLOW;

    GN_ASSERT( Ok() );

    GN_UNIMPL();

    data.Clear();

    GN_UNGUARD_SLOW;
}
