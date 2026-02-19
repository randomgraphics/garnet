#pragma once

#include "vk-gpu-context.h"

namespace GN::rdg {

struct DrawActionContextVulkan : public Action::ExecutionContext {
    inline static const uint64_t TYPE = RuntimeType::getNextUniqueTypeId();

    uint64_t commandBufferId {};

    DrawActionContextVulkan(): Action::ExecutionContext(TYPE) {}
};

/// Create Vulkan-backed ClearRenderTarget. Called from ClearRenderTarget::create() when context is Vulkan.
/// Returns null on failure or duplicate type+name.
AutoRef<ClearRenderTarget> createVulkanClearRenderTarget(ArtifactDatabase & db, const StrA & name, const ClearRenderTarget::CreateParameters & params);

} // namespace GN::rdg
