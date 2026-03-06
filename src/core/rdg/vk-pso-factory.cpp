#include "pch.h"
#include "vk-pso-factory.h"
#include <unordered_map>

namespace GN::rdg {

class PsoFactoryVulkan::Impl {
public:
    GpuContextVulkan * gpu = nullptr;
    std::unordered_map<GraphicsPsoKey, rapid_vulkan::Ref<rapid_vulkan::GraphicsPipeline>> cache;

    explicit Impl(GpuContextVulkan & gpu_) : gpu(&gpu_) {}
};

PsoFactoryVulkan::PsoFactoryVulkan(GpuContextVulkan & gpu) { _impl = new Impl(gpu); }

PsoFactoryVulkan::~PsoFactoryVulkan() {
    delete _impl;
    _impl = nullptr;
}

rapid_vulkan::Ref<const rapid_vulkan::GraphicsPipeline> PsoFactoryVulkan::getOrCreateGraphicsPso(const GraphicsPsoKey & key) {
    if (!_impl || !_impl->gpu) return {};
    auto it = _impl->cache.find(key);
    if (it != _impl->cache.end()) return it->second;
    // TODO: create pipeline from key (Task 2.1)
    return {};
}

} // namespace GN::rdg
