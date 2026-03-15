#include "pch.h"
#include "vk-draw-actions.h"
#include "vk-submission-context.h"
#include "vk-backbuffer.h"
#include "vk-texture.h"
#include "vk-transient-buffer.h"
#include "vk-pso-factory.h"
#include "vk-gpu-resource-group.h"

namespace GN::rdg {

static GN::Logger * sLogger = getLogger("GN.rdg");

// =====================================================================================================================
// ClearRenderTargetVulkan
// =====================================================================================================================

class ClearRenderTargetVulkan : public ClearRenderTarget {
    AutoRef<GpuContextVulkan> mGpu;

public:
    ClearRenderTargetVulkan(ArtifactDatabase & db, const StrA & name, AutoRef<GpuContextVulkan> gpu): ClearRenderTarget(db, TYPE_INFO(), name), mGpu(gpu) {}

    ExecutionResult prepare(TaskInfo & taskInfo, Arguments & arguments) override {
        auto & submission = taskInfo.submission;

        auto a = runtimeCast<ClearRenderTarget::A>(arguments);
        GN_RDG_FAIL_ON_FALSE(a, "{} - arguments is not ClearRenderTarget::A", taskInfo);

        // standard preparation.
        auto & submissionContext = submission.ensureSubmissionContext<SubmissionContextVulkan>(mGpu);
        GN_RDG_FAIL_ON_FAIL(submissionContext.commandBufferManager.prepare(taskInfo, CommandBufferManagerVulkan::GRAPHICS));
        GN_RDG_FAIL_ON_FAIL(submissionContext.renderPassManager.prepareDraw(taskInfo, a->renderTarget));

        // done
        return Action::PASSED;
    }

    ExecutionResult execute(TaskInfo & taskInfo, Arguments & arguments) override {
        auto & submission = taskInfo.submission;

        auto a = runtimeCast<ClearRenderTarget::A>(arguments);
        GN_RDG_FAIL_ON_FALSE(a, "{} - arguments is not ClearRenderTarget::A", taskInfo);

        // standard execution
        auto & sc = submission.ensureSubmissionContext<SubmissionContextVulkan>(mGpu);
        auto   cb = sc.commandBufferManager.execute(taskInfo);
        GN_RDG_FAIL_ON_FAIL(cb);
        auto rp = sc.renderPassManager.execute(taskInfo, cb.commandBuffer().handle());
        GN_RDG_FAIL_ON_FAIL(rp);

        // done
        return PASSED;
    }
};

AutoRef<ClearRenderTarget> createVulkanClearRenderTarget(ArtifactDatabase & db, const StrA & name, const ClearRenderTarget::CreateParameters & params) {
    auto gpu = params.gpu.staticCastTo<GpuContextVulkan>();
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
        : GpuDraw(db, TYPE_INFO(), name), mGpu(gpu), mCreateParams(params) {}

    ExecutionResult prepare(TaskInfo & taskInfo, Arguments & arguments) override {
        auto & submission = taskInfo.submission;

        auto a = runtimeCast<GpuDraw::A>(arguments);
        GN_RDG_FAIL_ON_FALSE(a, "{} - arguments is not GpuDraw::A", taskInfo);

        // standard preparation.
        auto & submissionContext = submission.ensureSubmissionContext<SubmissionContextVulkan>(mGpu);
        GN_RDG_FAIL_ON_FAIL(submissionContext.commandBufferManager.prepare(taskInfo, CommandBufferManagerVulkan::GRAPHICS));
        GN_RDG_FAIL_ON_FAIL(submissionContext.renderPassManager.prepareDraw(taskInfo, a->renderTarget));

        return PASSED;
    }

    ExecutionResult execute(TaskInfo & taskInfo, Arguments & arguments) override {
        auto & submission = taskInfo.submission;

        auto a = runtimeCast<GpuDraw::A>(arguments);
        GN_RDG_FAIL_ON_FALSE(a, "{} - arguments is not GpuDraw::A", taskInfo);

        const auto size = static_cast<uint32_t>(a->immediates.size());
        if (size > 128) GN_UNLIKELY {
                GN_ERROR(sLogger)("{} - inline constants size is too large (max = 128 bytes), size={}", taskInfo, size);
                return FAILED;
            }

        auto & sc = submission.ensureSubmissionContext<SubmissionContextVulkan>(mGpu);
        auto   cb = sc.commandBufferManager.execute(taskInfo);
        GN_RDG_FAIL_ON_FAIL(cb);
        auto rp = sc.renderPassManager.execute(taskInfo, cb.commandBuffer().handle());
        GN_RDG_FAIL_ON_FAIL(rp);

        const GpuGeometry & geom = a->geometry;
        if (0 == geom.vertexCount && 0 == geom.indexCount) GN_UNLIKELY {
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
            .pushConstantSize = a->immediates.empty() ? 0u : static_cast<uint32_t>(a->immediates.size()),
        };

        auto pipeline = mGpu->psoFactory().getOrCreateGraphicsPso(createParams);
        if (!pipeline) GN_UNLIKELY {
                GN_ERROR(sLogger)("{} - PSO factory returned null pipeline", taskInfo);
                return FAILED;
            }

        rapid_vulkan::Drawable::ConstructParameters dcp;
        dcp.setPipeline(pipeline);
        rapid_vulkan::Drawable drawable(dcp);

        if (!a->immediates.empty()) drawable.c(0, a->immediates.size(), a->immediates.data(), vk::ShaderStageFlagBits::eVertex);

        // Bind per-set descriptor groups before rendering.
        if (!a->descriptorGroups.empty()) {
            vk::CommandBuffer  cmd    = cb.commandBuffer().handle();
            vk::PipelineLayout layout = pipeline->layout();
            for (uint32_t setIdx = 0; setIdx < (uint32_t) a->descriptorGroups.size(); ++setIdx) {
                const auto & grpRef = a->descriptorGroups[setIdx];
                if (!grpRef) continue;
                auto *            vkGrp = static_cast<GpuResourceGroupVulkan *>(grpRef.get());
                vk::DescriptorSet ds    = vkGrp->vkSet();
                if (ds) cmd.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, layout, setIdx, {ds}, {});
            }
        }

        if (!geom.vertices.empty() && geom.vertices[0].buffer) {
            auto ref = BufferUtils::toRapid(geom.vertices[0].buffer);
            if (ref) GN_LIKELY {
                    const rapid_vulkan::BufferView view {ref.get(), geom.vertices[0].offset, vk::DeviceSize(-1)};
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

        cb.commandBuffer().render(drawPack);

        // done
        return PASSED;
    }
};

AutoRef<GpuDraw> createVulkanGpuDraw(ArtifactDatabase & db, const StrA & name, const GpuDraw::CreateParameters & params) {
    auto gpu = params.context.staticCastTo<GpuContextVulkan>();
    if (!gpu) GN_UNLIKELY {
            GN_ERROR(sLogger)("createVulkanGpuDraw: gpu is empty, name='{}'", name);
            return {};
        }
    return AutoRef<GpuDraw>(new GpuDrawVulkan(db, name, gpu, params));
}

} // namespace GN::rdg