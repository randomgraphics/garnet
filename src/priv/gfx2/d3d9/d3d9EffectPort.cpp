#include "pch.h"
#include "d3d9VtxBuf.h"
#include "d3d9IdxBuf.h"

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
GN::gfx2::D3D9RenderTargetPort::D3D9RenderTargetPort( D3D9GraphicsSystem & gs )
    : D3D9EffectPort(gs)
{
    mDesc.portType    = D3D9_EFFECT_PORT_RENDER_TARGET;
    mDesc.surfaceType = SURFACE_TYPE_RTT_2D;
    GN_UNIMPL_WARNING();
}

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
void GN::gfx2::D3D9RenderTargetPort::bind( const EffectBindingTarget & ) const
{
    GN_UNIMPL_WARNING();
}

// *****************************************************************************
// D3D9DepthBufferPort
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::gfx2::D3D9DepthBufferPort::D3D9DepthBufferPort( D3D9GraphicsSystem & gs )
    : D3D9EffectPort(gs)
{
    mDesc.portType    = D3D9_EFFECT_PORT_DEPTH_BUFFER;
    mDesc.surfaceType = SURFACE_TYPE_RTS_DEPTH;
    GN_UNIMPL_WARNING();
}

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
void GN::gfx2::D3D9DepthBufferPort::bind( const EffectBindingTarget & ) const
{
    GN_UNIMPL_WARNING();
}

// *****************************************************************************
// D3D9TexturePort
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::gfx2::D3D9TexturePort::D3D9TexturePort( D3D9GraphicsSystem & gs )
    : D3D9EffectPort(gs)
{
    mDesc.portType    = D3D9_EFFECT_PORT_TEXTURE;
    mDesc.surfaceType = SURFACE_TYPE_TEX_2D;
    GN_UNIMPL_WARNING();
}

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
void GN::gfx2::D3D9TexturePort::bind( const EffectBindingTarget & ) const
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
    mDesc.portType    = D3D9_EFFECT_PORT_VTXBUF;
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
bool GN::gfx2::D3D9VtxBufPort::compatible( const Surface * surf ) const
{
    if( 0 == surf )
    {
        GN_ERROR(sLogger)( "Vertex buffer port does not accept NULL surface!" );
        return false;
    }

    const D3D9Surface * d3d9surf = GN_SAFE_CAST<const D3D9Surface*>(surf);

    const D3D9SurfaceDesc & desc = d3d9surf->getD3D9Desc();

    GN_ASSERT( SURFACE_DIMENSION_1D == desc.layout.dim );
    GN_ASSERT( 1 == desc.layout.faces );
    GN_ASSERT( 1 == desc.layout.levels );

    if( SURFACE_TYPE_VB != desc.type )
    {
        GN_ERROR(sLogger)( "Vertex buffer port accepts vertex buffer surface only!" );
        return false;
    }

    if( desc.layout.basemap.rowBytes < desc.layout.basemap.width * desc.layout.format.stride )
    {
        GN_ERROR(sLogger)( "rowBytes must >= stride * width." );
        return false;
    }

    return true;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx2::D3D9VtxBufPort::bind( const EffectBindingTarget & target ) const
{
    IDirect3DDevice9 * dev = gs().d3ddev();

    GN_ASSERT( target.surf );

    D3D9VtxBuf * vb = safeCast<D3D9VtxBuf*>(target.surf);
    const SurfaceDesc & desc = vb->getDesc();
    dev->SetStreamSource( mStage, vb->getSurface(), 0, desc.layout.format.stride );
}


// *****************************************************************************
// D3D9IdxBufPort
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::gfx2::D3D9IdxBufPort::D3D9IdxBufPort( D3D9GraphicsSystem & gs )
    : D3D9EffectPort(gs)
{
    mDesc.portType    = D3D9_EFFECT_PORT_IDXBUF;
    mDesc.surfaceType = SURFACE_TYPE_IB;

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

    // accept 16-bits or 32-bits indices
    mDesc.layout.flags.attributes = 1;
    mDesc.layout.attributes = 1;
    mDesc.layout.requiredAttributes.resize( 1 );
    mDesc.layout.requiredAttributes[0].semantic.set( "INDEX" );
    mDesc.layout.requiredAttributes[0].offset = 0;
    mDesc.layout.requiredAttributes[0].allowedFormats.insert( gfx::FMT_R_16_UINT );
    mDesc.layout.requiredAttributes[0].allowedFormats.insert( gfx::FMT_R_32_UINT );
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx2::D3D9IdxBufPort::compatible( const Surface * surf ) const
{
    if( 0 == surf ) return true;

    const D3D9Surface * d3d9surf = GN_SAFE_CAST<const D3D9Surface*>(surf);

    const D3D9SurfaceDesc & desc = d3d9surf->getD3D9Desc();

    if( SURFACE_TYPE_IB != desc.type )
    {
        GN_ERROR(sLogger)( "Vertex buffer port accepts index buffer surface only!" );
        return false;
    }

    const SurfaceLayout & layout = desc.layout;

    GN_ASSERT( SURFACE_DIMENSION_1D == layout.dim );
    GN_ASSERT( 1 == layout.faces );
    GN_ASSERT( 1 == layout.levels );
    GN_ASSERT( 1 == layout.format.count );
    GN_ASSERT( 0 == layout.format.attribs[0].offset );

    // check format
    if( gfx::FMT_R_16_UINT != layout.format.attribs[0].format &&
        gfx::FMT_R_32_UINT != layout.format.attribs[0].format )
    {
        GN_ERROR(sLogger)( "incorrect index format." );
        return false;
    }
    if( layout.format.stride * 8 != gfx::getClrFmtDesc(layout.format.attribs[0].format).bits )
    {
        GN_ERROR(sLogger)( "incorrect stride." );
        return false;
    }
    if( layout.basemap.rowBytes != layout.basemap.width * layout.format.stride )
    {
        GN_ERROR(sLogger)( "incorrect row bytes." );
        return false;
    }
    if( layout.basemap.sliceBytes != layout.basemap.rowBytes )
    {
        GN_ERROR(sLogger)( "incorrect slice bytes." );
        return false;
    }

    return true;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx2::D3D9IdxBufPort::bind( const EffectBindingTarget & target ) const
{
    GN_ASSERT( target.surf );
    D3D9IdxBuf * ib = safeCast<D3D9IdxBuf*>(target.surf);
    gs().d3ddev()->SetIndices( ib->getSurface() );
}
