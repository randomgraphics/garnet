#include "pch.h"
#include "d3d10Renderer.h"
#include "d3d10RenderTargetMgr.h"

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
    const GN::Vector4f & c, float z, UInt8 s, BitFields flags )
{
    GN_GUARD_SLOW;

    if( CLEAR_C & flags )
    {
        for( UInt32 i = 0; i < mRTMgr->getRenderTargetCount(); ++i )
        {
            mDevice->ClearRenderTargetView( mRTMgr->getRenderTargetView(i), c );
        }
    }

    UInt32 d3dflag = 0;
    if( CLEAR_Z ) d3dflag |= D3D10_CLEAR_DEPTH;
    if( CLEAR_S ) d3dflag |= D3D10_CLEAR_STENCIL;

    if( d3dflag )
    {
        mDevice->ClearDepthStencilView( mRTMgr->getDepthStencilView(), d3dflag, z, s );
    }

    GN_UNGUARD_SLOW;
}
