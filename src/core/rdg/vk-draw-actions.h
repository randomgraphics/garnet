#pragma once

#include "vk-gpu-context.h"
#include "runtime-type.h"

namespace GN::rdg {

struct DrawActionContextVulkan : public Action::ExecutionContext {
    inline static const uint64_t         TYPE_ID   = getNextUniqueTypeId();
    inline static constexpr const char * TYPE_NAME = "DrawActionContextVulkan";

    uint64_t commandBufferId {};

    DrawActionContextVulkan(): Action::ExecutionContext(TYPE_ID, TYPE_NAME) {}
};

/// Create Vulkan-backed ClearRenderTarget. Called from ClearRenderTarget::create() when context is Vulkan.
/// Returns null on failure or duplicate type+name.
AutoRef<ClearRenderTarget> createVulkanClearRenderTarget(ArtifactDatabase & db, const StrA & name, const ClearRenderTarget::CreateParameters & params);

/// Create Vulkan-backed GenericDraw. Called from GenericDraw::create() when context is Vulkan.
AutoRef<GenericDraw> createVulkanGenericDrawAction(ArtifactDatabase & db, const StrA & name, const GenericDraw::CreateParameters & params);

} // namespace GN::rdg
