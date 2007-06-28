#include "pch.h"
#include "d3d9VtxBuf.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx2.D3D9VtxBuf");

// *****************************************************************************
// public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::gfx2::D3D9VtxBuf * GN::gfx2::D3D9VtxBuf::sNewInstance(
    D3D9GraphicsSystem          & gs,
    const SurfaceLayout         & layout,
    int                           access,
    const SurfaceCreationHints  & hints )
{
    GN_GUARD;

    D3D9SurfaceDesc desc;
    desc.type = SURFACE_TYPE_VB;
    desc.layout = layout;
    desc.access = access;

    AutoObjPtr<D3D9VtxBuf> vb( new D3D9VtxBuf(gs,desc,hints) );

    if( !vb->init() ) return 0;

    return vb.detach();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx2::D3D9VtxBuf::~D3D9VtxBuf()
{
    safeRelease( mSurface );
}

#pragma warning(disable:4100)

//
//
// -----------------------------------------------------------------------------
const GN::gfx2::SubSurfaceLayout *
GN::gfx2::D3D9VtxBuf::getSubSurfaceLayout( size_t subsurface ) const
{
    if( 0 == subsurface )
    {
        GN_ERROR(sLogger)( "Vertex buffer has no subsurfaces" );
        return 0;
    }

    return &mSubsurfaceLayout;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx2::D3D9VtxBuf::download(
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

    UInt8 * data;

    GN_DX9_CHECK_R( mSurface->Lock( (UINT)(area.x * stride), (UINT)(area.w * stride), (void**)&data, 0 ) );

    memcpy( data, source, area.w * stride );

    GN_DX9_CHECK( mSurface->Unlock() );

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx2::D3D9VtxBuf::upload(
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
void GN::gfx2::D3D9VtxBuf::save( NativeSurfaceData & ) const
{
    GN_UNIMPL();
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx2::D3D9VtxBuf::load( const NativeSurfaceData & )
{
    GN_UNIMPL();
}

// *****************************************************************************
// private methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::gfx2::D3D9VtxBuf::D3D9VtxBuf( D3D9GraphicsSystem & gs, const D3D9SurfaceDesc & desc, const SurfaceCreationHints & hints )
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
bool GN::gfx2::D3D9VtxBuf::init()
{
    GN_GUARD;

    GN_ASSERT( 0 == mSurface );

    const D3D9SurfaceDesc & desc = getD3D9Desc();

    GN_ASSERT( SURFACE_TYPE_VB == desc.type );
    GN_ASSERT( SURFACE_DIMENSION_1D == desc.layout.dim );
    GN_ASSERT( 1 == desc.layout.faces );
    GN_ASSERT( 1 == desc.layout.levels );
    GN_ASSERT( desc.layout.basemap.rowBytes >= desc.layout.basemap.width * desc.layout.format.stride );

    IDirect3DDevice9 * dev = mGraphicsSystem.d3ddev();

    // create vertex buffer
    GN_TODO( "setup usage and pool from hints" );
    GN_DX9_CHECK_RV( dev->CreateVertexBuffer( desc.layout.basemap.rowBytes, 0, 0, D3DPOOL_DEFAULT, &mSurface, 0 ), false );

    // success
    return true;

    GN_UNGUARD;
}
