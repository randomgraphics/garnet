#pragma once

#include "vk-gpu-context.h"

namespace GN::rdg {

struct DrawActionContextVulkan: public Action::ExecutionContext {
    inline static constexpr Guid TYPE = {0x6ad8b59d, 0xe672, 0x4b5e, {0x8e, 0xec, 0xf7, 0xac, 0xd4, 0xf1, 0x99, 0xdd}};
    
    uint64_t renderTargetId {};
    uint64_t commandBufferId {};

    DrawActionContextVulkan() : Action::ExecutionContext(TYPE) {}
};

/// Create Vulkan-backed ClearRenderTarget. Called from ClearRenderTarget::create() when context is Vulkan.
/// Returns null on failure or duplicate type+name.
AutoRef<ClearRenderTarget> createVulkanClearRenderTarget(ArtifactDatabase & db, const StrA & name, const ClearRenderTarget::CreateParameters & params);

} // namespace GN::rdg
