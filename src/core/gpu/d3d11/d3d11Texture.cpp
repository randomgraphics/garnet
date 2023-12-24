#include "pch.h"
#include "d3d11Gpu.h"
#include "d3d11Texture.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx.gpu.D3D11");

// *****************************************************************************
// local functions
// *****************************************************************************

using namespace GN;
using namespace GN::gfx;

//
// Determine depth reading format from typeless format
// ----------------------------------------------------------------------------
static DXGI_FORMAT sGetDepthReadingFormat(DXGI_FORMAT format) {
    switch (format) {
    case DXGI_FORMAT_R32_TYPELESS:
        return DXGI_FORMAT_R32_FLOAT;
    case DXGI_FORMAT_R32G8X24_TYPELESS:
        return DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS;
    case DXGI_FORMAT_R24G8_TYPELESS:
        return DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
    case DXGI_FORMAT_R16_TYPELESS:
        return DXGI_FORMAT_R16_UNORM;
    default:
        GN_ERROR(sLogger)("Format %s is not a valid typeless depth buffer format.", dxgi::getDXGIFormatDesc(format).name);
        return DXGI_FORMAT_UNKNOWN;
    }
}

//
// Determine depth writing format from typeless format
// ----------------------------------------------------------------------------
static DXGI_FORMAT sGetDepthWritingFormat(DXGI_FORMAT format) {
    switch (format) {
    case DXGI_FORMAT_R32_TYPELESS:
        return DXGI_FORMAT_D32_FLOAT;
    case DXGI_FORMAT_R32G8X24_TYPELESS:
        return DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
    case DXGI_FORMAT_R24G8_TYPELESS:
        return DXGI_FORMAT_D24_UNORM_S8_UINT;
    case DXGI_FORMAT_R16_TYPELESS:
        return DXGI_FORMAT_D16_UNORM;
    default:
        GN_ERROR(sLogger)("Format %s is not a valid depth buffer format.", dxgi::getDXGIFormatDesc(format).name);
        return DXGI_FORMAT_UNKNOWN;
    }
}

//
// Determine texture dimension
// ----------------------------------------------------------------------------
static D3D11_SRV_DIMENSION sDetermineTextureDimension(const GN::gfx::TextureDesc & desc) {
    if (1 == desc.height && 1 == desc.depth) {
        return desc.faces > 1 ? D3D11_SRV_DIMENSION_TEXTURE1DARRAY : D3D11_SRV_DIMENSION_TEXTURE1D;
    } else if (1 == desc.depth) {
        if (6 == desc.faces && desc.width == desc.height) {
            return D3D11_SRV_DIMENSION_TEXTURECUBE;
        } else if (desc.faces > 1) {
            return D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
        } else {
            return D3D11_SRV_DIMENSION_TEXTURE2D;
        }
    } else {
        return D3D11_SRV_DIMENSION_TEXTURE3D;
    }
}

// ****************************************************************************
//  init / quit functions
// ****************************************************************************

//
//
// ----------------------------------------------------------------------------
bool GN::gfx::D3D11Texture::init(const TextureDesc & desc) {
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT();

    // create device data
    if (!setDesc(desc) || !createTexture()) return failure();

    // success
    return success();

    GN_UNGUARD;
}

//
//
// ----------------------------------------------------------------------------
void GN::gfx::D3D11Texture::quit() {
    GN_GUARD;

    mSRViews.clear();
    mRTViews.clear();
    mDSViews.clear();
    safeRelease(mTexture);

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// ****************************************************************************
// from Texture
// ****************************************************************************

//
//
// ----------------------------------------------------------------------------
void GN::gfx::D3D11Texture::updateMipmap(uint32 face, uint32 level, const Box<uint32> * area, uint32 rowPitch, uint32 slicePitch, const void * data,
                                         SurfaceUpdateFlag flag) {
    Box<uint32> clippedArea;
    if (!validateUpdateParameters(face, level, area, flag, clippedArea)) return;

    const TextureDesc & texdesc = getDesc();

    if (TextureUsage::FAST_CPU_WRITE == texdesc.usage) {
        GN_TODO("Updating dynamic texture is not implemented yet.");
    } else {
        const dxgi::DXGI_FORMAT_DESCRIPTION & fmtdesc = dxgi::getDXGIFormatDesc(mTextureFormat);

        // area must be aligned to texel blocks
        if (0 != (clippedArea.x % fmtdesc.blockWidth) || 0 != (clippedArea.y % fmtdesc.blockHeight)) {
            GN_ERROR(sLogger)("For block-compressed texture, \"area\" parameter must be aligned to texel block boundary.");
            return;
        }
        clippedArea.w = math::alignToPowerOf2(clippedArea.w, fmtdesc.blockWidth);
        clippedArea.h = math::alignToPowerOf2(clippedArea.h, fmtdesc.blockHeight);

        D3D11_BOX box = {
            clippedArea.x, clippedArea.y, clippedArea.z, clippedArea.x + clippedArea.w, clippedArea.y + clippedArea.h, clippedArea.z + clippedArea.d,
        };

        getDeviceContextRef().UpdateSubresource(mTexture, D3D11CalcSubresource((uint32) level, (uint32) face, texdesc.levels), &box, data,
                                                (uint32) (rowPitch * fmtdesc.blockHeight), (uint32) slicePitch);
    }
}

//
//
// ----------------------------------------------------------------------------
void GN::gfx::D3D11Texture::readMipmap(uint32 /*face*/, uint32 /*level*/, MipmapData & /*data*/) { GN_UNIMPL(); }

// ****************************************************************************
// public methods
// ****************************************************************************

//
//
// ----------------------------------------------------------------------------
ID3D11ShaderResourceView * GN::gfx::D3D11Texture::getSRView(DXGI_FORMAT format, uint32 firstFace, uint32 numFaces, uint32 firstMipLevel, uint32 numLevels,
                                                            uint32 firstSlice, uint32 numSlices) {
    D3D11_SHADER_RESOURCE_VIEW_DESC srvdesc;
    memset(&srvdesc, 0, sizeof(srvdesc));
    srvdesc.Format        = format;
    srvdesc.ViewDimension = mDimension;

    switch (mDimension) {
    case D3D11_SRV_DIMENSION_BUFFER:
        GN_UNIMPL();
        break;

    case D3D11_SRV_DIMENSION_TEXTURE1D:
        srvdesc.Texture1D.MostDetailedMip = firstMipLevel;
        srvdesc.Texture1D.MipLevels       = numLevels;
        break;

    case D3D11_SRV_DIMENSION_TEXTURE1DARRAY:
        srvdesc.Texture1DArray.FirstArraySlice = firstFace;
        srvdesc.Texture1DArray.ArraySize       = numFaces;
        srvdesc.Texture1DArray.MostDetailedMip = firstMipLevel;
        srvdesc.Texture1DArray.MipLevels       = numLevels;
        break;

    case D3D11_SRV_DIMENSION_TEXTURE2D:
        srvdesc.Texture2D.MostDetailedMip = firstMipLevel;
        srvdesc.Texture2D.MipLevels       = numLevels;
        break;

    case D3D11_SRV_DIMENSION_TEXTURE2DARRAY:
        srvdesc.Texture2DArray.FirstArraySlice = firstFace;
        srvdesc.Texture2DArray.ArraySize       = numFaces;
        srvdesc.Texture2DArray.MostDetailedMip = firstMipLevel;
        srvdesc.Texture2DArray.MipLevels       = numLevels;
        break;

    case D3D11_SRV_DIMENSION_TEXTURE2DMS:
        // do nothing
        break;

    case D3D11_SRV_DIMENSION_TEXTURE2DMSARRAY:
        // do nothing
        srvdesc.Texture2DMSArray.FirstArraySlice = firstFace;
        srvdesc.Texture2DMSArray.ArraySize       = numFaces;
        break;

    case D3D11_SRV_DIMENSION_TEXTURE3D:
        srvdesc.Texture3D.MostDetailedMip = firstMipLevel;
        srvdesc.Texture3D.MipLevels       = numLevels;
        break;

    case D3D11_SRV_DIMENSION_TEXTURECUBE:
        srvdesc.TextureCube.MostDetailedMip = firstMipLevel;
        srvdesc.TextureCube.MipLevels       = numLevels;
        break;

    default:
        GN_UNEXPECTED();
        return NULL;
    }

    AutoComPtr<ID3D11ShaderResourceView> & srv = mSRViews[srvdesc];

    if (srv.empty()) {
        ID3D11Device & dev = getDeviceRef();

        GN_DX_CHECK_RETURN(dev.CreateShaderResourceView(mTexture, &srvdesc, &srv), NULL);
    }

    return srv;
}

//
//
// ----------------------------------------------------------------------------
ID3D11RenderTargetView * GN::gfx::D3D11Texture::getRTView(uint32 face, uint32 level, uint32 slice) {
    // must be a color render target texture
    GN_ASSERT(TextureUsage::COLOR_RENDER_TARGET == getDesc().usage);

    D3D11_RENDER_TARGET_VIEW_DESC rtvdesc;
    memset(&rtvdesc, 0, sizeof(rtvdesc));
    rtvdesc.Format = mWritingFormat;

    switch (mDimension) {
    case D3D11_SRV_DIMENSION_BUFFER:
        rtvdesc.ViewDimension = D3D11_RTV_DIMENSION_BUFFER;
        GN_UNIMPL();
        return NULL;

    case D3D11_SRV_DIMENSION_TEXTURE1D:
        if (0 != face || 0 != slice) {
            GN_ERROR(sLogger)("face or slice is out of range.");
            return NULL;
        }
        rtvdesc.ViewDimension      = D3D11_RTV_DIMENSION_TEXTURE1D;
        rtvdesc.Texture1D.MipSlice = level;
        break;

    case D3D11_SRV_DIMENSION_TEXTURE1DARRAY:
        if (0 != slice) {
            GN_ERROR(sLogger)("slice is out of range.");
            return NULL;
        }
        rtvdesc.ViewDimension                  = D3D11_RTV_DIMENSION_TEXTURE1DARRAY;
        rtvdesc.Texture1DArray.FirstArraySlice = face;
        rtvdesc.Texture1DArray.ArraySize       = 1;
        rtvdesc.Texture1DArray.MipSlice        = level;
        break;

    case D3D11_SRV_DIMENSION_TEXTURE2D:
        if (0 != face || 0 != slice) {
            GN_ERROR(sLogger)("face or slice is out of range.");
            return NULL;
        }
        rtvdesc.ViewDimension      = D3D11_RTV_DIMENSION_TEXTURE2D;
        rtvdesc.Texture2D.MipSlice = level;
        break;

    case D3D11_SRV_DIMENSION_TEXTURE2DARRAY:
    case D3D11_SRV_DIMENSION_TEXTURECUBE:
        if (0 != slice) {
            GN_ERROR(sLogger)("slice is out of range.");
            return NULL;
        }
        rtvdesc.ViewDimension                  = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
        rtvdesc.Texture2DArray.FirstArraySlice = face;
        rtvdesc.Texture2DArray.ArraySize       = 1;
        rtvdesc.Texture2DArray.MipSlice        = level;
        break;

    case D3D11_SRV_DIMENSION_TEXTURE2DMS:
    case D3D11_SRV_DIMENSION_TEXTURE2DMSARRAY:
        GN_UNIMPL();
        return NULL;

    case D3D11_SRV_DIMENSION_TEXTURE3D:
        if (0 != face) {
            GN_ERROR(sLogger)("face is out of range.");
            return NULL;
        }
        rtvdesc.ViewDimension         = D3D11_RTV_DIMENSION_TEXTURE3D;
        rtvdesc.Texture3D.MipSlice    = level;
        rtvdesc.Texture3D.FirstWSlice = slice;
        rtvdesc.Texture3D.WSize       = 1;
        break;

    default:
        GN_UNEXPECTED();
        return NULL;
    }

    AutoComPtr<ID3D11RenderTargetView> & rtv = mRTViews[rtvdesc];

    if (rtv.empty()) {
        ID3D11Device & dev = getDeviceRef();

        GN_DX_CHECK_RETURN(dev.CreateRenderTargetView(mTexture, &rtvdesc, &rtv), NULL);
    }

    return rtv;
}

//
//
// ----------------------------------------------------------------------------
ID3D11DepthStencilView * GN::gfx::D3D11Texture::getDSView(uint32 face, uint32 level, uint32 slice) {
    // must be a depth texture
    GN_ASSERT(TextureUsage::DEPTH_RENDER_TARGET == getDesc().usage);

    D3D11_DEPTH_STENCIL_VIEW_DESC dsvdesc;
    memset(&dsvdesc, 0, sizeof(dsvdesc));
    dsvdesc.Format = mWritingFormat;

    switch (mDimension) {
    case D3D11_SRV_DIMENSION_BUFFER:
        GN_ERROR(sLogger)("A D3D11Buffer cannot be used as depth buffer.");
        return NULL;

    case D3D11_SRV_DIMENSION_TEXTURE1D:
        if (0 != face || 0 != slice) {
            GN_ERROR(sLogger)("face or slice is out of range.");
            return NULL;
        }
        dsvdesc.ViewDimension      = D3D11_DSV_DIMENSION_TEXTURE1D;
        dsvdesc.Texture1D.MipSlice = level;
        break;

    case D3D11_SRV_DIMENSION_TEXTURE1DARRAY:
        if (0 != slice) {
            GN_ERROR(sLogger)("slice is out of range.");
            return NULL;
        }
        dsvdesc.ViewDimension                  = D3D11_DSV_DIMENSION_TEXTURE1DARRAY;
        dsvdesc.Texture1DArray.FirstArraySlice = face;
        dsvdesc.Texture1DArray.ArraySize       = 1;
        dsvdesc.Texture1DArray.MipSlice        = level;
        break;

    case D3D11_SRV_DIMENSION_TEXTURE2D:
        if (0 != face || 0 != slice) {
            GN_ERROR(sLogger)("face or slice is out of range.");
            return NULL;
        }
        dsvdesc.ViewDimension      = D3D11_DSV_DIMENSION_TEXTURE2D;
        dsvdesc.Texture2D.MipSlice = level;
        break;

    case D3D11_SRV_DIMENSION_TEXTURE2DARRAY:
    case D3D11_SRV_DIMENSION_TEXTURECUBE:
        if (0 != slice) {
            GN_ERROR(sLogger)("slice is out of range.");
            return NULL;
        }
        dsvdesc.ViewDimension                  = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
        dsvdesc.Texture2DArray.FirstArraySlice = face;
        dsvdesc.Texture2DArray.ArraySize       = 1;
        dsvdesc.Texture2DArray.MipSlice        = level;
        break;

    case D3D11_SRV_DIMENSION_TEXTURE2DMS:
    case D3D11_SRV_DIMENSION_TEXTURE2DMSARRAY:
        GN_UNIMPL();
        return NULL;

    case D3D11_SRV_DIMENSION_TEXTURE3D:
        GN_ERROR(sLogger)("A 3D texture cannot be used as depth buffer.");
        return NULL;

    default:
        GN_UNEXPECTED();
        return NULL;
    }

    AutoComPtr<ID3D11DepthStencilView> & dsv = mDSViews[dsvdesc];

    if (dsv.empty()) {
        ID3D11Device & dev = getDeviceRef();

        GN_DX_CHECK_RETURN(dev.CreateDepthStencilView(mTexture, &dsvdesc, &dsv), NULL);
    }

    return dsv;
}

// ****************************************************************************
//      private functions
// ****************************************************************************

//
//
// ----------------------------------------------------------------------------
bool GN::gfx::D3D11Texture::createTexture() {
    GN_GUARD;

    GN_ASSERT(!mTexture);

    const TextureDesc & texdesc = getDesc();

    // determine texture formats
    if (TextureUsage::DEPTH_RENDER_TARGET == texdesc.usage) {
        // special case for depth texture

        mTextureFormat = dxgi::getDXGIFormatDesc((DXGI_FORMAT) texdesc.format.toDXGI()).typelessFormat;
        mReadingFormat = sGetDepthReadingFormat(mTextureFormat);
        mWritingFormat = sGetDepthWritingFormat(mTextureFormat);
        if (DXGI_FORMAT_UNKNOWN == mTextureFormat || DXGI_FORMAT_UNKNOWN == mReadingFormat || DXGI_FORMAT_UNKNOWN == mWritingFormat) {
            GN_ERROR(sLogger)("Fail to determine depth texture format.");
            return false;
        }
    } else {
        mReadingFormat = (DXGI_FORMAT) texdesc.format.toDXGI();
        mTextureFormat = mReadingFormat;
        mWritingFormat = mReadingFormat;
        if (DXGI_FORMAT_UNKNOWN == mReadingFormat) {
            GN_ERROR(sLogger)("Fail to determine texture format.");
            return false;
        }
    }

    // determine usage, CPU access and binding flags
    D3D11_USAGE usage = D3D11_USAGE_DEFAULT;
    UINT        caf   = 0;
    UINT        bf    = D3D11_BIND_SHADER_RESOURCE;
    switch (texdesc.usage) {
    case TextureUsage::DEFAULT:
        // nothing to do
        break;

    case TextureUsage::COLOR_RENDER_TARGET:
        bf |= D3D11_BIND_RENDER_TARGET;
        break;

    case TextureUsage::DEPTH_RENDER_TARGET:
        bf |= D3D11_BIND_DEPTH_STENCIL;
        break;

    case TextureUsage::FAST_CPU_WRITE:
        usage = D3D11_USAGE_DYNAMIC;
        caf   = D3D11_CPU_ACCESS_WRITE;
        break;

    default:
        GN_ERROR(sLogger)("Invalid texture usage enumeration.");
        return false;
    }

    // determine texture dimension
    mDimension = sDetermineTextureDimension(texdesc);

    // determine cube flag
    UINT mf = 0;
    if (D3D11_SRV_DIMENSION_TEXTURECUBE == mDimension) mf |= D3D11_RESOURCE_MISC_TEXTURECUBE;

    // create texture instance
    ID3D11Device & dev = getDeviceRef();
    if (D3D11_SRV_DIMENSION_TEXTURE1D == mDimension || D3D11_SRV_DIMENSION_TEXTURE1DARRAY == mDimension) {
        ID3D11Texture1D *    tex1d;
        D3D11_TEXTURE1D_DESC desc1d;
        desc1d.Width          = texdesc.width;
        desc1d.MipLevels      = texdesc.levels;
        desc1d.ArraySize      = texdesc.faces;
        desc1d.Format         = mTextureFormat;
        desc1d.Usage          = usage;
        desc1d.BindFlags      = bf;
        desc1d.CPUAccessFlags = caf;
        desc1d.MiscFlags      = mf;
        GN_DX_CHECK_RETURN(dev.CreateTexture1D(&desc1d, 0, &tex1d), false);
        mTexture = tex1d;
    } else if (D3D11_SRV_DIMENSION_TEXTURE2D == mDimension || D3D11_SRV_DIMENSION_TEXTURE2DARRAY == mDimension ||
               D3D11_SRV_DIMENSION_TEXTURECUBE == mDimension) {
        ID3D11Texture2D *    tex2d;
        D3D11_TEXTURE2D_DESC desc2d;
        desc2d.Width              = texdesc.width;
        desc2d.Height             = texdesc.height;
        desc2d.MipLevels          = texdesc.levels;
        desc2d.ArraySize          = texdesc.faces;
        desc2d.Format             = mTextureFormat;
        desc2d.SampleDesc.Count   = 1;
        desc2d.SampleDesc.Quality = 0;
        desc2d.Usage              = usage;
        desc2d.BindFlags          = bf;
        desc2d.CPUAccessFlags     = caf;
        desc2d.MiscFlags          = mf;
        GN_DX_CHECK_RETURN(dev.CreateTexture2D(&desc2d, 0, &tex2d), false);
        mTexture = tex2d;
    } else if (D3D11_SRV_DIMENSION_TEXTURE3D == mDimension) {
        ID3D11Texture3D *    tex3d;
        D3D11_TEXTURE3D_DESC desc3d;
        desc3d.Width          = texdesc.width;
        desc3d.Height         = texdesc.height;
        desc3d.Depth          = texdesc.depth;
        desc3d.MipLevels      = texdesc.levels;
        desc3d.Format         = mTextureFormat;
        desc3d.Usage          = usage;
        desc3d.BindFlags      = bf;
        desc3d.CPUAccessFlags = caf;
        desc3d.MiscFlags      = mf;
        GN_DX_CHECK_RETURN(dev.CreateTexture3D(&desc3d, 0, &tex3d), false);
        mTexture = tex3d;
    } else {
        GN_ERROR(sLogger)("Invalid texture dimension: %d", mDimension);
        GN_UNEXPECTED();
        return false;
    }

    // calculate mipmap sizes
    Vector3<uint32> mipSize(texdesc.width, texdesc.height, texdesc.depth);
    for (uint32 i = 0; i < texdesc.levels; ++i) {
        setMipSize(i, mipSize);
        if (mipSize.x > 1) mipSize.x >>= 1;
        if (mipSize.y > 1) mipSize.y >>= 1;
        if (mipSize.z > 1) mipSize.z >>= 1;
    }

    // success
    return true;

    GN_UNGUARD;
}
