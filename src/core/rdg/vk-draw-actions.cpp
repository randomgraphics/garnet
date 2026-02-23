#include "pch.h"
#include "vk-draw-actions.h"
#include "vk-submission-context.h"
#include "vk-backbuffer.h"
#include "vk-texture.h"

namespace GN::rdg {

static GN::Logger * sLogger = getLogger("GN.rdg");

static void trackRenderTargetState(const RenderTarget * renderTarget) {
    if (!renderTarget) GN_UNLIKELY return;

    // track the state of the color targets.
    for (size_t i = 0; i < renderTarget->colors.size(); i++) {
        const auto & color = renderTarget->colors[i];
        if (0 == color.target.index()) {
            // this color target is a texture.
            auto tex = std::get<0>(color.target).castTo<TextureVulkan>().get();
            if (tex)
                tex->trackImageState(color.subresourceIndex.mip, 1, color.subresourceIndex.face, 1,
                                     {vk::ImageLayout::eColorAttachmentOptimal,
                                      vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite,
                                      vk::PipelineStageFlagBits::eColorAttachmentOutput});
        } else {
            // this color target is a backbuffer.
            auto bb = std::get<1>(color.target).castTo<BackbufferVulkan>().get();
            if (bb)
                bb->trackImageState({vk::ImageLayout::eColorAttachmentOptimal,
                                     vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite,
                                     vk::PipelineStageFlagBits::eColorAttachmentOutput});
        }
    }

    // track the state of the depth stencil target.
    auto depth = renderTarget->depthStencil.target.castTo<TextureVulkan>().get();
    if (depth)
        depth->trackImageState(renderTarget->depthStencil.subresourceIndex.mip, 1, renderTarget->depthStencil.subresourceIndex.face, 1,
                               {vk::ImageLayout::eDepthStencilAttachmentOptimal,
                                vk::AccessFlagBits::eDepthStencilAttachmentRead | vk::AccessFlagBits::eDepthStencilAttachmentWrite,
                                vk::PipelineStageFlagBits::eEarlyFragmentTests | vk::PipelineStageFlagBits::eLateFragmentTests});
}

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

        auto taskContext = std::make_unique<DrawActionContextVulkan>();

        // prepare command buffer.
        auto & submissionContext     = SubmissionContextVulkan::get(submissionImpl, mGpu);
        taskContext->commandBufferId = submissionContext.commandBufferManager.prepare(CommandBufferManagerVulkan::GRAPHICS);
        if (!taskContext->commandBufferId) GN_UNLIKELY {
                GN_ERROR(sLogger)("ClearRenderTargetVulkan::prepare: failed to prepare command buffer");
                return std::make_pair(FAILED, nullptr);
            }

        // prepare render pass
        if (!submissionContext.renderPassManager.prepare(taskInfo, *a->renderTarget.get())) GN_UNLIKELY {
                GN_ERROR(sLogger)("ClearRenderTargetVulkan::prepare: failed to prepare render pass");
                return std::make_pair(FAILED, nullptr);
            }

        // done
        return std::make_pair(PASSED, taskContext.release());
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
        auto & sc = SubmissionContextVulkan::get(submissionImpl, mGpu);
        auto   cb = sc.commandBufferManager.execute(ctx->commandBufferId);
        if (!cb.queue || !cb.commandBuffer) GN_UNLIKELY {
                GN_ERROR(sLogger)("ClearRenderTargetVulkan::execute: failed to acquire command buffer");
                return FAILED;
            }

        // execute resource tracker to update GPU resource layout and memory usage.
        trackRenderTargetState(a->renderTarget.get());

        // acquire render pass.
        RenderPassManagerVulkan::RenderPassArguments rpa {
            .renderTarget  = *renderTarget,
            .commandBuffer = cb.commandBuffer.handle(),
            .clearValues   = a->clearValues.getAsOptional(),
        };
        auto rp = sc.renderPassManager.execute(taskInfo, rpa);
        if (!rp) GN_UNLIKELY {
                GN_ERROR(sLogger)("ClearRenderTargetVulkan::execute: failed to acquire render pass");
                return FAILED;
            }

        // TODO: do more graphics commands here.

        // end render pass, if this is the last task of the render pass.
        if (rp->lastTaskIndex == taskInfo.index) {
            // Not like old render pass, dynamic rendering does not implicitly updates the render target's layout.
            // So here we don't have to call resource tracker to update the layout either.
            cb.commandBuffer.handle().endRendering();
        }

        // submit command buffer, if asked to do so.
        // TODO: we need to remember the submission somewhere.
        if (cb.submit) cb.queue->submit(rapid_vulkan::CommandQueue::SubmitParameters {.commandBuffers = {cb.commandBuffer}});

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

class GenericDrawVulkan : public GenericDraw {
    AutoRef<GpuContextVulkan>       mGpu;
    GenericDraw::CreateParameters  mCreateParams {};
    vk::Pipeline                   mPipeline {}; // null until pipeline creation (Phase 6 / Task 6.3)

public:
    GenericDrawVulkan(ArtifactDatabase & db, const StrA & name, AutoRef<GpuContextVulkan> gpu, const GenericDraw::CreateParameters & params)
        : GenericDraw(db, TYPE, name), mGpu(gpu), mCreateParams(params) {}

    std::pair<ExecutionResult, ExecutionContext *> prepare(TaskInfo & taskInfo, Arguments & arguments) override {
        auto & submissionImpl = static_cast<SubmissionImpl &>(taskInfo.submission);

        auto a = arguments.castTo<GenericDraw::A>();
        if (!a) GN_UNLIKELY {
                GN_ERROR(sLogger)("GenericDrawVulkan::prepare: arguments is not GenericDraw::A");
                return std::make_pair(FAILED, nullptr);
            }
        auto renderTarget = a->renderTarget.get();
        if (!renderTarget) GN_UNLIKELY {
                GN_ERROR(sLogger)("GenericDrawVulkan::prepare: render target is null");
                return std::make_pair(FAILED, nullptr);
            }

        auto taskContext = std::make_unique<DrawActionContextVulkan>();

        // prepare command buffer.
        auto & submissionContext     = SubmissionContextVulkan::get(submissionImpl, mGpu);
        taskContext->commandBufferId = submissionContext.commandBufferManager.prepare(CommandBufferManagerVulkan::GRAPHICS);
        if (!taskContext->commandBufferId) GN_UNLIKELY {
                GN_ERROR(sLogger)("GenericDrawVulkan::prepare: failed to prepare command buffer");
                return std::make_pair(FAILED, nullptr);
            }

        // prepare render pass
        if (!submissionContext.renderPassManager.prepare(taskInfo, *a->renderTarget.get())) GN_UNLIKELY {
                GN_ERROR(sLogger)("GenericDrawVulkan::prepare: failed to prepare render pass");
                return std::make_pair(FAILED, nullptr);
            }

        return std::make_pair(PASSED, taskContext.release());
    }

    ExecutionResult execute(TaskInfo & taskInfo, Arguments & arguments, ExecutionContext * context) override {
        bool hasWarning = false;

        auto & submissionImpl = static_cast<SubmissionImpl &>(taskInfo.submission);

        auto a = arguments.castTo<GenericDraw::A>();
        if (!a) GN_UNLIKELY {
                GN_ERROR(sLogger)("GenericDrawVulkan::execute: arguments is not GenericDraw::A");
                return FAILED;
            }
        auto renderTarget = a->renderTarget.get();
        if (!renderTarget) GN_UNLIKELY {
                GN_ERROR(sLogger)("GenericDrawVulkan::execute: render target is null");
                return FAILED;
            }

        auto ctx = static_cast<DrawActionContextVulkan *>(context);
        if (!ctx) GN_UNLIKELY {
                GN_ERROR(sLogger)("GenericDrawVulkan::execute: context is not DrawActionContextVulkan");
                return FAILED;
            }

        // acquire command buffer.
        auto & sc = SubmissionContextVulkan::get(submissionImpl, mGpu);
        auto   cb = sc.commandBufferManager.execute(ctx->commandBufferId);
        if (!cb.queue || !cb.commandBuffer) GN_UNLIKELY {
                GN_ERROR(sLogger)("GenericDrawVulkan::execute: failed to acquire command buffer");
                return FAILED;
            }

        // execute resource tracker to update GPU resource layout and memory usage.
        trackRenderTargetState(a->renderTarget.get());

        // acquire render pass (no clear values for generic draw).
        RenderPassManagerVulkan::RenderPassArguments rpa {
            .renderTarget  = *renderTarget,
            .commandBuffer = cb.commandBuffer.handle(),
            .clearValues   = std::nullopt,
        };
        auto rp = sc.renderPassManager.execute(taskInfo, rpa);
        if (!rp) GN_UNLIKELY {
                GN_ERROR(sLogger)("GenericDrawVulkan::execute: failed to acquire render pass");
                return FAILED;
            }

        // When pipeline is valid: bind pipeline and issue draw (Task 3.3).
        if (mPipeline) {
            cb.commandBuffer.handle().bindPipeline(vk::PipelineBindPoint::eGraphics, mPipeline);
            const auto * dp = a->drawParams.get();
            const uint32_t vertexCount   = dp ? dp->vertexCount : 0;
            const uint32_t instanceCount = dp ? dp->instanceCount : 1;
            const uint32_t firstVertex   = dp ? dp->firstVertex : 0;
            const uint32_t firstInstance = dp ? dp->firstInstance : 0;
            if (vertexCount > 0)
                cb.commandBuffer.handle().draw(vertexCount, instanceCount, firstVertex, firstInstance);
        }

        // end render pass, if this is the last task of the render pass.
        if (rp->lastTaskIndex == taskInfo.index) { cb.commandBuffer.handle().endRendering(); }

        // submit command buffer, if asked to do so.
        if (cb.submit) cb.queue->submit(rapid_vulkan::CommandQueue::SubmitParameters {.commandBuffers = {cb.commandBuffer}});

        return hasWarning ? WARNING : PASSED;
    }
};

AutoRef<GenericDraw> createVulkanGenericDrawAction(ArtifactDatabase & db, const StrA & name, const GenericDraw::CreateParameters & params) {
    auto gpu = params.context.castTo<GpuContextVulkan>();
    if (!gpu) GN_UNLIKELY {
            GN_ERROR(sLogger)("createVulkanGenericDrawAction: gpu is empty, name='{}'", name);
            return {};
        }
    return AutoRef<GenericDraw>(new GenericDrawVulkan(db, name, gpu, params));
}

} // namespace GN::rdg