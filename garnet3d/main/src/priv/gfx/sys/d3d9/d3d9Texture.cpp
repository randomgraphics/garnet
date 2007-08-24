#include "pch.h"
#include "d3d9Texture.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx2.D3D9Texture");

// *****************************************************************************
// local functions
// *****************************************************************************

// *****************************************************************************
// public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::gfx::Surface * GN::gfx::D3D9Texture::sNewTex2D(
    BaseGraphicsSystem          & gs,
    const SurfaceDesc           & desc,
    const SurfaceCreationHints  & hints )
{
    GN_GUARD;

    AutoObjPtr<D3D9Texture> tex( new D3D9Texture(safeCastRef<D3D9GraphicsSystem>(gs),desc,hints,D3DRTYPE_TEXTURE) );

    if( !tex->init() ) return 0;

    return tex.detach();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::Surface * GN::gfx::D3D9Texture::sNewTex3D(
    BaseGraphicsSystem          & gs,
    const SurfaceDesc           & desc,
    const SurfaceCreationHints  & hints )
{
    GN_GUARD;

    AutoObjPtr<D3D9Texture> tex( new D3D9Texture(safeCastRef<D3D9GraphicsSystem>(gs),desc,hints,D3DRTYPE_VOLUMETEXTURE) );

    if( !tex->init() ) return 0;

    return tex.detach();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::Surface * GN::gfx::D3D9Texture::sNewTexCube(
    BaseGraphicsSystem          & gs,
    const SurfaceDesc           & desc,
    const SurfaceCreationHints  & hints )
{
    GN_GUARD;

    AutoObjPtr<D3D9Texture> tex( new D3D9Texture(safeCastRef<D3D9GraphicsSystem>(gs),desc,hints,D3DRTYPE_CUBETEXTURE) );

    if( !tex->init() ) return 0;

    return tex.detach();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
const GN::gfx::SubSurfaceLayout *
GN::gfx::D3D9Texture::getSubSurfaceLayout( size_t subsurface ) const
{
    if( subsurface >= mSubsurfaces.size() )
    {
        GN_ERROR(sLogger)( "Subsurface index is too large." );
        return 0;
    }

    return &mSubsurfaces[subsurface];
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9Texture::download(
    size_t                 subsurface,
    const Box<size_t>    * area,
    const void           * source,
    size_t                 srcRowBytes,
    size_t                 srcSliceBytes )
{
    GN_GUARD;

    // check parameter
    if( subsurface >= mSubsurfaces.size() )
    {
        GN_ERROR(sLogger)( "Subsurface index is too large." );
        return;
    }
    if( 0 == source )
    {
        GN_ERROR(sLogger)( "null source data" );
        return;
    }

    Box<size_t> clippedArea;
    if( !adjustArea( clippedArea, area ) ) return;

    static DynaArray<UInt8> bgra;
    if( mIsRGBA )
    {
        // convert from R-G-B-A to B-G-R-A
        bgra.resize( srcSliceBytes * clippedArea.d );
        memcpy( bgra.cptr(), source, bgra.size() );
        Vector4<UInt8> * p = (Vector4<UInt8>*)bgra.cptr();
        size_t w = srcRowBytes / 4;
        size_t h = srcSliceBytes / srcRowBytes;
        GN_ASSERT( w >= clippedArea.w && h >= clippedArea.h );
        UInt8 tmp;
        for( size_t z = 0; z < clippedArea.d; ++z )
        {
            for( size_t y = 0; y < clippedArea.h; ++y )
            {
                for( size_t x = 0; x < clippedArea.w; ++x, ++p )
                {
                    // swap R and B
                    tmp = p->r;
                    p->r = p->b;
                    p->b = tmp;
                }
                p += (w - clippedArea.w );
            }
            p += w * ( h - clippedArea.h );
        }
        source = bgra.cptr();
    }

    const SurfaceDesc & desc = getDesc();

    const SubSurfaceLayout & ssl = mSubsurfaces[subsurface];

    const ClrFmtDesc & cfd = getClrFmtDesc(desc.layout.format.attribs[0].format);

    switch( mDim )
    {
        case D3DRTYPE_TEXTURE:
        {
            IDirect3DTexture9 * tex2d = (IDirect3DTexture9*)mSurface;

            RECT rc;
            rc.left   = (int)clippedArea.x;
            rc.top    = (int)clippedArea.y;
            rc.right  = (int)( clippedArea.x + clippedArea.w );
            rc.bottom = (int)( clippedArea.y + clippedArea.h );

            D3DLOCKED_RECT lrc;
            GN_DX9_CHECK_R( tex2d->LockRect( (UINT)subsurface, &lrc, &rc, 0 ) );

            GN_ASSERT( ssl.rowBytes == (size_t)lrc.Pitch / cfd.blockHeight );

            const UInt8 * s = (const UInt8 * )source;
            UInt8       * d = (UInt8*)lrc.pBits;
            for( size_t i = 0; i < clippedArea.h; ++i )
            {
                memcpy( d, s, clippedArea.w * cfd.bits / 8 );
                s += srcRowBytes;
                d += ssl.rowBytes;
            }

            GN_DX9_CHECK( tex2d->UnlockRect( (UINT)subsurface ) );
            break;
        }

        case D3DRTYPE_VOLUMETEXTURE:
            GN_UNIMPL();
            break;

        case D3DRTYPE_CUBETEXTURE:
            GN_UNIMPL();
            break;

        default:
            GN_UNEXPECTED();
            return;
    }

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9Texture::upload(
    size_t              subsurface,
    const Box<size_t> * area,
    void              * destination,
    size_t              destRowBytes,
    size_t              destSliceBytes )
{
    // check parameter
    if( subsurface >= mSubsurfaces.size() )
    {
        GN_ERROR(sLogger)( "Subsurface index is too large." );
        return;
    }
    if( 0 == destination )
    {
        GN_ERROR(sLogger)( "null destination pointer" );
        return;
    }

    Box<size_t> clippedArea;
    if( !adjustArea( clippedArea, area ) ) return;

    const SurfaceDesc & desc = getDesc();

    const SubSurfaceLayout & ssl = mSubsurfaces[subsurface];

    const ClrFmtDesc & cfd = getClrFmtDesc(desc.layout.format.attribs[0].format);

    switch( mDim )
    {
        case D3DRTYPE_TEXTURE:
        {
            IDirect3DTexture9 * tex2d = (IDirect3DTexture9*)mSurface;

            RECT rc;
            rc.left   = (int)clippedArea.x;
            rc.top    = (int)clippedArea.y;
            rc.right  = (int)( clippedArea.x + clippedArea.w );
            rc.bottom = (int)( clippedArea.y + clippedArea.h );

            D3DLOCKED_RECT lrc;
            GN_DX9_CHECK_R( tex2d->LockRect( (UINT)subsurface, &lrc, &rc, D3DLOCK_READONLY ) );

            GN_ASSERT( ssl.rowBytes == (size_t)lrc.Pitch / cfd.blockHeight );

            SafeArrayAccessor<const UInt8> s( (const UInt8*)lrc.pBits, ssl.sliceBytes );
            SafeArrayAccessor<UInt8>       d( (UInt8*)destination, destSliceBytes );

            for( size_t i = 0; i < clippedArea.h; ++i )
            {
                s.copyTo( 0, d, 0, clippedArea.w * cfd.bits / 8 );
                s += ssl.rowBytes;
                d += destRowBytes;
            }

            GN_DX9_CHECK( tex2d->UnlockRect( (UINT)subsurface ) );
            break;
        }

        case D3DRTYPE_VOLUMETEXTURE:
            GN_UNIMPL();
            break;

        case D3DRTYPE_CUBETEXTURE:
            GN_UNIMPL();
            break;

        default:
            GN_UNEXPECTED();
            return;
    }

    if( mIsRGBA )
    {
        // convert from B-G-R-A to R-G-B-A
        SafeArrayAccessor<UInt8> p( (UInt8*)destination, destSliceBytes );

        size_t w = destRowBytes / 4;
        size_t h = destSliceBytes / destRowBytes;

        GN_ASSERT( w >= clippedArea.w && h >= clippedArea.h );

        UInt8 tmp;

        for( size_t z = 0; z < clippedArea.d; ++z )
        {
            for( size_t y = 0; y < clippedArea.h; ++y )
            {
                for( size_t x = 0; x < clippedArea.w; ++x, p+=4 )
                {
                    // swap R and B
                    tmp  = p[0];
                    p[0] = p[2];
                    p[2] = tmp;
                }
                p += ( w - clippedArea.w ) * 4;
            }
            p += w * ( h - clippedArea.h ) * 4;
        }
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9Texture::save( NativeSurfaceData & ) const
{
    GN_UNIMPL();
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9Texture::load( const NativeSurfaceData & )
{
    GN_UNIMPL();
}

// *****************************************************************************
// private methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::gfx::D3D9Texture::D3D9Texture( D3D9GraphicsSystem & gs, const SurfaceDesc & desc, const SurfaceCreationHints & hints, D3DRESOURCETYPE dim )
    : D3D9Surface( desc )
    , mGraphicsSystem( gs )
    , mSurface( 0 )
    , mHints( hints )
    , mDim( dim )
{
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D9Texture::init()
{
    GN_GUARD;

    GN_ASSERT( 0 == mSurface );

    switch( mDim )
    {
        case D3DRTYPE_TEXTURE       : return create2DTexture();
        case D3DRTYPE_VOLUMETEXTURE : return create3DTexture();
        case D3DRTYPE_CUBETEXTURE   : return createCubeTexture();
        default                     : GN_UNEXPECTED(); return false;
    }

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D9Texture::create2DTexture()
{
    GN_GUARD;

    IDirect3DDevice9 * dev = mGraphicsSystem.d3ddev();

    const SurfaceDesc & desc = getDesc();

    GN_ASSERT( D3DRTYPE_TEXTURE == mDim );
    GN_ASSERT( 1 == desc.layout.faces );
    GN_ASSERT( 1 == desc.layout.basemap.depth );
    GN_ASSERT( 1 == desc.layout.format.count );

    // determine format
    D3DFORMAT format = (D3DFORMAT)clrFmt2D3D9Format( desc.layout.format.attribs[0].format );
    if( D3DFMT_UNKNOWN == format )
    {
        GN_ERROR(sLogger)( "invalid texture format." );
        return false;
    }

    // special case for R-G-B-A texture
    if( D3DFMT_A8B8G8R8 == format )
    {
        format = D3DFMT_A8R8G8B8;
        mIsRGBA = true;
    }
    else if( D3DFMT_X8B8G8R8 == format )
    {
        format = D3DFMT_X8R8G8B8;
        mIsRGBA = true;
    }
    else
    {
        mIsRGBA = false;
    }

    // create texture
    AutoComPtr<IDirect3DTexture9> tex2d;
    GN_DX9_CHECK_RV( dev->CreateTexture(
        (UINT)desc.layout.basemap.width,
        (UINT)desc.layout.basemap.height,
        (UINT)desc.layout.levels,
        0, // no usage
        format,
        D3DPOOL_MANAGED,
        &tex2d, 0 ), false );

    // setup subsurface layouts
    mSubsurfaces.resize( desc.layout.levels * desc.layout.faces );
    const ClrFmtDesc & fd = getClrFmtDesc( desc.layout.format.attribs[0].format );
    D3DSURFACE_DESC subdesc;
    D3DLOCKED_RECT lrc;
    for( UINT i = 0; i < desc.layout.levels; ++i )
    {
        GN_DX9_CHECK_RV( tex2d->GetLevelDesc( i, &subdesc ), false );

        GN_DX9_CHECK_RV( tex2d->LockRect( i, &lrc, 0, D3DLOCK_READONLY ), false );
        tex2d->UnlockRect( i );
        
        mSubsurfaces[i].width      = subdesc.Width;
        mSubsurfaces[i].height     = subdesc.Width;
        mSubsurfaces[i].depth      = 1;
        mSubsurfaces[i].rowBytes   = lrc.Pitch / fd.blockHeight;
        mSubsurfaces[i].sliceBytes = mSubsurfaces[i].rowBytes * desc.layout.basemap.height;
    }

    // success
    mSurface = tex2d.detach();
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D9Texture::create3DTexture()
{
    GN_GUARD;

    IDirect3DDevice9 * dev = mGraphicsSystem.d3ddev();

    const SurfaceDesc & desc = getDesc();

    GN_ASSERT( D3DRTYPE_VOLUMETEXTURE == mDim );
    GN_ASSERT( 1 == desc.layout.basemap.depth );
    GN_ASSERT( 1 == desc.layout.format.count );

    // determine format
    D3DFORMAT format = (D3DFORMAT)clrFmt2D3D9Format( desc.layout.format.attribs[0].format );
    if( D3DFMT_UNKNOWN == format )
    {
        GN_ERROR(sLogger)( "invalid texture format." );
        return false;
    }

    // create texture
    AutoComPtr<IDirect3DVolumeTexture9> tex3d;
    GN_DX9_CHECK_RV( dev->CreateVolumeTexture(
        (UINT)desc.layout.basemap.width,
        (UINT)desc.layout.basemap.height,
        (UINT)desc.layout.basemap.depth,
        (UINT)desc.layout.levels,
        0, // no usage
        format,
        D3DPOOL_MANAGED,
        &tex3d, 0 ), false );

    // setup subsurface layouts
    mSubsurfaces.resize( desc.layout.levels * desc.layout.faces );
    const ClrFmtDesc & fd = getClrFmtDesc( desc.layout.format.attribs[0].format );
    D3DVOLUME_DESC subdesc;
    D3DLOCKED_BOX lbox;
    for( UINT i = 0; i < desc.layout.levels; ++i )
    {
        GN_DX9_CHECK_RV( tex3d->GetLevelDesc( i, &subdesc ), false );

        GN_DX9_CHECK_RV( tex3d->LockBox( i, &lbox, 0, D3DLOCK_READONLY ), false );
        tex3d->UnlockBox( i );
        
        mSubsurfaces[i].width      = subdesc.Width;
        mSubsurfaces[i].height     = subdesc.Width;
        mSubsurfaces[i].depth      = subdesc.Depth;
        mSubsurfaces[i].rowBytes   = lbox.RowPitch / fd.blockHeight;
        mSubsurfaces[i].sliceBytes = lbox.SlicePitch;
    }

    // success
    mSurface = tex3d.detach();
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D9Texture::createCubeTexture()
{
    GN_GUARD;

    IDirect3DDevice9 * dev = mGraphicsSystem.d3ddev();

    const SurfaceDesc & desc = getDesc();

    GN_ASSERT( D3DRTYPE_CUBETEXTURE == mDim );
    GN_ASSERT( 6 == desc.layout.faces );
    GN_ASSERT( 1 == desc.layout.basemap.depth );
    GN_ASSERT( 1 == desc.layout.format.count );

    // determine format
    D3DFORMAT format = (D3DFORMAT)clrFmt2D3D9Format( desc.layout.format.attribs[0].format );
    if( D3DFMT_UNKNOWN == format )
    {
        GN_ERROR(sLogger)( "invalid texture format." );
        return false;
    }

    // create texture
    AutoComPtr<IDirect3DCubeTexture9> texcube;
    GN_DX9_CHECK_RV( dev->CreateCubeTexture(
        (UINT)desc.layout.basemap.width,
        (UINT)desc.layout.levels,
        0, // no usage
        format,
        D3DPOOL_MANAGED,
        &texcube, 0 ), false );

    // setup subsurface layouts
    mSubsurfaces.resize( desc.layout.levels * desc.layout.faces );
    const ClrFmtDesc & fd = getClrFmtDesc( desc.layout.format.attribs[0].format );
    D3DSURFACE_DESC subdesc;
    D3DLOCKED_RECT lrc;
    UINT subsurfaceIndex = 0;
    for( UINT f = 0; f < desc.layout.faces; ++f )
    for( UINT i = 0; i < desc.layout.levels; ++i, ++subsurfaceIndex )
    {
        GN_DX9_CHECK_RV( texcube->GetLevelDesc( i, &subdesc ), false );

        GN_DX9_CHECK_RV( texcube->LockRect( (D3DCUBEMAP_FACES)f, i, &lrc, 0, D3DLOCK_READONLY ), false );
        texcube->UnlockRect( (D3DCUBEMAP_FACES)f, i );
        
        mSubsurfaces[subsurfaceIndex].width      = subdesc.Width;
        mSubsurfaces[subsurfaceIndex].height     = subdesc.Width;
        mSubsurfaces[subsurfaceIndex].depth      = 1;
        mSubsurfaces[subsurfaceIndex].rowBytes   = lrc.Pitch / fd.blockHeight;
        mSubsurfaces[subsurfaceIndex].sliceBytes = mSubsurfaces[subsurfaceIndex].rowBytes * desc.layout.basemap.height;
    }

    // success
    mSurface = texcube.detach();
    return true;
    GN_UNGUARD;
}
