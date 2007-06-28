#include "pch.h"
#include "d3d9IdxBuf.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx2.D3D9IdxBuf");

// *****************************************************************************
// public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::gfx2::D3D9IdxBuf * GN::gfx2::D3D9IdxBuf::sNewInstance(
    D3D9GraphicsSystem          & gs,
    const SurfaceLayout         & layout,
    int                           access,
    const SurfaceCreationHints  & hints )
{
    GN_GUARD;

    D3D9SurfaceDesc desc;
    desc.type = SURFACE_TYPE_IB;
    desc.layout = layout;
    desc.access = access;

    AutoObjPtr<D3D9IdxBuf> ib( new D3D9IdxBuf(gs,desc,hints) );

    if( !ib->init() ) return 0;

    return ib.detach();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx2::D3D9IdxBuf::~D3D9IdxBuf()
{
    safeRelease( mSurface );
}

#pragma warning(disable:4100)

//
//
// -----------------------------------------------------------------------------
const GN::gfx2::SubSurfaceLayout *
GN::gfx2::D3D9IdxBuf::getSubSurfaceLayout( size_t subsurface ) const
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
void GN::gfx2::D3D9IdxBuf::download(
    size_t                 subsurface,
    const Box<size_t>    & area,
    const void           * source,
    size_t                 srcRowBytes,
    size_t                 srcSliceBytes )
{
    GN_GUARD;

    // check parameter
    if( 0 != subsurface )
    {
        GN_ERROR(sLogger)( "Vertex buffer has no subsurfaces" );
        return;
    }

    size_t stride = getDesc().layout.format.stride;

    GN_ASSERT( 2 == stride || 4 == stride );

    UInt8 * data;

    GN_DX9_CHECK_R( mSurface->Lock( (UINT)(area.x * stride), (UINT)(area.w * stride), (void**)&data, 0 ) );

    memcpy( data, source, area.w * stride );

    GN_DX9_CHECK( mSurface->Unlock() );

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx2::D3D9IdxBuf::upload(
    size_t              subsurface,
    const Box<size_t> & area,
    void              * destination,
    size_t              destRowBytes,
    size_t              destSliceBytes )
{
    GN_UNIMPL();
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx2::D3D9IdxBuf::save( NativeSurfaceData & ) const
{
    GN_UNIMPL();
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx2::D3D9IdxBuf::load( const NativeSurfaceData & )
{
    GN_UNIMPL();
}

// *****************************************************************************
// private methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::gfx2::D3D9IdxBuf::D3D9IdxBuf( D3D9GraphicsSystem & gs, const D3D9SurfaceDesc & desc, const SurfaceCreationHints & hints )
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
bool GN::gfx2::D3D9IdxBuf::init()
{
    GN_GUARD;

    GN_ASSERT( 0 == mSurface );

    const D3D9SurfaceDesc & desc = getD3D9Desc();

    GN_ASSERT( SURFACE_TYPE_IB == desc.type );

    // check descriptor
    GN_ASSERT( SURFACE_DIMENSION_1D == desc.layout.dim );
    GN_ASSERT( 1 == desc.layout.faces );
    GN_ASSERT( 1 == desc.layout.levels );
    GN_ASSERT( 1 == desc.layout.format.count );
    GN_ASSERT( 2 == desc.layout.format.stride || 4 == desc.layout.format.stride );
    //GN_ASSERT( desc.layout.format.attribs[0].sematic == "INDEX" );
    GN_ASSERT( 0 == desc.layout.format.attribs[0].offset );
    GN_ASSERT( gfx::FMT_R_16_UINT == desc.layout.format.attribs[0].format ||
               gfx::FMT_R_32_UINT == desc.layout.format.attribs[0].format );
    GN_ASSERT( gfx::getClrFmtDesc(desc.layout.format.attribs[0].format).bits == desc.layout.format.stride * 8 );
    GN_ASSERT( desc.layout.basemap.rowBytes == desc.layout.basemap.width * desc.layout.format.stride );
    GN_ASSERT( desc.layout.basemap.rowBytes == desc.layout.basemap.sliceBytes );

    IDirect3DDevice9 * dev = mGraphicsSystem.d3ddev();

    // create index buffer
    GN_TODO( "setup usage and pool based on hints" );
    GN_DX9_CHECK_RV( dev->CreateIndexBuffer(
        desc.layout.basemap.rowBytes,
        0, // usage
        gfx::FMT_R_16_UINT == desc.layout.format.attribs[0].format ? D3DFMT_INDEX16 : D3DFMT_INDEX32,
        D3DPOOL_DEFAULT,
        &mSurface, 0 ), false );

    // success
    return true;

    GN_UNGUARD;
}
