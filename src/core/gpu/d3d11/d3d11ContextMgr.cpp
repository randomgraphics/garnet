#include "pch.h"
#include "d3d11Gpu.h"
#include "d3d11RenderTargetMgr.h"
#include "d3d11StateObject.h"
#include "d3d11Shader.h"
#include "d3d11Texture.h"
#include "d3d11Buffer.h"

// *****************************************************************************
// local data and functions
// *****************************************************************************

static const D3D11_FILL_MODE FILL_MODE_TO_D3D11[] = {
    D3D11_FILL_SOLID,     // FILL_SOLID = 0,
    D3D11_FILL_WIREFRAME, // FILL_WIREFRAME,
    D3D11_FILL_SOLID,     // FILL_POINT,
};
GN_CASSERT(GN_ARRAY_COUNT(FILL_MODE_TO_D3D11) == GN::gfx::GpuContext::NUM_FILL_MODES);

static const D3D11_CULL_MODE CULL_MODE_TO_D3D11[] = {
    D3D11_CULL_NONE,  // CULL_NONE = 0,
    D3D11_CULL_FRONT, // CULL_FRONT,
    D3D11_CULL_BACK,  // CULL_BACK,
};
GN_CASSERT(GN_ARRAY_COUNT(CULL_MODE_TO_D3D11) == GN::gfx::GpuContext::NUM_CULL_MODES);

static const D3D11_BLEND_OP BLEND_OP_TO_D3D11[] = {
    D3D11_BLEND_OP_ADD,          // BLEND_OP_ADD = 0,
    D3D11_BLEND_OP_SUBTRACT,     // BLEND_OP_SUB,
    D3D11_BLEND_OP_REV_SUBTRACT, // BLEND_OP_REV_SUB,
    D3D11_BLEND_OP_MIN,          // BLEND_OP_MIN,
    D3D11_BLEND_OP_MAX,          // BLEND_OP_MAX,
};
GN_CASSERT(GN_ARRAY_COUNT(BLEND_OP_TO_D3D11) == GN::gfx::GpuContext::NUM_BLEND_OPERATIONS);

static const D3D11_BLEND BLEND_TO_D3D11[] = {
    D3D11_BLEND_ZERO,             // BLEND_ZERO = 0,
    D3D11_BLEND_ONE,              // BLEND_ONE,
    D3D11_BLEND_SRC_COLOR,        // BLEND_SRC_COLOR,
    D3D11_BLEND_INV_SRC_COLOR,    // BLEND_INV_SRC_COLOR,
    D3D11_BLEND_SRC_ALPHA,        // BLEND_SRC_ALPHA,
    D3D11_BLEND_INV_SRC_ALPHA,    // BLEND_INV_SRC_ALPHA,
    D3D11_BLEND_DEST_ALPHA,       // BLEND_DEST_ALPHA,
    D3D11_BLEND_INV_DEST_ALPHA,   // BLEND_INV_DEST_ALPHA,
    D3D11_BLEND_DEST_COLOR,       // BLEND_DEST_COLOR,
    D3D11_BLEND_INV_DEST_COLOR,   // BLEND_INV_DEST_COLOR,
    D3D11_BLEND_BLEND_FACTOR,     // BLEND_BLEND_FACTOR,
    D3D11_BLEND_INV_BLEND_FACTOR, // BLEND_INV_BLEND_FACTOR,
};
GN_CASSERT(GN_ARRAY_COUNT(BLEND_TO_D3D11) == GN::gfx::GpuContext::NUM_BLEND_ARGUMENTS);

static const D3D11_COMPARISON_FUNC CMP_TO_D3D11[] = {
    D3D11_COMPARISON_NEVER,         // CMP_NEVER = 0,
    D3D11_COMPARISON_LESS,          // CMP_LESS,
    D3D11_COMPARISON_LESS_EQUAL,    // CMP_LESS_EQUAL,
    D3D11_COMPARISON_EQUAL,         // CMP_EQUAL,
    D3D11_COMPARISON_GREATER_EQUAL, // CMP_GREATER_EQUAL,
    D3D11_COMPARISON_GREATER,       // CMP_GREATER,
    D3D11_COMPARISON_NOT_EQUAL,     // CMP_NOT_EQUAL,
    D3D11_COMPARISON_ALWAYS,        // CMP_ALWAYS,
};
GN_CASSERT(GN_ARRAY_COUNT(CMP_TO_D3D11) == GN::gfx::GpuContext::NUM_CMP_FUNCTIONS);

static const D3D11_STENCIL_OP STENCIL_OP_TO_D3D11[] = {
    D3D11_STENCIL_OP_KEEP,     // STENCIL_KEEP = 0,
    D3D11_STENCIL_OP_ZERO,     // STENCIL_ZERO,
    D3D11_STENCIL_OP_REPLACE,  // STENCIL_REPLACE,
    D3D11_STENCIL_OP_INCR_SAT, // STENCIL_INC_SAT,
    D3D11_STENCIL_OP_DECR_SAT, // STENCIL_DEC_SAT,
    D3D11_STENCIL_OP_INVERT,   // STENCIL_INVERT,
    D3D11_STENCIL_OP_INCR,     // STENCIL_INC,
    D3D11_STENCIL_OP_DECR,     // STENCIL_DEC,
};
GN_CASSERT(GN_ARRAY_COUNT(STENCIL_OP_TO_D3D11) == GN::gfx::GpuContext::NUM_STENCIL_OPERATIONS);

// *****************************************************************************
// init/shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D11Gpu::contextInit() {
    GN_GUARD;

    // create default samplers, then bind to D3D device
    D3D11_SAMPLER_DESC sd;
    GN::d3d11::constructDefaultSamplerDesc(sd);
    GN_DX_CHECK_RETURN(mDevice->CreateSamplerState(&sd, &mDefaultSampler), false);
    ID3D11SamplerState * samplers[D3D11_COMMONSHADER_SAMPLER_REGISTER_COUNT];
    for (size_t i = 0; i < D3D11_COMMONSHADER_SAMPLER_REGISTER_COUNT; ++i) { samplers[i] = mDefaultSampler; }
    mDeviceContext->HSSetSamplers(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, samplers);
    mDeviceContext->VSSetSamplers(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, samplers);
    mDeviceContext->DSSetSamplers(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, samplers);
    mDeviceContext->GSSetSamplers(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, samplers);
    mDeviceContext->PSSetSamplers(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, samplers);

    // create state object manager
    mSOMgr = new D3D11StateObjectManager(*mDevice, *mDeviceContext);
    if (0 == mSOMgr) return false;

    // create render target manager
    mRTMgr = new D3D11RTMgr(*this);
    if (!mRTMgr->init()) return false;

    // bind default context
    rebindContext();

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D11Gpu::contextQuit() {
    GN_GUARD;

    if (NULL != mDeviceContext) {
        mDeviceContext->ClearState();
        mDeviceContext->Flush();
    }

    mContext.clear();

    mCurrentVertexLayout = NULL;

    safeDelete(mRTMgr);
    safeDelete(mSOMgr);

    mDefaultSampler.clear();
    mVertexLayouts.clear();

    GN_UNGUARD;
}

// *****************************************************************************
// from BasicGpu
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D11Gpu::bindContextImpl(const GpuContext & newContext, bool skipDirtyCheck) {
    PIXPERF_FUNCTION_EVENT(mDeviceContext);

    if (!bindContextRenderTarget(newContext, skipDirtyCheck)) return false;
    if (!bindContextShader(newContext, skipDirtyCheck)) return false;
    if (!bindContextState(newContext, skipDirtyCheck)) return false;
    if (!bindContextResource(newContext, skipDirtyCheck)) return false;

    return true;
}

// *****************************************************************************
// public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D11Gpu::setSampler(ShaderStage                shaderStage,
                                   int                        samplerStage, // sampler stage
                                   const D3D11_SAMPLER_DESC & ssdesc, bool skipDirtyCheck) {
    switch (shaderStage.toRawEnum()) {
    case ShaderStage::VS:
        mSOMgr->setVSSampler(ssdesc, samplerStage, skipDirtyCheck);
        break;
    case ShaderStage::PS:
        mSOMgr->setPSSampler(ssdesc, samplerStage, skipDirtyCheck);
        break;
    case ShaderStage::GS:
        mSOMgr->setGSSampler(ssdesc, samplerStage, skipDirtyCheck);
        break;
    case ShaderStage::HS:
    case ShaderStage::DS:
        GN_UNIMPL();
        break;
    default:
        GN_UNEXPECTED();
        break;
    }
}

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
inline bool GN::gfx::D3D11Gpu::bindContextRenderTarget(const GpuContext & newContext, bool skipDirtyCheck) {
    //
    // bind render targets
    //
    bool renderTargetSizeChanged = false;
    if (!mRTMgr->bind(*(const RenderTargetDesc *) &mContext.colortargets, *(const RenderTargetDesc *) &newContext.colortargets, skipDirtyCheck,
                      renderTargetSizeChanged)) {
        return false;
    }

    // bind viewport
    const Vector2<uint32> & rtsize = mRTMgr->getRenderTargetSize();
    const Rect<uint32> &    newvp  = newContext.rs.viewport;
    D3D11_VIEWPORT          d3dvp;
    d3dvp.MinDepth = 0.0f;
    d3dvp.MaxDepth = 1.0f;
    if (0 == newvp.x && 0 == newvp.y && 0 == newvp.w && 0 == newvp.h) {
        d3dvp.TopLeftX = 0;
        d3dvp.TopLeftY = 0;
        d3dvp.Width    = (float) rtsize.x;
        d3dvp.Height   = (float) rtsize.y;
    } else {
        uint32 l       = math::clamp<uint32>(newvp.x, 0, rtsize.width);
        uint32 t       = math::clamp<uint32>(newvp.y, 0, rtsize.height);
        uint32 r       = math::clamp<uint32>(newvp.x + newvp.w, 0, rtsize.width);
        uint32 b       = math::clamp<uint32>(newvp.y + newvp.h, 0, rtsize.height);
        d3dvp.TopLeftX = (float) l;
        d3dvp.TopLeftY = (float) t;
        d3dvp.Width    = (float) (r - l);
        d3dvp.Height   = (float) (b - t);
    }
    mDeviceContext->RSSetViewports(1, &d3dvp);

    // update scissor (always same as viewport)
    D3D11_RECT scissor = {
        (long) (d3dvp.TopLeftX),
        (long) (d3dvp.TopLeftY),
        (long) (d3dvp.TopLeftX + d3dvp.Width),
        (long) (d3dvp.TopLeftY + d3dvp.Height),
    };
    mDeviceContext->RSSetScissorRects(1, &scissor);

    return true;
}

//
//
// -----------------------------------------------------------------------------
inline bool GN::gfx::D3D11Gpu::bindContextShader(const GpuContext & newContext, bool skipDirtyCheck) {
    //
    // bind shaders
    //
    if (newContext.gpuProgram) {
        D3D11GpuProgram * newProg = (D3D11GpuProgram *) newContext.gpuProgram.rawptr();
        D3D11GpuProgram * oldProg = (D3D11GpuProgram *) mContext.gpuProgram.rawptr();

        // apply shader
        if (skipDirtyCheck || newProg != oldProg) { newProg->apply(); }

        // Make sure size of AutoRef<T> and T* are same. So we can safely convert AutoRef<T> * to T **
        GN_CASSERT(sizeof(AutoRef<Uniform>) == sizeof(Uniform *));

        // apply GPU program resources
        newProg->applyUniforms((const Uniform * const *) newContext.uniforms.rawptr(), (uint32) newContext.uniforms.size(), skipDirtyCheck);
        newProg->applyTextures(newContext.textures.rawptr(), (uint32) newContext.textures.size(), skipDirtyCheck);
    } else if (skipDirtyCheck || (NULL != mContext.gpuProgram)) {
        mDeviceContext->VSSetShader(NULL, NULL, 0);
        mDeviceContext->GSSetShader(NULL, NULL, 0);
        mDeviceContext->PSSetShader(NULL, NULL, 0);
    } else {
        // Both old and new program are NULL. Do nothing
    }

    return true;
}

//
//
// -----------------------------------------------------------------------------
inline bool GN::gfx::D3D11Gpu::bindContextState(const GpuContext & newContext, bool skipDirtyCheck) {
    // rasterization states
    D3D11_RASTERIZER_DESC rsdesc;
    memset(&rsdesc, 0, sizeof(rsdesc));
    rsdesc.FillMode              = FILL_MODE_TO_D3D11[newContext.rs.fillMode];
    rsdesc.CullMode              = CULL_MODE_TO_D3D11[newContext.rs.cullMode];
    rsdesc.FrontCounterClockwise = GpuContext::FRONT_CCW == newContext.rs.frontFace;
    rsdesc.DepthBias             = 0;
    rsdesc.DepthBiasClamp        = 0.0f;
    rsdesc.SlopeScaledDepthBias  = 0.0f;
    rsdesc.DepthClipEnable       = false;
    rsdesc.ScissorEnable         = true;
    rsdesc.MultisampleEnable     = 0 != newContext.rs.msaaEnabled;
    rsdesc.AntialiasedLineEnable = false;
    if (!mSOMgr->setRS(rsdesc, skipDirtyCheck)) return false;

    // depth stencil states
    D3D11_DEPTH_STENCIL_DESC dsdesc;
    memset(&dsdesc, 0, sizeof(dsdesc));
    dsdesc.DepthEnable                  = newContext.rs.depthTestEnabled;
    dsdesc.DepthWriteMask               = newContext.rs.depthWriteEnabled ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
    dsdesc.DepthFunc                    = CMP_TO_D3D11[newContext.rs.depthFunc];
    dsdesc.StencilEnable                = newContext.rs.stencilEnabled;
    dsdesc.StencilReadMask              = 0xFF;
    dsdesc.StencilWriteMask             = 0xFF;
    dsdesc.FrontFace.StencilFunc        = CMP_TO_D3D11[newContext.rs.stencilFunc];
    dsdesc.FrontFace.StencilPassOp      = STENCIL_OP_TO_D3D11[newContext.rs.stencilPassOp];
    dsdesc.FrontFace.StencilFailOp      = STENCIL_OP_TO_D3D11[newContext.rs.stencilFailOp];
    dsdesc.FrontFace.StencilDepthFailOp = STENCIL_OP_TO_D3D11[newContext.rs.stencilZFailOp];
    dsdesc.BackFace                     = dsdesc.FrontFace;
    uint32 stencilRef                   = 0;
    if (!mSOMgr->setDS(dsdesc, stencilRef, skipDirtyCheck)) return false;

    // blend states
    D3D11_BLEND_DESC bsdesc;
    memset(&bsdesc, 0, sizeof(bsdesc));
    bsdesc.AlphaToCoverageEnable  = false;
    bsdesc.IndependentBlendEnable = newContext.rs.independentAlphaBlending;
    for (size_t i = 0; i < (newContext.rs.independentAlphaBlending ? 1 : GN_ARRAY_COUNT(newContext.rs.alphaBlend)); ++i) {
        bsdesc.RenderTarget[i].BlendEnable    = newContext.rs.alphaBlend[i].blendEnabled;
        bsdesc.RenderTarget[i].SrcBlend       = BLEND_TO_D3D11[newContext.rs.alphaBlend[i].blendSrc];
        bsdesc.RenderTarget[i].DestBlend      = BLEND_TO_D3D11[newContext.rs.alphaBlend[i].blendDst];
        bsdesc.RenderTarget[i].BlendOp        = BLEND_OP_TO_D3D11[newContext.rs.alphaBlend[i].blendOp];
        bsdesc.RenderTarget[i].SrcBlendAlpha  = BLEND_TO_D3D11[newContext.rs.alphaBlend[i].blendAlphaSrc];
        bsdesc.RenderTarget[i].DestBlendAlpha = BLEND_TO_D3D11[newContext.rs.alphaBlend[i].blendAlphaDst];
        bsdesc.RenderTarget[i].BlendOpAlpha   = BLEND_OP_TO_D3D11[newContext.rs.alphaBlend[i].blendAlphaOp];
    }
    bsdesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    bsdesc.RenderTarget[1].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    bsdesc.RenderTarget[2].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    bsdesc.RenderTarget[3].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    bsdesc.RenderTarget[4].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    bsdesc.RenderTarget[5].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    bsdesc.RenderTarget[6].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    bsdesc.RenderTarget[7].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    uint32 sampleMask                            = 0xFFFFFFFF;
    if (!mSOMgr->setBS(bsdesc, newContext.rs.blendFactors, sampleMask, skipDirtyCheck)) return false;

    // Note: input and sampler states are handled in bindContextResource()

    return true;
}

//
//
// -----------------------------------------------------------------------------
inline bool GN::gfx::D3D11Gpu::bindContextResource(const GpuContext & newContext, bool skipDirtyCheck) {
    //
    // bind input layout
    //
    D3D11GpuProgram *   gpuProgram = (D3D11GpuProgram *) newContext.gpuProgram.rawptr();
    D3D11VertexLayout * layout     = NULL;
    if (NULL != gpuProgram) {
        VertexFormatKey vfk = {newContext.vtxbind, gpuProgram->getUniqueID()};
        layout              = &mVertexLayouts[vfk];
        if (NULL == layout->il) {
            AutoObjPtr<D3D11VertexLayout> newLayout(new D3D11VertexLayout());
            if (!layout->init(*mDevice, newContext.vtxbind, *gpuProgram)) {
                mVertexLayouts.remove(vfk);
                return false;
            }
        }
    }
    if (skipDirtyCheck || layout != mCurrentVertexLayout) {
        mDeviceContext->IASetInputLayout(layout ? (ID3D11InputLayout *) layout->il : NULL);
        mCurrentVertexLayout = layout;
    }

    ///
    /// bind vertex buffers, only when input layout is valid.
    ///
    if (layout) {
        ID3D11Buffer * buf[GpuContext::MAX_VERTEX_BUFFERS];
        UINT           strides[GpuContext::MAX_VERTEX_BUFFERS];
        UINT           offsets[GpuContext::MAX_VERTEX_BUFFERS];
        for (UINT i = 0; i < GpuContext::MAX_VERTEX_BUFFERS; ++i) {
            const VertexBufferBinding & b = newContext.vtxbufs[i];

            buf[i]     = b.vtxbuf ? safeCastPtr<const D3D11VtxBuf>(b.vtxbuf.rawptr())->getD3DBuffer() : NULL;
            strides[i] = b.stride;
            offsets[i] = b.offset;

            if (0 == b.stride && b.vtxbuf) {
                const VertexElement & e = newContext.vtxbind[i];
                strides[i]              = e.format.getBytesPerBlock();
            }
        }
        mDeviceContext->IASetVertexBuffers(0, GpuContext::MAX_VERTEX_BUFFERS, buf, strides, offsets);
    }

    //
    // bind index buffer
    //
    if (skipDirtyCheck || newContext.idxbuf != mContext.idxbuf) {
        if (newContext.idxbuf) {
            const D3D11IdxBuf * ib = (const D3D11IdxBuf *) newContext.idxbuf.rawptr();

            const IdxBufDesc & ibdesc = ib->getDesc();

            mDeviceContext->IASetIndexBuffer(ib->getD3DBuffer(), ibdesc.bits32 ? DXGI_FORMAT_R32_UINT : DXGI_FORMAT_R16_UINT, 0);
        } else {
            mDeviceContext->IASetIndexBuffer(NULL, DXGI_FORMAT_R16_UINT, 0);
        }
    }

    return true;
}
