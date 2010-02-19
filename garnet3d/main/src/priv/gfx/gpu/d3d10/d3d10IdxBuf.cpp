#include "pch.h"
#include "d3d10Buffer.h"
#include "d3d10Gpu.h"

// *****************************************************************************
// init / quit functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D10IdxBuf::Init( const IdxBufDesc & desc )
{
    GN_GUARD;

    UInt32 bytesPerIndex = 2 << (UInt32)desc.bits32;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::D3D10IdxBuf, (desc.numidx*bytesPerIndex, desc.fastCpuWrite, D3D10_BIND_INDEX_BUFFER) );

    // store buffer parameters
    setDesc( desc );

    // success
    return Success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D10IdxBuf::Quit()
{
    GN_GUARD;

    // standard Quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// interface functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D10IdxBuf::update(
    size_t            startidx,
    size_t            numidx,
    const void      * data,
    SurfaceUpdateFlag flag )
{
    if( !validateUpdateParameters( startidx, &numidx, data, flag ) ) return;

    UInt32 bytesPerIndex = 2 << (UInt32)getDesc().bits32;

    D3D10Buffer::update( startidx*bytesPerIndex, numidx*bytesPerIndex, data, flag );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D10IdxBuf::readback( std::vector<UInt8> & data )
{
    D3D10Buffer::readback( data );
}
