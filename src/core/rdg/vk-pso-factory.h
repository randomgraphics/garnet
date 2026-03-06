#pragma once

#include "vk-gpu-context.h"
#include <cstdint>

namespace GN::rdg {

/// Memory-efficient key for graphics PSO cache lookup. All fields are bit-packed into 16 bytes.
struct GraphicsPsoKey {
    uint64_t shaderHash = 0;

    uint64_t noVertexInput : 1;
    uint64_t vertexStride : 16;
    uint64_t vertexFormatHash : 27;
    uint64_t colorFormat : 12;
    uint64_t pushConstantSize : 8;
    uint64_t reserved : 4;

    GraphicsPsoKey()
        : noVertexInput(1), vertexStride(0), vertexFormatHash(0), colorFormat(0), pushConstantSize(0), reserved(0) {}

    bool operator==(const GraphicsPsoKey & other) const {
        return shaderHash == other.shaderHash && noVertexInput == other.noVertexInput &&
               vertexStride == other.vertexStride && vertexFormatHash == other.vertexFormatHash &&
               colorFormat == other.colorFormat && pushConstantSize == other.pushConstantSize;
    }
    bool operator!=(const GraphicsPsoKey & other) const { return !(*this == other); }
};

/// PSO factory: get-or-create graphics pipelines keyed by GraphicsPsoKey. Owned by GpuContextVulkan.
class PsoFactoryVulkan {
public:
    explicit PsoFactoryVulkan(GpuContextVulkan & gpu);
    ~PsoFactoryVulkan();

    PsoFactoryVulkan(const PsoFactoryVulkan &) = delete;
    PsoFactoryVulkan & operator=(const PsoFactoryVulkan &) = delete;

    /// Returns cached or newly created graphics pipeline for the key. Returns null on failure.
    rapid_vulkan::Ref<const rapid_vulkan::GraphicsPipeline> getOrCreateGraphicsPso(const GraphicsPsoKey & key);

private:
    class Impl;
    Impl * _impl = nullptr;
};

} // namespace GN::rdg

namespace std {

template<>
struct hash<GN::rdg::GraphicsPsoKey> {
    size_t operator()(const GN::rdg::GraphicsPsoKey & k) const {
        size_t h = std::hash<uint64_t>{}(k.shaderHash);
        const uint64_t w = (uint64_t)(k.noVertexInput & 1) | ((uint64_t)k.vertexStride << 1) |
                           ((uint64_t)k.vertexFormatHash << 17) | ((uint64_t)k.colorFormat << 44) |
                           ((uint64_t)k.pushConstantSize << 56);
        return h ^ (std::hash<uint64_t>{}(w) + 0x9e3779b9 + (h << 6) + (h >> 2));
    }
};

} // namespace std
