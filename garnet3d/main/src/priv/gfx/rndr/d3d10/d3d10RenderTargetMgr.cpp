#include "pch.h"
#include "d3d10Renderer.h"
#include "d3d10RenderTargetMgr.h"
#include "d3d10Texture.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx.rndr.D3D10");

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D10RTMgr::init()
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::D3D10RTMgr, () );

    // create default color view
    AutoComPtr<ID3D10Texture2D> backBuffer;
    GN_DX10_CHECK_RV( mRenderer.getSwapChain()->GetBuffer( 0, __uuidof( ID3D10Texture2D ), (void**)&backBuffer ), failure() );
    GN_DX10_CHECK_RV( mRenderer.getDevice()->CreateRenderTargetView( backBuffer, NULL, &mAutoColor0 ), failure() );
    GN_ASSERT( mAutoColor0 );

    // get default depth view
    mRenderer.getDevice()->OMGetRenderTargets( 0, 0, &mAutoDepth );

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D10RTMgr::quit()
{
    GN_GUARD;

    mAutoColor0.clear();
    mAutoDepth.clear();

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D10RTMgr::bind(
    const RenderTargetDesc & oldDesc,
    const RenderTargetDesc & newDesc,
    bool forceRebind,
    bool & needRebindViewport )
{
    GN_GUARD_SLOW;

    needRebindViewport = false;

    if( forceRebind || oldDesc != newDesc )
    {
        Vector2<UInt32> newsize;

        if( 0 == newDesc.count )
        {
            mNumColors = 1;
            mColors[0] = mAutoColor0;
            const DispDesc & dd = mRenderer.getDispDesc();
            newsize.set( dd.width, dd.height );
        }
        else
        {
            GN_ASSERT( newDesc.count <= GN_ARRAY_COUNT(mColors) );
            mNumColors = newDesc.count;
            for( UInt32 i = 0; i < newDesc.count; ++i )
            {
                GN_ASSERT( newDesc.cbuffers[i].texture );
                mColors[i] = 0;
                GN_UNIMPL();
            }

            GN_ASSERT( newDesc.cbuffers[0].texture );
            newDesc.cbuffers[0].texture->getMipSize( newDesc.cbuffers[0].level, &newsize.x, &newsize.y );
        }

        if( newDesc.zbuffer.texture )
        {
            mDepth = 0;
            GN_UNIMPL();
        }
        else
        {
            mDepth = mAutoDepth;
        }

        mRenderer.getDevice()->OMSetRenderTargets( mNumColors, mColors, mDepth );

        if( newsize != mRenderTargetSize )
        {
            needRebindViewport = true;
            mRenderTargetSize = newsize;
        }
    }

    GN_UNGUARD_SLOW;
}

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------

