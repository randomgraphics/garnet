#include "pch.h"
#include "d3d11Buffer.h"
#include "d3d11Gpu.h"

// *****************************************************************************
// init / quit functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D11IdxBuf::Init( const IdxBufDesc & desc )
{
    GN_GUARD;

    UInt32 bytesPerIndex = 2 << (UInt32)desc.bits32;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::D3D11IdxBuf, (desc.numidx*bytesPerIndex, desc.fastCpuWrite, D3D11_BIND_INDEX_BUFFER) );

    // store buffer parameters
    SetDesc( desc );

    // success
    return Success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D11IdxBuf::Quit()
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
void GN::gfx::D3D11IdxBuf::Update(
    size_t            startidx,
    size_t            numidx,
    const void      * data,
    SurfaceUpdateFlag flag )
{
    if( !validateUpdateParameters( startidx, &numidx, data, flag ) ) return;

    UInt32 bytesPerIndex = 2 << (UInt32)GetDesc().bits32;

    D3D11Buffer::Update( startidx*bytesPerIndex, numidx*bytesPerIndex, data, flag );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D11IdxBuf::Readback( DynaArray<UInt8> & data )
{
    D3D11Buffer::Readback( data );
}
