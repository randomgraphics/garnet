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
void GN::gfx2::D3D9RenderTargetPort::bind( const D3D9SurfaceBindingDesc & ) const
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
void GN::gfx2::D3D9DepthBufferPort::bind( const D3D9SurfaceBindingDesc & ) const
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
void GN::gfx2::D3D9TexturePort::bind( const D3D9SurfaceBindingDesc & ) const
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
    GN_UNIMPL_WARNING();
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
void GN::gfx2::D3D9VtxBufPort::bind( const D3D9SurfaceBindingDesc & ) const
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
void GN::gfx2::D3D9IdxBufPort::bind( const D3D9SurfaceBindingDesc & ) const
{
    GN_UNIMPL_WARNING();
}

