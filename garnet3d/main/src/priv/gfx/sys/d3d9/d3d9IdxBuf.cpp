#include "pch.h"
#include "d3d9IdxBuf.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx2.D3D9IdxBuf");

// *****************************************************************************
// public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::gfx::Surface * GN::gfx::D3D9IdxBuf::sNewInstance(
    BaseGraphicsSystem          & gs,
    const SurfaceDesc           & desc,
    const SurfaceCreationHints  & hints )
{
    GN_GUARD;

    AutoObjPtr<D3D9IdxBuf> ib( new D3D9IdxBuf(safeCastRef<D3D9GraphicsSystem>(gs),desc,hints) );

    if( !ib->init() ) return 0;

    return ib.detach();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
const GN::gfx::SubSurfaceLayout *
GN::gfx::D3D9IdxBuf::getSubSurfaceLayout( size_t subsurface ) const
{
    if( 0 == subsurface )
    {
        GN_ERROR(sLogger)( "Index buffer has no subsurfaces" );
        return 0;
    }

    return &mSubsurfaceLayout;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9IdxBuf::download(
    size_t                 subsurface,
    const Box<size_t>    * area,
    const void           * source,
    size_t                 srcRowBytes,
    size_t                 srcSliceBytes )
{
    GN_GUARD;

    GN_UNUSED_PARAM( srcRowBytes );
    GN_UNUSED_PARAM( srcSliceBytes );

    // check parameter
    if( 0 != subsurface )
    {
        GN_ERROR(sLogger)( "Vertex buffer has no subsurfaces" );
        return;
    }

    Box<size_t> clippedArea;
    if( !adjustArea( clippedArea, area ) ) return;

    size_t stride = getDesc().layout.format.stride;

    GN_ASSERT( 2 == stride || 4 == stride );

    UInt8 * data;

    GN_DX9_CHECK_R( mSurface->Lock( (UINT)(clippedArea.x * stride), (UINT)(clippedArea.w * stride), (void**)&data, 0 ) );

    memcpy( data, source, clippedArea.w * stride );

    GN_DX9_CHECK( mSurface->Unlock() );

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9IdxBuf::upload(
    size_t              subsurface,
    const Box<size_t> * area,
    void              * destination,
    size_t              destRowBytes,
    size_t              destSliceBytes )
{
    GN_UNUSED_PARAM( destRowBytes );
    GN_UNUSED_PARAM( destSliceBytes );

    // check parameter
    if( 0 != subsurface )
    {
        GN_ERROR(sLogger)( "Vertex buffer has no subsurfaces" );
        return;
    }

    Box<size_t> clippedArea;
    if( !adjustArea( clippedArea, area ) ) return;

    size_t stride = getDesc().layout.format.stride;

    GN_ASSERT( 2 == stride || 4 == stride );

    void * source;

    GN_DX9_CHECK_R( mSurface->Lock( (UINT)(clippedArea.x * stride), (UINT)(clippedArea.w * stride), &source, D3DLOCK_READONLY ) );

    memcpy( destination, source, clippedArea.w * stride );

    GN_DX9_CHECK( mSurface->Unlock() );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9IdxBuf::save( NativeSurfaceData & ) const
{
    GN_UNIMPL();
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9IdxBuf::load( const NativeSurfaceData & )
{
    GN_UNIMPL();
}

// *****************************************************************************
// private methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::gfx::D3D9IdxBuf::D3D9IdxBuf( D3D9GraphicsSystem & gs, const SurfaceDesc & desc, const SurfaceCreationHints & hints )
    : D3D9Surface( desc )
    , mGraphicsSystem( gs )
    , mSurface( 0 )
    , mHints( hints )
{
    mSubsurfaceLayout = desc.layout.basemap;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D9IdxBuf::init()
{
    GN_GUARD;

    GN_ASSERT( 0 == mSurface );

    const SurfaceDesc & desc = getDesc();

    // check descriptor
    GN_ASSERT( SURFACE_DIMENSION_1D == desc.layout.dim );
    GN_ASSERT( 1 == desc.layout.faces );
    GN_ASSERT( 1 == desc.layout.levels );
    GN_ASSERT( 1 == desc.layout.format.attribs.size() );
    GN_ASSERT( 2 == desc.layout.format.stride || 4 == desc.layout.format.stride );
    //GN_ASSERT( desc.layout.format.attribs[0].sematic == "INDEX" );
    GN_ASSERT( 0 == desc.layout.format.attribs[0].offset );
    GN_ASSERT( COLOR_FORMAT_R_16_UINT == desc.layout.format.attribs[0].format ||
               COLOR_FORMAT_R_32_UINT == desc.layout.format.attribs[0].format );
    GN_ASSERT( desc.layout.format.attribs[0].format.getBitsPerPixel() == desc.layout.format.stride * 8 );
    GN_ASSERT( desc.layout.basemap.rowBytes == desc.layout.basemap.width * desc.layout.format.stride );
    GN_ASSERT( desc.layout.basemap.rowBytes == desc.layout.basemap.sliceBytes );

    IDirect3DDevice9 * dev = mGraphicsSystem.d3ddev();

    // create index buffer
    GN_TODO( "setup usage and pool" );
    GN_DX9_CHECK_RV( dev->CreateIndexBuffer(
        (UINT)desc.layout.basemap.rowBytes,
        0, // usage
        COLOR_FORMAT_R_16_UINT == desc.layout.format.attribs[0].format ? D3DFMT_INDEX16 : D3DFMT_INDEX32,
        D3DPOOL_MANAGED,
        &mSurface, 0 ), false );

    // success
    return true;

    GN_UNGUARD;
}
