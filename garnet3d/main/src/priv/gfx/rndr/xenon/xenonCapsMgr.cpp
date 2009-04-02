#include "pch.h"
#include "xenonRenderer.h"

// *****************************************************************************
// init / quit
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::XenonRenderer::capsInit()
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    mCaps.maxTex1DSize[0] = 16777216;
    mCaps.maxTex1DSize[1] = 1;
    
    mCaps.maxTex2DSize[0] = 8192;
    mCaps.maxTex2DSize[1] = 8192;
    mCaps.maxTex2DSize[2] = 64;

    mCaps.maxTex3DSize[0] = 2048;
    mCaps.maxTex3DSize[1] = 2048;
    mCaps.maxTex3DSize[2] = 1024;
    mCaps.maxTex3DSize[3] = 1;

    mCaps.maxTextures = 26;
    mCaps.maxColorRenderTargets = 4;
    mCaps.vsProfiles = RendererCaps::GPP_XVS_3_0 | RendererCaps::GPP_D3D_3_0;
    mCaps.gsProfiles = 0;
    mCaps.psProfiles = RendererCaps::GPP_XPS_3_0 | RendererCaps::GPP_D3D_3_0;

    // successful
    return true;

    GN_UNGUARD;
}

// *****************************************************************************
// from Renderer
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::XenonRenderer::checkTextureFormatSupport(
    ColorFormat   format,
    TextureUsages usages ) const
{
    GN_GUARD;

    GN_ASSERT( getCurrentThreadId() == mThreadId );

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
GN::gfx::XenonRenderer::getDefaultTextureFormat( TextureUsages usages ) const
{
    GN_ASSERT( getCurrentThreadId() == mThreadId );

    if( usages.depth )
    {
        return COLOR_FORMAT_RG_24_FLOAT_8_UINT;
    }
    else
    {
        return COLOR_FORMAT_BGRA_8_8_8_8_UNORM;
    }
}
