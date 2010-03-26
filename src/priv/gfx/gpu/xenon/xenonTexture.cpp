#include "pch.h"
#include "xenonGpu.h"
#include "xenonTexture.h"

static GN::Logger * sLogger = GN::GetLogger("GN.gfx.gpu.xenon");

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
bool GN::gfx::XenonTexture::Init( const TextureDesc & inputDesc )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::XenonTexture, () );

    if( !SetDesc( inputDesc ) ) return Failure();

    // Note: always use descriptor returned by GetDesc(), since it may differ with inputDesc.
    const TextureDesc & desc = GetDesc();

    // determine texture format
    mD3DFormat = (D3DFORMAT)ColorFormat2XenonFormat( desc.format );
    if( D3DFMT_UNKNOWN == mD3DFormat )
    {
        GN_ERROR(sLogger)( "Fail to convert color format '%s' to D3DFORMAT.", desc.format.ToString().ToRawPtr() );
        return Failure();
    }

    // determine dimension
    mD3DDimension = sDetermineTextureDimension( desc );

    // determine texture usage
    mD3DUsage = (TextureUsage::FAST_CPU_WRITE == desc.usage ) ? D3DUSAGE_CPU_CACHED_MEMORY : 0;

    // create texture
    IDirect3DDevice9 & dev = GetGpu().getDeviceInlined();
    switch( mD3DDimension )
    {
        case XENON_TEXDIM_1D:
        {
            IDirect3DLineTexture9 * tex1d;
            GN_DX_CHECK_RETURN(
                dev.CreateLineTexture( desc.width, desc.levels, mD3DUsage, mD3DFormat, 0, &tex1d, NULL ),
                Failure() );
            mD3DTexture = tex1d;
            break;
        }

        case XENON_TEXDIM_2D:
        {
            if( 1 == desc.faces )
            {
                IDirect3DTexture9 * tex2d;
                GN_DX_CHECK_RETURN(
                    dev.CreateTexture( desc.width, desc.height, desc.levels, mD3DUsage, mD3DFormat, 0, &tex2d, NULL ),
                    Failure() );
                mD3DTexture = tex2d;
            }
            else
            {
                IDirect3DArrayTexture9 * tex2d;
                GN_DX_CHECK_RETURN(
                    dev.CreateArrayTexture( desc.width, desc.height, desc.faces, desc.levels, mD3DUsage, mD3DFormat, 0, &tex2d, NULL ),
                    Failure() );
                mD3DTexture = tex2d;
            }
            break;
        }

        case XENON_TEXDIM_3D:
        {
            IDirect3DVolumeTexture9 * tex3d;
            GN_DX_CHECK_RETURN(
                dev.CreateVolumeTexture( desc.width, desc.height, desc.depth, desc.levels, mD3DUsage, mD3DFormat, 0, &tex3d, NULL ),
                Failure() );
            mD3DTexture = tex3d;
            break;
        }

        case XENON_TEXDIM_CUBE:
        {
            IDirect3DCubeTexture9 * texcube;
            GN_DX_CHECK_RETURN(
                dev.CreateCubeTexture( desc.width, desc.levels, mD3DUsage, mD3DFormat, 0, &texcube, NULL ),
                Failure() );
            mD3DTexture = texcube;
            break;
        }

        default:
            GN_UNEXPECTED(); // invalid dimension
            return Failure();
    }

    // calculate mipmap sizes
    XGTEXTURE_DESC xdesc;
    for( size_t i = 0; i < desc.levels; ++i )
    {
        XGGetTextureDesc( mD3DTexture, i, &xdesc );

        DWORD depth = ( D3DRTYPE_VOLUMETEXTURE == xdesc.ResourceType ) ? xdesc.Depth : 1;

        SetMipSize( i, Vector3<UInt32>(xdesc.Width, xdesc.Height, depth) );
    }

    // success
    return Success();

    GN_UNGUARD;
}

//
//
// ----------------------------------------------------------------------------
void GN::gfx::XenonTexture::Quit()
{
    GN_GUARD;

    SafeRelease( mD3DTexture );

    // standard Quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// ****************************************************************************
// from class Texture
// ****************************************************************************

//
//
// ----------------------------------------------------------------------------
void GN::gfx::XenonTexture::UpdateMipmap(
    size_t              face,
    size_t              level,
    const Box<UInt32> * area,
    size_t              rowPitch,
    size_t              slicePitch,
    const void        * data,
    SurfaceUpdateFlag   flag )
{
    // check update parameters
    Box<UInt32> clippedArea;
    if( !validateUpdateParameters( face, level, area, flag, clippedArea ) ) return;

    // prepare for update
    const TextureDesc & desc = GetDesc();
    const ColorLayoutDesc & ld = desc.format.GetLayoutDesc();
    size_t srcBlockRowPitch = rowPitch * ld.blockHeight;
    XGTEXTURE_DESC xdesc;
    XGGetTextureDesc( mD3DTexture, level, &xdesc );
    DWORD gpuformat = XGGetGpuFormat(xdesc.Format);
    DWORD updateFlag = 0;
    if( XGIsBorderTexture( mD3DTexture ) ) updateFlag |= XGTILE_BORDER;
    if( !XGIsPackedTexture( mD3DTexture ) ) updateFlag |= XGTILE_NONPACKED;
    POINT pt = { clippedArea.x, clippedArea.y };
    RECT  rc = { 0, 0, clippedArea.w, clippedArea.h };

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
            return;

        default:
            GN_UNEXPECTED();
            return;
    };
}
