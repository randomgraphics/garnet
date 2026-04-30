// DISABLED: superseded by GpuPayloadVulkan::recordForVulkanSubmit. See vk-gpu-command-buffer.h.
#if 0
    #include "vk-gpu-command-buffer.h"
    #include <garnet/GNrdg2.h>

static GN::Logger * sLogger = GN::getLogger("GN.gfx.gpu2.vk");

namespace GN::gpu2 {

namespace {

static bool isPendingPayload(const ArrayContainer<GpuCommandBuffer::Payload *> & pending, GpuCommandBuffer::Payload * p) {
    for (size_t i = 0; i < pending.size(); ++i)
        if (pending[i] == p) return true;
    return false;
}

static bool removeOnePending(ArrayContainer<GpuCommandBuffer::Payload *> & pending, GpuCommandBuffer::Payload * p) {
    for (size_t i = 0; i < pending.size(); ++i) {
        if (pending[i] == p) {
            pending.eraseIdx(i);
            return true;
        }
    }
    return false;
}

/// Remove \p p from \p pending (if present) and delete it. Used for submit jobs and error cleanup.
static void releasePayloadJob(ArrayContainer<GpuCommandBuffer::Payload *> & pending, GpuCommandBuffer::Payload * p) {
    if (!p) return;
    (void) removeOnePending(pending, p);
    delete p;
}

} // namespace

GpuCommandBufferVulkan2::GpuCommandBufferVulkan2(const StrA & entityName, AutoRef<GpuContextVulkan2> gpu)
    : GpuCommandBufferCommon(TYPE_INFO(), entityName, gpu.template staticCastTo<GpuContext>()) {}

GpuCommandBufferVulkan2::~GpuCommandBufferVulkan2() {
    for (size_t i = 0; i < mPendingSealedPayloads.size(); ++i) delete mPendingSealedPayloads[i];
    mPendingSealedPayloads.clear();
}

void GpuCommandBufferVulkan2::adoptPayloadFromSeal(GpuCommandBuffer::Payload * p) {
    if (!p) return;
    mPendingSealedPayloads.append(p);
}

AutoRef<GpuContextVulkan2> GpuCommandBufferVulkan2::vkGpu() const { return mGpu.template staticCastTo<GpuContextVulkan2>(); }

GpuCommandBuffer::Fence * GpuCommandBufferVulkan2::submit(const SubmitParameters & sp) { return submitVk(sp, {}); }

GpuCommandBuffer::Fence * GpuCommandBufferVulkan2::submitVk(const SubmitParameters & sp, const std::vector<vk::Semaphore> & waitSemaphores) {
    for (size_t i = 0; i < sp.jobs.size(); ++i) {
        GpuCommandBuffer::Payload * pl = sp.jobs[i];
        if (!pl) continue;
        for (size_t j = i + 1; j < sp.jobs.size(); ++j) {
            if (sp.jobs[j] == pl) {
                GN_ERROR(sLogger)("GpuCommandBufferVulkan2::submit: duplicate payload pointer in jobs");
                return nullptr;
            }
        }
        if (!isPendingPayload(mPendingSealedPayloads, pl)) {
            GN_ERROR(sLogger)("GpuCommandBufferVulkan2::submit: payload not created from this command buffer (or already submitted)");
            return nullptr;
        }
    }

    auto releaseAllJobs = [&]() {
        for (size_t i = 0; i < sp.jobs.size(); ++i) releasePayloadJob(mPendingSealedPayloads, sp.jobs[i]);
    };

    AutoRef<GpuContextVulkan2> gpu = vkGpu();
    if (!gpu || !gpu->ready()) {
        GN_ERROR(sLogger)("GpuCommandBufferVulkan2::submit: invalid context");
        releaseAllJobs();
        return nullptr;
    }
    const rv::Device * dev = gpu->vulkanDevice();
    if (!dev || !dev->handle()) {
        GN_ERROR(sLogger)("GpuCommandBufferVulkan2::submit: no device");
        releaseAllJobs();
        return nullptr;
    }

    rv::CommandQueue * queue = dev->graphics();
    if (!queue) {
        GN_ERROR(sLogger)("GpuCommandBufferVulkan2::submit: no graphics queue");
        releaseAllJobs();
        return nullptr;
    }

    VulkanSubmitRecordContext ctx;
    ctx.dev   = dev;
    ctx.queue = queue;
    ctx.cmd   = queue->begin(sp.name.empty() ? "rdg2_cmd" : sp.name.c_str(), vk::CommandBufferLevel::ePrimary);

    for (size_t i = 0; i < sp.jobs.size(); ++i) {
        GpuCommandBuffer::Payload * pl = sp.jobs[i];
        if (pl) pl->recordForVulkanSubmit(&ctx);
    }

    rv::CommandQueue::SubmitParameters qsp;
    qsp.commandBuffers = {ctx.cmd};
    qsp.waitSemaphores = {(uint32_t) waitSemaphores.size(), waitSemaphores.data()};
    queue->submit(qsp);

    dev->waitIdle();

    for (size_t i = 0; i < sp.jobs.size(); ++i) {
        GpuCommandBuffer::Payload * pl = sp.jobs[i];
        if (pl) pl->flushResourceStatesAfterSubmit();
    }

    releaseAllJobs();

    return nullptr;
}

AutoRef<GpuCommandBuffer> createGpuCommandBufferVulkan2(const GpuCommandBuffer::CreateParameters & params) {
    if (!params.gpu) return {};
    AutoRef<GpuContextVulkan2> vk = params.gpu.staticCastTo<GpuContextVulkan2>();
    if (!vk || !vk->ready()) return {};
    StrA n = params.gpu->name.empty() ? StrA("command_buffer") : params.gpu->name + "/cmdbuf";
    return AutoRef<GpuCommandBuffer>(new GpuCommandBufferVulkan2(n, vk));
}

} // namespace GN::gpu2

#endif // 0
