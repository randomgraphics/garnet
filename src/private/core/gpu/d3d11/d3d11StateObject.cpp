#include "pch.h"
#include "d3d11StateObject.h"

static GN::Logger * sLogger = GN::GetLogger("GN.gfx.gpu.D3D11");

// *****************************************************************************
// public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::gfx::D3D11StateObjectManager::D3D11StateObjectManager( ID3D11Device & dev, ID3D11DeviceContext & cxt )
    : mRasterStates( dev, cxt )
    , mBlendStates( dev, cxt )
    , mDepthStates( dev, cxt )
    , mSamplerStates( dev, cxt )
{
    /* setup default rasterization states
    mRasterDirty = false;

    mRasterDesc.FillMode              = D3D11_FILL_SOLID;
    mRasterDesc.CullMode              = D3D11_CULL_NONE;
    mRasterDesc.FrontCounterClockwise = false;
    mRasterDesc.DepthBias             = 0;
    mRasterDesc.DepthBiasClamp        = 1.0f;
    mRasterDesc.SlopeScaledDepthBias  = .0f;
    mRasterDesc.DepthClipEnable       = false;
    mRasterDesc.ScissorEnable         = false;
    mRasterDesc.MultisampleEnable     = false;
    mRasterDesc.AntialiasedLineEnable = false;

    mRasterKey.u64    = 0;
    mRasterKey.u8     = 0;
    mRasterKey.bias   = 0;
    mRasterKey.slop   = 0.0f;
    mRasterKey.fill   = D3D11_FILL_SOLID;
    mRasterKey.cull   = D3D11_CULL_NONE;
    mRasterKey.msaa   = 0; // no MSAA
    mRasterKey.aaline = 0;

    // setup default blend states
    mBlendDirty = false;

    memset( &mBlendDesc, 0, sizeof(mBlendDesc) );
    mBlendDesc.SrcBlend                 = D3D11_BLEND_ONE;
    mBlendDesc.DestBlend                = D3D11_BLEND_ZERO;
    mBlendDesc.BlendOp                  = D3D11_BLEND_OP_ADD;
    mBlendDesc.SrcBlendAlpha            = D3D11_BLEND_ONE;
    mBlendDesc.DestBlendAlpha           = D3D11_BLEND_ZERO;
    mBlendDesc.BlendOpAlpha             = D3D11_BLEND_OP_ADD;
    mBlendDesc.RenderTargetWriteMask[0] = D3D11_COLOR_WRITE_ENABLE_ALL;
    mBlendDesc.RenderTargetWriteMask[1] = D3D11_COLOR_WRITE_ENABLE_ALL;
    mBlendDesc.RenderTargetWriteMask[2] = D3D11_COLOR_WRITE_ENABLE_ALL;
    mBlendDesc.RenderTargetWriteMask[3] = D3D11_COLOR_WRITE_ENABLE_ALL;
    mBlendDesc.RenderTargetWriteMask[4] = D3D11_COLOR_WRITE_ENABLE_ALL;
    mBlendDesc.RenderTargetWriteMask[5] = D3D11_COLOR_WRITE_ENABLE_ALL;
    mBlendDesc.RenderTargetWriteMask[6] = D3D11_COLOR_WRITE_ENABLE_ALL;
    mBlendDesc.RenderTargetWriteMask[7] = D3D11_COLOR_WRITE_ENABLE_ALL;

    mBlendKey.u64       = 0;
    mBlendKey.alphamask = 0;
    mBlendKey.blend     = 0;
    mBlendKey.blendsrc  = D3D11_BLEND_ONE;
    mBlendKey.blenddst  = D3D11_BLEND_ZERO;
    mBlendKey.blendop   = D3D11_BLEND_OP_ADD;
    mBlendKey.alphasrc  = D3D11_BLEND_ONE;
    mBlendKey.alphadst  = D3D11_BLEND_ZERO;
    mBlendKey.alphaop   = D3D11_BLEND_OP_ADD;
    mBlendKey.rtmask    = 0xFFFFFFFF;

    mBlendAaMask = 0xFFFFFFFF;

    // setup default depth stencil states
    mDepthDirty = false;

    mDepthDesc.DepthEnable = true;
    mDepthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    mDepthDesc.DepthFunc = D3D11_COMPARISON_LESS;
    mDepthDesc.StencilEnable = false;
    mDepthDesc.StencilReadMask = 0xff;
    mDepthDesc.StencilWriteMask = 0xff;
    mDepthDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    mDepthDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    mDepthDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    mDepthDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS; //( Note: Apr. 07 SDK document has bug about the default value of stencil op, check for newer SDK document for default valeu)
    mDepthDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    mDepthDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    mDepthDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    mDepthDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    mDepthKey.u64              = 0;
    mDepthKey.stencil          = mDepthDesc.StencilEnable;
    mDepthKey.zenable          = mDepthDesc.DepthEnable;
    mDepthKey.zwrite           = mDepthDesc.DepthWriteMask;
    mDepthKey.zfunc            = mDepthDesc.DepthFunc;
    mDepthKey.frontfail        = mDepthDesc.FrontFace.StencilFailOp;
    mDepthKey.frontzfail       = mDepthDesc.FrontFace.StencilDepthFailOp;
    mDepthKey.frontpass        = mDepthDesc.FrontFace.StencilPassOp;
    mDepthKey.frontfunc        = mDepthDesc.FrontFace.StencilFunc;
    mDepthKey.backfail         = mDepthDesc.BackFace.StencilFailOp;
    mDepthKey.backzfail        = mDepthDesc.BackFace.StencilDepthFailOp;
    mDepthKey.backpass         = mDepthDesc.BackFace.StencilPassOp;
    mDepthKey.backfunc         = mDepthDesc.BackFace.StencilFunc;
    mDepthKey.stencilreadmask  = mDepthDesc.StencilReadMask;
    mDepthKey.stencilwritemask = mDepthDesc.StencilWriteMask;
    */
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D11StateObjectManager::setRS(
    const D3D11_RASTERIZER_DESC & desc,
    bool                          skipDirtyCheck )
{
    ID3D11RasterizerState * stateObject = mRasterStates[desc];
    if( NULL == stateObject ) return false;

    if( skipDirtyCheck || stateObject != mCurrentRS )
    {
        mRasterStates.devcxt().RSSetState( stateObject );

        mCurrentRS = stateObject;
    }

    return true;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D11StateObjectManager::setBS(
    const D3D11_BLEND_DESC & desc,
    const Vector4f         & blendFactors,
    uint32                   sampleMask,
    bool                     skipDirtyCheck )
{
    ID3D11BlendState * stateObject = mBlendStates[desc];
    if( NULL == stateObject ) return false;

    if( skipDirtyCheck ||
        stateObject != mCurrentBS ||
        blendFactors != mCurrentBlendFactors ||
        sampleMask != mCurrentSampleMask )
    {
        mBlendStates.devcxt().OMSetBlendState( stateObject, blendFactors, sampleMask );

        mCurrentBS = stateObject;
        mCurrentBlendFactors = blendFactors;
        mCurrentSampleMask = sampleMask;
    }

    return true;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D11StateObjectManager::setDS(
    const D3D11_DEPTH_STENCIL_DESC & desc,
    uint32                           stencilRef,
    bool                             skipDirtyCheck )
{
    ID3D11DepthStencilState * stateObject = mDepthStates[desc];
    if( NULL == stateObject ) return false;

    if( skipDirtyCheck ||
        stateObject != mCurrentDS ||
        stencilRef != mCurrentStencilRef )
    {
        mDepthStates.devcxt().OMSetDepthStencilState( stateObject, stencilRef );

        mCurrentDS = stateObject;
        mCurrentStencilRef = stencilRef;
    }

    return true;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D11StateObjectManager::setVSSampler(
    const D3D11_SAMPLER_DESC & desc,
    uint32                     stage,
    bool                       skipDirtyCheck )
{
    ID3D11SamplerState * ss = mSamplerStates[desc];
    if( NULL == ss ) return false;

    if( stage >= GN_ARRAY_COUNT(mCurrentVSSamplers) )
    {
        GN_ERROR(sLogger)( "PS sampler state is too large." );
        return false;
    }
    if( skipDirtyCheck ||
        ss != mCurrentVSSamplers[stage] )
    {
        mDepthStates.devcxt().VSSetSamplers( stage, 1, &ss );

        mCurrentVSSamplers[stage] = ss;
    }

    return true;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D11StateObjectManager::setGSSampler(
    const D3D11_SAMPLER_DESC & desc,
    uint32                     stage,
    bool                       skipDirtyCheck )
{
    ID3D11SamplerState * ss = mSamplerStates[desc];
    if( NULL == ss ) return false;

    if( stage >= GN_ARRAY_COUNT(mCurrentGSSamplers) )
    {
        GN_ERROR(sLogger)( "PS sampler state is too large." );
        return false;
    }
    if( skipDirtyCheck ||
        ss != mCurrentGSSamplers[stage] )
    {
        mDepthStates.devcxt().GSSetSamplers( stage, 1, &ss );

        mCurrentGSSamplers[stage] = ss;
    }

    return true;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D11StateObjectManager::setPSSampler(
    const D3D11_SAMPLER_DESC & desc,
    uint32                     stage,
    bool                       skipDirtyCheck )
{
    ID3D11SamplerState * ss = mSamplerStates[desc];
    if( NULL == ss ) return false;

    if( stage >= GN_ARRAY_COUNT(mCurrentPSSamplers) )
    {
        GN_ERROR(sLogger)( "PS sampler state is too large." );
        return false;
    }
    if( skipDirtyCheck ||
        ss != mCurrentPSSamplers[stage] )
    {
        mDepthStates.devcxt().PSSetSamplers( stage, 1, &ss );

        mCurrentPSSamplers[stage] = ss;
    }

    return true;
}

