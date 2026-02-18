#include "vk-draw-actions.h"
#include "vk-command-buffer.h"
#include "vk-render-pass.h"
#include "vk-resource-tracker.h"
#include "vk-backbuffer.h"
#include "vk-texture.h"
#include "submission.h"

namespace GN::rdg {

static GN::Logger * sLogger = getLogger("GN.rdg");

class ClearRenderTargetVulkan : public ClearRenderTarget {
    AutoRef<GpuContextVulkan> mGpu;

public:
    ClearRenderTargetVulkan(ArtifactDatabase & db, const StrA & name, AutoRef<GpuContextVulkan> gpu): ClearRenderTarget(db, TYPE, name), mGpu(gpu) {}

    std::pair<ExecutionResult, ExecutionContext *> prepare(TaskInfo & taskInfo, Arguments & arguments) override {
        auto & submissionImpl = static_cast<SubmissionImpl &>(taskInfo.submission);

        auto a = arguments.castTo<ClearRenderTarget::A>();
        if (!a) GN_UNLIKELY {
                GN_ERROR(sLogger)("ClearRenderTargetVulkan::prepare: arguments is not ClearRenderTarget::A");
                return std::make_pair(FAILED, nullptr);
            }
        auto renderTarget = a->renderTarget.get();
        if (!renderTarget) GN_UNLIKELY {
                GN_ERROR(sLogger)("ClearRenderTargetVulkan::prepare: render target is null");
                return std::make_pair(FAILED, nullptr);
            }

        auto ctx = std::make_unique<DrawActionContextVulkan>();

        // prepare command buffer.
        auto & cbm           = submissionImpl.ensureExecutionContext<CommandBufferManagerVulkan>(CommandBufferManagerVulkan::ConstructParameters {.gpu = mGpu});
        ctx->commandBufferId = cbm.prepare(CommandBufferManagerVulkan::GRAPHICS);
        if (!ctx->commandBufferId) GN_UNLIKELY {
                GN_ERROR(sLogger)("ClearRenderTargetVulkan::prepare: failed to prepare command buffer");
                return std::make_pair(FAILED, nullptr);
            }

        // prepare render pass
        auto & rpm = submissionImpl.ensureExecutionContext<RenderPassManagerVulkan>(RenderPassManagerVulkan::ConstructParameters {.gpu = mGpu});
        if (!rpm.prepare(taskInfo, *a->renderTarget.get())) GN_UNLIKELY {
                GN_ERROR(sLogger)("ClearRenderTargetVulkan::prepare: failed to prepare render pass");
                return std::make_pair(FAILED, nullptr);
            }

        // done
        return std::make_pair(PASSED, ctx.release());
    }

    ExecutionResult execute(TaskInfo & taskInfo, Arguments & arguments, ExecutionContext * context) override {
        bool hasWarning = false;

        auto & submissionImpl = static_cast<SubmissionImpl &>(taskInfo.submission);

        auto a = arguments.castTo<ClearRenderTarget::A>();
        if (!a) GN_UNLIKELY {
                GN_ERROR(sLogger)("ClearRenderTargetVulkan::prepare: arguments is not ClearRenderTarget::A");
                return FAILED;
            }
        auto renderTarget = a->renderTarget.get();
        if (!renderTarget) GN_UNLIKELY {
                GN_ERROR(sLogger)("ClearRenderTargetVulkan::prepare: render target is null");
                return FAILED;
            }

        auto ctx = static_cast<DrawActionContextVulkan *>(context);
        if (!ctx) GN_UNLIKELY {
                GN_ERROR(sLogger)("ClearRenderTargetVulkan::execute: context is not DrawActionContextVulkan");
                return FAILED;
            }

        // acquire command buffer.
        auto cbm = submissionImpl.getExecutionContext<CommandBufferManagerVulkan>();
        if (!cbm) GN_UNLIKELY {
                GN_ERROR(sLogger)("ClearRenderTargetVulkan::execute: CommandBufferManagerVulkan not found");
                return FAILED;
            }
        auto cb = cbm->execute(ctx->commandBufferId);
        if (!cb.queue || !cb.commandBuffer) GN_UNLIKELY {
                GN_ERROR(sLogger)("ClearRenderTargetVulkan::execute: failed to acquire command buffer");
                return FAILED;
            }

        // execute resource tracker to update GPU resource layout and memory usage.
        auto & rt = submissionImpl.ensureExecutionContext<ResourceTrackerVulkan>(
            ResourceTrackerVulkan::ConstructParameters {.submission = submissionImpl, .gpu = mGpu});
        if (!rt.execute(ResourceTrackerVulkan::ActionParameters().setRenderTarget(a->renderTarget.get()), cb.commandBuffer->handle())) GN_UNLIKELY {
                GN_ERROR(sLogger)("ClearRenderTargetVulkan::execute: failed to execute resource tracker");
                return FAILED;
            }

        // acquire render pass.
        auto rpm = submissionImpl.getExecutionContext<RenderPassManagerVulkan>();
        if (!rpm) GN_UNLIKELY {
                GN_ERROR(sLogger)("ClearRenderTargetVulkan::execute: RenderPassManagerVulkan not found");
                return FAILED;
            }
        auto rp = rpm->execute(taskInfo, *renderTarget, rt, cb.commandBuffer->handle());
        if (!rp) GN_UNLIKELY {
                GN_ERROR(sLogger)("ClearRenderTargetVulkan::execute: failed to acquire render pass");
                return FAILED;
            }

        // TODO: do more graphics commands here.

        // end render pass, if this is the last task of the render pass.
        if (rp->lastTaskIndex == taskInfo.index) cb.commandBuffer->handle().endRendering();

        // submit command buffer, if asked to do so.
        if (cb.submit) cb.queue->submit(rapid_vulkan::CommandQueue::SubmitParameters {.commandBuffers = {*cb.commandBuffer}});

        // done
        return hasWarning ? WARNING : PASSED;
    }
};

AutoRef<ClearRenderTarget> createVulkanClearRenderTarget(ArtifactDatabase & db, const StrA & name, const ClearRenderTarget::CreateParameters & params) {
    auto gpu = params.gpu.castTo<GpuContextVulkan>();
    if (!gpu) GN_UNLIKELY {
            GN_ERROR(sLogger)("createVulkanClearRenderTarget: gpu is empty, name='{}'", name);
            return {};
        }
    return AutoRef<ClearRenderTarget>(new ClearRenderTargetVulkan(db, name, gpu));
}

} // namespace GN::rdg