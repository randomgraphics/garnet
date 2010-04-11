#include "pch.h"
#include "d3d10Buffer.h"
#include "d3d10Gpu.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx.gpu.D3D10.Buffer");

// *****************************************************************************
// init / quit functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D10Buffer::init( size_t bytes, bool fastCpuWrite, UInt32 bindFlags )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::D3D10Buffer, () );

    // check parameter
    if( 0 == bytes )
    {
        GN_ERROR(sLogger)( "Buffer length can not be zero!" );
        return failure();
    }

    ID3D10Device & dev = getDeviceRef();

    D3D10_BUFFER_DESC d3ddesc;
    d3ddesc.ByteWidth      = (UInt32)bytes;
    d3ddesc.Usage          = fastCpuWrite ? D3D10_USAGE_DYNAMIC : D3D10_USAGE_DEFAULT ;
    d3ddesc.BindFlags      = bindFlags;
    d3ddesc.CPUAccessFlags = fastCpuWrite ? D3D10_CPU_ACCESS_WRITE : 0;
    d3ddesc.MiscFlags      = 0;

    // create d3d ibuffer
    GN_DX_CHECK_RETURN( dev.CreateBuffer( &d3ddesc, 0, &mD3DBuffer ), failure() );

    // store buffer parameters
    mBytes        = bytes;
    mFastCpuWrite = fastCpuWrite;

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D10Buffer::quit()
{
    GN_GUARD;

    safeRelease( mD3DBuffer );

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// interface functions
// *****************************************************************************

static const D3D10_MAP SURFACE_UPDATE_FLAG_TO_D3D10_MAP[] =
{
    D3D10_MAP_WRITE,              // SurfaceUpdateFlag::DEFAULT
    D3D10_MAP_WRITE_DISCARD,      // SurfaceUpdateFlag::DISCARD
    D3D10_MAP_WRITE_NO_OVERWRITE  // SurfaceUpdateFlag::NO_OVERWRITE
};

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D10Buffer::update( size_t offset, size_t bytes, const void * data, SurfaceUpdateFlag flag )
{
    PIXPERF_FUNCTION_EVENT();

    // parameters should've already been verified by caller.
    GN_ASSERT( ( offset + bytes ) <= mBytes );
    GN_ASSERT( data );
    GN_ASSERT( 0 <= flag && flag < SurfaceUpdateFlag::NUM_FLAGS );

    ID3D10Device & dev = getDeviceRef();

    if( mFastCpuWrite )
    {
        // update dynamic d3d buffer
        UInt8 * dst;
        GN_DX_CHECK_RETURN_VOID( mD3DBuffer->Map( SURFACE_UPDATE_FLAG_TO_D3D10_MAP[flag], 0, (void**)&dst ) );
        memcpy( dst+offset, data, bytes );
        mD3DBuffer->Unmap();
    }
    else
    {
        // update non-dynamic d3d buffer
        D3D10_BOX box = { (UInt32)offset, 0, 0, (UInt32)(offset+bytes), 1, 1 };
        dev.UpdateSubresource(
            mD3DBuffer,
            0,   // subresource
            &box,
            data,
            0,   // row pitch
            0 ); // slice pitch
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D10Buffer::readback( DynaArray<UInt8> & data )
{
    PIXPERF_FUNCTION_EVENT();

    data.clear();

    GN_UNIMPL();
}
