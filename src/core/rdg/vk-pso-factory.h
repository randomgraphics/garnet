#pragma once

/// PSO factory: get-or-create Vulkan graphics pipelines keyed by GraphicsPsoKey; used by GpuDrawVulkan with Drawable/DrawPack.

#include "vk-gpu-context.h"
#include <garnet/GNrdg.h>
#include <cstdint>

namespace GN::rdg {

/// Self-contained key for graphics shader (vs+ps). No pointers.
struct ShaderKey {
    uint64_t hash = 0;

    bool operator==(const ShaderKey & o) const { return hash == o.hash; }
    bool operator!=(const ShaderKey & o) const { return !(*this == o); }

    static ShaderKey makeGraphics(const GpuShaderAction::ShaderBinary & vs, const GpuShaderAction::ShaderBinary & ps);
    static ShaderKey makeCompute(const GpuShaderAction::ShaderBinary & cs);
};

/// Self-contained key for render target: all non-empty color formats, depth format, and hash of blend/depth/stencil state.
struct RenderTargetKey {
    static constexpr size_t kMaxColorTargets = 8;

    uint32_t colorFormats[kMaxColorTargets] = {};
    uint8_t  colorCount                     = 0;
    uint32_t depthFormat                    = 0; ///< vk::Format (0 = eUndefined)
    uint64_t stateHash                      = 0; ///< hash of blend, depth, stencil state

    bool operator==(const RenderTargetKey & o) const {
        if (colorCount != o.colorCount || depthFormat != o.depthFormat || stateHash != o.stateHash) return false;
        for (size_t i = 0; i < colorCount; ++i)
            if (colorFormats[i] != o.colorFormats[i]) return false;
        return true;
    }
    bool operator!=(const RenderTargetKey & o) const { return !(*this == o); }

    static RenderTargetKey make(const RenderTarget & renderTarget);
};

/// Self-contained key for vertex input (no input / stride + format hash). No pointers.
struct GpuGeometryKey {
    uint64_t noVertexInput    : 1;
    uint64_t vertexStride     : 16;
    uint64_t vertexFormatHash : 27;
    uint64_t reserved         : 20;

    GpuGeometryKey(): noVertexInput(1), vertexStride(0), vertexFormatHash(0), reserved(0) {}

    bool operator==(const GpuGeometryKey & o) const {
        return noVertexInput == o.noVertexInput && vertexStride == o.vertexStride && vertexFormatHash == o.vertexFormatHash;
    }
    bool operator!=(const GpuGeometryKey & o) const { return !(*this == o); }

    static GpuGeometryKey make(const GpuGeometry & geometry);
};

/// Descriptor/push-constant layout for PSO.
struct DescriptorLayoutKey {
    uint32_t pushConstantSize = 128;

    bool operator==(const DescriptorLayoutKey & o) const { return pushConstantSize == o.pushConstantSize; }
    bool operator!=(const DescriptorLayoutKey & o) const { return !(*this == o); }
};

/// Full key for graphics PSO cache lookup.
struct GraphicsPsoKey {
    ShaderKey           shaderKey;
    RenderTargetKey     renderTargetKey;
    GpuGeometryKey      geometryKey;
    DescriptorLayoutKey descriptorLayoutKey;

    bool operator==(const GraphicsPsoKey & o) const {
        return shaderKey == o.shaderKey && renderTargetKey == o.renderTargetKey && geometryKey == o.geometryKey && descriptorLayoutKey == o.descriptorLayoutKey;
    }
    bool operator!=(const GraphicsPsoKey & o) const { return !(*this == o); }
};

/// All data needed to create a graphics PSO on cache miss. Key is derived from this.
struct GraphicsPsoCreateParams {
    GpuShaderAction::ShaderBinary vs;
    GpuShaderAction::ShaderBinary ps;
    const RenderTarget &          renderTarget;
    const GpuGeometry &           geometry;
    uint32_t                      pushConstantSize = 0;
};

/// PSO factory: get-or-create graphics pipelines. Owned by GpuContextVulkan.
class PsoFactoryVulkan {
public:
    explicit PsoFactoryVulkan(GpuContextVulkan & gpu);
    ~PsoFactoryVulkan();

    PsoFactoryVulkan(const PsoFactoryVulkan &)             = delete;
    PsoFactoryVulkan & operator=(const PsoFactoryVulkan &) = delete;

    /// Returns cached or newly created graphics pipeline. Key is derived from params.
    rapid_vulkan::Ref<const rapid_vulkan::GraphicsPipeline> getOrCreateGraphicsPso(const GraphicsPsoCreateParams & params);

private:
    class Impl;
    Impl * _impl = nullptr;
};

} // namespace GN::rdg

namespace std {

template<>
struct hash<GN::rdg::ShaderKey> {
    size_t operator()(const GN::rdg::ShaderKey & k) const { return std::hash<uint64_t> {}(k.hash); }
};

template<>
struct hash<GN::rdg::RenderTargetKey> {
    size_t operator()(const GN::rdg::RenderTargetKey & k) const {
        size_t h = std::hash<uint64_t> {}(k.stateHash);
        h ^= std::hash<uint32_t> {}(k.depthFormat) + 0x9e3779b9 + (h << 6) + (h >> 2);
        h ^= std::hash<uint8_t> {}(k.colorCount) + 0x9e3779b9 + (h << 6) + (h >> 2);
        for (size_t i = 0; i < k.colorCount; ++i) h ^= std::hash<uint32_t> {}(k.colorFormats[i]) + 0x9e3779b9 + (h << 6) + (h >> 2);
        return h;
    }
};

template<>
struct hash<GN::rdg::GpuGeometryKey> {
    size_t operator()(const GN::rdg::GpuGeometryKey & k) const {
        const uint64_t w = (uint64_t) k.noVertexInput | ((uint64_t) k.vertexStride << 1) | ((uint64_t) k.vertexFormatHash << 17);
        return std::hash<uint64_t> {}(w);
    }
};

template<>
struct hash<GN::rdg::DescriptorLayoutKey> {
    size_t operator()(const GN::rdg::DescriptorLayoutKey & k) const { return std::hash<uint32_t> {}(k.pushConstantSize); }
};

template<>
struct hash<GN::rdg::GraphicsPsoKey> {
    size_t operator()(const GN::rdg::GraphicsPsoKey & k) const {
        size_t h = std::hash<GN::rdg::ShaderKey> {}(k.shaderKey);
        h ^= std::hash<GN::rdg::RenderTargetKey> {}(k.renderTargetKey) + 0x9e3779b9 + (h << 6) + (h >> 2);
        h ^= std::hash<GN::rdg::GpuGeometryKey> {}(k.geometryKey) + 0x9e3779b9 + (h << 6) + (h >> 2);
        h ^= std::hash<GN::rdg::DescriptorLayoutKey> {}(k.descriptorLayoutKey) + 0x9e3779b9 + (h << 6) + (h >> 2);
        return h;
    }
};

} // namespace std
