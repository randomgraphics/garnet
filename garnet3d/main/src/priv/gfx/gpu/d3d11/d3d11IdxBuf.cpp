#include "pch.h"
#include "d3d11Buffer.h"
#include "d3d11Gpu.h"

// *****************************************************************************
// init / quit functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D11IdxBuf::init( const IdxBufDesc & desc )
{
    GN_GUARD;

    uint32 bytesPerIndex = 2 << (uint32)desc.bits32;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::D3D11IdxBuf, (desc.numidx*bytesPerIndex, desc.fastCpuWrite, D3D11_BIND_INDEX_BUFFER) );

    // store buffer parameters
    setDesc( desc );

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D11IdxBuf::quit()
{
    GN_GUARD;

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// interface functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D11IdxBuf::update(
    size_t            startidx,
    size_t            numidx,
    const void      * data,
    SurfaceUpdateFlag flag )
{
    if( !validateUpdateParameters( startidx, &numidx, data, flag ) ) return;

    uint32 bytesPerIndex = 2 << (uint32)getDesc().bits32;

    D3D11Buffer::update( startidx*bytesPerIndex, numidx*bytesPerIndex, data, flag );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D11IdxBuf::readback( DynaArray<uint8> & data )
{
    D3D11Buffer::readback( data );
}
