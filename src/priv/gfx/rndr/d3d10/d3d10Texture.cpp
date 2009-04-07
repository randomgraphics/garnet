#include "pch.h"
#include "d3d10Renderer.h"
#include "d3d10Texture.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx.rndr.D3D10");

// *****************************************************************************
// local functions
// *****************************************************************************

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

    mRTViews.clear();
    safeRelease( mSRView );
    safeRelease( mTexture );

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
void GN::gfx::D3D10Texture::updateMipmap(
    size_t              /*face*/,
    size_t              /*level*/,
    const Box<UInt32> * /*area*/,
    size_t              /*rowPitch*/,
    size_t              /*slicePitch*/,
    const void        * /*data*/,
    SurfaceUpdateFlag   /*flag*/ )
{
    GN_UNIMPL_WARNING();
}

//
//
// ----------------------------------------------------------------------------
void GN::gfx::D3D10Texture:: readMipmap(
    size_t       /*face*/,
    size_t       /*level*/,
    MipmapData & /*data*/ )
{
    GN_UNIMPL();
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

    GN_ASSERT( !mTexture );

    const TextureDesc & desc = getDesc();

    // determine texture format
    DXGI_FORMAT format = (DXGI_FORMAT)colorFormat2DxgiFormat( desc.format );
    if( DXGI_FORMAT_UNKNOWN == format )
    {
        GN_ERROR(sLogger)( "Fail to convert color format '%s' to DXGI_FORMAT.", desc.format.toString().cptr() );
        return false;
    }

    // determine usage and CPU access flag
    D3D10_USAGE usage;
    UINT        caf;
    if( desc.usages.fastCpuWrite )
    {
        usage = D3D10_USAGE_DYNAMIC;
        caf   = D3D10_CPU_ACCESS_WRITE;
    }
    else
    {
        usage = D3D10_USAGE_DEFAULT;
        caf   = 0;
    }

    // determine bind flags
    UINT bf = D3D10_BIND_SHADER_RESOURCE;
    if( desc.usages.depth )
    {
        bf |= D3D10_BIND_DEPTH_STENCIL;
    }
    else if( desc.usages.rendertarget )
    {
        bf |= D3D10_BIND_RENDER_TARGET;
    }

    GN_UNIMPL_WARNING();

    // TODO: determine texture dimension

    /* determine misc flags
    UINT mf = 0;
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
    }*/

    // success
    return true;

    GN_UNGUARD;
}

//
//
// ----------------------------------------------------------------------------
bool GN::gfx::D3D10Texture::createDefaultViews()
{
    GN_UNIMPL_WARNING();
    return true;
}
