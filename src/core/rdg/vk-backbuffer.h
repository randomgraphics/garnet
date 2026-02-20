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

    ~BackbufferVulkan() override;

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

struct FrameState {
    const rapid_vulkan::Swapchain::Frame * frame = nullptr;

    /// List of semaphores that present() call should wait on.
    DynaArray<vk::Semaphore> pendingSemaphores;

    /// Utility to get the handle of the backbuffer image.
    vk::Image backbufferImage() const {
        GN_ASSERT(frame != nullptr);
        GN_ASSERT(frame->backbuffer().image);
        auto handle = frame->backbuffer().image->handle();
        GN_ASSERT(handle);
        return handle;
    }
};

// Create a frame execution context to store graphics frame related information
struct FrameExecutionContextVulkan : SubmissionImpl::Context {
    inline static const uint64_t TYPE = getNextUniqueTypeId();

    // mapping from backbuffer artifact to frame pointer.
    std::unordered_map<uint64_t, FrameState> bb2frame;

    FrameExecutionContextVulkan(): SubmissionImpl::Context(TYPE) {}
};

/// Create a Vulkan-backed Backbuffer. Called from Backbuffer::create() when context is Vulkan.
/// Returns null on failure or duplicate type+name.
AutoRef<Backbuffer> createVulkanBackbuffer(ArtifactDatabase & db, const StrA & name, const Backbuffer::CreateParameters & params);

} // namespace GN::rdg
