#define RAPID_VULKAN_IMPLEMENTATION
#define RAPID_VULKAN_EXTERNAL_C_IMPL
#include "vk-gpu-context.h"
#include "vk-raster-pso-factory.h"
#include "vk-format-utils.h"
#include "vk-gpu-payload.h"

#include <functional>
#include <mutex>
#include <utility>
#include <vector>

static GN::Logger * sLoggerVk = GN::getLogger("GN.gpu2.vk");

namespace GN::gpu2 {

// =============================================================================
// Validation / verbosity (duplicated from vk-gpu-context.cpp; v2 uses enum class)
// =============================================================================

static rv::Instance::Validation getVkInstanceValidation(GpuContext::DebugMode debugMode) {
    switch (debugMode) {
    case GpuContext::DebugMode::DISABLED:
        return rv::Instance::Validation::VALIDATION_DISABLED;
    case GpuContext::DebugMode::ENABLED:
#if GN_BUILD_DEBUG_ENABLED
        return rv::Instance::Validation::LOG_ON_VK_ERROR | rv::Instance::Validation::BREAK_ON_VK_ERROR;
#else
        return rv::Instance::Validation::LOG_ON_VK_ERROR;
#endif
    default:
        return rv::Instance::Validation::VALIDATION_DISABLED;
    }
}

static rv::Device::Verbosity getVkDeviceVerbosity(GpuContext::Verbosity verbosity) {
    switch (verbosity) {
    case GpuContext::Verbosity::SILENCE:
        return rv::Device::Verbosity::SILENCE;
    case GpuContext::Verbosity::BRIEF:
        return rv::Device::Verbosity::BRIEF;
    case GpuContext::Verbosity::VERBOSE:
        return rv::Device::Verbosity::VERBOSE;
    default:
        return rv::Device::Verbosity::SILENCE;
    }
}

// =============================================================================
// Resource pool traits
// =============================================================================

struct SemaphoreTraits final : ResourcePoolVulkan<vk::Semaphore>::Traits {
    const rv::GlobalInfo & gi;

    explicit SemaphoreTraits(const rv::GlobalInfo & gi_): gi(gi_) {}

    vk::Semaphore create() override { return gi.device.createSemaphore({}, gi.allocator); }

    void destroy(vk::Semaphore & s) override {
        gi.device.destroySemaphore(s, gi.allocator);
        s = nullptr;
    }

    void onAcquire(vk::Semaphore & s, const char * name) override { rv::setVkHandleName(gi.device, s, name); }
};

struct FenceTraits final : ResourcePoolVulkan<vk::Fence>::Traits {
    const rv::GlobalInfo & gi;

    explicit FenceTraits(const rv::GlobalInfo & gi_): gi(gi_) {}

    vk::Fence create() override { return gi.device.createFence({}, gi.allocator); } // unsignaled by default

    void destroy(vk::Fence & f) override {
        gi.device.destroyFence(f, gi.allocator);
        f = nullptr;
    }

    void onAcquire(vk::Fence & f, const char * name) override {
        rv::setVkHandleName(gi.device, f, name);
        // Reset to unsignaled before returning to caller so every re-use starts clean.
        gi.device.resetFences({f});
    }
};

// =============================================================================
// GpuContextVulkan2
// =============================================================================

struct GpuContextVulkan2::Impl {
    struct PendingSubmission {
        StrA                                   name;
        rv::Ref<rv::CommandQueue>              queue;
        vk::UniqueFence                        fence;
        std::function<void()>                  onComplete;
        std::vector<AutoRef<GpuPayloadVulkan>> works;
        rv::CommandQueue::SubmissionID         submissionId; ///< ID returned by queue->submit(); used to flush rv::CommandQueue before fence is destroyed.
    };
    std::mutex                     mPendingMutex;
    std::vector<PendingSubmission> mPending;
};

GpuContextVulkan2::GpuContextVulkan2(const StrA & name, const CreateParameters & params)
    : GpuContextCommon2(name, GpuContextCommon2::Api::VULKAN), mImpl(std::make_unique<Impl>()) {

    // Create instance
    rv::Instance::ConstructParameters ip;
    ip.apiVersion = VK_API_VERSION_1_3;
    ip.backtrace  = []() -> std::string { return GN::backtrace().c_str(); };
    ip.setValidation(getVkInstanceValidation(params.debug));
    ip.setPrintVkInfo(getVkDeviceVerbosity(params.howToPrintDeviceCaps));
    mInstance.emplace(ip);
    if (!mInstance->handle()) {
        GN_ERROR(sLoggerVk)("GpuContextVulkan2: failed to create Vulkan instance, name='{}'", name);
        return;
    }

    // Create device
    rv::Device::ConstructParameters dp;
    dp.setPrintVkInfo(ip.printVkInfo);
#if GN_DARWIN
    dp.addDeviceExtension("VK_KHR_dynamic_rendering");
#endif
    dp.addFeature(vk::PhysicalDeviceVulkan13Features().setDynamicRendering(true));
    // Allow depth and stencil planes of a D+S image to be transitioned independently;
    // without this, separate-aspect barriers on D+S formats are a validation error.
    dp.addFeature(vk::PhysicalDeviceVulkan12Features().setSeparateDepthStencilLayouts(true));
    dp.setInstance(mInstance->handle());
    mDevice.emplace(dp);
    if (!mDevice->handle()) return;

    // Initialize caps
    vk::Format depthVk = rv::queryDepthFormat(mDevice->gi()->physical, 1);
    if (depthVk != vk::Format::eUndefined) {
        mCaps.defaultDepthFormat = vkFormatToPixelFormat(depthVk);
        if (mCaps.defaultDepthFormat == gfx::img::PixelFormat::UNKNOWN()) {
            GN_WARN(sLoggerVk)("GpuContextVulkan2: vkFormatToPixelFormat({}) returned UNKNOWN; caps.defaultDepthFormat stays UNKNOWN",
                               rv::vkFormat2String(depthVk));
        }
    } else {
        GN_WARN(sLoggerVk)("GpuContextVulkan2: no depth/stencil format from queryDepthFormat; caps.defaultDepthFormat stays UNKNOWN");
    }

    // Create sub objects
    mPsoFactory = std::make_unique<RasterPsoFactory>(*this);
    mSemaphorePool.emplace(std::make_unique<SemaphoreTraits>(*mDevice->gi()));
    mFencePool.emplace(std::make_unique<FenceTraits>(*mDevice->gi()));

    // All done
    mReady = true; // must be last; inline ready() is called throughout this TU
}

GpuContextVulkan2::~GpuContextVulkan2() {
    GN_INFO(sLoggerVk)("Wait for GPU idle ...");
    pumpInternal(true);
    GN_INFO(sLoggerVk)("Destroying Vulkan GPU context");
}

void GpuContextVulkan2::submit(const SubmitParameters & sp) {
    if (!ready()) GN_UNLIKELY {
            GN_ERROR(sLoggerVk)("GpuContextVulkan2::submit failed: device not ready, name='{}'", sp.name);
            return;
        }

    // Deal with empty submission.
    if (sp.work.empty()) GN_UNLIKELY {
            if (sp.onComplete) sp.onComplete();
            return;
        }

    const rv::Device & dev   = vulkanDevice();
    rv::CommandQueue * queue = dev.graphics(); // TODO: support sp.queue
    if (!queue) GN_UNLIKELY {
            GN_ERROR(sLoggerVk)("GpuContextVulkan2::submit failed: graphics queue not available, name='{}'", sp.name);
            return;
        }

    // construct a record context.
    GpuPayloadVulkan::RecordContext recordCtx;
    recordCtx.dev   = &dev;
    recordCtx.queue = queue;
    recordCtx.cmd   = queue->begin(sp.name.empty() ? "rdg2_cmd" : sp.name.c_str());
    if (recordCtx.cmd.empty()) {
        GN_ERROR(sLoggerVk)("GpuContextVulkan2::submit: failed to begin command buffer, name='{}'", sp.name);
        return;
    }

    // collect semaphores to wait for from dependencies
    std::vector<vk::Semaphore>             waitSems;
    std::vector<vk::Semaphore>             signalSems;
    std::vector<AutoRef<GpuPayloadVulkan>> works;
    for (size_t i = 0; i < sp.waitForGpu.size(); ++i) {
        auto w = RuntimeType::cast<GpuPayloadVulkan>(sp.waitForGpu[i]);
        if (w && w->semaphore) waitSems.push_back(w->semaphore);
    }
    for (size_t i = 0; i < sp.work.size(); ++i) {
        auto s = RuntimeType::cast<GpuPayloadVulkan>(sp.work[i]);
        if (s) {
            works.push_back(s);
            s->recordForVulkanSubmit(recordCtx);
            if (s->semaphore) signalSems.push_back(s->semaphore);
        }
    }

    rv::CommandQueue::SubmitParameters qsp;
    qsp.commandBuffers   = {recordCtx.cmd};
    qsp.waitSemaphores   = {(uint32_t) waitSems.size(), waitSems.data()};
    qsp.signalSemaphores = {(uint32_t) signalSems.size(), signalSems.data()};

    if (sp.onComplete) {
        vk::UniqueFence fence = mDevice->gi()->device.createFenceUnique({});
        qsp.signalFence       = fence.get();
        auto submissionId     = queue->submit(qsp);
        for (auto & w : works) {
            if (w) w->onSubmitComplete();
        }
        std::lock_guard<std::mutex> lock(mImpl->mPendingMutex);
        mImpl->mPending.push_back({sp.name, queue, std::move(fence), sp.onComplete, std::move(works), submissionId});
    } else {
        queue->submit(qsp);
        for (auto & w : works) {
            if (w) w->onSubmitComplete();
        }
    }
}

void GpuContextVulkan2::pumpInternal(bool waitForIdle) {
    if (!mDevice.has_value() || !mDevice->handle()) return;
    vk::Device device = mDevice->gi()->device;

    // Collect completed callbacks outside the lock so they can safely call submit() or pump() themselves.
    std::vector<std::pair<StrA, std::function<void()>>> readyCallbacks;
    {
        std::lock_guard<std::mutex> lock(mImpl->mPendingMutex);
        auto                        it = mImpl->mPending.begin();
        while (it != mImpl->mPending.end()) {
            bool fenceReady = false;
            if (waitForIdle) {
                (void) device.waitForFences({it->fence.get()}, true, std::numeric_limits<uint64_t>::max());
                fenceReady = true;
            } else {
                fenceReady = device.getFenceStatus(it->fence.get()) == vk::Result::eSuccess;
            }
            if (fenceReady) {
                // Flush rv::CommandQueue's InternalSubmission for this fence BEFORE erasing (which destroys the fence).
                // Without this, rv::CommandQueue::waitIdle() in the destructor would try to vkWaitForFences on an
                // already-destroyed handle, triggering a validation error and SIGTRAP.
                GN_ASSERT(it->queue);
                it->queue->wait({it->submissionId});
                readyCallbacks.push_back({std::move(it->name), std::move(it->onComplete)});
                it = mImpl->mPending.erase(it);
            } else {
                ++it;
            }
        }
    }
    GN_ASSERT(!waitForIdle || mImpl->mPending.empty());
    for (auto & cb : readyCallbacks) {
        try {
            cb.second();
        } catch (const std::exception & e) {
            GN_ERROR(sLoggerVk)("GpuContextVulkan2: {} onComplete callback threw exception: {}", cb.first, e.what());
        } catch (...) { GN_ERROR(sLoggerVk)("GpuContextVulkan2: {} onComplete callback threw unknown exception", cb.first); }
    }

    // Our pending list does not represent all submitted and pending GPU works, since we currently only track
    // submissions with completion callbacks. If waitForIdle is requested, on top of making sure all pending callbacks are flushed, we also need to call
    // device.waitIdle() to ensure all GPU works are done before returning. we also need to call device.waitIdle() to ensure all GPU works are done before
    // returning.
    if (waitForIdle) device.waitIdle();
}

// =============================================================================
// createVulkanGpuContext2
// =============================================================================

AutoRef<GpuContext> createVulkanGpuContext2(const StrA & name, const GpuContext::CreateParameters & params) {
    auto p = std::make_unique<GpuContextVulkan2>(name, params);
    if (!p->ready()) return {};
    return AutoRef<GpuContext>(p.release());
}

} // namespace GN::gpu2
