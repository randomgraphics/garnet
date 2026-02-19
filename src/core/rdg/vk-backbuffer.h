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
    auto readback() const -> gfx::img::Image override;

    Action::ExecutionResult prepare(SubmissionImpl & submission) override;
    Action::ExecutionResult present(SubmissionImpl & submission) override;

    rapid_vulkan::Swapchain *       swapchain() { return mSwapchain.valid() ? mSwapchain.get() : nullptr; }
    const rapid_vulkan::Swapchain * swapchain() const { return mSwapchain.valid() ? mSwapchain.get() : nullptr; }
};

// Create a frame execution context to store mapping from backbuffer artifact to frame pointer.
struct FrameExecutionContextVulkan : SubmissionImpl::Context {
    inline static const uint64_t                                         TYPE = RuntimeType::getNextUniqueTypeId();
    std::unordered_map<uint64_t, const rapid_vulkan::Swapchain::Frame *> bb2frame;
    FrameExecutionContextVulkan(): SubmissionImpl::Context(TYPE) {}
};

/// Create a Vulkan-backed Backbuffer. Called from Backbuffer::create() when context is Vulkan.
/// Returns null on failure or duplicate type+name.
AutoRef<Backbuffer> createVulkanBackbuffer(ArtifactDatabase & db, const StrA & name, const Backbuffer::CreateParameters & params);

} // namespace GN::rdg
