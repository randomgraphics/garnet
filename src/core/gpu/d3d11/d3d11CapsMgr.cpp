#include "pch.h"
#include "d3d11Gpu.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx.gpu.D3D11");

// *****************************************************************************
// init/quit
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D11Gpu::capsInit() {
    GN_GUARD;

    // clear all caps
    memset(&mCaps, 0, sizeof(mCaps));

    D3D_FEATURE_LEVEL feature = mDevice->GetFeatureLevel();

    // max texture size
    mCaps.maxTex1DSize[0] = D3D11_REQ_TEXTURE1D_U_DIMENSION;
    mCaps.maxTex1DSize[1] = D3D11_REQ_TEXTURE1D_ARRAY_AXIS_DIMENSION;

    mCaps.maxTex2DSize[0] = mCaps.maxTex2DSize[1] = D3D11_REQ_TEXTURE2D_U_OR_V_DIMENSION;
    mCaps.maxTex2DSize[2]                         = D3D11_REQ_TEXTURE2D_ARRAY_AXIS_DIMENSION;

    mCaps.maxTex3DSize[0] = mCaps.maxTex3DSize[1] = D3D11_REQ_TEXTURE3D_U_V_OR_W_DIMENSION;
    mCaps.maxTex3DSize[2]                         = 1;

    // max simultaneous textures
    mCaps.maxTextures = math::getmin<uint32>(D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, GpuContext::MAX_TEXTURES);

    // max simultaneous render targets
    mCaps.maxColorRenderTargets = math::getmin<uint32>(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, GpuContext::MAX_COLOR_RENDER_TARGETS);

    // shader caps
    if (feature >= D3D_FEATURE_LEVEL_9_1) mCaps.shaderModels |= ShaderModel::SM_2_0;
    if (feature >= D3D_FEATURE_LEVEL_10_0) mCaps.shaderModels |= ShaderModel::SM_3_0 | ShaderModel::SM_4_0;
    if (feature >= D3D_FEATURE_LEVEL_11_0) mCaps.shaderModels |= ShaderModel::SM_5_0;

    GN_INFO(sLogger)
    ("\n\n"
     "===================================================\n"
     "         D3D11 Implementation Informations\n"
     "---------------------------------------------------\n"
     "   Feature Level   : %u.%u\n"
     "===================================================\n"
     "\n\n",
     feature >> 12, (feature >> 8) & 0xF);

    // success
    return true;

    GN_UNGUARD;
}

// *****************************************************************************
// from Gpu
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D11Gpu::checkTextureFormatSupport(PixelFormat format, TextureUsage usage) const {
    DXGI_FORMAT d3dfmt = (DXGI_FORMAT) colorFormat2DxgiFormat(format);
    if (DXGI_FORMAT_UNKNOWN == d3dfmt) return false;

    UINT formatSupport;
    if (FAILED(mDevice->CheckFormatSupport(d3dfmt, &formatSupport))) return false;

    switch (usage) {
    case TextureUsage::DEFAULT:
        if (0 == (D3D11_FORMAT_SUPPORT_SHADER_SAMPLE & formatSupport)) { return false; }
        break;

    case TextureUsage::COLOR_RENDER_TARGET:
        if (0 == (D3D11_FORMAT_SUPPORT_RENDER_TARGET & formatSupport)) { return false; }
        break;

    case TextureUsage::DEPTH_RENDER_TARGET:
        if (0 == (D3D11_FORMAT_SUPPORT_DEPTH_STENCIL & formatSupport)) { return false; }

    case TextureUsage::FAST_CPU_WRITE:
        if (0 == (D3D11_FORMAT_SUPPORT_CPU_LOCKABLE & formatSupport)) { return false; }
        break;

    default:
        GN_ERROR(sLogger)("Invalid texture usage.");
        return false;
    }

    return true;
}
