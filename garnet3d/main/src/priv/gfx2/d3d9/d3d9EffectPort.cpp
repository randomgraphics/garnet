#include "pch.h"
#include "d3d9VtxBuf.h"

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
GN::gfx2::D3D9VtxBufPort::D3D9VtxBufPort( D3D9GraphicsSystem & gs, UInt32 stage )
    : D3D9EffectPort(gs)
    , mStage(stage)
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
    GN_UNIMPL();
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx2::D3D9VtxBufPort::setVertexCount( UInt32 )
{
    GN_UNIMPL();
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx2::D3D9VtxBufPort::addRequiredAttribute(
    const SurfaceAttributeTemplate & )
{
    GN_UNIMPL();
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx2::D3D9VtxBufPort::addRequiredAttribute(
    const char * semantic, UInt32 offset )
{
    GN_UNUSED_PARAM( semantic );
    GN_UNUSED_PARAM( offset );
    GN_UNIMPL();
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx2::D3D9VtxBufPort::addOptionalAttribute(
    const SurfaceAttributeTemplate & )
{
    GN_UNIMPL();
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx2::D3D9VtxBufPort::compatible( const Surface * ) const
{
    GN_UNIMPL();
    return false;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx2::D3D9VtxBufPort::bind( const EffectPortBinding & epb ) const
{
    IDirect3DDevice9 * dev = gs().d3ddev();

    if( epb.surf )
    {
        D3D9VtxBuf * vb = safeCast<D3D9VtxBuf*>(epb.surf);
        const SurfaceDesc & desc = vb->getDesc();
        dev->SetStreamSource( mStage, vb->getSurface(), 0, desc.layout.format.stride );
    }
    else
    {
        dev->SetStreamSource( mStage, 0, 0, 0 );
    }
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

