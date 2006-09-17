#include "pch.h"
#include "d3d10Renderer.h"

// *****************************************************************************
// interface functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D10Renderer::drawBegin()
{
    // TODO: handle render window size move
    mNumPrims = 0;
    mNumBatches = 0;
    return true;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D10Renderer::drawEnd()
{
    GN_GUARD_SLOW;
#if D3D10_SDK_VERSION >= 28
    GN_D3D10_CHECK( mSwapChain->Present( getOptions().vsync ? 1 : 0, 0 ) );
#else
    GN_D3D10_CHECK( mSwapChain->Present(
        0, 0, 0, 0,
        getOptions().vsync ? 1 : 0,
        0 ) );
#endif
    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D10Renderer::clearScreen(
    const GN::Vector4f & c, float z, uint32_t s, BitField flags )
{
    GN_GUARD_SLOW;
    GN_UNUSED_PARAM( z );
    GN_UNUSED_PARAM( s );
    GN_UNUSED_PARAM( flags );
    mDevice->ClearRenderTargetView( mRTView, c );
    GN_UNGUARD_SLOW;
}
