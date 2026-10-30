#include "pch.h"
#include "vk-bindless-pso-cache.h"
#include "vk-format-utils.h"

static GN::Logger * sLogger = GN::getLogger("GN.gpu2.vk.bindless.pso");

namespace GN::gpu2 {

static inline uint64_t hashMix(uint64_t h, uint64_t v) noexcept { return h ^ (v + 0x9e3779b97f4a7c15ULL + (h << 6) + (h >> 2)); }

bool BindlessPsoKey::operator==(const BindlessPsoKey & o) const noexcept {
    if (pipelineLayout != o.pipelineLayout || shaderHash != o.shaderHash || geomWord != o.geomWord || stateWord != o.stateWord || colorCount != o.colorCount ||
        depthFmt != o.depthFmt || blendHash != o.blendHash) {
        return false;
    }
    return std::equal(colorFmts, colorFmts + colorCount, o.colorFmts);
}

BindlessPsoKey BindlessPsoKey::make(vk::PipelineLayout layout, const GpuShaderVulkan & vs, const GpuShaderVulkan * hs, const GpuShaderVulkan * ds,
                                    const GpuShaderVulkan * gs, const GpuShaderVulkan * ps, const RasterState & rs, const RasterGeometry & geom,
                                    const PassFormats & formats, const StackArray<RasterTarget::ColorTarget, 8> & colorTargets) {
    BindlessPsoKey k {};
    k.pipelineLayout = layout;

    // Shader hash
    uint64_t sh = uint64_t(vs.id);
    if (hs) sh = hashMix(sh, uint64_t(hs->id));
    if (ds) sh = hashMix(sh, uint64_t(ds->id));
    if (gs) sh = hashMix(sh, uint64_t(gs->id));
    if (ps) sh = hashMix(sh, uint64_t(ps->id));
    k.shaderHash = sh;

    // Vertex input
    const bool hasInput = !geom.format.attributes.empty() && !geom.vertices.empty();
    if (!hasInput) {
        k.noInput = 1;
    } else {
        size_t bi = 0;
        for (const auto & vb : geom.vertices) {
            if (bi == 0) {
                k.stride0 = vb.stride & 0xFFF;
            } else if (bi == 1) {
                k.stride1 = vb.stride & 0xFFF;
            } else if (bi == 2) {
                k.stride2 = vb.stride & 0xFFF;
            }
            ++bi;
        }
        k.numBindings = uint32_t(std::min(geom.vertices.size(), size_t(7)));
        k.numAttribs  = uint32_t(std::min(geom.format.attributes.size(), size_t(31)));

        uint64_t ah = 0;
        for (const auto & a : geom.format.attributes) {
            ah = hashMix(ah, (uint64_t(a.location) << 24) | (uint64_t(a.binding) << 16) | (uint64_t(a.offset) << 8) | uint64_t(a.format));
        }
        k.attrHash = ah & 0xFFFF;
    }

    // Rasterization state
    if (rs.fillMode) k.fillMode = uint64_t(*rs.fillMode);
    if (rs.cullMode) k.cullMode = uint64_t(*rs.cullMode);
    if (rs.frontFace) k.frontFace = uint64_t(*rs.frontFace);

    // Depth & Stencil
    if (rs.depthState) {
        k.depthFunc  = uint64_t(rs.depthState->func);
        k.depthWrite = rs.depthState->write ? 1 : 0;
    }
    if (rs.stencilState) {
        const auto & ss  = *rs.stencilState;
        k.stencilEnable  = ss.enabled() ? 1 : 0;
        k.stencilCompare = uint64_t(ss.compare);
        k.stencilPass    = uint64_t(ss.pass);
        k.stencilFail    = uint64_t(ss.fail);
        k.stencilZFail   = uint64_t(ss.zFail);
        k.stencilRef     = ss.ref;
        k.stencilRdMask  = ss.readMask;
        k.stencilWrMask  = ss.writeMask;
    }

    // Dynamic rendering formats
    k.colorCount = uint8_t(std::min(formats.colors.size(), size_t(8)));
    for (size_t i = 0; i < k.colorCount; ++i) { k.colorFmts[i] = static_cast<uint16_t>(formats.colors[i]); }
    k.depthFmt = static_cast<uint16_t>(formats.depth);

    // Blend state hash
    uint64_t bh = 0;
    for (size_t i = 0; i < colorTargets.size(); ++i) {
        const auto & bs     = colorTargets[i].blendState;
        const auto   wm     = colorTargets[i].writeMask;
        uint64_t     packed = (uint64_t(wm) << 24) | (uint64_t(bs.colorOp) << 20) | (uint64_t(bs.colorSrc) << 16) | (uint64_t(bs.colorDst) << 12) |
                              (uint64_t(bs.alphaOp) << 8) | (uint64_t(bs.alphaSrc) << 4) | uint64_t(bs.alphaDst);
        bh                  = hashMix(bh, packed);
    }
    k.blendHash = bh;

    return k;
}

size_t BindlessPsoKeyHash::operator()(const BindlessPsoKey & k) const noexcept {
    uint64_t h = std::hash<uint64_t>()((uint64_t) (VkPipelineLayout) k.pipelineLayout);
    h          = hashMix(h, k.shaderHash);
    h          = hashMix(h, k.geomWord);
    h          = hashMix(h, k.stateWord);
    for (int i = 0; i < 4; ++i) { h = hashMix(h, (uint64_t) k.colorFmts[2 * i] | ((uint64_t) k.colorFmts[2 * i + 1] << 16)); }
    h = hashMix(h, (uint64_t) k.depthFmt | ((uint64_t) k.colorCount << 16));
    h = hashMix(h, k.blendHash);
    return size_t(h);
}

VkBindlessPsoCache::VkBindlessPsoCache(GpuContextVulkan2 & gpu): mGpu(gpu) {}

VkBindlessPsoCache::~VkBindlessPsoCache() {
    auto vkDev = mGpu.vulkanDevice().handle();
    for (auto & [k, pipeline] : mCache) {
        if (pipeline) { vkDev.destroyPipeline(pipeline); }
    }
    mCache.clear();
}

vk::Pipeline VkBindlessPsoCache::getOrCreate(vk::PipelineLayout layout, const GpuShaderVulkan * vs, const GpuShaderVulkan * hs, const GpuShaderVulkan * ds,
                                             const GpuShaderVulkan * gs, const GpuShaderVulkan * ps, const RasterState & state, const RasterGeometry & geom,
                                             const PassFormats & formats, const StackArray<RasterTarget::ColorTarget, 8> & colorTargets) {
    if (!vs || !vs->rvShader()) GN_UNLIKELY {
            GN_ERROR(sLogger, "VkBindlessPsoCache::getOrCreate: null vertex shader");
            return vk::Pipeline {};
        }

    const BindlessPsoKey key = BindlessPsoKey::make(layout, *vs, hs, ds, gs, ps, state, geom, formats, colorTargets);

    std::lock_guard<std::mutex> lock(mMutex);
    auto                        it = mCache.find(key);
    if (it != mCache.end()) return it->second;

    vk::Pipeline pipe = buildPipeline(layout, vs, hs, ds, gs, ps, state, geom, formats, colorTargets);
    if (pipe) { mCache.emplace(key, pipe); }
    return pipe;
}

vk::Pipeline VkBindlessPsoCache::buildPipeline(vk::PipelineLayout layout, const GpuShaderVulkan * vs, const GpuShaderVulkan * hs, const GpuShaderVulkan * ds,
                                               const GpuShaderVulkan * gs, const GpuShaderVulkan * ps, const RasterState & state, const RasterGeometry & geom,
                                               const PassFormats & formats, const StackArray<RasterTarget::ColorTarget, 8> & colorTargets) {
    auto vkDev = mGpu.vulkanDevice().handle();

    // 1. Shaders
    std::vector<vk::PipelineShaderStageCreateInfo> stages;
    stages.push_back(vk::PipelineShaderStageCreateInfo({}, vk::ShaderStageFlagBits::eVertex, vs->rvShader()->handle(), "main"));
    if (hs && hs->rvShader()) {
        stages.push_back(vk::PipelineShaderStageCreateInfo({}, vk::ShaderStageFlagBits::eTessellationControl, hs->rvShader()->handle(), "main"));
    }
    if (ds && ds->rvShader()) {
        stages.push_back(vk::PipelineShaderStageCreateInfo({}, vk::ShaderStageFlagBits::eTessellationEvaluation, ds->rvShader()->handle(), "main"));
    }
    if (gs && gs->rvShader()) { stages.push_back(vk::PipelineShaderStageCreateInfo({}, vk::ShaderStageFlagBits::eGeometry, gs->rvShader()->handle(), "main")); }
    if (ps && ps->rvShader()) { stages.push_back(vk::PipelineShaderStageCreateInfo({}, vk::ShaderStageFlagBits::eFragment, ps->rvShader()->handle(), "main")); }

    // 2. Vertex input
    std::vector<vk::VertexInputBindingDescription> bindings;
    bindings.reserve(geom.vertices.size());
    for (uint32_t i = 0; i < geom.vertices.size(); ++i) { bindings.emplace_back(i, geom.vertices[i].stride, vk::VertexInputRate::eVertex); }
    std::vector<vk::VertexInputAttributeDescription> attribs;
    attribs.reserve(geom.format.attributes.size());
    for (const auto & a : geom.format.attributes) { attribs.emplace_back(a.location, a.binding, vertexAttributeFormatToVk(a.format), a.offset); }
    vk::PipelineVertexInputStateCreateInfo vertexInputInfo({}, static_cast<uint32_t>(bindings.size()), bindings.data(), static_cast<uint32_t>(attribs.size()),
                                                           attribs.data());

    // 3. Input assembly
    vk::PipelineInputAssemblyStateCreateInfo inputAssembly({}, vk::PrimitiveTopology::eTriangleList, VK_FALSE);

    // 4. Dynamic state (viewport + scissor)
    vk::DynamicState                   dynamicStates[2] = {vk::DynamicState::eViewport, vk::DynamicState::eScissor};
    vk::PipelineDynamicStateCreateInfo dynamicState({}, 2, dynamicStates);

    vk::PipelineViewportStateCreateInfo viewportState({}, 1, nullptr, 1, nullptr);

    // 5. Rasterization
    vk::PipelineRasterizationStateCreateInfo rasterizer({}, VK_FALSE, VK_FALSE, vk::PolygonMode::eFill, vk::CullModeFlagBits::eNone,
                                                        vk::FrontFace::eCounterClockwise, VK_FALSE, 0.0f, 0.0f, 0.0f, 1.0f);
    if (state.fillMode && *state.fillMode == RasterState::FILL_WIREFRAME) { rasterizer.setPolygonMode(vk::PolygonMode::eLine); }
    if (state.cullMode) {
        switch (*state.cullMode) {
        case RasterState::CULL_NONE:
            rasterizer.setCullMode(vk::CullModeFlagBits::eNone);
            break;
        case RasterState::CULL_FRONT:
            rasterizer.setCullMode(vk::CullModeFlagBits::eFront);
            break;
        case RasterState::CULL_BACK:
            rasterizer.setCullMode(vk::CullModeFlagBits::eBack);
            break;
        }
    }
    if (state.frontFace && *state.frontFace == RasterState::FRONT_CW) { rasterizer.setFrontFace(vk::FrontFace::eClockwise); }

    // 6. Multisampling
    vk::PipelineMultisampleStateCreateInfo multisampling({}, vk::SampleCountFlagBits::e1, VK_FALSE);

    // 7. Depth Stencil
    vk::PipelineDepthStencilStateCreateInfo depthStencil;
    if (state.depthState) {
        const auto & depth = *state.depthState;
        depthStencil.setDepthTestEnable(depth.testEnabled()).setDepthWriteEnable(depth.writeEnabled()).setDepthCompareOp(compareToVk(depth.func));
    }
    if (state.stencilState) {
        const auto &       ss   = *state.stencilState;
        vk::StencilOpState face = vk::StencilOpState()
                                      .setFailOp(stencilOpToVk(ss.fail))
                                      .setPassOp(stencilOpToVk(ss.pass))
                                      .setDepthFailOp(stencilOpToVk(ss.zFail))
                                      .setCompareOp(compareToVk(ss.compare))
                                      .setCompareMask(ss.readMask)
                                      .setWriteMask(ss.writeMask)
                                      .setReference(ss.ref);
        depthStencil.setStencilTestEnable(ss.enabled()).setFront(face).setBack(face);
    }

    // 8. Blend state
    std::vector<vk::PipelineColorBlendAttachmentState> blendAttachments;
    blendAttachments.reserve(colorTargets.size());
    for (size_t i = 0; i < colorTargets.size(); ++i) {
        const auto &                          ct  = colorTargets[i];
        const auto &                          bs  = ct.blendState;
        vk::PipelineColorBlendAttachmentState att = {};
        att.setColorWriteMask(writeMaskToVk(ct.writeMask));
        if (bs.enabled()) {
            att.setBlendEnable(VK_TRUE)
                .setSrcColorBlendFactor(blendArgToVk(bs.colorSrc))
                .setDstColorBlendFactor(blendArgToVk(bs.colorDst))
                .setColorBlendOp(blendOpToVk(bs.colorOp))
                .setSrcAlphaBlendFactor(blendArgToVk(bs.alphaSrc))
                .setDstAlphaBlendFactor(blendArgToVk(bs.alphaDst))
                .setAlphaBlendOp(blendOpToVk(bs.alphaOp));
        }
        blendAttachments.push_back(att);
    }
    vk::PipelineColorBlendStateCreateInfo colorBlending({}, VK_FALSE, vk::LogicOp::eCopy, static_cast<uint32_t>(blendAttachments.size()),
                                                        blendAttachments.data());

    // 9. Dynamic rendering extension structure
    vk::PipelineRenderingCreateInfo renderingInfo(0, static_cast<uint32_t>(formats.colors.size()), formats.colors.data(), formats.depth,
                                                  vk::Format::eUndefined);

    // 10. Pipeline create info
    vk::GraphicsPipelineCreateInfo pipelineInfo({}, static_cast<uint32_t>(stages.size()), stages.data(), &vertexInputInfo, &inputAssembly, nullptr,
                                                &viewportState, &rasterizer, &multisampling, &depthStencil, &colorBlending, &dynamicState, layout,
                                                VK_NULL_HANDLE, 0);
    pipelineInfo.setPNext(&renderingInfo);

    try {
        auto res = vkDev.createGraphicsPipeline(VK_NULL_HANDLE, pipelineInfo);
        if (res.result != vk::Result::eSuccess) {
            GN_ERROR(sLogger, "VkBindlessPsoCache: createGraphicsPipeline failed: {}", vk::to_string(res.result));
            return vk::Pipeline {};
        }
        return res.value;
    } catch (const std::exception & e) {
        GN_ERROR(sLogger, "VkBindlessPsoCache: createGraphicsPipeline exception: {}", e.what());
        return vk::Pipeline {};
    }
}

size_t VkBindlessPsoCache::cacheSize() const { return mCache.size(); }

} // namespace GN::gpu2
