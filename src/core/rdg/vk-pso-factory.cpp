#include "pch.h"
#include "vk-pso-factory.h"
#include <unordered_map>
#include <memory>
#include <string>

namespace GN::rdg {

static GN::Logger * sLogger = GN::getLogger("GN.rdg.vk-pso-factory");

struct ShaderPair {
    std::unique_ptr<rapid_vulkan::Shader> vs;
    std::unique_ptr<rapid_vulkan::Shader> fs;
};

class PsoFactoryVulkan::Impl {
public:
    GpuContextVulkan *                                                                    gpu = nullptr;
    std::unordered_map<GraphicsPsoKey, rapid_vulkan::Ref<rapid_vulkan::GraphicsPipeline>> cache;
    std::unordered_map<uint64_t, ShaderPair>                                               shaderCache;

    explicit Impl(GpuContextVulkan & gpu_) : gpu(&gpu_) {}

    ShaderPair * getOrCreateShaders(uint64_t shaderHash, const GraphicsPsoCreateParams & params) {
        auto it = shaderCache.find(shaderHash);
        if (it != shaderCache.end()) return &it->second;
        const rapid_vulkan::GlobalInfo * gi = gpu->device().gi();
        if (!gi || !params.vsSpirv || params.vsSize < 4 || (params.vsSize % 4) != 0) return nullptr;
        rapid_vulkan::Shader::ConstructParameters vcp;
        vcp.name = "pso-factory-vs";
        vcp.setGi(gi);
        vcp.setSpirv(params.vsSize / 4, static_cast<const uint32_t *>(params.vsSpirv));
        vcp.entry = params.vsEntry ? params.vsEntry : "main";
        auto vs = std::make_unique<rapid_vulkan::Shader>(vcp);
        if (!vs->handle()) {
            GN_WARN(sLogger)("PsoFactoryVulkan: failed to create vertex shader");
            return nullptr;
        }
        std::unique_ptr<rapid_vulkan::Shader> fs;
        if (params.psSpirv && params.psSize >= 4 && (params.psSize % 4) == 0) {
            rapid_vulkan::Shader::ConstructParameters fcp;
            fcp.name = "pso-factory-fs";
            fcp.setGi(gi);
            fcp.setSpirv(params.psSize / 4, static_cast<const uint32_t *>(params.psSpirv));
            fcp.entry = params.psEntry ? params.psEntry : "main";
            fs = std::make_unique<rapid_vulkan::Shader>(fcp);
            if (!fs->handle()) fs.reset();
        }
        ShaderPair p;
        p.vs = std::move(vs);
        p.fs = std::move(fs);
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

rapid_vulkan::Ref<const rapid_vulkan::GraphicsPipeline> PsoFactoryVulkan::getOrCreateGraphicsPso(const GraphicsPsoKey & key,
                                                                                                  const GraphicsPsoCreateParams * params) {
    if (!_impl || !_impl->gpu) return {};
    auto it = _impl->cache.find(key);
    if (it != _impl->cache.end()) return it->second;
    if (!params || !params->vsSpirv || params->vsSize == 0) return {};
    ShaderPair * shaders = _impl->getOrCreateShaders(key.shaderHash, *params);
    if (!shaders || !shaders->vs) return {};
    const rapid_vulkan::GlobalInfo * gi = _impl->gpu->device().gi();
    if (!gi) return {};
    rapid_vulkan::GraphicsPipeline::ConstructParameters cp;
    cp.setName("pso-factory-pipeline");
    cp.setVS(shaders->vs.get());
    cp.setFS(shaders->fs.get());
    cp.setDynamicRendering(static_cast<vk::Format>(key.colorFormat));
    cp.dynamicViewport(1);
    cp.dynamicScissor(1);
    if (!key.noVertexInput && params->va && params->vb && params->vaCount > 0 && params->vbCount > 0) {
        const auto * va = static_cast<const vk::VertexInputAttributeDescription *>(params->va);
        const auto * vb = static_cast<const vk::VertexInputBindingDescription *>(params->vb);
        for (uint32_t i = 0; i < params->vbCount; ++i) cp.vb.push_back(vb[i]);
        for (uint32_t i = 0; i < params->vaCount; ++i) cp.va.push_back(va[i]);
    }
    rapid_vulkan::Ref<rapid_vulkan::GraphicsPipeline> pipe(new rapid_vulkan::GraphicsPipeline(cp));
    if (!pipe->handle()) {
        GN_WARN(sLogger)("PsoFactoryVulkan: failed to create graphics pipeline");
        return {};
    }
    _impl->cache[key] = pipe;
    return pipe;
}

} // namespace GN::rdg
