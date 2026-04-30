#define RAPID_VULKAN_IMPLEMENTATION
#define RAPID_VULKAN_EXTERNAL_C_IMPL
#include "vk-gpu-context.h"
#include "vk-format-utils.h"
#include "vk-gpu-payload.h"

#include <functional>
#include <mutex>
#include <utility>
#include <vector>

static GN::Logger * sLoggerVk = GN::getLogger("GN.gfx.gpu2.vk");

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
// GpuContextVulkan2
// =============================================================================

struct GpuContextVulkan2::Impl {
    std::optional<rv::Instance> mInstance;
    std::optional<rv::Device>   mDevice;
    GpuContext::Caps            mCaps;

    struct PendingSubmission {
        StrA                                   name;
        vk::UniqueFence                        fence;
        std::function<void()>                  onComplete;
        std::vector<AutoRef<GpuPayloadVulkan>> works;
    };
    std::mutex                     mPendingMutex;
    std::vector<PendingSubmission> mPending;
};

GpuContextVulkan2::GpuContextVulkan2(const StrA & name, const CreateParameters & params)
    : GpuContextCommon2(name, GpuContextCommon2::Api::VULKAN), mImpl(std::make_unique<Impl>()) {
    rv::Instance::ConstructParameters ip;
    ip.apiVersion = VK_API_VERSION_1_3;
    ip.backtrace  = []() -> std::string { return GN::backtrace().c_str(); };
    ip.setValidation(getVkInstanceValidation(params.debug));
    ip.setPrintVkInfo(getVkDeviceVerbosity(params.howToPrintDeviceCaps));
    mImpl->mInstance.emplace(ip);
    if (!mImpl->mInstance->handle()) {
        GN_ERROR(sLoggerVk)("GpuContextVulkan2: failed to create Vulkan instance, name='{}'", name);
        return;
    }
    rv::Device::ConstructParameters dp;
    dp.setPrintVkInfo(ip.printVkInfo);
#if GN_DARWIN
    dp.addDeviceExtension("VK_KHR_dynamic_rendering");
#endif
    dp.addFeature(vk::PhysicalDeviceVulkan13Features().setDynamicRendering(true));
    dp.setInstance(mImpl->mInstance->handle());
    mImpl->mDevice.emplace(dp);
    if (mImpl->mDevice->handle()) {
        vk::Format depthVk = rv::queryDepthFormat(mImpl->mDevice->gi()->physical, 1);
        if (depthVk != vk::Format::eUndefined) {
            mImpl->mCaps.defaultDepthFormat = vkFormatToPixelFormat(depthVk);
            if (mImpl->mCaps.defaultDepthFormat == gfx::img::PixelFormat::UNKNOWN()) {
                GN_WARN(sLoggerVk)("GpuContextVulkan2: vkFormatToPixelFormat({}) returned UNKNOWN; caps.defaultDepthFormat stays UNKNOWN",
                                   rv::vkFormat2String(depthVk));
            }
        } else {
            GN_WARN(sLoggerVk)("GpuContextVulkan2: no depth/stencil format from queryDepthFormat; caps.defaultDepthFormat stays UNKNOWN");
        }
    }
}

GpuContextVulkan2::~GpuContextVulkan2() {
    GN_INFO(sLoggerVk)("Wait for GPU idle ...");
    pumpInternal(true);
    GN_INFO(sLoggerVk)("Destroying Vulkan GPU context");
}

GpuContext::Caps GpuContextVulkan2::caps() const {
    if (!mImpl) return {};
    return mImpl->mCaps;
}

intptr_t GpuContextVulkan2::getVulkanInstanceHandle() const {
    if (!mImpl || !mImpl->mInstance || !mImpl->mInstance->handle()) return 0;
    return (intptr_t) (void *) mImpl->mInstance->handle();
}

bool GpuContextVulkan2::ready() const {
    if (!mImpl) return false;
    if (!mImpl->mInstance.has_value() || !mImpl->mInstance->handle()) return false;
    if (!mImpl->mDevice.has_value() || !mImpl->mDevice->handle()) return false;
    return true;
}

const rv::Device * GpuContextVulkan2::vulkanDevice() const {
    if (!ready()) return nullptr;
    return &mImpl->mDevice.value();
}

void GpuContextVulkan2::submit(const SubmitParameters & sp) {
    const rv::Device * dev = vulkanDevice();
    if (!dev) GN_UNLIKELY {
            GN_ERROR(sLoggerVk)("GpuContextVulkan2::submit failed: device not ready, name='{}'", sp.name);
            return;
        }

    // Deal with empty submission.
    if (sp.work.empty()) GN_UNLIKELY {
            if (sp.onComplete) sp.onComplete();
            return;
        }

    rv::CommandQueue * queue = dev->graphics(); // TODO: support sp.queue
    if (!queue) GN_UNLIKELY {
            GN_ERROR(sLoggerVk)("GpuContextVulkan2::submit failed: graphics queue not available, name='{}'", sp.name);
            return;
        }

    // construct a record context.
    GpuPayloadVulkan::RecordContext recordCtx;
    recordCtx.dev   = dev;
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
        vk::UniqueFence fence = mImpl->mDevice->gi()->device.createFenceUnique({});
        qsp.signalFence       = fence.get();
        queue->submit(qsp);
        std::lock_guard<std::mutex> lock(mImpl->mPendingMutex);
        mImpl->mPending.push_back({sp.name, std::move(fence), sp.onComplete, std::move(works)});
    } else {
        queue->submit(qsp);
    }
}

void GpuContextVulkan2::pumpInternal(bool waitForIdle) {
    if (!mImpl || !mImpl->mDevice || !mImpl->mDevice->handle()) return;
    vk::Device device = mImpl->mDevice->gi()->device;

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
