#include "pch.h"
#include "d3d10Renderer.h"

// *****************************************************************************
// init/quit
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D10Renderer::capsInit()
{
    GN_GUARD;

    // clear all caps
    memset( &mCaps, 0, sizeof(mCaps) );

    // max texture size
    mCaps.maxTex1DSize[0] = D3D10_REQ_TEXTURE1D_U_DIMENSION;
    mCaps.maxTex1DSize[1] = D3D10_REQ_TEXTURE1D_ARRAY_AXIS_DIMENSION;

    mCaps.maxTex2DSize[0] =
    mCaps.maxTex2DSize[1] = D3D10_REQ_TEXTURE2D_U_OR_V_DIMENSION;
    mCaps.maxTex2DSize[2] = D3D10_REQ_TEXTURE2D_ARRAY_AXIS_DIMENSION;

    mCaps.maxTex3DSize[0] =
    mCaps.maxTex3DSize[1] = D3D10_REQ_TEXTURE3D_U_V_OR_W_DIMENSION;
    mCaps.maxTex3DSize[2] = 1;

    // max simultaneous textures
    mCaps.maxTextures     = math::getmin<size_t>( D3D10_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, RendererContext::MAX_TEXTURES );

    // max simultaneous render targets
    mCaps.maxColorRenderTargets = math::getmin<size_t>( D3D10_SIMULTANEOUS_RENDER_TARGET_COUNT, RendererContext::MAX_COLOR_RENDER_TARGETS );

    // shader caps
    mCaps.vsProfiles |= RendererCaps::GPP_D3D_2_0;
    mCaps.vsProfiles |= RendererCaps::GPP_D3D_3_0;
    mCaps.vsProfiles |= RendererCaps::GPP_D3D_4_0;

    mCaps.gsProfiles |= RendererCaps::GPP_D3D_4_0;

    mCaps.psProfiles |= RendererCaps::GPP_D3D_2_0;
    mCaps.psProfiles |= RendererCaps::GPP_D3D_3_0;
    mCaps.psProfiles |= RendererCaps::GPP_D3D_4_0;

    // success
    return true;

    GN_UNGUARD;
}

// *****************************************************************************
// from Renderer
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D10Renderer::checkTextureFormatSupport(
    ColorFormat   format,
    TextureUsages usages ) const
{
    DXGI_FORMAT d3dfmt = (DXGI_FORMAT)colorFormat2DxgiFormat( format );
    if( DXGI_FORMAT_UNKNOWN == d3dfmt ) return false;

    UINT d3dUsages;
    if( FAILED( mDevice->CheckFormatSupport( d3dfmt, &d3dUsages ) ) ) return false;

    // TODO: check d3dUsages against input type and usage.
    return true;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::ColorFormat
GN::gfx::D3D10Renderer::getDefaultTextureFormat( TextureUsages usages ) const
{
    if( usages.depth )
    {
        return ColorFormat::RG_24_UNORM_8_UINT;
    }
    else
    {
        return ColorFormat::RGBA_8_8_8_8_UNORM;
    }
}
