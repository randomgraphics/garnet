#include "pch.h"
#include "vk-copy.h"
#include "vk-submission-context.h"
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
        auto     srcTransient = runtimeCast<const TransientBufferVulkan>(arguments.src);
        if (srcTransient) srcOff += srcTransient->offset();

        uint64_t dstOff       = arguments.dstOffset;
        auto     dstTransient = runtimeCast<const TransientBufferVulkan>(arguments.dst);
        if (dstTransient) dstOff += dstTransient->offset();

        auto & sc = taskInfo.submission.ensureSubmissionContext<SubmissionContextVulkan>(mGpu);
        auto   cb = sc.commandBufferManager.execute(taskInfo);
        GN_RDG_FAIL_ON_FAIL(cb);
        cb.commandBuffer().handle().copyBuffer(srcHandle, dstHandle, vk::BufferCopy(srcOff, dstOff, arguments.size));

        // done
        return PASSED;
    }

    ExecutionResult copyBufferToImage(TaskInfo & taskInfo, const BufferToImage & arguments) {
        (void) taskInfo;
        (void) arguments;
        GN_ERROR(sLogger)("GpuCopyVulkan::copyBufferToImage: not implemented");
        return FAILED;
    }

public:
    GpuCopyVulkan(ArtifactDatabase & db, const StrA & name, AutoRef<GpuContextVulkan> gpu, const GpuCopy::CreateParameters & params)
        : GpuCopy(db, TYPE_INFO(), name), mGpu(std::move(gpu)), mCreateParams(params) {}

    bool validate(const Arguments & arguments) const override {
        const auto * a = runtimeCast<BufferToBuffer>(&arguments);
        if (a) {
            if (!a->src) GN_UNLIKELY {
                    GN_ERROR(sLogger)("GpuCopyVulkan::validate: src is null");
                    return false;
                }
            if (!a->dst) {
                GN_ERROR(sLogger)("GpuCopyVulkan::validate: dst is null");
                return false;
            }
            // make sure the offset and size are valid
            const uint64_t srcSize = BufferUtils::getSize(a->src);
            const uint64_t dstSize = BufferUtils::getSize(a->dst);
            if ((a->srcOffset + a->size) > srcSize) GN_UNLIKELY {
                    GN_ERROR(sLogger)("GpuCopyVulkan::validate: src offset and size are out of bounds");
                    return false;
                }
            if ((a->dstOffset + a->size) > dstSize) GN_UNLIKELY {
                    GN_ERROR(sLogger)("GpuCopyVulkan::validate: dst offset and size are out of bounds");
                    return false;
                }
            // make sure the src and dst are not the same
            if (a->src == a->dst) GN_UNLIKELY {
                    GN_ERROR(sLogger)("GpuCopyVulkan::validate: src and dst are the same");
                    return false;
                }
            return true;
        }
        if (runtimeCast<BufferToImage>(&arguments)) {
            GN_ERROR(sLogger)("GpuCopyVulkan::validate: BufferToImage not implemented");
            return false;
        }
        GN_ERROR(sLogger)("GpuCopyVulkan::validate: arguments must be either BufferToBuffer or BufferToImage");
        return false;
    }

    ExecutionResult prepare(TaskInfo & taskInfo, Arguments &) override {
        auto & sc = taskInfo.submission.ensureSubmissionContext<SubmissionContextVulkan>(mGpu);
        GN_RDG_FAIL_ON_FAIL(sc.commandBufferManager.prepare(taskInfo, CommandBufferManagerVulkan::GRAPHICS));
        return PASSED;
    }

    ExecutionResult execute(TaskInfo & taskInfo, Arguments & arguments) override {
        const auto * bufferToBuffer = runtimeCast<BufferToBuffer>(arguments);
        if (bufferToBuffer) return copyBufferToBuffer(taskInfo, *bufferToBuffer);

        const auto * bufferToImage = runtimeCast<BufferToImage>(arguments);
        if (bufferToImage) return copyBufferToImage(taskInfo, *bufferToImage);

        GN_ERROR(sLogger)("GpuCopyVulkan::execute: arguments is not BufferToBuffer or BufferToImage");
        return FAILED;
    }
};

// ─────────────────────────────────────────────────────────────────────────────
// Factory
// ─────────────────────────────────────────────────────────────────────────────

AutoRef<GpuCopy> createVulkanGpuCopy(ArtifactDatabase & db, const StrA & name, const GpuCopy::CreateParameters & params) {
    if (!params.gpu) GN_UNLIKELY {
            GN_ERROR(sLogger)("createVulkanGpuCopy: gpu is null, name='{}'", name);
            return {};
        }
    auto gpu = params.gpu.staticCastTo<GpuContextVulkan>();
    if (!gpu) GN_UNLIKELY {
            GN_ERROR(sLogger)("createVulkanGpuCopy: gpu is not Vulkan, name='{}'", name);
            return {};
        }
    auto p = new GpuCopyVulkan(db, name, std::move(gpu), params);
    if (!p->sequence) GN_UNLIKELY {
            GN_ERROR(sLogger)("createVulkanGpuCopy: duplicate name, name='{}'", name);
            delete p;
            return {};
        }
    return AutoRef<GpuCopy>(p);
}

} // namespace GN::rdg
