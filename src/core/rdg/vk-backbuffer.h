#pragma once

#include "backbuffer.h"
#include "vk-gpu-context.h"

namespace GN::rdg {

/// Vulkan backbuffer: wraps a rapid-vulkan Swapchain (and optional Win32 surface).
class BackbufferVulkan : public BackbufferCommon {
    AutoRef<GpuContext>                        mGpuContext;
    Backbuffer::Descriptor                     mDescriptor;
    vk::UniqueSurfaceKHR                       mSurface;
    rapid_vulkan::Ref<rapid_vulkan::Swapchain> mSwapchain;

public:
    BackbufferVulkan(ArtifactDatabase & db, const StrA & name);

    /// Initialize swapchain and optional surface. Call once after construction. Returns false on failure.
    bool init(const Backbuffer::CreateParameters & params);

    auto gpu() const -> GpuContext & override { return *mGpuContext; }
    auto descriptor() const -> const Backbuffer::Descriptor & override { return mDescriptor; }

    void prepare() override;
    void present() override;

    rapid_vulkan::Swapchain *       swapchain() { return mSwapchain.valid() ? mSwapchain.get() : nullptr; }
    const rapid_vulkan::Swapchain * swapchain() const { return mSwapchain.valid() ? mSwapchain.get() : nullptr; }
};

/// Create a Vulkan-backed Backbuffer. Called from Backbuffer::create() when context is Vulkan.
/// Returns null on failure or duplicate type+name.
AutoRef<Backbuffer> createVulkanBackbuffer(ArtifactDatabase & db, const StrA & name, const Backbuffer::CreateParameters & params);

} // namespace GN::rdg
