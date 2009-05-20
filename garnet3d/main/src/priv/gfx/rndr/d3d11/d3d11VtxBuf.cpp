#include "pch.h"
#include "d3d11Buffer.h"
#include "d3d11Renderer.h"

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D11VtxBuf::init( const VtxBufDesc & desc )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( D3D11VtxBuf, (desc.length, desc.fastCpuWrite, D3D11_BIND_VERTEX_BUFFER) );

    setDesc( desc );

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D11VtxBuf::quit()
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
void GN::gfx::D3D11VtxBuf::update(
    size_t            offset,
    size_t            length,
    const void      * data,
    SurfaceUpdateFlag flag )
{
    if( !validateUpdateParameters( offset, &length, data, flag ) ) return;

    D3D11Buffer::update( offset, length, data, flag );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D11VtxBuf::readback( std::vector<UInt8> & data )
{
    D3D11Buffer::readback( data );
}
