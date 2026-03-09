#pragma once

#include "submission.h"
#include "vk-gpu-context.h"
#include "vk-texture.h"

namespace GN::rdg {

/// Vulkan backbuffer: wraps a rapid-vulkan Swapchain (and optional Win32 surface).
class BackbufferVulkan : public Backbuffer {
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
    /// Returns true if a layout/access transition is needed (state was updated); false if already in newState (redundant).
    bool trackImageState(const TextureVulkan::ImageState & newState, TextureVulkan::ImageStateTransitionFlags flags = {});

    /// Read the backbuffer image outside a render pass.
    auto readback() const -> gfx::img::Image override;

    /// Called by PrepareBackbuffer to begin a new frame.
    auto beginFrame(const TaskInfo & taskInfo) -> Action::ExecutionResult;

    /// Called by PresentBackbuffer to present the current frame.
    auto present(const TaskInfo & taskInfo) -> Action::ExecutionResult;

private:
    AutoRef<GpuContextVulkan>                  mGpuContext;
    Backbuffer::Descriptor                     mDescriptor;
    rapid_vulkan::Ref<rapid_vulkan::Swapchain> mSwapchain;
    TextureVulkan::ImageStateTransition        mBackbufferState;
    const rapid_vulkan::Swapchain::Frame *     mActiveFrame = nullptr;
    DynaArray<vk::Semaphore>                   mPendingSemaphores;

    /// Last presented backbuffer image (set in present()). Used by readbackOutsideRenderPass().
    const rapid_vulkan::Image * mLastPresentedImage = nullptr;
    /// Layout/access/stages of the last presented image after present (so we can restore after readback).
    TextureVulkan::ImageState mLastPresentedBackbufferState;

    auto readbackOutsideRenderPass() const -> gfx::img::Image;
    auto readbackInsideRenderPass() const -> gfx::img::Image;
};

/// Create a Vulkan-backed Backbuffer. Called from Backbuffer::create() when context is Vulkan.
/// Returns null on failure or duplicate type+name.
AutoRef<Backbuffer> createBackbufferVulkan(ArtifactDatabase & db, const StrA & name, const Backbuffer::CreateParameters & params);

/// Create a Vulkan-backed PrepareBackbuffer. Called from PrepareBackbuffer::create() when context is Vulkan.
AutoRef<PrepareBackbuffer> createPrepareBackbufferVulkan(ArtifactDatabase & db, const StrA & name, const PrepareBackbuffer::CreateParameters & params);

/// Create a Vulkan-backed PresentBackbuffer. Called from PresentBackbuffer::create() when context is Vulkan.
AutoRef<PresentBackbuffer> createPresentBackbufferVulkan(ArtifactDatabase & db, const StrA & name, const PresentBackbuffer::CreateParameters & params);
} // namespace GN::rdg
