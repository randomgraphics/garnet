#include "pch.h"
#include "d3d11Buffer.h"
#include "d3d11Gpu.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx.gpu.D3D11.Buffer");

// *****************************************************************************
// init / quit functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D11Buffer::init( uint32 bytes, bool fastCpuWrite, uint32 bindFlags )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT();

    // check parameter
    if( 0 == bytes )
    {
        GN_ERROR(sLogger)( "Buffer length can not be zero!" );
        return failure();
    }

    ID3D11Device & cxt = getDeviceRef();

    D3D11_BUFFER_DESC d3ddesc;
    d3ddesc.ByteWidth      = (uint32)bytes;
    d3ddesc.Usage          = fastCpuWrite ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT ;
    d3ddesc.BindFlags      = bindFlags;
    d3ddesc.CPUAccessFlags = fastCpuWrite ? D3D11_CPU_ACCESS_WRITE : 0;
    d3ddesc.MiscFlags      = 0;

    // create d3d ibuffer
    GN_DX_CHECK_RETURN( cxt.CreateBuffer( &d3ddesc, 0, &mD3DBuffer ), failure() );

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
void GN::gfx::D3D11Buffer::quit()
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

static const D3D11_MAP SURFACE_UPDATE_FLAG_TO_D3D11_MAP[] =
{
    D3D11_MAP_WRITE,              // SurfaceUpdateFlag::DEFAULT
    D3D11_MAP_WRITE_DISCARD,      // SurfaceUpdateFlag::DISCARD
    D3D11_MAP_WRITE_NO_OVERWRITE  // SurfaceUpdateFlag::NO_OVERWRITE
};

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D11Buffer::update( uint32 offset, uint32 bytes, const void * data, SurfaceUpdateFlag flag )
{
    ID3D11DeviceContext & cxt = getDeviceContextRef();

    PIXPERF_FUNCTION_EVENT(cxt);

    // parameters should've already been verified by caller.
    GN_ASSERT( ( offset + bytes ) <= mBytes );
    GN_ASSERT( data );
    GN_ASSERT( 0 <= flag && flag < SurfaceUpdateFlag::NUM_FLAGS );

    if( mFastCpuWrite )
    {
        // update dynamic d3d buffer
        D3D11_MAPPED_SUBRESOURCE mapped;
        GN_DX_CHECK_RETURN( cxt.Map( mD3DBuffer, 0, SURFACE_UPDATE_FLAG_TO_D3D11_MAP[flag], 0, &mapped ) );
        uint8 * dst = (uint8*)mapped.pData;
        memcpy( dst+offset, data, bytes );
        cxt.Unmap( mD3DBuffer, 0 );
    }
    else
    {
        // update non-dynamic d3d buffer
        D3D11_BOX box = { (uint32)offset, 0, 0, (uint32)(offset+bytes), 1, 1 };
        cxt.UpdateSubresource(
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
void GN::gfx::D3D11Buffer::readback( DynaArray<uint8> & data )
{
    PIXPERF_FUNCTION_EVENT(getDeviceContextRef());

    data.clear();

    GN_UNIMPL();
}
