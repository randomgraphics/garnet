#include "pch.h"
#include "vk-raster-pso-factory.h"
#include "vk-format-utils.h"

#include <algorithm>
#include <cstdint>
#include <string>
#include <unordered_map>

static GN::Logger * sLogger = GN::getLogger("GN.gpu2.vk.pso");

namespace GN::gpu2 {

// ── State → Vulkan conversion helpers ────────────────────────────────────────

static vk::CompareOp compareToVk(RasterState::Compare c) {
    switch (c) {
    case RasterState::Compare::NEVER:
        return vk::CompareOp::eNever;
    case RasterState::Compare::LESS:
        return vk::CompareOp::eLess;
    case RasterState::Compare::LESS_EQUAL:
        return vk::CompareOp::eLessOrEqual;
    case RasterState::Compare::EQUAL:
        return vk::CompareOp::eEqual;
    case RasterState::Compare::GREATER_EQUAL:
        return vk::CompareOp::eGreaterOrEqual;
    case RasterState::Compare::GREATER:
        return vk::CompareOp::eGreater;
    case RasterState::Compare::NOT_EQUAL:
        return vk::CompareOp::eNotEqual;
    case RasterState::Compare::ALWAYS:
        return vk::CompareOp::eAlways;
    default:
        return vk::CompareOp::eAlways;
    }
}

static vk::StencilOp stencilOpToVk(RasterState::StencilState::Op op) {
    switch (op) {
    case RasterState::StencilState::KEEP:
        return vk::StencilOp::eKeep;
    case RasterState::StencilState::ZERO:
        return vk::StencilOp::eZero;
    case RasterState::StencilState::REPLACE:
        return vk::StencilOp::eReplace;
    case RasterState::StencilState::INC_SAT:
        return vk::StencilOp::eIncrementAndClamp;
    case RasterState::StencilState::DEC_SAT:
        return vk::StencilOp::eDecrementAndClamp;
    case RasterState::StencilState::INVERT:
        return vk::StencilOp::eInvert;
    case RasterState::StencilState::INC:
        return vk::StencilOp::eIncrementAndWrap;
    case RasterState::StencilState::DEC:
        return vk::StencilOp::eDecrementAndWrap;
    default:
        return vk::StencilOp::eKeep;
    }
}

static vk::BlendFactor blendArgToVk(RasterTarget::BlendState::Arg a) {
    using Arg = RasterTarget::BlendState::Arg;
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
    default:
        return vk::BlendFactor::eOne;
    }
}

static vk::BlendOp blendOpToVk(RasterTarget::BlendState::Op o) {
    using Op = RasterTarget::BlendState::Op;
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
    default:
        return vk::BlendOp::eAdd;
    }
}

static vk::ColorComponentFlags writeMaskToVk(uint8_t w) {
    vk::ColorComponentFlags f;
    if (w & 1) f |= vk::ColorComponentFlagBits::eR;
    if (w & 2) f |= vk::ColorComponentFlagBits::eG;
    if (w & 4) f |= vk::ColorComponentFlagBits::eB;
    if (w & 8) f |= vk::ColorComponentFlagBits::eA;
    // Default to full write mask if nothing is set.
    return f ? f : (vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA);
}

// ── Gpu2RasterPsoKey ──────────────────────────────────────────────────────────
//
// Self-contained, compact key for the gpu2 raster PSO cache.
// Every field that affects VkGraphicsPipeline creation appears here exactly once.
// Viewport and scissor are always dynamic state and are NOT keyed.

struct Gpu2RasterPsoKey {
    uint64_t shaderHash = 0; ///< FNV-multiply of vs->id and ps->id

    union {
        uint64_t geomWord = 0;
        struct {
            uint64_t noInput     : 1; ///< 1 = fullscreen triangle, no VBOs
            uint64_t numBindings : 3;
            uint64_t stride0     : 12; ///< binding 0 stride in bytes (max 4095)
            uint64_t stride1     : 12;
            uint64_t stride2     : 12;
            uint64_t instanced0  : 1; ///< binding is per-instance
            uint64_t instanced1  : 1;
            uint64_t instanced2  : 1;
            uint64_t numAttribs  : 5;
            uint64_t attrHash    : 16; ///< hash of {location, binding, offset, format} × N
        };
    };

    union {
        uint64_t stateWord = 0;
        struct {
            uint64_t fillMode       : 2;
            uint64_t cullMode       : 2;
            uint64_t frontFace      : 1;
            uint64_t depthFunc      : 3;
            uint64_t depthWrite     : 1;
            uint64_t stencilEnable  : 1;
            uint64_t stencilCompare : 3;
            uint64_t stencilPass    : 3;
            uint64_t stencilFail    : 3;
            uint64_t stencilZFail   : 3;
            uint64_t stencilRef     : 8;
            uint64_t stencilRdMask  : 8;
            uint64_t stencilWrMask  : 8;
            uint64_t _statePad      : 18;
        };
    };

    uint16_t colorFmts[8] = {};
    uint16_t depthFmt     = 0;
    uint8_t  colorCount   = 0;
    uint8_t  _rtPad       = 0;
    uint64_t blendHash    = 0;

    bool operator==(const Gpu2RasterPsoKey & o) const noexcept;

    static Gpu2RasterPsoKey make(const GpuShaderVulkan & vs, const GpuShaderVulkan * ps, const RasterState & state, const RasterGeometry & geom,
                                 const PassFormats & formats, const StackArray<RasterTarget::ColorTarget, 8> & colorTargets);
};

bool Gpu2RasterPsoKey::operator==(const Gpu2RasterPsoKey & o) const noexcept {
    if (shaderHash != o.shaderHash || geomWord != o.geomWord || stateWord != o.stateWord) return false;
    if (colorCount != o.colorCount || depthFmt != o.depthFmt || blendHash != o.blendHash) return false;
    // Only compare populated color format slots so unused trailing slots don't matter.
    return std::equal(colorFmts, colorFmts + colorCount, o.colorFmts);
}

Gpu2RasterPsoKey Gpu2RasterPsoKey::make(const GpuShaderVulkan & vs, const GpuShaderVulkan * ps, const RasterState & rs, const RasterGeometry & geom,
                                        const PassFormats & formats, const StackArray<RasterTarget::ColorTarget, 8> & colorTargets) {
    Gpu2RasterPsoKey k {};

    // ── 1. Shader identity ────────────────────────────────────────────────────
    // Combine VS and PS ids using FNV multiply to avoid trivial cancellations.
    k.shaderHash = vs.id * 0x100000001b3ULL ^ (ps ? ps->id : 0ULL);

    // ── 2. Vertex input ───────────────────────────────────────────────────────
    const bool hasInput = !geom.format.attributes.empty() && !geom.vertices.empty();
    if (!hasInput) {
        k.noInput = 1;
    } else {
        size_t bi = 0; // binding index
        for (const auto & vb : geom.vertices) {
            if (bi == 0)
                k.stride0 = vb.stride & 0xFFF;
            else if (bi == 1)
                k.stride1 = vb.stride & 0xFFF;
            else if (bi == 2)
                k.stride2 = vb.stride & 0xFFF;
            ++bi;
        }
        for (const auto & ib : geom.instances) {
            if (bi == 0) {
                k.stride0    = ib.stride & 0xFFF;
                k.instanced0 = 1;
            } else if (bi == 1) {
                k.stride1    = ib.stride & 0xFFF;
                k.instanced1 = 1;
            } else if (bi == 2) {
                k.stride2    = ib.stride & 0xFFF;
                k.instanced2 = 1;
            }
            ++bi;
        }
        k.numBindings = (uint64_t) std::min(bi, (size_t) 7);

        const auto & attrs = geom.format.attributes;
        k.numAttribs       = (uint64_t) std::min(attrs.size(), (size_t) 31);
        // Pack (location, binding, offset, format) into a running product hash.
        uint64_t ah = 0;
        for (const auto & a : attrs)
            ah = ah * 31 + a.location + ((uint64_t) a.binding << 5) + ((uint64_t) a.offset << 9) + ((uint64_t) (uint8_t) a.format << 21);
        k.attrHash = ah & 0xFFFF;
    }

    // ── 3. Rasterization + depth/stencil ─────────────────────────────────────
    // The merged StoredDraw::states always has all fields populated (RasterTarget::states
    // starts as WithDefaults and draw overrides are merged on top), so dereferencing is safe.
    if (rs.fillMode) k.fillMode = (uint64_t) *rs.fillMode;
    if (rs.cullMode) k.cullMode = (uint64_t) *rs.cullMode;
    if (rs.frontFace) k.frontFace = (uint64_t) *rs.frontFace;
    if (rs.depthState) {
        k.depthFunc  = (uint64_t) rs.depthState->func;
        k.depthWrite = rs.depthState->write ? 1u : 0u;
    }
    if (rs.stencilState) {
        const auto & ss  = *rs.stencilState;
        k.stencilEnable  = ss.enabled() ? 1u : 0u;
        k.stencilCompare = (uint64_t) ss.compare;
        k.stencilPass    = (uint64_t) ss.pass;
        k.stencilFail    = (uint64_t) ss.fail;
        k.stencilZFail   = (uint64_t) ss.zFail;
        k.stencilRef     = ss.ref;
        k.stencilRdMask  = ss.readMask;
        k.stencilWrMask  = ss.writeMask;
    }

    // ── 4. Render-target formats ──────────────────────────────────────────────
    k.colorCount = (uint8_t) std::min(formats.colors.size(), (size_t) 8);
    for (uint8_t i = 0; i < k.colorCount; ++i) k.colorFmts[i] = (uint16_t) (uint32_t) formats.colors[i];
    k.depthFmt = (uint16_t) (uint32_t) formats.depth;

    // ── 5. Blend state hash ───────────────────────────────────────────────────
    uint64_t bh  = 0xcbf29ce484222325ULL;
    auto     mix = [&](uint64_t v) { bh = (bh ^ v) * 0x100000001b3ULL; };
    for (size_t i = 0; i < colorTargets.size(); ++i) {
        const auto & ct = colorTargets[i];
        const auto & bs = ct.blendState;
        mix((uint64_t) bs.enabled());
        mix((uint64_t) bs.colorOp);
        mix((uint64_t) bs.colorSrc);
        mix((uint64_t) bs.colorDst);
        mix((uint64_t) bs.alphaOp);
        mix((uint64_t) bs.alphaSrc);
        mix((uint64_t) bs.alphaDst);
        mix(ct.writeMask);
    }
    k.blendHash = bh;

    return k;
}

} // namespace GN::gpu2

namespace std {

template<>
struct hash<GN::gpu2::Gpu2RasterPsoKey> {
    size_t operator()(const GN::gpu2::Gpu2RasterPsoKey & k) const noexcept {
        auto   mix = [](size_t h, uint64_t v) -> size_t { return h ^ (std::hash<uint64_t> {}(v) + 0x9e3779b9 + (h << 6) + (h >> 2)); };
        size_t h   = std::hash<uint64_t> {}(k.shaderHash);
        h          = mix(h, k.geomWord);
        h          = mix(h, k.stateWord);
        // Pack 4 pairs of uint16_t color formats into uint64_t words for hashing.
        for (int i = 0; i < 4; ++i) h = mix(h, (uint64_t) k.colorFmts[2 * i] | ((uint64_t) k.colorFmts[2 * i + 1] << 16));
        h = mix(h, (uint64_t) k.depthFmt | ((uint64_t) k.colorCount << 16));
        h = mix(h, k.blendHash);
        return h;
    }
};

} // namespace std

namespace GN::gpu2 {

// ── RasterPsoFactory::Impl ────────────────────────────────────────────────────

class RasterPsoFactory::Impl {
public:
    GpuContextVulkan2 &                                                 gpu;
    std::unordered_map<Gpu2RasterPsoKey, rv::Ref<rv::GraphicsPipeline>> cache;

    explicit Impl(GpuContextVulkan2 & g): gpu(g) {}
};

// ── Pipeline construction ─────────────────────────────────────────────────────

static rv::Ref<rv::GraphicsPipeline> buildPipeline(const Gpu2RasterPsoCreateParams & params) {
    rv::GraphicsPipeline::ConstructParameters gcp;
    gcp.setName("gpu2_raster_pso");

    // Dynamic rendering: pass resolved formats from the active render pass.
    gcp.setDynamicRendering(vk::ArrayProxy<const vk::Format>((uint32_t) params.formats.colors.size(), params.formats.colors.data()), params.formats.depth);

    // Shaders.
    gcp.setVS(params.vs->rvShader());
    if (params.ps && params.ps->rvShader()) gcp.setFS(params.ps->rvShader());

    // Viewport/scissor are always dynamic in this backend.
    gcp.dynamicViewport(1).dynamicScissor(1);

    // Vertex input: vertex buffers first, then instance buffers (mirrors binding index assignment).
    for (const auto & vb : params.geometry.vertices) gcp.addVertexBuffer(vb.stride);
    for (const auto & ib : params.geometry.instances) gcp.addInstanceBuffer(ib.stride);
    for (const auto & a : params.geometry.format.attributes) gcp.setVertexAttribute(a.location, a.binding, a.offset, vertexAttributeFormatToVk(a.format));

    // Per-color-target blend state and write mask.
    // Must match the color format count passed to setDynamicRendering.
    gcp.attachments.clear();
    for (size_t i = 0; i < params.colorTargets.size(); ++i) {
        const auto &                          ct  = params.colorTargets[i];
        const auto &                          bs  = ct.blendState;
        vk::PipelineColorBlendAttachmentState att = vk::PipelineColorBlendAttachmentState {}.setColorWriteMask(writeMaskToVk(ct.writeMask));
        if (bs.enabled()) {
            att.setBlendEnable(true)
                .setSrcColorBlendFactor(blendArgToVk(bs.colorSrc))
                .setDstColorBlendFactor(blendArgToVk(bs.colorDst))
                .setColorBlendOp(blendOpToVk(bs.colorOp))
                .setSrcAlphaBlendFactor(blendArgToVk(bs.alphaSrc))
                .setDstAlphaBlendFactor(blendArgToVk(bs.alphaDst))
                .setAlphaBlendOp(blendOpToVk(bs.alphaOp));
        }
        gcp.attachments.push_back(att);
    }

    // Rasterization state.
    const RasterState & rs = params.state;
    if (rs.fillMode && *rs.fillMode == RasterState::FILL_WIREFRAME) gcp.rast.setPolygonMode(vk::PolygonMode::eLine);
    if (rs.cullMode) {
        switch (*rs.cullMode) {
        case RasterState::CULL_NONE:
            gcp.rast.setCullMode(vk::CullModeFlagBits::eNone);
            break;
        case RasterState::CULL_FRONT:
            gcp.rast.setCullMode(vk::CullModeFlagBits::eFront);
            break;
        case RasterState::CULL_BACK:
            gcp.rast.setCullMode(vk::CullModeFlagBits::eBack);
            break;
        }
    }
    if (rs.frontFace && *rs.frontFace == RasterState::FRONT_CW) gcp.rast.setFrontFace(vk::FrontFace::eClockwise);

    // Depth state.
    if (rs.depthState) {
        const auto & ds = *rs.depthState;
        gcp.depth.setDepthTestEnable(ds.testEnabled()).setDepthWriteEnable(ds.writeEnabled()).setDepthCompareOp(compareToVk(ds.func));
    }

    // Stencil state.
    if (rs.stencilState) {
        const auto &       ss   = *rs.stencilState;
        vk::StencilOpState face = vk::StencilOpState()
                                      .setFailOp(stencilOpToVk(ss.fail))
                                      .setPassOp(stencilOpToVk(ss.pass))
                                      .setDepthFailOp(stencilOpToVk(ss.zFail))
                                      .setCompareOp(compareToVk(ss.compare))
                                      .setCompareMask(ss.readMask)
                                      .setWriteMask(ss.writeMask)
                                      .setReference(ss.ref);
        // RasterState has no per-face stencil distinction; apply the same to both faces.
        gcp.depth.setStencilTestEnable(ss.enabled()).setFront(face).setBack(face);
    }

    rv::Ref<rv::GraphicsPipeline> pipe(new rv::GraphicsPipeline(gcp));
    if (!pipe->handle()) {
        GN_ERROR(sLogger, "RasterPsoFactory: failed to create VkPipeline");
        return {};
    }
    return pipe;
}

// ── RasterPsoFactory ──────────────────────────────────────────────────────────

RasterPsoFactory::RasterPsoFactory(GpuContextVulkan2 & gpu) { mImpl = new Impl(gpu); }

RasterPsoFactory::~RasterPsoFactory() {
    delete mImpl;
    mImpl = nullptr;
}

rv::Ref<const rv::GraphicsPipeline> RasterPsoFactory::getOrCreate(const Gpu2RasterPsoCreateParams & params) {
    if (!params.vs) GN_UNLIKELY {
            GN_ERROR(sLogger, "RasterPsoFactory::getOrCreate: null vertex shader");
            return {};
        }

    const Gpu2RasterPsoKey key = Gpu2RasterPsoKey::make(*params.vs, params.ps, params.state, params.geometry, params.formats, params.colorTargets);

    auto it = mImpl->cache.find(key);
    if (it != mImpl->cache.end()) return it->second;

    rv::Ref<rv::GraphicsPipeline> pipe = buildPipeline(params);
    if (!pipe) return {};

    GN_VVTRACE(sLogger, "RasterPsoFactory: created new PSO (cache size now {})", mImpl->cache.size() + 1);
    mImpl->cache.emplace(key, pipe);
    return pipe;
}

size_t RasterPsoFactory::cacheSize() const { return mImpl ? mImpl->cache.size() : 0; }

} // namespace GN::gpu2
