#include "vk-swapchain.h"
#include "vk-format-utils.h"
#include "vk-gpu-context.h"
#include "vk-gpu-payload.h"
#include "vk-texture.h"

#include <unordered_map>

static GN::Logger * sLogger = GN::getLogger("GN.gpu2.vk");

namespace GN::gpu2 {
namespace {

/// Swapchain-specific payload that exposes the protected setSemaphore() hook publicly,
/// so the swapchain can inject the rapid-vulkan-owned imageAvailable handle into the
/// "ready" payload after each acquire.
class SwapchainReadyPayloadVulkan final : public GpuPayloadVulkan {
public:
    GN_REGISTER_RUNTIME_TYPE(GpuPayloadVulkan);

    using GpuPayloadVulkan::GpuPayloadVulkan;

    // makes setSemaphore() public.
    using GpuPayloadVulkan::setSemaphore;
};

/// One stable \c Texture per swapchain backbuffer \c Image (non-owning view of rapid-vulkan swapchain memory).
class SwapchainBackbufferTextureVulkan final : public TextureVulkanBase {
public:
    GN_REGISTER_RUNTIME_TYPE(TextureVulkanBase);

    SwapchainBackbufferTextureVulkan(const StrA & entityName, AutoRef<GpuContextVulkan2> gpu)
        : TextureVulkanBase(SwapchainBackbufferTextureVulkan::TYPE_INFO(), entityName) {
        mGpu = std::move(gpu);
    }

    void bindToSwapchainBackbuffer(rv::Image * image, uint32_t w, uint32_t h, gfx::img::PixelFormat format) {
        setVulkanHandles(image);
        mDescriptor = Texture::Descriptor {}.setFormat(format).setDimensions(w, h, 1).setFaces(1).setLevels(1).setSamples(1);
        // No manual state reset: rapid-vulkan tracks the image layout internally after acquire.
    }

    void clearAcquiredBinding() {
        setVulkanHandles(nullptr);
        mDescriptor = {};
    }

    /// stable per-backbuffer payload; returned as frame.ready from prepare()
    AutoRef<SwapchainReadyPayloadVulkan> readyPayload = AutoRef<SwapchainReadyPayloadVulkan>::make(name + "/ready");

    /// Binary semaphore used to bridge a timeline render-done point into a form vkQueuePresentKHR can wait on.
    /// One per backbuffer: the swapchain won't recycle the same image until present consumed the previous signal.
    vk::UniqueSemaphore bridgeSemaphore;
};

} // namespace

class SwapchainVulkan2 final : public Swapchain {
public:
    GN_REGISTER_RUNTIME_TYPE(Swapchain);

    explicit SwapchainVulkan2(const StrA & entityName): Swapchain(TYPE_INFO(), entityName) {}

    bool init(const Swapchain::CreateDesc & desc);

    Swapchain::Frame prepare() override;
    void             present(GpuPayload & waitFor) override;

private:
    AutoRef<GpuContextVulkan2> mGpu;
    gfx::img::PixelFormat      mSurfaceFormat = gfx::img::PixelFormat::UNKNOWN();

    rv::Ref<rv::Swapchain> mRvSwapchain;

    std::unordered_map<const rv::Image *, AutoRef<SwapchainBackbufferTextureVulkan>> mBackbufferTextures;
    mutable SwapchainBackbufferTextureVulkan *                                       mActiveBackbufferTexture = nullptr;

    rv::Swapchain::Frame mActiveFrame = {};

    GpuResourceView makeFrameColorView() const;
    void            clearAcquiredFrameBindings();
    /// Submit an empty command buffer that waits on \p tp and signals a per-backbuffer binary semaphore.
    /// Returns the binary semaphore handle for use with vkQueuePresentKHR.
    vk::Semaphore bridgeTimelineToBinary(const rv::CommandQueue::SyncPoint & tp);
};

GpuResourceView SwapchainVulkan2::makeFrameColorView() const {
    GpuResourceView out;
    if (!mActiveBackbufferTexture) return out;
    Texture * t = mActiveBackbufferTexture;
    out.resource.set(t);
    out.setImageViewFormat(mSurfaceFormat);
    return out;
}

void SwapchainVulkan2::clearAcquiredFrameBindings() {
    mBackbufferTextures.clear();
    mActiveBackbufferTexture = nullptr;
    mActiveFrame             = {};
}

bool SwapchainVulkan2::init(const Swapchain::CreateDesc & desc) {
    mGpu = desc.gpu.staticCastTo<GpuContextVulkan2>();
    if (!mGpu || !mGpu->ready()) {
        GN_ERROR(sLogger)("SwapchainVulkan2::init: invalid or non-ready Vulkan GpuContext, name='{}'", name);
        return false;
    }

    const rv::Device & dev = mGpu->vulkanDevice();

    if (desc.width == 0 || desc.height == 0) {
        GN_ERROR(sLogger)("SwapchainVulkan2::init: invalid dimensions {}x{}, name='{}'", desc.width, desc.height, name);
        return false;
    }

    rv::Swapchain::ConstructParameters scp;
    scp.setDevice(dev);
    scp.setDimensions(desc.width, desc.height);
    scp.setSurface(vk::SurfaceKHR((VkSurfaceKHR) (void *) desc.surface));
    scp.depthStencilFormat.mode = rv::Swapchain::DepthStencilFormat::DISABLED;
    scp.backbufferFormat        = pixelFormatToVkFormat(desc.format);

    try {
        mRvSwapchain = rv::Ref<rv::Swapchain>::make(scp);
    } catch (const std::exception & e) {
        GN_ERROR(sLogger)("SwapchainVulkan2::init: Swapchain creation failed: {}, name='{}'", e.what(), name);
        return false;
    }

    mSurfaceFormat = vkFormatToPixelFormat(mRvSwapchain->cp().backbufferFormat);
    if (mSurfaceFormat == gfx::img::PixelFormat::UNKNOWN()) {
        GN_ERROR(sLogger)("SwapchainVulkan2::init: swapchain backbuffer format resolved to UNKNOWN, name='{}'", name);
        mRvSwapchain.clear();
        return false;
    }

    mBackbufferTextures.clear();
    mActiveFrame             = {};
    mActiveBackbufferTexture = nullptr;

    return true;
}

Swapchain::Frame SwapchainVulkan2::prepare() {
    if (!mRvSwapchain.valid()) {
        GN_ERROR(sLogger)("SwapchainVulkan2::prepare: swapchain not initialized, name='{}'", name);
        return {};
    }

    if (mActiveFrame.valid()) {
        GN_VERBOSE(sLogger)("SwapchainVulkan2::prepare: already prepared; redundant call ignored, name='{}'", name);
        GN_ASSERT(mActiveBackbufferTexture);
        return Swapchain::Frame {makeFrameColorView(), mActiveBackbufferTexture->readyPayload};
    }

    mActiveFrame = mRvSwapchain->beginFrame();
    if (!mActiveFrame.valid()) {
        GN_VERBOSE(sLogger)("SwapchainVulkan2::prepare: beginFrame failed, name='{}'", name);
        clearAcquiredFrameBindings();
        return {};
    }

    const uint32_t w  = (uint32_t) mRvSwapchain->cp().width;
    const uint32_t h  = (uint32_t) mRvSwapchain->cp().height;
    const auto &   bb = *mActiveFrame.backbuffer;

    auto it = mBackbufferTextures.find(bb.image);
    if (it == mBackbufferTextures.end()) {
        StrA                                      texName = StrA::format("{}_bb_{}", name, (uint64_t) (uintptr_t) bb.image);
        AutoRef<SwapchainBackbufferTextureVulkan> tex(new SwapchainBackbufferTextureVulkan(texName, mGpu));
        it = mBackbufferTextures.emplace(bb.image, tex).first;
    }
    it->second->bindToSwapchainBackbuffer(bb.image, w, h, mSurfaceFormat); // this will reset the texture's GPU state to UNDEFINED,
                                                                           // which matches the swapchain image state after acquire and before any rendering
    mActiveBackbufferTexture = it->second.get();

    // set the semaphore. This also marks this payload as "submitted".
    mActiveBackbufferTexture->readyPayload->setSemaphore(mActiveFrame.imageAvailable);
    GN_ASSERT(mActiveBackbufferTexture->readyPayload->syncpoint());

    return Swapchain::Frame {makeFrameColorView(), mActiveBackbufferTexture->readyPayload};
}

void SwapchainVulkan2::present(GpuPayload & waitFor) {

    if (!mRvSwapchain.valid()) {
        GN_ERROR(sLogger)("SwapchainVulkan2::present: swapchain not initialized, name='{}'", name);
        return;
    }

    if (!mActiveFrame.valid()) {
        GN_VERBOSE(sLogger)("SwapchainVulkan2::present: not prepared; ignored, name='{}'", name);
        return;
    }

    SwapchainBackbufferTextureVulkan * backbufferTex = mActiveBackbufferTexture;
    if (!backbufferTex) {
        GN_VERBOSE(sLogger)("SwapchainVulkan2::present: no active backbuffer texture; ignored, name='{}'", name);
        return;
    }

    vk::Semaphore renderFinished = nullptr;
    auto          vkWaitFor      = RuntimeType::cast<GpuPayloadVulkan>(waitFor);
    if (vkWaitFor) GN_LIKELY {
            const auto & sp = vkWaitFor->syncpoint();
            if (const auto * b = sp.asBinarySemaphore()) {
                renderFinished = *b;
            } else if (const auto * t = sp.asTimelinePoint()) {
                // vkQueuePresentKHR only accepts binary semaphores; bridge via an empty submit
                // that waits on the timeline and signals a per-backbuffer binary semaphore.
                renderFinished = bridgeTimelineToBinary(*t);
            } else {
                GN_ERROR(sLogger)("present() must wait on an already-submitted payload: {}({})", waitFor.name, waitFor.id);
            }
        }
    else { GN_ERROR(sLogger)("waitFor is not a GpuPayloadVulkan: {}({})", waitFor.name, waitFor.id); }

    rv::Swapchain::PresentResult result;
    try {
        rv::Swapchain::PresentParameters pp;
        pp.setRenderFinished(vk::ArrayProxy<const vk::Semaphore>(renderFinished ? 1u : 0u, &renderFinished));
        result = mRvSwapchain->present(pp);
    } catch (const std::exception & e) {
        GN_VERBOSE(sLogger)("SwapchainVulkan2::present failed: {}, name='{}'", e.what(), name);
        clearAcquiredFrameBindings();
        return;
    } catch (...) {
        GN_VERBOSE(sLogger)("SwapchainVulkan2::present failed (unknown), name='{}'", name);
        clearAcquiredFrameBindings();
        return;
    }

    // rapid-vulkan updates the image's internal state after present; no manual setState needed.

    // clear active frame
    mActiveBackbufferTexture = nullptr;
    mActiveFrame             = {};
}

vk::Semaphore SwapchainVulkan2::bridgeTimelineToBinary(const rv::CommandQueue::SyncPoint & tp) {
    if (!mActiveBackbufferTexture) return {};

    auto & bridgeSem = mActiveBackbufferTexture->bridgeSemaphore;
    if (!bridgeSem) {
        const auto & gi = *mGpu->vulkanDevice().gi();
        bridgeSem       = gi.device.createSemaphoreUnique({}, gi.allocator);
    }

    rv::CommandQueue * queue = mGpu->vulkanDevice().graphics();
    if (!queue) {
        GN_ERROR(sLogger)("SwapchainVulkan2: no graphics queue for timeline bridge submit");
        return {};
    }

    // An empty command buffer is needed because rapid-vulkan's submit() rejects empty cmd lists.
    rv::CommandBuffer emptyCmd = queue->begin("bridge_timeline_to_binary");
    if (emptyCmd.empty()) {
        GN_ERROR(sLogger)("SwapchainVulkan2: failed to begin bridge command buffer");
        return {};
    }

    // Ensure stage flags are set; caller usually sets eAllCommands already.
    rv::CommandQueue::SyncPoint waitTp = tp;
    if (!waitTp.stages) waitTp.stages = vk::PipelineStageFlagBits::eAllCommands;

    rv::CommandQueue::SyncPoint        signalSp = {bridgeSem.get(), 0, vk::PipelineStageFlagBits::eAllCommands};
    rv::CommandQueue::SubmitParameters bsp;
    bsp.commandBuffers = {emptyCmd};
    bsp.waitPoints     = {1, &waitTp};
    bsp.signalBinaries = {1, &signalSp};
    queue->submit2(bsp);
    return signalSp.semaphore;
}

AutoRef<Swapchain> createSwapchainVulkan2(const Swapchain::CreateDesc & desc) {
    StrA entityName = desc.name.empty() ? StrA("swapchain") : desc.name;
    auto sc         = AutoRef<SwapchainVulkan2>(new SwapchainVulkan2(entityName));
    if (!sc->init(desc)) return {};
    return sc;
}

} // namespace GN::gpu2
