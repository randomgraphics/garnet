#include "pch.h"
#include "xenonGpu.h"
#include "xenonTexture.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx.gpu.xenon");

// *****************************************************************************
// local functions
// *****************************************************************************

//
//
// ----------------------------------------------------------------------------
static GN::gfx::XenonTextureDimension
sDetermineTextureDimension( const GN::gfx::TextureDesc & desc )
{
    if( 1 == desc.height && 1 == desc.depth )
    {
        if( 1 == desc.faces )
        {
            return GN::gfx::XENON_TEXDIM_1D;
        }
        else
        {
            // Xenon don't have 1D array texture, so we use 2D array texture instead.
            return GN::gfx::XENON_TEXDIM_2D_ARRAY;
        }
    }
    else if( 1 == desc.depth )
    {
        if( desc.width == desc.height && 6 == desc.faces )
            return GN::gfx::XENON_TEXDIM_CUBE;
        else if( desc.faces > 1 )
            return GN::gfx::XENON_TEXDIM_2D_ARRAY;
        else
            return GN::gfx::XENON_TEXDIM_2D;
    }
    else
    {
        return GN::gfx::XENON_TEXDIM_3D;
    }
}
/*
// convert garnet cube face to D3D tag
// -----------------------------------------------------------------------------
static D3DCUBEMAP_FACES sCubeFace2D3D( size_t face )
{
    static D3DCUBEMAP_FACES sTable[6] =
    {
        D3DCUBEMAP_FACE_POSITIVE_X,
        D3DCUBEMAP_FACE_NEGATIVE_X,
        D3DCUBEMAP_FACE_POSITIVE_Y,
        D3DCUBEMAP_FACE_NEGATIVE_Y,
        D3DCUBEMAP_FACE_POSITIVE_Z,
        D3DCUBEMAP_FACE_NEGATIVE_Z,
    };
    GN_ASSERT( face < 6 );
    return sTable[face];
}*/

// ****************************************************************************
//  init / quit functions
// ****************************************************************************

//
//
// ----------------------------------------------------------------------------
bool GN::gfx::XenonTexture::init( const TextureDesc & inputDesc )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::XenonTexture, () );

    if( !setDesc( inputDesc ) ) return failure();

    // Note: always use descriptor returned by getDesc(), since it may differ with inputDesc.
    const TextureDesc & texdesc = getDesc();

    // determine texture format
    mD3DFormat = (D3DFORMAT)colorFormat2XenonFormat( texdesc.format );
    if( D3DFMT_UNKNOWN == mD3DFormat )
    {
        GN_ERROR(sLogger)( "Fail to convert color format '%s' to D3DFORMAT.", texdesc.format.toString().cptr() );
        return failure();
    }

    // determine dimension
    mD3DDimension = sDetermineTextureDimension( texdesc );

    // determine texture usage
    mD3DUsage = (TextureUsage::FAST_CPU_WRITE == texdesc.usage ) ? D3DUSAGE_CPU_CACHED_MEMORY : 0;

    // create texture
    IDirect3DDevice9 & dev = getGpu().getDeviceInlined();
    switch( mD3DDimension )
    {
        case XENON_TEXDIM_1D:
        {
            IDirect3DLineTexture9 * tex1d;
            GN_DX_CHECK_RETURN(
                dev.CreateLineTexture( texdesc.width, texdesc.levels, mD3DUsage, mD3DFormat, 0, &tex1d, NULL ),
                failure() );
            mD3DTexture = tex1d;
            break;
        }

        case XENON_TEXDIM_2D:
        {
            if( 1 == texdesc.faces )
            {
                IDirect3DTexture9 * tex2d;
                GN_DX_CHECK_RETURN(
                    dev.CreateTexture( texdesc.width, texdesc.height, texdesc.levels, mD3DUsage, mD3DFormat, 0, &tex2d, NULL ),
                    failure() );
                mD3DTexture = tex2d;
            }
            else
            {
                IDirect3DArrayTexture9 * tex2d;
                GN_DX_CHECK_RETURN(
                    dev.CreateArrayTexture( texdesc.width, texdesc.height, texdesc.faces, texdesc.levels, mD3DUsage, mD3DFormat, 0, &tex2d, NULL ),
                    failure() );
                mD3DTexture = tex2d;
            }
            break;
        }

        case XENON_TEXDIM_3D:
        {
            IDirect3DVolumeTexture9 * tex3d;
            GN_DX_CHECK_RETURN(
                dev.CreateVolumeTexture( texdesc.width, texdesc.height, texdesc.depth, texdesc.levels, mD3DUsage, mD3DFormat, 0, &tex3d, NULL ),
                failure() );
            mD3DTexture = tex3d;
            break;
        }

        case XENON_TEXDIM_CUBE:
        {
            IDirect3DCubeTexture9 * texcube;
            GN_DX_CHECK_RETURN(
                dev.CreateCubeTexture( texdesc.width, texdesc.levels, mD3DUsage, mD3DFormat, 0, &texcube, NULL ),
                failure() );
            mD3DTexture = texcube;
            break;
        }

        default:
            GN_UNEXPECTED(); // invalid dimension
            return failure();
    }

    // calculate mipmap sizes
    XGTEXTURE_DESC xdesc;
    for( size_t i = 0; i < texdesc.levels; ++i )
    {
        XGGetTextureDesc( mD3DTexture, i, &xdesc );

        DWORD depth = ( D3DRTYPE_VOLUMETEXTURE == xdesc.ResourceType ) ? xdesc.Depth : 1;

        setMipSize( i, Vector3<uint32>(xdesc.Width, xdesc.Height, depth) );
    }

    // success
    return success();

    GN_UNGUARD;
}

//
//
// ----------------------------------------------------------------------------
void GN::gfx::XenonTexture::quit()
{
    GN_GUARD;

    // should not be bind to the device any more.
    GN_ASSERT( NULL == mD3DTexture || !mD3DTexture->IsSet( &getGpu().getDeviceInlined() ) );

    // Delete the D3D texture instance.
    safeRelease( mD3DTexture );

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// ****************************************************************************
// from class Texture
// ****************************************************************************

//
//
// ----------------------------------------------------------------------------
void GN::gfx::XenonTexture::sBindToDevice( IDirect3DDevice9 & dev, UINT stage, const XenonTexture * tex )
{
    if( tex )
    {
        GN_ASSERT( &dev == &tex->getGpu().getDeviceInlined() );
        dev.SetTexture( stage, tex->mD3DTexture );
        tex->mStage = stage;
    }
    else
    {
        dev.SetTexture( stage, NULL );
    }
}

//
//
// ----------------------------------------------------------------------------
void GN::gfx::XenonTexture::updateMipmap(
    size_t              face,
    size_t              level,
    const Box<uint32> * area,
    size_t              rowPitch,
    size_t              slicePitch,
    const void        * data,
    SurfaceUpdateFlag   flag )
{
    // check update parameters
    Box<uint32> clippedArea;
    if( !validateUpdateParameters( face, level, area, flag, clippedArea ) ) return;

    // prepare for update
    const TextureDesc & texdesc = getDesc();
    const ColorLayoutDesc & ld = texdesc.format.layoutDesc();
    size_t srcBlockRowPitch = rowPitch * ld.blockHeight;
    XGTEXTURE_DESC xdesc;
    XGGetTextureDesc( mD3DTexture, level, &xdesc );
    DWORD gpuformat = XGGetGpuFormat(xdesc.Format);
    DWORD updateFlag = 0;
    if( XGIsBorderTexture( mD3DTexture ) ) updateFlag |= XGTILE_BORDER;
    if( !XGIsPackedTexture( mD3DTexture ) ) updateFlag |= XGTILE_NONPACKED;
    POINT pt = { clippedArea.x, clippedArea.y };
    RECT  rc = { 0, 0, clippedArea.w, clippedArea.h };

    // unbind from device first
    IDirect3DDevice9 & dev = getGpu().getDeviceInlined();
    BOOL isSet = mD3DTexture->IsSet( &dev );
    if( isSet )
    {
        GN_ASSERT( mStage != NOT_BIND );

        dev.SetTexture( mStage, NULL );
    }

    // do update
    switch( mD3DDimension )
    {
        case XENON_TEXDIM_2D:
        {
            D3DTexture * tex2d = (D3DTexture *)mD3DTexture;
            D3DLOCKED_RECT lrc;
            tex2d->LockRect( level, &lrc, NULL, 0 );

            GN_ASSERT( XGIsTiledFormat( mD3DFormat ) );
            XGTileTextureLevel(
                xdesc.Width,      // Width,
                xdesc.Height,     // Height,
                level,            // Level,
                gpuformat,        // GpuFormat
                updateFlag,       // Flags,
                lrc.pBits,        // pDestination,
                &pt,              // pPoint,
                data,             // pSource,
                srcBlockRowPitch, // RowPitch,
                &rc );            // pRect

            tex2d->UnlockRect( level );
            break;
        }

        case XENON_TEXDIM_1D:
        case XENON_TEXDIM_2D_ARRAY:
        case XENON_TEXDIM_3D:
        case XENON_TEXDIM_CUBE:
            GN_UNIMPL();
            break;

        default:
            GN_UNEXPECTED();
            break;
    }

    // rebind to texture
    if( isSet )
    {
        GN_ASSERT( mStage != NOT_BIND );
        dev.SetTexture( mStage, mD3DTexture );
    }
}
