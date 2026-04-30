#include "vk-swapchain.h"
#include "vk-format-utils.h"
#include "vk-gpu-context.h"
#include "vk-gpu-image-state.h"
#include "vk-gpu-payload.h"
#include "vk-texture.h"

#include <unordered_map>

static GN::Logger * sLogger = GN::getLogger("GN.gfx.gpu2.vk");

namespace GN::gpu2 {
namespace {

using GpuImageState = TextureGpuImageState::ImageState;

static GpuResourceView::SubresourceRange swapchainBackbufferSubresourceRange() {
    GpuResourceView::SubresourceRange r;
    r.i                = {};
    r.e.numMipLevels   = 1;
    r.e.numArrayLayers = 1;
    return r;
}

/// One stable \c Texture per swapchain backbuffer \c Image (non-owning view of rapid-vulkan swapchain memory).
class SwapchainBackbufferTextureVulkan final : public TextureVulkanBase {
public:
    GN_REGISTER_RUNTIME_TYPE(TextureVulkanBase);

    SwapchainBackbufferTextureVulkan(const StrA & entityName, AutoRef<GpuContextVulkan2> gpu)
        : TextureVulkanBase(SwapchainBackbufferTextureVulkan::TYPE_INFO(), entityName) {
        mGpu = std::move(gpu);
    }

    void bindToSwapchainBackbuffer(const rv::Image * image, vk::ImageView view, uint32_t w, uint32_t h, gfx::img::PixelFormat format) {
        mImage      = image;
        mView       = view;
        mDescriptor = Texture::Descriptor {}.setFormat(format).setDimensions(w, h, 1).setFaces(1).setLevels(1).setSamples(1);
        gpuStates.reset(1, 1, GpuImageState::UNDEFINED());
    }

    void clearAcquiredBinding() {
        mImage      = nullptr;
        mView       = vk::ImageView(nullptr);
        mDescriptor = {};
        gpuStates.reset(0, 0);
    }

    /// stable per-backbuffer payload; returned as frame.ready from prepare()
    AutoRef<GpuPayloadVulkan> readyPayload = AutoRef<GpuPayloadVulkan>::make(name + "/ready");

private:
    const rv::Image * vulkanSourceImageForReadback() const override { return mImage; }

    bool vulkanColorAttachmentHandles(vk::Image * outImage, vk::ImageView * outView) const override {
        if (!mImage || !mView) return false;
        *outImage = mImage->handle();
        *outView  = mView;
        return true;
    }

    const rv::Image * mImage = nullptr;
    vk::ImageView     mView {};
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
};

GpuResourceView SwapchainVulkan2::makeFrameColorView() const {
    GpuResourceView out;
    if (!mActiveBackbufferTexture) return out;
    Texture * t = mActiveBackbufferTexture;
    out.resource.set(t);
    out.imageView.range = swapchainBackbufferSubresourceRange();
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

    const rv::Device * dev = mGpu->vulkanDevice();
    GN_ASSERT(dev);

    if (desc.width == 0 || desc.height == 0) {
        GN_ERROR(sLogger)("SwapchainVulkan2::init: invalid dimensions {}x{}, name='{}'", desc.width, desc.height, name);
        return false;
    }

    rv::Swapchain::ConstructParameters scp;
    scp.setDevice(*dev);
    scp.setDimensions(desc.width, desc.height);
    scp.setSurface(vk::SurfaceKHR((VkSurfaceKHR) (void *) desc.window));
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
    it->second->bindToSwapchainBackbuffer(
        bb.image, bb.view, w, h,
        mSurfaceFormat); // this will reset the texture's GPU state to UNDEFINED, which matches the swapchain image state after acquire and before any rendering
    mActiveBackbufferTexture                          = it->second.get();
    mActiveBackbufferTexture->readyPayload->semaphore = mActiveFrame.imageAvailable;

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

    // Get the latest state of the backbuffer image directly from the back buffer texture. This assumes all pending
    // GpuPayloads have been submitted to the GPU already.
    GpuImageState tracked = *backbufferTex->gpuStates.get(0, 0);

    auto &        vkWaitFor      = static_cast<GpuPayloadVulkan &>(waitFor);
    vk::Semaphore renderFinished = vkWaitFor.semaphore;

    rv::Swapchain::PresentResult result;
    try {
        auto pp = rv::Swapchain::PresentParameters({tracked.layout, tracked.access, tracked.stages});
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

    if (backbufferTex) {
        auto & bs = result.backbufferStatus;
        backbufferTex->gpuStates.set(swapchainBackbufferSubresourceRange(), {bs.layout, bs.access, bs.stages});
    }

    clearAcquiredFrameBindings();
}

AutoRef<Swapchain> createSwapchainVulkan2(const Swapchain::CreateDesc & desc) {
    StrA entityName = desc.name.empty() ? StrA("swapchain") : desc.name;
    auto sc         = AutoRef<SwapchainVulkan2>(new SwapchainVulkan2(entityName));
    if (!sc->init(desc)) return {};
    return sc;
}

} // namespace GN::gpu2
