#include "pch.h"
#include "vk-draw-actions.h"
#include "vk-submission-context.h"
#include "vk-backbuffer.h"
#include "vk-texture.h"
#include "vk-buffer.h"
#include "vk-pso-factory.h"

namespace GN::rdg {

static GN::Logger * sLogger = getLogger("GN.rdg");

// =====================================================================================================================
// ClearRenderTargetVulkan
// =====================================================================================================================

class ClearRenderTargetVulkan : public ClearRenderTarget {
    AutoRef<GpuContextVulkan> mGpu;

public:
    ClearRenderTargetVulkan(ArtifactDatabase & db, const StrA & name, AutoRef<GpuContextVulkan> gpu)
        : ClearRenderTarget(db, TYPE_ID, TYPE_NAME, name), mGpu(gpu) {}

    ExecutionResult prepare(TaskInfo & taskInfo, Arguments & arguments) override {
        auto & submission = taskInfo.submission;

        auto a = arguments.castTo<ClearRenderTarget::A>();
        GN_RDG_FAIL_ON_FALSE(a, "{} - arguments is not ClearRenderTarget::A", taskInfo);

        // standard preparation.
        auto & submissionContext = submission.ensureSubmissionContext<SubmissionContextVulkan>(mGpu);
        GN_RDG_FAIL_ON_FAIL(submissionContext.commandBufferManager.prepare(taskInfo, CommandBufferManagerVulkan::GRAPHICS));
        GN_RDG_FAIL_ON_FALSE(submissionContext.renderPassManager.prepareDraw(taskInfo, a->renderTarget.value));

        // done
        return Action::PASSED;
    }

    ExecutionResult execute(TaskInfo & taskInfo, Arguments & arguments) override {
        bool hasWarning = false;

        auto & submission = taskInfo.submission;

        auto a = arguments.castTo<ClearRenderTarget::A>();
        GN_RDG_FAIL_ON_FALSE(a, "{} - arguments is not ClearRenderTarget::A", taskInfo);

        // stadnard execution
        auto & sc = submission.ensureSubmissionContext<SubmissionContextVulkan>(mGpu);
        auto   cb = sc.commandBufferManager.execute(taskInfo);
        GN_RDG_FAIL_ON_FALSE(cb.queue && cb.commandBuffer);

        // acquire render pass. End if if needed.
        auto rp = sc.renderPassManager.execute(taskInfo, cb.commandBuffer.handle());
        GN_RDG_FAIL_ON_FAIL(rp.result);
        if (rp.needToEnd) cb.commandBuffer.handle().endRendering();

        // submit command buffer, if asked to do so.
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

// =====================================================================================================================
// GpuDrawVulkan
// =====================================================================================================================

class GpuDrawVulkan : public GpuDraw {
    AutoRef<GpuContextVulkan> mGpu;
    GpuDraw::CreateParameters mCreateParams {};

public:
    GpuDrawVulkan(ArtifactDatabase & db, const StrA & name, AutoRef<GpuContextVulkan> gpu, const GpuDraw::CreateParameters & params)
        : GpuDraw(db, TYPE_ID, TYPE_NAME, name), mGpu(gpu), mCreateParams(params) {}

    ExecutionResult prepare(TaskInfo & taskInfo, Arguments & arguments) override {
        auto & submissionImpl = static_cast<SubmissionImpl &>(taskInfo.submission);

        auto a = arguments.castTo<GpuDraw::A>();
        GN_RDG_FAIL_ON_FALSE(a, "{} - arguments is not GpuDraw::A", taskInfo);

        // standard preparation.
        auto & submissionContext = submissionImpl.ensureSubmissionContext<SubmissionContextVulkan>(mGpu);
        GN_RDG_FAIL_ON_FAIL(submissionContext.commandBufferManager.prepare(taskInfo, CommandBufferManagerVulkan::GRAPHICS));
        GN_RDG_FAIL_ON_FALSE(submissionContext.renderPassManager.prepareDraw(taskInfo, {}));

        return PASSED;
    }

    ExecutionResult execute(TaskInfo & taskInfo, Arguments & arguments) override {
        auto & submission = taskInfo.submission;

        auto a = arguments.castTo<GpuDraw::A>();
        GN_RDG_FAIL_ON_FALSE(a, "{} - arguments is not GpuDraw::A", taskInfo);

        const auto size = static_cast<uint32_t>(a->constants.size());
        if (size > 128) GN_UNLIKELY {
                GN_ERROR(sLogger)("{} - inline constants size is too large (max = 128 bytes), size={}", taskInfo, size);
                return FAILED;
            }

        auto & sc = submission.ensureSubmissionContext<SubmissionContextVulkan>(mGpu);
        auto   cb = sc.commandBufferManager.execute(taskInfo);
        GN_RDG_FAIL_ON_FALSE(cb.queue && cb.commandBuffer);
        auto rp = sc.renderPassManager.execute(taskInfo, cb.commandBuffer.handle());
        GN_RDG_FAIL_ON_FAIL(rp.result);
        auto onExit = AutoFinalizer([&]() {
            if (rp.needToEnd) {
                GN_VERBOSE(sLogger)("{} - ending render pass", taskInfo);
                cb.commandBuffer.handle().endRendering();
            }
            if (cb.submit) cb.queue->submit(rapid_vulkan::CommandQueue::SubmitParameters {.commandBuffers = {cb.commandBuffer}});
        });

        const GpuGeometry & geom = a->geometry;
        if (0 == geom.vertexCount && 0 == geom.indices.indexCount) GN_UNLIKELY {
                GN_VERBOSE(sLogger)("{} - vertex and index count are zero. nothing to draw", taskInfo);
                return PASSED;
            }

        if (!mCreateParams.vs.binary || mCreateParams.vs.size == 0) GN_UNLIKELY {
                GN_ERROR(sLogger)("{} - vertex shader is missing for GpuDraw action", taskInfo);
                return FAILED;
            }

        const RenderTarget * currentRt = sc.renderPassManager.getCurrentDrawTarget(taskInfo.index);
        if (!currentRt) GN_UNLIKELY {
                GN_ERROR(sLogger)("{} - current render target is not set for GpuDraw action", taskInfo);
                return FAILED;
            }

        GraphicsPsoCreateParams createParams = {
            .vs               = mCreateParams.vs,
            .ps               = mCreateParams.ps,
            .renderTarget     = *currentRt,
            .geometry         = geom,
            .pushConstantSize = 128,
        };

        auto pipeline = mGpu->psoFactory().getOrCreateGraphicsPso(createParams);
        if (!pipeline) GN_UNLIKELY {
                GN_ERROR(sLogger)("{} - PSO factory returned null pipeline", taskInfo);
                return FAILED;
            }

        rapid_vulkan::Drawable::ConstructParameters dcp;
        dcp.setPipeline(pipeline);
        rapid_vulkan::Drawable drawable(dcp);

        if (!a->constants.empty()) drawable.c(0, a->constants.size(), a->constants.data(), vk::ShaderStageFlagBits::eVertex);

        if (!geom.vertices.empty() && geom.vertices[0].buffer) {
            auto *                                  bv  = static_cast<BufferVulkan *>(geom.vertices[0].buffer.get());
            rapid_vulkan::Ref<rapid_vulkan::Buffer> ref = bv->refVkBuffer();
            if (ref) {
                const rapid_vulkan::BufferView view {ref, geom.vertices[0].offset, vk::DeviceSize(-1)};
                drawable.v(vk::ArrayProxy<const rapid_vulkan::BufferView>(1, &view));
            }
        }

        uint32_t                                       vertexCount = geom.vertexCount;
        rapid_vulkan::GraphicsPipeline::DrawParameters drawParams {};
        drawParams.setNonIndexed(vertexCount, 0).setInstance(1, 0);
        drawable.draw(drawParams);

        rapid_vulkan::Ref<const rapid_vulkan::DrawPack> drawPack = drawable.compile();
        if (!drawPack || drawPack->empty()) GN_UNLIKELY {
                GN_ERROR(sLogger)("{} - Drawable compile produced empty DrawPack", taskInfo);
                return FAILED;
            }

        cb.commandBuffer.render(drawPack);

        return PASSED;
    }
};

AutoRef<GpuDraw> createVulkanGpuDraw(ArtifactDatabase & db, const StrA & name, const GpuDraw::CreateParameters & params) {
    auto gpu = params.context.castTo<GpuContextVulkan>();
    if (!gpu) GN_UNLIKELY {
            GN_ERROR(sLogger)("createVulkanGpuDraw: gpu is empty, name='{}'", name);
            return {};
        }
    return AutoRef<GpuDraw>(new GpuDrawVulkan(db, name, gpu, params));
}

} // namespace GN::rdg