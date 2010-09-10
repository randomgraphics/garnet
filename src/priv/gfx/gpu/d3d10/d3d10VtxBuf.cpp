#include "pch.h"
#include "d3d10Buffer.h"
#include "d3d10Gpu.h"

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
    GN_STDCLASS_INIT( D3D10VtxBuf, (desc.length, desc.fastCpuWrite, D3D10_BIND_VERTEX_BUFFER) );

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
void GN::gfx::D3D10VtxBuf::update(
    size_t            offset,
    size_t            length,
    const void      * data,
    SurfaceUpdateFlag flag )
{
    if( !validateUpdateParameters( offset, &length, data, flag ) ) return;

    D3D10Buffer::update( offset, length, data, flag );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D10VtxBuf::readback( DynaArray<uint8> & data )
{
    D3D10Buffer::readback( data );
}
