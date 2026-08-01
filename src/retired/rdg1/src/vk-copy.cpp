#include "pch.h"
#include "vk-barrier-log.h"
#include "vk-copy.h"
#include "vk-submission-context.h"
#include "vk-buffer-state.h"
#include "vk-command-buffer.h"
#include "vk-transient-buffer.h"

static GN::Logger * sLogger = GN::getLogger("GN.rdg");

namespace GN::rdg {

class GpuCopyVulkan : public GpuCopy {
    AutoRef<GpuContextVulkan> mGpu;
    GpuCopy::CreateParameters mCreateParams {};

    ExecutionResult copyBufferToBuffer(TaskInfo & taskInfo, const BufferToBuffer & arguments) {
        if (0 == arguments.size) return PASSED;

        GN_RDG_FAIL_ON_FALSE(arguments.src, "{} - CopyBufferVulkan::execute: srcBuffer is null", taskInfo);
        GN_RDG_FAIL_ON_FALSE(arguments.dst, "{} - CopyBufferVulkan::execute: dstBuffer is null", taskInfo);

        auto srcHandle = BufferUtils::getHandle(arguments.src);
        auto dstHandle = BufferUtils::getHandle(arguments.dst);
        GN_RDG_FAIL_ON_FALSE(srcHandle && dstHandle, "{} - CopyBufferVulkan::execute: srcBuffer or dstBuffer is properly initialized", taskInfo);

        uint64_t srcOff       = arguments.srcOffset;
        auto     srcTransient = RuntimeType::cast<const TransientBufferVulkan>(arguments.src);
        if (srcTransient) srcOff += srcTransient->offset();

        uint64_t dstOff       = arguments.dstOffset;
        auto     dstTransient = RuntimeType::cast<const TransientBufferVulkan>(arguments.dst);
        if (dstTransient) dstOff += dstTransient->offset();

        auto & sc = taskInfo.submission.ensureSubmissionContext<SubmissionContextVulkan>(mGpu);
        auto   cb = sc.commandBufferManager.execute(taskInfo, CommandBufferManagerVulkan::GRAPHICS);
        GN_RDG_FAIL_ON_FALSE(cb);

        const vk::AccessFlags        transferRead  = vk::AccessFlagBits::eTransferRead;
        const vk::AccessFlags        transferWrite = vk::AccessFlagBits::eTransferWrite;
        const vk::PipelineStageFlags transferStage = vk::PipelineStageFlagBits::eTransfer;

        rapid_vulkan::Barrier  barrier;
        vk::PipelineStageFlags combinedSrcStage {};
        vk::PipelineStageFlags combinedDstStage {};

        Buffer * srcBuf = arguments.src.get();
        Buffer * dstBuf = arguments.dst.get();
        if (cb.transitionBuffer(srcBuf, srcOff, arguments.size, transferRead, transferStage)) {
            const auto * st = cb.getBufferState(srcBuf);
            if (st) {
                combinedSrcStage |= st->prev.stage;
                combinedDstStage |= st->curr.stage;
                barrier.b(srcHandle, st->prev.access, st->curr.access, srcOff, arguments.size);
            }
        }
        if (cb.transitionBuffer(dstBuf, dstOff, arguments.size, transferWrite, transferStage)) {
            const auto * st = cb.getBufferState(dstBuf);
            if (st) {
                combinedSrcStage |= st->prev.stage;
                combinedDstStage |= st->curr.stage;
                barrier.b(dstHandle, st->prev.access, st->curr.access, dstOff, arguments.size);
            }
        }
        if (combinedSrcStage) barrier.s(combinedSrcStage, combinedDstStage ? combinedDstStage : transferStage);
        logBarrierBatchVerbose(sLogger, "gpu-copy buffer-to-buffer", barrier);
        barrier.cmdWrite(cb.rapid().handle());

        cb.rapid().handle().copyBuffer(srcHandle, dstHandle, vk::BufferCopy(srcOff, dstOff, arguments.size));

        // done
        return PASSED;
    }

    ExecutionResult copyBufferToImage(TaskInfo & taskInfo, const BufferToImage & arguments) {
        (void) taskInfo;
        (void) arguments;
        GN_ERROR(sLogger, "GpuCopyVulkan::copyBufferToImage: not implemented");
        return FAILED;
    }

public:
    GpuCopyVulkan(const StrA & name, AutoRef<GpuContextVulkan> gpu, const GpuCopy::CreateParameters & params)
        : GpuCopy(TYPE_INFO(), name), mGpu(std::move(gpu)), mCreateParams(params) {}

    bool validate(const TaskInfo & taskInfo, const Arguments & arguments) const {
        const auto * a = RuntimeType::cast<BufferToBuffer>(&arguments);
        if (a) {
            if (!a->src) GN_UNLIKELY {
                    GN_ERROR(sLogger, "{} - GpuCopyVulkan::validate: src is null", taskInfo);
                    return false;
                }
            if (!a->dst) {
                GN_ERROR(sLogger, "{} - GpuCopyVulkan::validate: dst is null", taskInfo);
                return false;
            }
            // make sure the offset and size are valid
            const uint64_t srcSize = BufferUtils::getSize(a->src);
            const uint64_t dstSize = BufferUtils::getSize(a->dst);
            if ((a->srcOffset + a->size) > srcSize) GN_UNLIKELY {
                    GN_ERROR(sLogger, "{} - GpuCopyVulkan::validate: src offset and size are out of bounds", taskInfo);
                    return false;
                }
            if ((a->dstOffset + a->size) > dstSize) GN_UNLIKELY {
                    GN_ERROR(sLogger, "{} - GpuCopyVulkan::validate: dst offset and size are out of bounds", taskInfo);
                    return false;
                }
            // make sure the src and dst are not the same
            if (a->src == a->dst) GN_UNLIKELY {
                    GN_ERROR(sLogger, "{} - GpuCopyVulkan::validate: src and dst are the same", taskInfo);
                    return false;
                }
            return true;
        }
        if (RuntimeType::cast<BufferToImage>(&arguments)) {
            GN_ERROR(sLogger, "{} - GpuCopyVulkan::validate: BufferToImage not implemented", taskInfo);
            return false;
        }
        GN_ERROR(sLogger, "{} - GpuCopyVulkan::validate: arguments must be either BufferToBuffer or BufferToImage", taskInfo);
        return false;
    }

    Action::PrepareResult prepare(TaskInfo & taskInfo, Arguments & arguments) override {
        GN_RDG_FAIL_ON_FALSE(validate(taskInfo, arguments));
        auto & sc = taskInfo.submission.ensureSubmissionContext<SubmissionContextVulkan>(mGpu);
        GN_RDG_FAIL_ON_FAIL(sc.commandBufferManager.prepare(taskInfo));
        // One execute step: record/cmd+barriers+copy, then let the manager submit/flush.
        return {PASSED, 1};
    }

    ExecutionResult execute(TaskInfo & taskInfo, [[maybe_unused]] size_t step, Arguments & arguments) override {
        GN_ASSERT(0 == step);
        const auto * bufferToBuffer = RuntimeType::cast<BufferToBuffer>(arguments);
        if (bufferToBuffer) return copyBufferToBuffer(taskInfo, *bufferToBuffer);

        const auto * bufferToImage = RuntimeType::cast<BufferToImage>(arguments);
        if (bufferToImage) return copyBufferToImage(taskInfo, *bufferToImage);

        GN_ERROR(sLogger, "GpuCopyVulkan::execute: arguments is not BufferToBuffer or BufferToImage");
        return FAILED;
    }
};

// ─────────────────────────────────────────────────────────────────────────────
// Factory
// ─────────────────────────────────────────────────────────────────────────────

AutoRef<GpuCopy> createVulkanGpuCopy(const StrA & name, const GpuCopy::CreateParameters & params) {
    if (!params.gpu) GN_UNLIKELY {
            GN_ERROR(sLogger, "createVulkanGpuCopy: gpu is null, name='{}'", name);
            return {};
        }
    auto gpu = params.gpu.staticCastTo<GpuContextVulkan>();
    if (!gpu) GN_UNLIKELY {
            GN_ERROR(sLogger, "createVulkanGpuCopy: gpu is not Vulkan, name='{}'", name);
            return {};
        }
    return AutoRef<GpuCopy>(new GpuCopyVulkan(name, std::move(gpu), params));
}

} // namespace GN::rdg
