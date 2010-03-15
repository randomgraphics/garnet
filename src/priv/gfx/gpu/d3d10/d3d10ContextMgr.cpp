#include "pch.h"
#include "d3d10Gpu.h"
#include "d3d10RenderTargetMgr.h"
#include "d3d10StateObject.h"
#include "d3d10Shader.h"
#include "d3d10Texture.h"
#include "d3d10VtxLayout.h"
#include "d3d10Buffer.h"

// *****************************************************************************
// local data and functions
// *****************************************************************************

static const D3D10_FILL_MODE FILL_MODE_TO_D3D10[] =
{
    D3D10_FILL_SOLID,     // FILL_SOLID = 0,
    D3D10_FILL_WIREFRAME, // FILL_WIREFRAME,
    D3D10_FILL_SOLID,     // FILL_POINT,
};
GN_CASSERT( GN_ARRAY_COUNT(FILL_MODE_TO_D3D10) == GN::gfx::GpuContext::NUM_FILL_MODES );

static const D3D10_CULL_MODE CULL_MODE_TO_D3D10[] =
{
    D3D10_CULL_NONE,    // CULL_NONE = 0,
    D3D10_CULL_FRONT,   // CULL_FRONT,
    D3D10_CULL_BACK,    // CULL_BACK,
};
GN_CASSERT( GN_ARRAY_COUNT(CULL_MODE_TO_D3D10) == GN::gfx::GpuContext::NUM_CULL_MODES );

static const D3D10_BLEND_OP BLEND_OP_TO_D3D10[] =
{
    D3D10_BLEND_OP_ADD,          // BLEND_OP_ADD = 0,
    D3D10_BLEND_OP_SUBTRACT,     // BLEND_OP_SUB,
    D3D10_BLEND_OP_REV_SUBTRACT, // BLEND_OP_REV_SUB,
    D3D10_BLEND_OP_MIN,          // BLEND_OP_MIN,
    D3D10_BLEND_OP_MAX,          // BLEND_OP_MAX,
};
GN_CASSERT( GN_ARRAY_COUNT(BLEND_OP_TO_D3D10) == GN::gfx::GpuContext::NUM_BLEND_OPERATIONS );

static const D3D10_BLEND BLEND_TO_D3D10[] =
{
    D3D10_BLEND_ZERO,             // BLEND_ZERO = 0,
    D3D10_BLEND_ONE,              // BLEND_ONE,
    D3D10_BLEND_SRC_COLOR,        // BLEND_SRC_COLOR,
    D3D10_BLEND_INV_SRC_COLOR,    // BLEND_INV_SRC_COLOR,
    D3D10_BLEND_SRC_ALPHA,        // BLEND_SRC_ALPHA,
    D3D10_BLEND_INV_SRC_ALPHA,    // BLEND_INV_SRC_ALPHA,
    D3D10_BLEND_DEST_ALPHA,       // BLEND_DEST_ALPHA,
    D3D10_BLEND_INV_DEST_ALPHA,   // BLEND_INV_DEST_ALPHA,
    D3D10_BLEND_DEST_COLOR,       // BLEND_DEST_COLOR,
    D3D10_BLEND_INV_DEST_COLOR,   // BLEND_INV_DEST_COLOR,
    D3D10_BLEND_BLEND_FACTOR,     // BLEND_BLEND_FACTOR,
    D3D10_BLEND_INV_BLEND_FACTOR, // BLEND_INV_BLEND_FACTOR,
};
GN_CASSERT( GN_ARRAY_COUNT(BLEND_TO_D3D10) == GN::gfx::GpuContext::NUM_BLEND_ARGUMENTS );

static const D3D10_COMPARISON_FUNC CMP_TO_D3D10[] =
{
    D3D10_COMPARISON_NEVER,         // CMP_NEVER = 0,
    D3D10_COMPARISON_LESS,          // CMP_LESS,
    D3D10_COMPARISON_LESS_EQUAL,    // CMP_LESS_EQUAL,
    D3D10_COMPARISON_EQUAL,         // CMP_EQUAL,
    D3D10_COMPARISON_GREATER_EQUAL, // CMP_GREATER_EQUAL,
    D3D10_COMPARISON_GREATER,       // CMP_GREATER,
    D3D10_COMPARISON_NOT_EQUAL,     // CMP_NOT_EQUAL,
    D3D10_COMPARISON_ALWAYS,        // CMP_ALWAYS,
};
GN_CASSERT( GN_ARRAY_COUNT(CMP_TO_D3D10) == GN::gfx::GpuContext::NUM_CMP_FUNCTIONS );

static const D3D10_STENCIL_OP STENCIL_OP_TO_D3D10[] =
{
    D3D10_STENCIL_OP_KEEP,     // STENCIL_KEEP = 0,
    D3D10_STENCIL_OP_ZERO,     // STENCIL_ZERO,
    D3D10_STENCIL_OP_REPLACE,  // STENCIL_REPLACE,
    D3D10_STENCIL_OP_INCR_SAT, // STENCIL_INC_SAT,
    D3D10_STENCIL_OP_DECR_SAT, // STENCIL_DEC_SAT,
    D3D10_STENCIL_OP_INVERT,   // STENCIL_INVERT,
    D3D10_STENCIL_OP_INCR,     // STENCIL_INC,
    D3D10_STENCIL_OP_DECR,     // STENCIL_DEC,
};
GN_CASSERT( GN_ARRAY_COUNT(STENCIL_OP_TO_D3D10) == GN::gfx::GpuContext::NUM_STENCIL_OPERATIONS );

// *****************************************************************************
// init/shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D10Gpu::contextInit()
{
    GN_GUARD;

    // create default sampler, then bind to D3D device
    D3D10_SAMPLER_DESC sd;
    GN::d3d10::constructDefaultSamplerDesc( sd );
    GN_DX_CHECK_RETURN( mDevice->CreateSamplerState( &sd, &mDefaultSampler ), false );
    ID3D10SamplerState * samplers[D3D10_COMMONSHADER_SAMPLER_REGISTER_COUNT];
    for( size_t i = 0; i < D3D10_COMMONSHADER_SAMPLER_REGISTER_COUNT; ++i )
    {
        samplers[i] = mDefaultSampler;
    }
    mDevice->VSSetSamplers( 0, D3D10_COMMONSHADER_SAMPLER_REGISTER_COUNT, samplers );
    mDevice->GSSetSamplers( 0, D3D10_COMMONSHADER_SAMPLER_REGISTER_COUNT, samplers );
    mDevice->PSSetSamplers( 0, D3D10_COMMONSHADER_SAMPLER_REGISTER_COUNT, samplers );

    // create state object manager
    mSOMgr = new D3D10StateObjectManager( *mDevice );
    if( 0 == mSOMgr ) return false;

    // create render target manager
    mRTMgr = new D3D10RTMgr( *this );
    if( !mRTMgr->Init() ) return false;

    // bind default context
    rebindContext();

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D10Gpu::contextQuit()
{
    GN_GUARD;

    if( mDevice )
    {
        mDevice->ClearState();
    }

    mContext.Clear();

    SafeDelete( mRTMgr );

    SafeDelete( mSOMgr );

    mDefaultSampler.Clear();

    mVertexLayouts.Clear();

    GN_UNGUARD;
}

// *****************************************************************************
// public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D10Gpu::bindContextImpl( const GpuContext & newContext, bool skipDirtyCheck )
{
    PIXPERF_FUNCTION_EVENT();

    //
    // Parameter check
    //
    if( paramCheckEnabled() )
    {
        // TODO: verify data in new context
        // TODO: make sure all fields in current context are valid.
    }

    if( !bindContextRenderTarget( newContext, skipDirtyCheck ) ) return false;
    if( !bindContextShader( newContext, skipDirtyCheck ) ) return false;
    if( !bindContextState( newContext, skipDirtyCheck ) ) return false;
    if( !bindContextResource( newContext, skipDirtyCheck ) ) return false;

    return true;
}


//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D10Gpu::setSampler(
    int                        shaderStage,  // 0: VS, 1: GS: 2: PS
    int                        samplerStage, // sampler stage
    const D3D10_SAMPLER_DESC & ssdesc,
    bool                       skipDirtyCheck )
{
    switch( shaderStage )
    {
        case 0: mSOMgr->setVSSampler( ssdesc, samplerStage, skipDirtyCheck ); break;
        case 1: mSOMgr->setGSSampler( ssdesc, samplerStage, skipDirtyCheck ); break;
        case 2: mSOMgr->setPSSampler( ssdesc, samplerStage, skipDirtyCheck ); break;
        case 3: GN_UNEXPECTED();
    }
}

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
inline bool GN::gfx::D3D10Gpu::bindContextRenderTarget(
    const GpuContext & newContext,
    bool               skipDirtyCheck )
{
    //
    // bind render targets
    //
    bool renderTargetSizeChanged = false;
    if( !mRTMgr->bind(
            *(const RenderTargetDesc*)&mContext.colortargets,
            *(const RenderTargetDesc*)&newContext.colortargets,
            skipDirtyCheck,
            renderTargetSizeChanged ) )
    {
        return false;
    }

    // bind viewport
    const Vector2<UInt32> & rtsize = mRTMgr->getRenderTargetSize();
    const Rect<UInt32> & newvp = newContext.rs.viewport;
    D3D10_VIEWPORT d3dvp;
    d3dvp.MinDepth = 0.0f;
    d3dvp.MaxDepth = 1.0f;
    if( 0 == newvp.x && 0 == newvp.y && 0 == newvp.w && 0 == newvp.h )
    {
        d3dvp.TopLeftX = 0;
        d3dvp.TopLeftY = 0;
        d3dvp.Width    = rtsize.x;
        d3dvp.Height   = rtsize.y;
    }
    else
    {
        UInt32 l = newvp.x;
        UInt32 t = newvp.y;
        UInt32 r = newvp.x + newvp.w;
        UInt32 b = newvp.y + newvp.h;

        math::ClampMinMax<UInt32>( l, 0, rtsize.width );
        math::ClampMinMax<UInt32>( t, 0, rtsize.height );
        math::ClampMinMax<UInt32>( r, 0, rtsize.width );
        math::ClampMinMax<UInt32>( b, 0, rtsize.height );

        d3dvp.TopLeftX = l;
        d3dvp.TopLeftY = t;
        d3dvp.Width    = r - l;
        d3dvp.Height   = b - t;
    }
    mDevice->RSSetViewports( 1, &d3dvp );

    // update scissor (always same as viewport)
    D3D10_RECT scissor = {
        (long)( d3dvp.TopLeftX ),
        (long)( d3dvp.TopLeftY ),
        (long)( d3dvp.TopLeftX+d3dvp.Width ),
        (long)( d3dvp.TopLeftY+d3dvp.Height ),
    };
    mDevice->RSSetScissorRects( 1, &scissor );

    return true;
}

//
//
// -----------------------------------------------------------------------------
inline bool GN::gfx::D3D10Gpu::bindContextShader(
    const GpuContext & newContext,
    bool               skipDirtyCheck )
{
    //
    // bind shaders
    //
    if( newContext.gpuProgram )
    {
        D3D10GpuProgram * newProg = (D3D10GpuProgram*)newContext.gpuProgram.Get();
        D3D10GpuProgram * oldProg = (D3D10GpuProgram*)mContext.gpuProgram.Get();

        // apply shader
        if( skipDirtyCheck || newProg != oldProg )
        {
            newProg->apply();
        }

        // Make sure size of AutoRef<T> and T* are same. So we can safely convert AutoRef<T> * to T **
        GN_CASSERT( sizeof(AutoRef<Uniform>) == sizeof(Uniform*) );

        // apply GPU program resources
        newProg->applyUniforms( (const Uniform * const *)newContext.uniforms.ToRawPtr(), newContext.uniforms.Size(), skipDirtyCheck );
        newProg->applyTextures( newContext.textures.ToRawPtr(), newContext.textures.Size(), skipDirtyCheck );
    }
    else if( skipDirtyCheck || (NULL != mContext.gpuProgram) )
    {
        mDevice->VSSetShader( NULL );
        mDevice->GSSetShader( NULL );
        mDevice->PSSetShader( NULL );
    }
    else
    {
        // Both old and new program are NULL. Do nothing
    }

    return true;
}

//
//
// -----------------------------------------------------------------------------
inline bool GN::gfx::D3D10Gpu::bindContextState(
    const GpuContext & newContext,
    bool               skipDirtyCheck )
{
    // rasterization states
    D3D10_RASTERIZER_DESC rsdesc;
    memset( &rsdesc, 0, sizeof(rsdesc) );
    rsdesc.FillMode              = FILL_MODE_TO_D3D10[newContext.rs.fillMode];
    rsdesc.CullMode              = CULL_MODE_TO_D3D10[newContext.rs.cullMode];
    rsdesc.FrontCounterClockwise = GpuContext::FRONT_CCW == newContext.rs.frontFace;
    rsdesc.DepthBias             = 0;
    rsdesc.DepthBiasClamp        = 0.0f;
    rsdesc.SlopeScaledDepthBias  = 0.0f;
    rsdesc.DepthClipEnable       = false;
    rsdesc.ScissorEnable         = true;
    rsdesc.MultisampleEnable     = 0 != newContext.rs.msaaEnabled;
    rsdesc.AntialiasedLineEnable = false;
    if( !mSOMgr->setRS( rsdesc, skipDirtyCheck ) ) return false;

    // depth stencil states
    D3D10_DEPTH_STENCIL_DESC dsdesc;
    memset( &dsdesc, 0, sizeof(dsdesc) );
    dsdesc.DepthEnable = newContext.rs.depthTestEnabled;
    dsdesc.DepthWriteMask = newContext.rs.depthWriteEnabled ? D3D10_DEPTH_WRITE_MASK_ALL : D3D10_DEPTH_WRITE_MASK_ZERO;
    dsdesc.DepthFunc = CMP_TO_D3D10[newContext.rs.depthFunc];
    dsdesc.StencilEnable = newContext.rs.stencilEnabled;
    dsdesc.StencilReadMask = 0xFF;
    dsdesc.StencilWriteMask = 0xFF;
    dsdesc.FrontFace.StencilFunc = CMP_TO_D3D10[newContext.rs.stencilFunc];
    dsdesc.FrontFace.StencilPassOp = STENCIL_OP_TO_D3D10[newContext.rs.stencilPassOp];
    dsdesc.FrontFace.StencilFailOp = STENCIL_OP_TO_D3D10[newContext.rs.stencilFailOp];
    dsdesc.FrontFace.StencilDepthFailOp = STENCIL_OP_TO_D3D10[newContext.rs.stencilZFailOp];
    dsdesc.BackFace = dsdesc.FrontFace;
    UInt32 stencilRef = 0;
    if( !mSOMgr->setDS( dsdesc, stencilRef, skipDirtyCheck ) ) return false;

    // blend states
    D3D10_BLEND_DESC bsdesc;
    memset( &bsdesc, 0, sizeof(bsdesc) );
    bsdesc.AlphaToCoverageEnable    = false;
    if( newContext.rs.independentAlphaBlending )
    {
        for( int i = 0; i < 8; ++i )
        {
            bsdesc.BlendEnable[0]   = newContext.rs.alphaBlend[i].blendEnabled;
        }
    }
    else
    {
        bsdesc.BlendEnable[0]       =
        bsdesc.BlendEnable[1]       =
        bsdesc.BlendEnable[2]       =
        bsdesc.BlendEnable[3]       =
        bsdesc.BlendEnable[4]       =
        bsdesc.BlendEnable[5]       =
        bsdesc.BlendEnable[6]       =
        bsdesc.BlendEnable[7]       = newContext.rs.alphaBlend[0].blendEnabled;
    }
    bsdesc.SrcBlend                 = BLEND_TO_D3D10[newContext.rs.alphaBlend[0].blendSrc];
    bsdesc.DestBlend                = BLEND_TO_D3D10[newContext.rs.alphaBlend[0].blendDst];
    bsdesc.BlendOp                  = BLEND_OP_TO_D3D10[newContext.rs.alphaBlend[0].blendOp];
    bsdesc.SrcBlendAlpha            = BLEND_TO_D3D10[newContext.rs.alphaBlend[0].blendAlphaSrc];
    bsdesc.DestBlendAlpha           = BLEND_TO_D3D10[newContext.rs.alphaBlend[0].blendAlphaDst];
    bsdesc.BlendOpAlpha             = BLEND_OP_TO_D3D10[newContext.rs.alphaBlend[0].blendAlphaOp];
    bsdesc.RenderTargetWriteMask[0] = D3D10_COLOR_WRITE_ENABLE_ALL;
    bsdesc.RenderTargetWriteMask[1] = D3D10_COLOR_WRITE_ENABLE_ALL;
    bsdesc.RenderTargetWriteMask[2] = D3D10_COLOR_WRITE_ENABLE_ALL;
    bsdesc.RenderTargetWriteMask[3] = D3D10_COLOR_WRITE_ENABLE_ALL;
    bsdesc.RenderTargetWriteMask[4] = D3D10_COLOR_WRITE_ENABLE_ALL;
    bsdesc.RenderTargetWriteMask[5] = D3D10_COLOR_WRITE_ENABLE_ALL;
    bsdesc.RenderTargetWriteMask[6] = D3D10_COLOR_WRITE_ENABLE_ALL;
    bsdesc.RenderTargetWriteMask[7] = D3D10_COLOR_WRITE_ENABLE_ALL;
    UInt32 sampleMask = 0xFFFFFFFF;
    if( !mSOMgr->setBS( bsdesc, newContext.rs.blendFactors, sampleMask, skipDirtyCheck ) ) return false;

    // Note: input and sampler states are handled in bindContextResource()

    return true;
}

//
//
// -----------------------------------------------------------------------------
inline bool GN::gfx::D3D10Gpu::bindContextResource(
    const GpuContext & newContext,
    bool               skipDirtyCheck )
{
    //
    // bind vertex format
    //
    D3D10VertexLayout * layout;
    if( skipDirtyCheck || mContext.vtxfmt != newContext.vtxfmt )
    {
        if( 0 == newContext.vtxfmt.numElements )
        {
            mDevice->IASetInputLayout( NULL );
            layout = NULL;
        }
        else
        {
            layout = &mVertexLayouts[newContext.vtxfmt];

            if( NULL == layout->il )
            {
                if( !layout->Init( *mDevice, newContext.vtxfmt ) ) return false;
            }

            mDevice->IASetInputLayout( layout->il );
        }
    }
    else
    {
        layout = &mVertexLayouts[mContext.vtxfmt];
    }

    ///
    /// bind vertex buffers, only when input layout is valid.
    ///
    if( layout )
    {
        ID3D10Buffer * buf[GpuContext::MAX_VERTEX_BUFFERS];
        UINT           strides[GpuContext::MAX_VERTEX_BUFFERS];
        UINT           offsets[GpuContext::MAX_VERTEX_BUFFERS];
        for( UINT i = 0; i < GpuContext::MAX_VERTEX_BUFFERS; ++i )
        {
            const VertexBufferBinding & b = newContext.vtxbufs[i];

            buf[i]     = b.vtxbuf ? SafeCastPtr<const D3D10VtxBuf>(b.vtxbuf.Get())->getD3DBuffer() : NULL;
            strides[i] = b.stride;
            offsets[i] = b.offset;
        }
        mDevice->IASetVertexBuffers( 0, GpuContext::MAX_VERTEX_BUFFERS, buf, strides, offsets );
    }

    //
    // bind index buffer
    //
    if( skipDirtyCheck || newContext.idxbuf != mContext.idxbuf )
    {
        if( newContext.idxbuf )
        {
            const D3D10IdxBuf * ib = (const D3D10IdxBuf*)newContext.idxbuf.Get();

            const IdxBufDesc & ibdesc = ib->getDesc();

            mDevice->IASetIndexBuffer(
                ib->getD3DBuffer(),
                ibdesc.bits32 ? DXGI_FORMAT_R32_UINT : DXGI_FORMAT_R16_UINT,
                0 );
        }
        else
        {
            mDevice->IASetIndexBuffer( NULL, DXGI_FORMAT_R16_UINT, 0 );
        }
    }

    return true;
}
