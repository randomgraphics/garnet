#include "pch.h"
#include "vk-backbuffer.h"
#include "vk-submission-context.h"

static GN::Logger * sLogger = GN::getLogger("GN.rdg.vk");

namespace GN::rdg {

// =============================================================================
// BackbufferVulkan - constructor and init
// =============================================================================

BackbufferVulkan::BackbufferVulkan(ArtifactDatabase & db, const StrA & name): Backbuffer(db, TYPE_ID, TYPE_NAME, name) {
    if (0 == sequence) { GN_ERROR(sLogger)("BackbufferVulkan::BackbufferVulkan: duplicate type+name, name='{}'", name); }
}

BackbufferVulkan::~BackbufferVulkan() { GN_INFO(sLogger)("Destorying Vulkan backbuffer, name='{}'", name); }

bool BackbufferVulkan::init(const Backbuffer::CreateParameters & params) {
    if (0 == sequence) return false;

    // store GPU context and descriptor.
    mGpuContext = params.context.castTo<GpuContextVulkan>();
    mDescriptor = params.descriptor;

    if (mDescriptor.width == 0 || mDescriptor.height == 0) {
        GN_ERROR(sLogger)("BackbufferVulkan::init: invalid dimensions {}x{} (caller must set positive width/height)", mDescriptor.width, mDescriptor.height);
        return false;
    }

    rapid_vulkan::Swapchain::ConstructParameters scp;
    scp.setDevice(mGpuContext->device());
    scp.setDimensions(mDescriptor.width, mDescriptor.height);
    scp.setSurface(vk::SurfaceKHR((VkSurfaceKHR) (void *) mDescriptor.window));
    scp.depthStencilFormat.mode = rapid_vulkan::Swapchain::DepthStencilFormat::DISABLED; // do not automatically create depth buffer

    try {
        mSwapchain = rapid_vulkan::Ref<rapid_vulkan::Swapchain>::make(scp);
    } catch (const std::exception & e) {
        GN_ERROR(sLogger)("BackbufferVulkan::init: Swapchain creation failed: {}, name='{}'", e.what(), name);
        return false;
    }

    return true;
}

namespace {

gfx::img::PixelFormat vkFormatToPixelFormat(vk::Format vkFmt) {
    switch (vkFmt) {
    case vk::Format::eR8G8B8A8Unorm:
        return gfx::img::PixelFormat::RGBA_8_8_8_8_UNORM();
    case vk::Format::eR8G8B8A8Srgb:
        return gfx::img::PixelFormat::RGBA_8_8_8_8_SRGB();
    case vk::Format::eB8G8R8A8Unorm:
        return gfx::img::PixelFormat::BGRA_8_8_8_8_UNORM();
    case vk::Format::eB8G8R8A8Srgb:
        return gfx::img::PixelFormat::BGRA_8_8_8_8_UNORM(); // no BGRA sRGB in rapid-image; use UNORM
    default:
        return gfx::img::PixelFormat::UNKNOWN();
    }
}

gfx::img::Image contentToImage(const rapid_vulkan::Image::Content & content) {
    if (content.subresources.empty() || content.storage.empty()) return gfx::img::Image();
    gfx::img::PixelFormat pf = vkFormatToPixelFormat(content.format);
    if (pf == gfx::img::PixelFormat::UNKNOWN()) return gfx::img::Image();
    const auto & sub = content.subresources[0];
    uint32_t     w   = sub.extent.width;
    uint32_t     h   = sub.extent.height;
    uint32_t     d   = sub.extent.depth;
    if (w == 0 || h == 0) return gfx::img::Image();
    gfx::img::Extent3D extent;
    extent.set(w, h, d);
    gfx::img::PlaneDesc planeDesc = gfx::img::PlaneDesc::make(pf, extent);
    gfx::img::ImageDesc imageDesc = gfx::img::ImageDesc::make(planeDesc, 1, 1, 1);
    return gfx::img::Image(imageDesc, content.storage.data(), content.storage.size());
}

} // namespace

gfx::img::Image BackbufferVulkan::readback() const {
    if (!mSwapchain.valid()) GN_UNLIKELY {
            GN_ERROR(sLogger)("BackbufferVulkan::readback: swapchain not initialized");
            return gfx::img::Image();
        }
    return (mActiveFrame) ? readbackInsideRenderPass() : readbackOutsideRenderPass();
}

gfx::img::Image BackbufferVulkan::readbackInsideRenderPass() const {
    // Inside a render pass (mActiveFrame != nullptr). Sync with current pass and layout transitions are more complex.
    GN_UNIMPL();
    return gfx::img::Image();
}

gfx::img::Image BackbufferVulkan::readbackOutsideRenderPass() const {
    GN_ASSERT(mActiveFrame == nullptr); // can only be called when not actively rendering to the backbuffer.
    if (!mLastPresentedImage) GN_UNLIKELY {
            GN_ERROR(sLogger)("BackbufferVulkan::readbackOutsideRenderPass: no last presented image (present at least one frame first)");
            return gfx::img::Image();
        }

    const rapid_vulkan::Device & dev = mGpuContext->device();
    rapid_vulkan::CommandQueue * gq  = dev.graphics();
    const rapid_vulkan::Image *  img = mLastPresentedImage;

    dev.waitIdle();

    rapid_vulkan::Image::ReadContentParameters readParams;
    readParams.setQueue(*gq);
    auto content = img->readContent(readParams);

    // Restore the backbuffer image to its layout after present so normal rendering is unaffected.
    auto cb = gq->begin("readback restore layout", vk::CommandBufferLevel::ePrimary);
    if (cb) {
        rapid_vulkan::Barrier()
            .i(img->handle(), vk::AccessFlagBits::eTransferRead, mLastPresentedBackbufferState.access, vk::ImageLayout::eTransferSrcOptimal,
               mLastPresentedBackbufferState.layout, vk::ImageAspectFlagBits::eColor)
            .s(vk::PipelineStageFlagBits::eTransfer, mLastPresentedBackbufferState.stages)
            .cmdWrite(cb.handle());
        gq->submit(rapid_vulkan::CommandQueue::SubmitParameters {.commandBuffers = {cb}});
    }

    dev.waitIdle();
    return contentToImage(content);
}

Action::ExecutionResult BackbufferVulkan::beginFrame(const TaskInfo & taskInfo) {
    GN_RDG_FAIL_ON_FALSE(mSwapchain.valid(), "{} - swapchain not initialized", taskInfo);

    // Check if the backbuffer is already prepared.
    if (mActiveFrame) GN_UNLIKELY {
            GN_VERBOSE(sLogger)("BackbufferVulkan::prepare: already prepared. Redundant call is ignored.");
            return Action::ExecutionResult::WARNING;
        }

    try {
        GN_VERBOSE(sLogger)("{} - begin frame", taskInfo);
        mActiveFrame = mSwapchain->beginFrame();
    } catch (const std::exception & e) {
        GN_VERBOSE(sLogger)("{} - beginFrame failed (e.g. window closed): {}", taskInfo, e.what());
        return Action::ExecutionResult::FAILED;
    } catch (...) {
        GN_VERBOSE(sLogger)("{} - beginFrame failed with unknown exception", taskInfo);
        return Action::ExecutionResult::FAILED;
    }
    GN_RDG_FAIL_ON_FALSE(mActiveFrame, "{} - beginFrame failed", taskInfo);

    // mSwapChain->beginFrame() updated the backbuffer layout. So we
    // need to track those changes here.
    trackImageState(TextureVulkan::ImageState::UNDEFINED());

    // Update the pending semaphores.
    mPendingSemaphores.clear();
    mPendingSemaphores.append(mActiveFrame->imageAvailable());

    // done
    return Action::ExecutionResult::PASSED;
}

Action::ExecutionResult BackbufferVulkan::present(const TaskInfo & taskInfo) {
    GN_RDG_FAIL_ON_FALSE(mSwapchain.valid(), "{} - swapchain not initialized", taskInfo);

    // Check if the backbuffer is already prepared. If not, ignore this present call.
    if (!mActiveFrame) GN_UNLIKELY {
            GN_VERBOSE(sLogger)("BackbufferVulkan::present: ignore. The backbuffer is not prepared yet.");
            return Action::ExecutionResult::WARNING;
        }

    // Call present() and update the image state to post-present layout.
    GN_VERBOSE(sLogger)("BackbufferVulkan::present: present frame");
    rapid_vulkan::Swapchain::BackbufferStatus newStatus;
    try {
        auto pp = rapid_vulkan::Swapchain::PresentParameters(
            rapid_vulkan::Swapchain::BackbufferStatus {mBackbufferState.curr.layout, mBackbufferState.curr.access, mBackbufferState.curr.stages});
        pp.setRenderFinished(vk::ArrayProxy<vk::Semaphore>((uint32_t) mPendingSemaphores.size(), mPendingSemaphores.data()));
        newStatus = mSwapchain->present(pp);
    } catch (const std::exception & e) {
        GN_VERBOSE(sLogger)("BackbufferVulkan::present failed (e.g. window closed): {}", e.what());
        mActiveFrame = nullptr;
        return Action::ExecutionResult::FAILED;
    } catch (...) {
        GN_VERBOSE(sLogger)("BackbufferVulkan::present failed with unknown exception");
        mActiveFrame = nullptr;
        return Action::ExecutionResult::FAILED;
    }
    trackImageState({newStatus.layout, newStatus.access, newStatus.stages});

    // Remember the backbuffer image and its post-present state for readbackOutsideRenderPass() (before frame is invalidated).
    mLastPresentedImage           = mActiveFrame->backbuffer().image;
    mLastPresentedBackbufferState = {newStatus.layout, newStatus.access, newStatus.stages};

    // We are done. Close the frame
    mActiveFrame = nullptr;
    return Action::ExecutionResult::PASSED;
}

bool BackbufferVulkan::trackImageState(const TextureVulkan::ImageState & newState, TextureVulkan::ImageStateTransitionFlags flags) {
    if (mBackbufferState.curr == newState) return false;
    mBackbufferState.transitTo(newState, flags);
    return true;
}

AutoRef<Backbuffer> createBackbufferVulkan(ArtifactDatabase & db, const StrA & name, const Backbuffer::CreateParameters & params) {
    auto * p = new BackbufferVulkan(db, name);
    if (p->sequence == 0) {
        GN_ERROR(sLogger)("createVulkanBackbuffer: duplicate type+name, name='{}'", name);
        delete p;
        return {};
    }
    if (!p->init(params)) {
        delete p;
        return {};
    }
    return AutoRef<Backbuffer>(p);
}

// =============================================================================
// PrepareBackbuffer - API-neutral impl (uses BackbufferCommon::prepare())
// =============================================================================

/// \note
///  - Can't use this as a mark of beginning of a frame, since it could be called very late in the frame, like
///    in a deferred rendering pipeline.
class PrepareBackbufferVulkan : public PrepareBackbuffer {
public:
    PrepareBackbufferVulkan(ArtifactDatabase & db, const StrA & name): PrepareBackbuffer(db, TYPE_ID, TYPE_NAME, name) {}

    ExecutionResult prepare(TaskInfo &, Arguments &) override { return PASSED; }

    ExecutionResult execute(TaskInfo & taskInfo, Arguments & arguments) override {
        auto a = arguments.castTo<PrepareBackbuffer::A>();
        GN_RDG_FAIL_ON_FALSE(a, "{} - arguments is not PrepareBackbuffer::A", taskInfo);
        GN_RDG_FAIL_ON_FALSE(a->backbuffer.value, "{} - backbuffer not set", taskInfo);
        auto bb = a->backbuffer.value->castTo<BackbufferVulkan>();
        GN_RDG_FAIL_ON_FALSE(bb, "{} - backbuffer is not BackbufferVulkan", taskInfo);
        return bb->beginFrame(taskInfo);
    }
};

AutoRef<PrepareBackbuffer> createPrepareBackbufferVulkan(ArtifactDatabase & db, const StrA & name, const PrepareBackbuffer::CreateParameters &) {
    auto * p = new PrepareBackbufferVulkan(db, name);
    if (p->sequence == 0) {
        GN_ERROR(sLogger)("createVulkanPrepareBackbuffer: duplicate type+name, name='{}'", name);
        delete p;
        return {};
    }
    return AutoRef<PrepareBackbuffer>(p);
}

// =============================================================================
// PresentBackbufferVulkan
// =============================================================================

class PresentBackbufferVulkan : public PresentBackbuffer {
    AutoRef<GpuContextVulkan> mGpu;

public:
    PresentBackbufferVulkan(ArtifactDatabase & db, const StrA & name): PresentBackbuffer(db, TYPE_ID, TYPE_NAME, name) {}

    ExecutionResult prepare(TaskInfo & taskInfo, Arguments & arguments) override {
        auto a = arguments.castTo<PresentBackbuffer::A>();
        GN_RDG_FAIL_ON_FALSE(a, "{} - arguments is not PresentBackbuffer::A", taskInfo);
        auto & backbuffer = a->backbuffer.value;
        GN_RDG_FAIL_ON_FALSE(backbuffer, "{} - backbuffer not set", taskInfo);

        // standard preparation.
        auto & sc = taskInfo.submission.ensureSubmissionContext<SubmissionContextVulkan>(mGpu);
        GN_RDG_FAIL_ON_FALSE(sc.renderPassManager.preparePresent(taskInfo, backbuffer));

        // done
        return PASSED;
    }

    ExecutionResult execute(TaskInfo & taskInfo, Arguments & arguments) override {
        auto a = arguments.castTo<PresentBackbuffer::A>();
        GN_RDG_FAIL_ON_FALSE(a, "{} - arguments is not PresentBackbuffer::A", taskInfo);
        auto bb = a->backbuffer.value->castTo<BackbufferVulkan>();
        GN_RDG_FAIL_ON_FALSE(bb, "{} - backbuffer is not BackbufferVulkan", taskInfo);

        // standard execution.
        auto & sc = taskInfo.submission.ensureSubmissionContext<SubmissionContextVulkan>(mGpu);
        GN_RDG_FAIL_ON_FAIL(sc.renderPassManager.execute(taskInfo, {}).result);

        // done
        return bb->present(taskInfo);
    }
};

AutoRef<PresentBackbuffer> createPresentBackbufferVulkan(ArtifactDatabase & db, const StrA & name, const PresentBackbuffer::CreateParameters &) {
    auto * p = new PresentBackbufferVulkan(db, name);
    if (p->sequence == 0) {
        GN_ERROR(sLogger)("createVulkanPresentBackbuffer: duplicate type+name, name='{}'", name);
        delete p;
        return {};
    }
    return AutoRef<PresentBackbuffer>(p);
}

} // namespace GN::rdg
