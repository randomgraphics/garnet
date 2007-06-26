#include "pch.h"

static GN::Logger * sLogger = GN::getLogger( "GN.gfx2.D3D9EffectPort" );

// *****************************************************************************
// D3D9EffectPort
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------

// *****************************************************************************
// D3D9RenderTargetPort
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx2::D3D9RenderTargetPort::compatible( const Surface * ) const
{
    GN_UNIMPL_WARNING();
    return false;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx2::D3D9RenderTargetPort::bind( const EffectPortBinding & ) const
{
    GN_UNIMPL_WARNING();
}

// *****************************************************************************
// D3D9DepthBufferPort
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx2::D3D9DepthBufferPort::compatible( const Surface * ) const
{
    GN_UNIMPL_WARNING();
    return false;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx2::D3D9DepthBufferPort::bind( const EffectPortBinding & ) const
{
    GN_UNIMPL_WARNING();
}

// *****************************************************************************
// D3D9TextureferPort
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx2::D3D9TexturePort::compatible( const Surface * ) const
{
    GN_UNIMPL_WARNING();
    return false;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx2::D3D9TexturePort::bind( const EffectPortBinding & ) const
{
    GN_UNIMPL_WARNING();
}

// *****************************************************************************
// D3D9VtxBufPort
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::gfx2::D3D9VtxBufPort::D3D9VtxBufPort()
{
    mDesc.surfaceType = SURFACE_TYPE_VB;

    mDesc.input = true;
    mDesc.output = false;

    mDesc.layout.flags.u32 = 0;

    // 1D
    mDesc.layout.flags.dim = 1;
    mDesc.layout.dim = SURFACE_DIMENSION_1D;

    // no mipmap
    mDesc.layout.flags.levels = 1;
    mDesc.layout.levels = 1;

    // no multiple faces
    mDesc.layout.flags.faces = 1;
    mDesc.layout.faces = 1;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx2::D3D9VtxBufPort::setStride( UInt32 )
{
    GN_UNIMPL_WARNING();
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx2::D3D9VtxBufPort::setVertexCount( UInt32 )
{
    GN_UNIMPL_WARNING();
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx2::D3D9VtxBufPort::addRequiredAttribute(
    const SurfaceAttributeTemplate & )
{
    GN_UNIMPL_WARNING();
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx2::D3D9VtxBufPort::addRequiredAttribute(
    const char * semantic, UInt32 offset )
{
    GN_UNUSED_PARAM( semantic );
    GN_UNUSED_PARAM( offset );
    GN_UNIMPL_WARNING();
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx2::D3D9VtxBufPort::addOptionalAttribute(
    const SurfaceAttributeTemplate & )
{
    GN_UNIMPL_WARNING();
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx2::D3D9VtxBufPort::compatible( const Surface * ) const
{
    GN_UNIMPL_WARNING();
    return false;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx2::D3D9VtxBufPort::bind( const EffectPortBinding & ) const
{
    GN_UNIMPL_WARNING();
}


// *****************************************************************************
// D3D9IdxBufPort
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx2::D3D9IdxBufPort::compatible( const Surface * ) const
{
    GN_UNIMPL_WARNING();
    return false;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx2::D3D9IdxBufPort::bind( const EffectPortBinding & ) const
{
    GN_UNIMPL_WARNING();
}

