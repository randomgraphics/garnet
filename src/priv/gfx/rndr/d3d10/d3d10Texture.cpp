#include "pch.h"
#include "d3d10Renderer.h"
#include "d3d10Texture.h"
#include "garnet/GNd3d10.h"

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
    if( !setDesc( desc ) || !createTexture() || !createViews() ) return failure();

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
    safeRelease( mRTView );
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
    const Boxi * area,
    LockFlag flag )
{
    GN_ASSERT( ok() );
    GN_UNIMPL_WARNING();
    return false;
}

//
//
// ----------------------------------------------------------------------------
void GN::gfx::D3D10Texture::unlock()
{
    GN_ASSERT( ok() );
    GN_UNIMPL_WARNING();
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
        format = ( TEXUSAGE_DEPTH & desc.usage )
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
    if( TEXUSAGE_DYNAMIC & desc.usage ) usage = D3D10_USAGE_DYNAMIC;
    else usage = D3D10_USAGE_DEFAULT;

    // determin bind flags
    UINT bf = D3D10_BIND_SHADER_RESOURCE;
    if( TEXUSAGE_DEPTH & desc.usage )
    {
        bf |= D3D10_BIND_DEPTH_STENCIL;
    }
    else if( TEXUSAGE_RENDER_TARGET & desc.usage )
    {
        bf |= D3D10_BIND_RENDER_TARGET;
    }

    // determine CPU access flags
    UINT caf = 0;
    if( 0 == ((TEXUSAGE_RENDER_TARGET|TEXUSAGE_DEPTH) & desc.usage) ) caf |= D3D10_CPU_ACCESS_WRITE;
    if( TEXUSAGE_READBACK & desc.usage ) caf |= D3D10_CPU_ACCESS_READ;

    // determine misc flags
    UINT mf = 0;
    if( TEXUSAGE_AUTOGEN_MIPMAP & desc.usage ) mf |= D3D10_RESOURCE_MISC_GENERATE_MIPS;
    if( TEXTYPE_CUBE == desc.type ) mf |= D3D10_RESOURCE_MISC_TEXTURECUBE;

    // create texture instance
    ID3D10Device * dev = mRenderer.getDevice();
    if( TEXTYPE_1D == desc.type )
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
    else if( TEXTYPE_2D == desc.type || TEXTYPE_CUBE == desc.type )
    {
        D3D10_TEXTURE2D_DESC desc2d;
        desc2d.Width = desc.width;
        desc2d.Height = desc.height;
        desc2d.MipLevels = desc.levels;
        desc2d.ArraySize = desc.faces;
        desc2d.Format = format;
        desc2d.SampleDesc.Count = 0;
        desc2d.SampleDesc.Quality = 0;
        desc2d.Usage = usage;
        desc2d.BindFlags = bf;
        desc2d.CPUAccessFlags = caf;
        desc2d.MiscFlags = mf;
        GN_DX10_CHECK_RV( dev->CreateTexture2D( &desc2d, 0, &mD3DTexture.tex2d ), false );
    }
    else if( TEXTYPE_3D == desc.type )
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
        GN_UNEXPECTED();
        GN_ERROR(sLogger)( "Invalid texture type: %d", desc.type );
        return false;
    }

    // calculate mipmap sizes
    if( !isPowerOf2(desc.width) || !isPowerOf2(desc.height) || !isPowerOf2(desc.depth) )
    {
        GN_TODO( "mipsize may be inacurate, if texture size is not 2^n." );
    }
    Vector3<uint32_t> mipSize( desc.width, desc.height, desc.depth );
    for( size_t i = 0; i < getDesc().levels; ++i )
    {
        setMipSize( i, mipSize );
        if( mipSize.x > 1 ) mipSize.x >>= 1;
        if( mipSize.y > 1 ) mipSize.y >>= 1;
        if( mipSize.z > 1 ) mipSize.z >>= 1;
    }

    // call user-defined content loader
    if( !getLoader().empty() && !getLoader()( *this ) ) return false;

    // success
    return true;

    GN_UNGUARD;
}

//
//
// ----------------------------------------------------------------------------
bool GN::gfx::D3D10Texture::createViews()
{
    GN_ASSERT( mD3DTexture.res );
    GN_UNIMPL_WARNING();
    return true;
}
