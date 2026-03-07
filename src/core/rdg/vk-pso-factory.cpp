#include "pch.h"
#include "vk-pso-factory.h"
#include "vk-format-utils.h"
#include <unordered_map>
#include <memory>
#include <string>

namespace GN::rdg {

static GN::Logger * sLogger = GN::getLogger("GN.rdg.vk-pso-factory");

// --- ShaderKey ---
static uint64_t hashShaderBinary(const void * vs, size_t vsSize, const void * ps, size_t psSize) {
    uint64_t     h = 0xcbf29ce484222325ULL;
    const auto * p = static_cast<const uint8_t *>(vs);
    for (size_t i = 0; i < vsSize; ++i) h = (h ^ p[i]) * 0x100000001b3ULL;
    if (ps && psSize > 0) {
        p = static_cast<const uint8_t *>(ps);
        for (size_t i = 0; i < psSize; ++i) h = (h ^ p[i]) * 0x100000001b3ULL;
    }
    return h;
}

ShaderKey ShaderKey::makeGraphics(const GpuShaderAction::ShaderBinary & vs, const GpuShaderAction::ShaderBinary & ps) {
    ShaderKey k;
    k.hash = hashShaderBinary(vs.binary, vs.size, ps.binary, ps.size);
    return k;
}

ShaderKey ShaderKey::makeCompute(const GpuShaderAction::ShaderBinary & cs) {
    ShaderKey    k;
    const auto * p = static_cast<const uint8_t *>(cs.binary);
    uint64_t     h = 0xcbf29ce484222325ULL;
    for (size_t i = 0; i < cs.size; ++i) h = (h ^ p[i]) * 0x100000001b3ULL;
    k.hash = h;
    return k;
}

// --- RenderTargetKey: hash and conversion helpers ---
static uint64_t hashBlendState(const RenderTarget::BlendState & b) {
    uint64_t h = 0;
    h          = h * 31 + static_cast<uint32_t>(b.colorOp);
    h          = h * 31 + static_cast<uint32_t>(b.colorSrc);
    h          = h * 31 + static_cast<uint32_t>(b.colorDst);
    h          = h * 31 + static_cast<uint32_t>(b.alphaOp);
    h          = h * 31 + static_cast<uint32_t>(b.alphaSrc);
    h          = h * 31 + static_cast<uint32_t>(b.alphaDst);
    return h;
}
static uint64_t hashDepthState(const RenderTarget::DepthState & d) { return (static_cast<uint64_t>(static_cast<uint32_t>(d.func)) << 1) | (d.write ? 1u : 0u); }
static uint64_t hashStencilState(const RenderTarget::StencilState & s) {
    uint64_t h = 0;
    h          = h * 31 + static_cast<uint32_t>(s.compare);
    h          = h * 31 + static_cast<uint32_t>(s.pass);
    h          = h * 31 + static_cast<uint32_t>(s.fail);
    h          = h * 31 + static_cast<uint32_t>(s.zFail);
    h          = h * 31 + s.ref;
    h          = h * 31 + s.readMask;
    h          = h * 31 + s.writeMask;
    return h;
}

RenderTargetKey RenderTargetKey::make(const RenderTarget & renderTarget) {
    RenderTargetKey k;
    k.colorCount    = static_cast<uint8_t>(renderTarget.colors.size());
    uint64_t stateH = 0;
    for (size_t i = 0; i < renderTarget.colors.size() && k.colorCount < kMaxColorTargets; ++i) {
        const auto & c = renderTarget.colors[i];
        if (c.target.format == gfx::img::PixelFormat::UNKNOWN()) continue;
        k.colorFormats[k.colorCount++] = static_cast<uint32_t>(pixelFormatToVkFormat(c.target.format));
        stateH ^= hashBlendState(c.blendState) + (static_cast<uint64_t>(c.writeMask) << 32) + (i * 0x9e3779b9);
    }
    if (renderTarget.depthStencil.target && renderTarget.depthStencil.format != gfx::img::PixelFormat::UNKNOWN())
        k.depthFormat = static_cast<uint32_t>(pixelFormatToVkFormat(renderTarget.depthStencil.format));
    stateH ^= hashDepthState(renderTarget.depthStencil.depthState) * 31;
    stateH ^= hashStencilState(renderTarget.depthStencil.stencilState) * 31;
    k.stateHash = stateH;
    return k;
}

// --- RDG -> Vulkan state conversion (for pipeline create) ---
static vk::BlendFactor blendArgToVk(RenderTarget::BlendState::Arg a) {
    using Arg = RenderTarget::BlendState::Arg;
    switch (a) {
    case Arg::ZERO:
        return vk::BlendFactor::eZero;
    case Arg::ONE:
        return vk::BlendFactor::eOne;
    case Arg::SRC_COLOR:
        return vk::BlendFactor::eSrcColor;
    case Arg::INV_SRC_COLOR:
        return vk::BlendFactor::eOneMinusSrcColor;
    case Arg::SRC_ALPHA:
        return vk::BlendFactor::eSrcAlpha;
    case Arg::INV_SRC_ALPHA:
        return vk::BlendFactor::eOneMinusSrcAlpha;
    case Arg::DEST_ALPHA:
        return vk::BlendFactor::eDstAlpha;
    case Arg::INV_DEST_ALPHA:
        return vk::BlendFactor::eOneMinusDstAlpha;
    case Arg::DEST_COLOR:
        return vk::BlendFactor::eDstColor;
    case Arg::INV_DEST_COLOR:
        return vk::BlendFactor::eOneMinusDstColor;
    case Arg::BLEND_FACTOR:
        return vk::BlendFactor::eConstantColor;
    case Arg::INV_BLEND_FACTOR:
        return vk::BlendFactor::eOneMinusConstantColor;
    }
    return vk::BlendFactor::eOne;
}
static vk::BlendOp blendOpToVk(RenderTarget::BlendState::Op o) {
    using Op = RenderTarget::BlendState::Op;
    switch (o) {
    case Op::ADD:
        return vk::BlendOp::eAdd;
    case Op::SUB:
        return vk::BlendOp::eSubtract;
    case Op::REV_SUB:
        return vk::BlendOp::eReverseSubtract;
    case Op::MIN:
        return vk::BlendOp::eMin;
    case Op::MAX:
        return vk::BlendOp::eMax;
    }
    return vk::BlendOp::eAdd;
}
static vk::CompareOp compareToVk(RenderTarget::Compare c) {
    switch (c) {
    case RenderTarget::Compare::NEVER:
        return vk::CompareOp::eNever;
    case RenderTarget::Compare::LESS:
        return vk::CompareOp::eLess;
    case RenderTarget::Compare::LESS_EQUAL:
        return vk::CompareOp::eLessOrEqual;
    case RenderTarget::Compare::EQUAL:
        return vk::CompareOp::eEqual;
    case RenderTarget::Compare::GREATER_EQUAL:
        return vk::CompareOp::eGreaterOrEqual;
    case RenderTarget::Compare::GREATER:
        return vk::CompareOp::eGreater;
    case RenderTarget::Compare::NOT_EQUAL:
        return vk::CompareOp::eNotEqual;
    case RenderTarget::Compare::ALWAYS:
        return vk::CompareOp::eAlways;
    }
    return vk::CompareOp::eAlways;
}
static vk::StencilOp stencilOpToVk(RenderTarget::StencilState::Op o) {
    using Op = RenderTarget::StencilState::Op;
    switch (o) {
    case Op::KEEP:
        return vk::StencilOp::eKeep;
    case Op::ZERO:
        return vk::StencilOp::eZero;
    case Op::REPLACE:
        return vk::StencilOp::eReplace;
    case Op::INC_SAT:
        return vk::StencilOp::eIncrementAndClamp;
    case Op::DEC_SAT:
        return vk::StencilOp::eDecrementAndClamp;
    case Op::INVERT:
        return vk::StencilOp::eInvert;
    case Op::INC:
        return vk::StencilOp::eIncrementAndWrap;
    case Op::DEC:
        return vk::StencilOp::eDecrementAndWrap;
    }
    return vk::StencilOp::eKeep;
}
static vk::ColorComponentFlags writeMaskToVk(uint8_t w) {
    vk::ColorComponentFlags f;
    if (w & 1) f |= vk::ColorComponentFlagBits::eR;
    if (w & 2) f |= vk::ColorComponentFlagBits::eG;
    if (w & 4) f |= vk::ColorComponentFlagBits::eB;
    if (w & 8) f |= vk::ColorComponentFlagBits::eA;
    return f ? f : (vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA);
}

static void applyRenderTargetToPipeline(rapid_vulkan::GraphicsPipeline::ConstructParameters & cp, const RenderTarget & rt) {
    std::vector<vk::Format> colorFormats;
    for (size_t i = 0; i < rt.colors.size(); ++i) {
        if (rt.colors[i].target.format == gfx::img::PixelFormat::UNKNOWN()) continue;
        colorFormats.push_back(static_cast<vk::Format>(pixelFormatToVkFormat(rt.colors[i].target.format)));
    }
    vk::Format depthFormat = vk::Format::eUndefined;
    if (rt.depthStencil.target && rt.depthStencil.format != gfx::img::PixelFormat::UNKNOWN())
        depthFormat = static_cast<vk::Format>(pixelFormatToVkFormat(rt.depthStencil.format));
    if (!colorFormats.empty()) cp.setDynamicRendering(colorFormats, depthFormat);

    cp.attachments.clear();
    for (size_t i = 0; i < rt.colors.size(); ++i) {
        const auto & c = rt.colors[i];
        if (c.target.format == gfx::img::PixelFormat::UNKNOWN()) continue;
        const auto & b = c.blendState;
        auto         v = vk::PipelineColorBlendAttachmentState {}.setColorWriteMask(writeMaskToVk(c.writeMask));
        if (b.enabled()) {
            v.setBlendEnable(true)
                .setSrcColorBlendFactor(blendArgToVk(b.colorSrc))
                .setDstColorBlendFactor(blendArgToVk(b.colorDst))
                .setColorBlendOp(blendOpToVk(b.colorOp))
                .setSrcAlphaBlendFactor(blendArgToVk(b.alphaSrc))
                .setDstAlphaBlendFactor(blendArgToVk(b.alphaDst))
                .setAlphaBlendOp(blendOpToVk(b.alphaOp));
        }
        cp.attachments.push_back(v);
    }
    if (cp.attachments.empty()) {
        cp.attachments.push_back(vk::PipelineColorBlendAttachmentState().setColorWriteMask(vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
                                                                                           vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA));
    }

    const auto & ds = rt.depthStencil;
    cp.depth.setDepthTestEnable(ds.depthState.testEnabled())
        .setDepthWriteEnable(ds.depthState.writeEnabled())
        .setDepthCompareOp(compareToVk(ds.depthState.func));
    if (ds.stencilState.enabled()) {
        cp.depth.setStencilTestEnable(true)
            .setFront(vk::StencilOpState(stencilOpToVk(ds.stencilState.fail), stencilOpToVk(ds.stencilState.pass), stencilOpToVk(ds.stencilState.zFail),
                                         compareToVk(ds.stencilState.compare), ds.stencilState.readMask, ds.stencilState.writeMask, ds.stencilState.ref))
            .setBack(vk::StencilOpState(stencilOpToVk(ds.stencilState.fail), stencilOpToVk(ds.stencilState.pass), stencilOpToVk(ds.stencilState.zFail),
                                        compareToVk(ds.stencilState.compare), ds.stencilState.readMask, ds.stencilState.writeMask, ds.stencilState.ref));
    } else {
        cp.depth.setStencilTestEnable(false);
    }
    if (!rt.colors.empty() && rt.colors[0].blendState.enabled()) {
        const auto & v    = rt.colors[0].blendState.factors;
        cp.blendConstants = {v.x, v.y, v.z, v.w};
    }
}

// --- GpuGeometryKey ---
static uint64_t hashVertexFormat(const GpuGeometry::VertexFormat & fmt) {
    uint64_t h = 0;
    for (const auto & a : fmt.attributes) h = h * 31 + (uint64_t) a.location + ((uint64_t) a.offset << 8) + ((uint64_t) static_cast<uint8_t>(a.format) << 16);
    return h & ((1ULL << 27) - 1);
}

GpuGeometryKey GpuGeometryKey::make(const GpuGeometry & geometry) {
    GpuGeometryKey k;
    const bool     hasGeometry = !geometry.format.empty() && !geometry.vertices.empty() && geometry.vertices[0].stride > 0;
    k.noVertexInput            = hasGeometry ? 0 : 1;
    k.vertexStride             = hasGeometry ? (static_cast<uint64_t>(geometry.vertices[0].stride) & ((1ULL << 16) - 1)) : 0;
    k.vertexFormatHash         = hasGeometry ? hashVertexFormat(geometry.format) : 0;
    return k;
}

// --- makeKey ---
static GraphicsPsoKey makeKey(const GraphicsPsoCreateParams & params) {
    GraphicsPsoKey key;
    key.shaderKey                            = ShaderKey::makeGraphics(params.vs, params.ps);
    key.renderTargetKey                      = RenderTargetKey::make(params.renderTarget);
    key.geometryKey                          = GpuGeometryKey::make(params.geometry);
    key.descriptorLayoutKey.pushConstantSize = params.pushConstantSize;
    return key;
}

// --- Impl ---
struct ShaderPair {
    std::unique_ptr<rapid_vulkan::Shader> vs;
    std::unique_ptr<rapid_vulkan::Shader> fs;
};

class PsoFactoryVulkan::Impl {
public:
    GpuContextVulkan *                                                                    gpu = nullptr;
    std::unordered_map<GraphicsPsoKey, rapid_vulkan::Ref<rapid_vulkan::GraphicsPipeline>> cache;
    std::unordered_map<uint64_t, ShaderPair>                                              shaderCache;

    explicit Impl(GpuContextVulkan & gpu_): gpu(&gpu_) {}

    ShaderPair * getOrCreateShaders(uint64_t shaderHash, const GraphicsPsoCreateParams & params) {
        auto it = shaderCache.find(shaderHash);
        if (it != shaderCache.end()) return &it->second;
        const rapid_vulkan::GlobalInfo * gi = gpu->device().gi();
        if (!gi || !params.vs.binary || params.vs.size < 4 || (params.vs.size % 4) != 0) return nullptr;
        rapid_vulkan::Shader::ConstructParameters vcp;
        vcp.name = "pso-factory-vs";
        vcp.setGi(gi);
        vcp.setSpirv(params.vs.size / 4, static_cast<const uint32_t *>(params.vs.binary));
        vcp.entry = params.vs.entry ? params.vs.entry : "main";
        auto vs   = std::make_unique<rapid_vulkan::Shader>(vcp);
        if (!vs->handle()) {
            GN_WARN(sLogger)("PsoFactoryVulkan: failed to create vertex shader");
            return nullptr;
        }
        std::unique_ptr<rapid_vulkan::Shader> fs;
        if (params.ps.binary && params.ps.size >= 4 && (params.ps.size % 4) == 0) {
            rapid_vulkan::Shader::ConstructParameters fcp;
            fcp.name = "pso-factory-fs";
            fcp.setGi(gi);
            fcp.setSpirv(params.ps.size / 4, static_cast<const uint32_t *>(params.ps.binary));
            fcp.entry = params.ps.entry ? params.ps.entry : "main";
            fs        = std::make_unique<rapid_vulkan::Shader>(fcp);
            if (!fs->handle()) fs.reset();
        }
        ShaderPair p;
        p.vs     = std::move(vs);
        p.fs     = std::move(fs);
        auto ins = shaderCache.emplace(shaderHash, std::move(p));
        return &ins.first->second;
    }
};

PsoFactoryVulkan::PsoFactoryVulkan(GpuContextVulkan & gpu) { _impl = new Impl(gpu); }

PsoFactoryVulkan::~PsoFactoryVulkan() {
    _impl->cache.clear();
    _impl->shaderCache.clear();
    delete _impl;
    _impl = nullptr;
}

rapid_vulkan::Ref<const rapid_vulkan::GraphicsPipeline> PsoFactoryVulkan::getOrCreateGraphicsPso(const GraphicsPsoCreateParams & params) {
    if (!_impl || !_impl->gpu) {
        GN_ERROR(sLogger)("PsoFactoryVulkan: no impl or gpu");
        return {};
    }
    const GraphicsPsoKey key = makeKey(params);
    auto it = _impl->cache.find(key);
    if (it != _impl->cache.end()) return it->second;
    if (!params.vs.binary || params.vs.size == 0) {
        GN_ERROR(sLogger)("PsoFactoryVulkan: invalid vertex shader");
        return {};
    }
    ShaderPair * shaders = _impl->getOrCreateShaders(key.shaderKey.hash, params);
    if (!shaders || !shaders->vs) {
        GN_ERROR(sLogger)("PsoFactoryVulkan: failed to create shaders");
        return {};
    }
    const rapid_vulkan::GlobalInfo * gi = _impl->gpu->device().gi();
    if (!gi) {
        GN_ERROR(sLogger)("PsoFactoryVulkan: no GlobalInfo");
        return {};
    }
    rapid_vulkan::GraphicsPipeline::ConstructParameters cp;
    cp.setName("pso-factory-pipeline");
    cp.setVS(shaders->vs.get());
    cp.setFS(shaders->fs.get());
    applyRenderTargetToPipeline(cp, params.renderTarget);
    cp.dynamicViewport(1);
    cp.dynamicScissor(1);
    if (!key.geometryKey.noVertexInput && !params.geometry.format.empty() && !params.geometry.vertices.empty()) {
        const uint32_t stride = static_cast<uint32_t>(params.geometry.vertices[0].stride);
        cp.vb.push_back({0, stride, vk::VertexInputRate::eVertex});
        for (const auto & a : params.geometry.format.attributes) cp.va.push_back({a.location, 0, vertexAttributeFormatToVk(a.format), a.offset});
    }
    rapid_vulkan::Ref<rapid_vulkan::GraphicsPipeline> pipe(new rapid_vulkan::GraphicsPipeline(cp));
    if (!pipe->handle()) {
        GN_ERROR(sLogger)("PsoFactoryVulkan: failed to create graphics pipeline");
        return {};
    }
    _impl->cache[key] = pipe;
    return pipe;
}

} // namespace GN::rdg
