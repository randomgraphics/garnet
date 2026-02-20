#pragma once

#include "backbuffer.h"
#include "vk-gpu-context.h"
#include "vk-texture.h"

namespace GN::rdg {

/// Vulkan backbuffer: wraps a rapid-vulkan Swapchain (and optional Win32 surface).
class BackbufferVulkan : public BackbufferCommon {
public:
    BackbufferVulkan(ArtifactDatabase & db, const StrA & name);

    ~BackbufferVulkan() override;

    /// Initialize swapchain and optional surface. Call once after construction. Returns false on failure.
    bool init(const Backbuffer::CreateParameters & params);

    auto gpu() const -> GpuContext & override { return *mGpuContext; }
    auto descriptor() const -> const Backbuffer::Descriptor & override { return mDescriptor; }
    auto swapchain() -> rapid_vulkan::Swapchain * { return mSwapchain.valid() ? mSwapchain.get() : nullptr; }
    auto swapchain() const -> const rapid_vulkan::Swapchain * { return mSwapchain.valid() ? mSwapchain.get() : nullptr; }
    auto backBufferImage() const -> const rapid_vulkan::Image * { return mActiveFrame ? mActiveFrame->backbuffer().image : nullptr; }

    auto getImageState() const -> const TextureVulkan::ImageStateTransition & { return mBackbufferState; }
    auto trackImageState(const TextureVulkan::ImageState & newState) -> TextureVulkan::ImageStateTransition { return mBackbufferState.transitTo(newState); }

    auto prepare(SubmissionImpl & submission) -> Action::ExecutionResult override;
    auto present(SubmissionImpl & submission) -> Action::ExecutionResult override;
    auto readback() const -> gfx::img::Image override;

    // /// Utility to get the handle of the backbuffer image.
    // vk::Image backbufferImage() const {
    //     GN_ASSERT(frame != nullptr);
    //     GN_ASSERT(frame->backbuffer().image);
    //     auto handle = frame->backbuffer().image->handle();
    //     GN_ASSERT(handle);
    //     return handle;
    // }

private:
    AutoRef<GpuContextVulkan>                  mGpuContext;
    Backbuffer::Descriptor                     mDescriptor;
    vk::UniqueSurfaceKHR                       mSurface;
    rapid_vulkan::Ref<rapid_vulkan::Swapchain> mSwapchain;
    TextureVulkan::ImageStateTransition        mBackbufferState;
    const rapid_vulkan::Swapchain::Frame *     mActiveFrame = nullptr;
    DynaArray<vk::Semaphore>                   mPendingSemaphores;
};

/// Create a Vulkan-backed Backbuffer. Called from Backbuffer::create() when context is Vulkan.
/// Returns null on failure or duplicate type+name.
AutoRef<Backbuffer> createVulkanBackbuffer(ArtifactDatabase & db, const StrA & name, const Backbuffer::CreateParameters & params);

} // namespace GN::rdg
