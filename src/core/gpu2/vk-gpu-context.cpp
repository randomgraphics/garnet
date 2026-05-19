#include "pch.h"
#define RAPID_VULKAN_IMPLEMENTATION
#include "vk-gpu-context.h"
#include "gpu-payload-group.h"
#include "vk-raster-pso-factory.h"
#include "vk-format-utils.h"
#include "vk-gpu-payload.h"

#include <functional>
#include <list>
#include <mutex>
#include <utility>
#include <vector>

static GN::Logger * sLoggerVk = GN::getLogger("GN.gpu2.vk");

namespace GN::gpu2 {

static bool containsPayload(const std::vector<const GpuPayload *> & payloads, const GpuPayload * payload) {
    for (auto * p : payloads) {
        if (p == payload) return true;
    }
    return false;
}

static void collectDependencyWaits(const AutoRef<GpuPayload> & payload, std::vector<rv::CommandQueue::SyncPoint> & waitPoints,
                                   std::vector<rv::CommandQueue::SyncPoint> & waitBinaries, std::vector<const GpuPayload *> & seen) {
    if (!payload) GN_UNLIKELY return;
    if (containsPayload(seen, payload.get())) GN_UNLIKELY {
            GN_ERROR(sLoggerVk)("Duplicate dependency payload ignored: {}({})", payload->name, payload->id);
            return;
        }
    seen.push_back(payload.get());

    if (auto group = RuntimeType::cast<GpuPayloadGroup>(payload)) {
        for (const auto & child : group->children()) collectDependencyWaits(child, waitPoints, waitBinaries, seen);
        return;
    }

    auto w = RuntimeType::cast<GpuPayloadVulkan>(*payload);
    if (!w) GN_UNLIKELY {
            GN_ERROR(sLoggerVk)("Unrecognized payload type: {}({})", payload->name, payload->id);
            return;
        }
    const auto & s = w->syncpoint();
    if (const auto * t = s.asTimelinePoint()) {
        waitPoints.push_back(*t);
    } else if (const auto * b = s.asBinarySemaphore()) {
        waitBinaries.push_back({*b, 0, vk::PipelineStageFlagBits::eAllCommands});
    } else {
        GN_ERROR(sLoggerVk)("Can't depend on un-submitted payload: {}({})", payload->name, payload->id);
    }
}

static void collectWorkLeaves(const AutoRef<GpuPayload> & payload, std::vector<AutoRef<GpuPayloadVulkan>> & works, std::vector<const GpuPayload *> & seen) {
    if (!payload) GN_UNLIKELY return;
    if (containsPayload(seen, payload.get())) GN_UNLIKELY {
            GN_ERROR(sLoggerVk)("Duplicate work payload ignored: {}({})", payload->name, payload->id);
            return;
        }
    seen.push_back(payload.get());

    if (auto group = RuntimeType::cast<GpuPayloadGroup>(payload)) {
        for (const auto & child : group->children()) collectWorkLeaves(child, works, seen);
        return;
    }

    auto w = RuntimeType::cast<GpuPayloadVulkan>(payload);
    if (!w) GN_UNLIKELY {
            GN_ERROR(sLoggerVk)("Unrecognized payload type: {}({})", payload->name, payload->id);
            return;
        }
    if (w->syncpoint()) GN_UNLIKELY {
            // This payload has a sync point already. it means it has been submit to GPU already. Reject it.
            GN_ERROR(sLoggerVk)("Can't submit payload {}({}) multiple times to GPU.", w->name, w->id);
            return;
        }
    works.push_back(w);
}

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

struct GpuQueueTimeline {
    vk::UniqueSemaphore semaphore = {};
    uint64_t            counter   = {};

    operator bool() const { return !!semaphore; }
};

struct GpuContextVulkan2::Impl {
    struct PendingSubmission {
        StrA                                   name;
        rv::Ref<rv::CommandQueue>              queue;
        rv::CommandQueue::SyncPoint            timelinePoint;
        PooledFenceVulkan                      fence;
        std::function<void()>                  onComplete; ///< the user provided completion function. might be empty.
        std::vector<AutoRef<GpuPayloadVulkan>> works;
        rv::CommandQueue::SubmissionID         submissionId; ///< ID returned by queue->submit(); used to flush rv::CommandQueue before fence is destroyed.
    };
    std::mutex                   mPendingMutex;
    std::list<PendingSubmission> mPending;

    std::map<rv::CommandQueue *, GpuQueueTimeline> queueTimelines;

    rv::CommandQueue::SyncPoint getNextTimelinePoint(rv::CommandQueue * queue) {
        GN_ASSERT(queue);
        auto & timeline = queueTimelines[queue];
        if (!timeline.semaphore) GN_UNLIKELY {
                vk::SemaphoreTypeCreateInfo timelineInfo {
                    vk::SemaphoreType::eTimeline,
                    0 // initial value
                };
                vk::SemaphoreCreateInfo semaphoreInfo {};
                semaphoreInfo.setPNext(&timelineInfo);
                timeline.semaphore = queue->desc().gi->device.createSemaphoreUnique(semaphoreInfo);
            }
        return {timeline.semaphore.get(), ++timeline.counter, vk::PipelineStageFlagBits::eAllCommands};
    }
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
    dp.addFeature(vk::PhysicalDeviceVulkan13Features().setDynamicRendering(true).setSynchronization2(true));
    // Allow depth and stencil planes of a D+S image to be transitioned independently;
    // without this, separate-aspect barriers on D+S formats are a validation error.
    dp.addFeature(vk::PhysicalDeviceVulkan12Features().setSeparateDepthStencilLayouts(true).setTimelineSemaphore(true));
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

    // collect semaphores to wait from dependencies
    std::vector<rv::CommandQueue::SyncPoint> waitPoints;   // timeline dependencies
    std::vector<rv::CommandQueue::SyncPoint> waitBinaries; // binary dependencies
    std::vector<const GpuPayload *>          seenDependencies;
    for (size_t i = 0; i < sp.dependencies.size(); ++i) { collectDependencyWaits(sp.dependencies[i], waitPoints, waitBinaries, seenDependencies); }

    // allocate the main fence and sync point for this submission.
    auto mainFence = fencePool().acquire(sp.name);
    auto mainPoint = mImpl->getNextTimelinePoint(queue); // stages already set to eAllCommands
    if (!mainFence || !mainPoint) {
        GN_ERROR(sLoggerVk)("failed to acquire the main fence and semaphore for the submission.");
        return;
    }

    // Shared resource-state tracker for this entire submit batch. All payloads use it to
    // compute barriers (reading state committed by prior payloads).
    // Flushed to actual resource objects after successful vkQueueSubmit.
    GpuResourceStateTrackerVulkan batchTracker;

    // construct a record context.
    GpuPayloadVulkan::RecordContext recordCtx;
    recordCtx.dev          = &dev;
    recordCtx.queue        = queue;
    recordCtx.batchTracker = &batchTracker;
    recordCtx.cmd          = queue->begin(sp.name.empty() ? "rdg2_cmd" : sp.name.c_str());
    if (recordCtx.cmd.empty()) {
        GN_ERROR(sLoggerVk)("GpuContextVulkan2::submit: failed to begin command buffer, name='{}'", sp.name);
        return;
    }

    // Record everything to the command buffer, skipping payloads that were already submitted.
    std::vector<AutoRef<GpuPayloadVulkan>> works;
    std::vector<const GpuPayload *>        seenWorks;
    for (size_t i = 0; i < sp.work.size(); ++i) { collectWorkLeaves(sp.work[i], works, seenWorks); }
    for (auto & w : works) { w->recordForVulkanSubmit(recordCtx); }

    // Submit to GPU.
    rv::CommandQueue::SubmitParameters qsp;
    qsp.commandBuffers = {recordCtx.cmd};
    qsp.waitBinaries   = {(uint32_t) waitBinaries.size(), waitBinaries.data()};
    qsp.waitPoints     = {(uint32_t) waitPoints.size(), waitPoints.data()};
    qsp.signalPoints   = {1, &mainPoint};
    qsp.signalFence    = mainFence->get();
    auto submissionId  = queue->submit2(qsp);
    if (!submissionId) return; // submission failed somehow. bail out.

    // The GPU has accepted the commands. Update CPU-side resource state now so subsequent
    // submissions compute correct "from" layouts for their barriers.
    batchTracker.flushToResources();

    // Mark all workload as "submitted"
    for (auto & w : works) {
        w->setTimelinePoint(mainPoint);
        w->onSubmitComplete();
    }

    // Add the submission to pending list.
    std::lock_guard<std::mutex> lock(mImpl->mPendingMutex);
    mImpl->mPending.push_back({sp.name, queue, mainPoint, mainFence, sp.onComplete, std::move(works), submissionId});
}

void GpuContextVulkan2::pumpInternal(bool waitForIdle) {
    if (!mDevice.has_value() || !mDevice->handle()) return;
    vk::Device device = mDevice->gi()->device;

    // Collect finished submissions
    std::list<Impl::PendingSubmission> finishedSubmissions;
    {
        std::lock_guard<std::mutex> lock(mImpl->mPendingMutex);
        auto                        it = mImpl->mPending.begin();
        while (it != mImpl->mPending.end()) {
            bool fenceReady = false;
            if (waitForIdle) {
                (void) device.waitForFences({it->fence->get()}, true, std::numeric_limits<uint64_t>::max());
                fenceReady = true;
            } else {
                fenceReady = device.getFenceStatus(it->fence->get()) == vk::Result::eSuccess;
            }
            if (fenceReady) {
                // Flush rv::CommandQueue's InternalSubmission for this fence BEFORE erasing (which destroys the fence).
                // Without this, rv::CommandQueue::waitIdle() in the destructor would try to vkWaitForFences on an
                // already-destroyed handle, triggering a validation error and SIGTRAP.
                GN_ASSERT(it->queue);
                it->queue->wait({it->submissionId});
                auto curr = it;
                ++it;
                finishedSubmissions.splice(finishedSubmissions.end(), mImpl->mPending, curr);
            } else {
                ++it;
            }
        }
        GN_ASSERT(!waitForIdle || mImpl->mPending.empty());
    }

    // Invoke completion callbacks, outside of the pending lock.
    for (auto & s : finishedSubmissions) {
        try {
            if (s.onComplete) s.onComplete();
        } catch (const std::exception & e) {
            GN_ERROR(sLoggerVk)("GpuContextVulkan2: {} onComplete callback threw exception: {}", s.name, e.what());
        } catch (...) { GN_ERROR(sLoggerVk)("GpuContextVulkan2: {} onComplete callback threw unknown exception", s.name); }
    }

    // Our pending list doesn't cover all in-flight work (e.g. bridge submits in the swapchain
    // that have no completion callback). A device-level idle wait covers those too.
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
