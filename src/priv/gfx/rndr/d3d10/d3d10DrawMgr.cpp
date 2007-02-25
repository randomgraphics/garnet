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

    GN_DX10_CHECK( mSwapChain->Present( getOptions().vsync ? 1 : 0, 0 ) );

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D10Renderer::clearScreen(
    const GN::Vector4f & c, float z, UInt32 s, BitFields flags )
{
    GN_GUARD_SLOW;
    GN_UNUSED_PARAM( z );
    GN_UNUSED_PARAM( s );
    GN_UNUSED_PARAM( flags );
    mDevice->ClearRenderTargetView( mRTView, c );
    GN_UNGUARD_SLOW;
}
