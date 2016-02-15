#include "pch.h"
#include "xenonGpu.h"

// *****************************************************************************
// init / quit
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::XenonGpu::capsInit()
{
    GN_GUARD;

    ZeroMemory( &mCaps, sizeof(mCaps) );

    mCaps.maxTex1DSize[0] = 16777216;
    mCaps.maxTex1DSize[1] = 1;

    mCaps.maxTex2DSize[0] = 8192;
    mCaps.maxTex2DSize[1] = 8192;
    mCaps.maxTex2DSize[2] = 64;

    mCaps.maxTex3DSize[0] = 2048;
    mCaps.maxTex3DSize[1] = 2048;
    mCaps.maxTex3DSize[2] = 1024;
    mCaps.maxTex3DSize[3] = 1;

    mCaps.maxTextures = GPU_D3D_PIXEL_TEXTURE_FETCH_CONSTANT_COUNT;
    mCaps.maxColorRenderTargets = 4;

    mCaps.shaderModels = ShaderModel::SM_2_0 | ShaderModel::SM_3_0 | ShaderModel::SM_3_X;

    mCaps.cg = false;

    // successful
    return true;

    GN_UNGUARD;
}

// *****************************************************************************
// from Gpu
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::XenonGpu::checkTextureFormatSupport(
    ColorFormat   format,
    TextureUsage usages ) const
{
    GN_GUARD;

    GN_ASSERT( isGpuThread() );

    GN_UNIMPL_WARNING()

    GN_UNUSED_PARAM( format );
    GN_UNUSED_PARAM( usages );

    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::ColorFormat
GN::gfx::XenonGpu::getDefaultTextureFormat( TextureUsage usage ) const
{
    GN_ASSERT( isGpuThread() );

    if( TextureUsage::DEPTH_RENDER_TARGET == usage )
    {
        return ColorFormat::RG_24_FLOAT_8_UINT;
    }
    else
    {
        return ColorFormat::BGRA_8_8_8_8_UNORM;
    }
}
