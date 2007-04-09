#include "pch.h"
#include "d3d10Renderer.h"
#include "d3d10Texture.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx.rndr.D3D10");
GN::Logger * GN::gfx::D3D10Texture::sLogger = GN::getLogger("GN.gfx.rndr.D3D10");

// *****************************************************************************
// local functions
// *****************************************************************************

// ****************************************************************************
//  public utils
// ****************************************************************************

// ****************************************************************************
//  init / quit functions
// ****************************************************************************

//
//
// ----------------------------------------------------------------------------
bool GN::gfx::D3D10Texture::init( const TextureDesc & desc )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::D3D10Texture, () );

    // create device data
    if( !setDesc( desc ) || !createTexture() || !createDefaultViews() ) return failure();

    // success
    return success();

    GN_UNGUARD;
}

//
//
// ----------------------------------------------------------------------------
void GN::gfx::D3D10Texture::quit()
{
    GN_GUARD;

    // check if locked
    if( isLocked() )
    {
        GN_WARN(sLogger)( "You are destroying a locked texture!" );
        unlock();
    }

    safeRelease( mSRView );
    mRTViews.clear();
    safeRelease( mD3DTexture.res );

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// ****************************************************************************
//      interface functions
// ****************************************************************************

// ****************************************************************************
//      interface functions
// ****************************************************************************

//
//
// ----------------------------------------------------------------------------
void GN::gfx::D3D10Texture::setFilter( TexFilter min, TexFilter mag ) const
{
    GN_ASSERT( ok() );
    GN_UNIMPL_WARNING();
}

//
//
// ----------------------------------------------------------------------------
void GN::gfx::D3D10Texture::setWrap( TexWrap s, TexWrap t, TexWrap r ) const
{
    GN_ASSERT( ok() );
    GN_UNIMPL_WARNING();
}

//
//
// ----------------------------------------------------------------------------
bool GN::gfx::D3D10Texture::lock(
    TexLockedResult & result,
    size_t face,
    size_t level,
    const TexLockArea * area,
    LockFlag flag )
{
    GN_ASSERT( ok() );

    TexLockArea clippedArea;
    if( !basicLock( face, level, area, flag, clippedArea ) ) return false;

    const TextureDesc & desc = getDesc();

    // create temporary buffer
    size_t rowbytes = getMipSize(level).x * getClrFmtDesc(desc.format).bits / 8;
    size_t slicebytes = rowbytes * getMipSize(level).y;
    mLockedBuffer.resize( slicebytes * getMipSize(level).z );
    result.data = mLockedBuffer.cptr();
    result.rowBytes = rowbytes;
    result.sliceBytes = slicebytes;

    return true;
}

//
//
// ----------------------------------------------------------------------------
void GN::gfx::D3D10Texture::unlock()
{
    GN_ASSERT( ok() );
    basicUnlock();
    //GN_UNIMPL_WARNING();
}

//
//
// ----------------------------------------------------------------------------
void GN::gfx::D3D10Texture::updateMipmap()
{
    GN_ASSERT( ok() );
    GN_UNIMPL_WARNING();
}

// ****************************************************************************
//      private functions
// ****************************************************************************

//
//
// ----------------------------------------------------------------------------
bool GN::gfx::D3D10Texture::createTexture()
{
    GN_GUARD;

    GN_ASSERT( !mD3DTexture.res );

    const TextureDesc & desc = getDesc();

    // determine texture format
    DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN;
    if( FMT_DEFAULT == desc.format )
    {
        format = desc.usage.depthstencil
            ? DXGI_FORMAT_D32_FLOAT // TODO: is this correct depth texture format?
            : DXGI_FORMAT_B8G8R8A8_UNORM;
        if( DXGI_FORMAT_UNKNOWN == format )
        {
            GN_ERROR(sLogger)( "Fail to detect default texture format." );
            return false;
        }
        GN_TRACE(sLogger)( "Use default texture format: %s", d3d10::dxgiFormat2Str( format ) );
    }
    else
    {
        format = d3d10::clrFmt2DxgiFormat( desc.format );
        if( DXGI_FORMAT_UNKNOWN == format )
        {
            GN_ERROR(sLogger)( "Fail to convert color format '%s' to DXGI_FORMAT.", clrFmt2Str(desc.format) );
            return false;
        }
    }

    // determine usage
    D3D10_USAGE usage;
    if( desc.usage.dynamic ) usage = D3D10_USAGE_DYNAMIC;
    else usage = D3D10_USAGE_DEFAULT;

    // determin bind flags
    UINT bf = D3D10_BIND_SHADER_RESOURCE;
    if( desc.usage.depthstencil )
    {
        bf |= D3D10_BIND_DEPTH_STENCIL;
    }
    else if( desc.usage.rendertarget )
    {
        bf |= D3D10_BIND_RENDER_TARGET;
    }

    // determine CPU access flags
    UINT caf = 0;
    if( D3D10_USAGE_DEFAULT != usage )
    {
        if( !desc.usage.rendertarget && !desc.usage.depthstencil ) caf |= D3D10_CPU_ACCESS_WRITE;
        if( desc.usage.readback ) caf |= D3D10_CPU_ACCESS_READ;
    }

    // determine misc flags
    UINT mf = 0;
    if( desc.usage.automip ) mf |= D3D10_RESOURCE_MISC_GENERATE_MIPS;
    if( TEXDIM_CUBE == desc.dim ) mf |= D3D10_RESOURCE_MISC_TEXTURECUBE;

    // create texture instance
    ID3D10Device * dev = mRenderer.getDevice();
    if( TEXDIM_1D == desc.dim )
    {
        D3D10_TEXTURE1D_DESC desc1d;
        desc1d.Width = desc.width;
        desc1d.MipLevels = desc.levels;
        desc1d.ArraySize = desc.faces;
        desc1d.Format = format;
        desc1d.Usage = usage;
        desc1d.BindFlags = bf;
        desc1d.CPUAccessFlags = caf;
        desc1d.MiscFlags = mf;
        GN_DX10_CHECK_RV( dev->CreateTexture1D( &desc1d, 0, &mD3DTexture.tex1d ), false );
    }
    else if( TEXDIM_2D == desc.dim || TEXDIM_CUBE == desc.dim )
    {
        D3D10_TEXTURE2D_DESC desc2d;
        desc2d.Width = desc.width;
        desc2d.Height = desc.height;
        desc2d.MipLevels = desc.levels;
        desc2d.ArraySize = desc.faces;
        desc2d.Format = format;
        desc2d.SampleDesc.Count = 1;
        desc2d.SampleDesc.Quality = 0;
        desc2d.Usage = usage;
        desc2d.BindFlags = bf;
        desc2d.CPUAccessFlags = caf;
        desc2d.MiscFlags = mf;
        GN_DX10_CHECK_RV( dev->CreateTexture2D( &desc2d, 0, &mD3DTexture.tex2d ), false );
    }
    else if( TEXDIM_3D == desc.dim )
    {
        D3D10_TEXTURE3D_DESC desc3d;
        desc3d.Width = desc.width;
        desc3d.Height = desc.height;
        desc3d.Depth = desc.depth;
        desc3d.MipLevels = desc.levels;
        desc3d.Format = format;
        desc3d.Usage = usage;
        desc3d.BindFlags = bf;
        desc3d.CPUAccessFlags = caf;
        desc3d.MiscFlags = mf;
        GN_DX10_CHECK_RV( dev->CreateTexture3D( &desc3d, 0, &mD3DTexture.tex3d ), false );
    }
    else
    {
        GN_ERROR(sLogger)( "Invalid texture dimension: %d", desc.dim );
        GN_UNEXPECTED();
        return false;
    }

    // calculate mipmap sizes
    Vector3<UInt32> mipSize( desc.width, desc.height, desc.depth );
    for( size_t i = 0; i < desc.levels; ++i )
    {
        setMipSize( i, mipSize );
        if( mipSize.x > 1 ) mipSize.x >>= 1;
        if( mipSize.y > 1 ) mipSize.y >>= 1;
        if( mipSize.z > 1 ) mipSize.z >>= 1;
    }

    // success
    return true;

    GN_UNGUARD;
}

//
//
// ----------------------------------------------------------------------------
bool GN::gfx::D3D10Texture::createDefaultViews()
{
    GN_ASSERT( mD3DTexture.res );
    GN_UNIMPL_WARNING();
    return true;
}
