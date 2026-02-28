#include "pch.h"
#include "vk-draw-actions.h"
#include "vk-submission-context.h"
#include "vk-backbuffer.h"
#include "vk-texture.h"

namespace GN::rdg {

static GN::Logger * sLogger = getLogger("GN.rdg");

class ClearRenderTargetVulkan : public ClearRenderTarget {
    AutoRef<GpuContextVulkan> mGpu;

public:
    ClearRenderTargetVulkan(ArtifactDatabase & db, const StrA & name, AutoRef<GpuContextVulkan> gpu)
        : ClearRenderTarget(db, TYPE_ID, TYPE_NAME, name), mGpu(gpu) {}

    std::pair<ExecutionResult, ExecutionContext *> prepare(TaskInfo & taskInfo, Arguments & arguments) override {
        auto & submissionImpl = static_cast<SubmissionImpl &>(taskInfo.submission);

        auto a = arguments.castTo<ClearRenderTarget::A>();
        if (!a) GN_UNLIKELY {
                GN_ERROR(sLogger)("ClearRenderTargetVulkan::prepare: arguments is not ClearRenderTarget::A");
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

        // collect render target usage.
        if (!submissionContext.renderPassManager.collectRenderTargetUsage(taskInfo, a->renderTarget.value)) GN_UNLIKELY {
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

        // acquire render pass.
        RenderPassManagerVulkan::RenderPassArguments rpa {
            .renderTarget  = a->renderTarget,
            .commandBuffer = cb.commandBuffer.handle(),
        };
        auto rp = sc.renderPassManager.execute(taskInfo, rpa);
        if (!rp) GN_UNLIKELY {
                GN_ERROR(sLogger)("ClearRenderTargetVulkan::execute: failed to acquire render pass");
                return FAILED;
            }

        // end render pass, if this is the last task of the render pass.
        if (rp->lastTaskIndex == taskInfo.index && rp->renderTarget) {
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

class GpuDrawVulkan : public GpuDraw {
    AutoRef<GpuContextVulkan> mGpu;
    GpuDraw::CreateParameters mCreateParams {};
    vk::ShaderModule          mVertModule {};
    vk::ShaderModule          mFragModule {};
    vk::PipelineLayout        mPipelineLayout {};
    vk::Pipeline              mPipeline {}; // null until pipeline creation (Phase 6 / Task 6.3)

    static vk::ShaderModule createShaderModule(vk::Device device, const void * code, size_t codeSize) {
        if (!code || codeSize == 0 || (codeSize % 4) != 0) return {};
        vk::ShaderModuleCreateInfo ci {};
        ci.codeSize = codeSize;
        ci.pCode    = reinterpret_cast<const uint32_t *>(code);
        return device.createShaderModule(ci);
    }

    void createPipelineIfNeeded() {
        if (mPipeline || !mVertModule || !mFragModule) return;
        const auto dev = mGpu->device().handle();

        // Pipeline layout: no descriptor sets, no push constants (Task 6.2)
        vk::PipelineLayoutCreateInfo layoutCi {};
        mPipelineLayout = dev.createPipelineLayout(layoutCi);
        if (!mPipelineLayout) GN_UNLIKELY {
                GN_WARN(sLogger)("GpuDrawVulkan: createPipelineLayout failed, name='{}'", this->name.c_str());
                return;
            }

        const char * vertEntry = mCreateParams.vs.entry ? mCreateParams.vs.entry : "main";
        const char * fragEntry = mCreateParams.ps.entry ? mCreateParams.ps.entry : "main";

        vk::PipelineShaderStageCreateInfo stages[2] = {
            {vk::PipelineShaderStageCreateFlags(), vk::ShaderStageFlagBits::eVertex, mVertModule, vertEntry},
            {vk::PipelineShaderStageCreateFlags(), vk::ShaderStageFlagBits::eFragment, mFragModule, fragEntry},
        };

        vk::PipelineVertexInputStateCreateInfo   vertexInput {};
        vk::PipelineInputAssemblyStateCreateInfo inputAssembly {{}, vk::PrimitiveTopology::eTriangleList};
        vk::PipelineRasterizationStateCreateInfo rasterization {};
        rasterization.lineWidth = 1.0f;
        vk::PipelineColorBlendAttachmentState blendAttachment {};
        blendAttachment.colorWriteMask =
            vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
        vk::PipelineColorBlendStateCreateInfo  colorBlend {{}, false, {}, blendAttachment};
        vk::PipelineMultisampleStateCreateInfo multisample {};
        multisample.rasterizationSamples = vk::SampleCountFlagBits::e1;

        vk::PipelineViewportStateCreateInfo viewportState {};
        viewportState.viewportCount = 1;
        viewportState.scissorCount  = 1;

        vk::DynamicState                   dynamicStates[] = {vk::DynamicState::eViewport, vk::DynamicState::eScissor};
        vk::PipelineDynamicStateCreateInfo dynamicState {{}, 2, dynamicStates};

        vk::PipelineRenderingCreateInfo renderingCi {};
        vk::Format                      colorFormat = vk::Format::eB8G8R8A8Unorm; // common swapchain format
        renderingCi.setColorAttachmentFormats(colorFormat);

        vk::GraphicsPipelineCreateInfo pipeCi {};
        pipeCi.setPNext(&renderingCi);
        pipeCi.setStages(stages);
        pipeCi.setPVertexInputState(&vertexInput);
        pipeCi.setPInputAssemblyState(&inputAssembly);
        pipeCi.setPRasterizationState(&rasterization);
        pipeCi.setPViewportState(&viewportState);
        pipeCi.setPDynamicState(&dynamicState);
        pipeCi.setPColorBlendState(&colorBlend);
        pipeCi.setPMultisampleState(&multisample);
        pipeCi.setLayout(mPipelineLayout);
        pipeCi.setRenderPass(nullptr); // dynamic rendering

        auto result = dev.createGraphicsPipeline(nullptr, pipeCi);
        if (result.result != vk::Result::eSuccess) GN_UNLIKELY {
                GN_ERROR(sLogger)("GpuDrawVulkan: createGraphicsPipeline failed, name='{}'", this->name.c_str());
                dev.destroyPipelineLayout(mPipelineLayout);
                mPipelineLayout = nullptr;
                return;
            }
        mPipeline = result.value;
    }

public:
    GpuDrawVulkan(ArtifactDatabase & db, const StrA & name, AutoRef<GpuContextVulkan> gpu, const GpuDraw::CreateParameters & params)
        : GpuDraw(db, TYPE_ID, TYPE_NAME, name), mGpu(gpu), mCreateParams(params) {
        const auto dev = mGpu->device().handle();
        if (params.vs.binary) mVertModule = createShaderModule(dev, params.vs.binary, params.vs.size);
        if (params.ps.binary) mFragModule = createShaderModule(dev, params.ps.binary, params.ps.size);
        bool vsFail = (params.vs.binary && !mVertModule);
        bool psFail = (params.ps.binary && !mFragModule);
        if (vsFail || psFail) { GN_WARN(sLogger)("GpuDrawVulkan: failed to create one or more shader modules, name='{}'", name.c_str()); }
    }

    ~GpuDrawVulkan() override {
        const auto dev = mGpu->device().handle();
        if (mPipeline) dev.destroyPipeline(mPipeline);
        if (mPipelineLayout) dev.destroyPipelineLayout(mPipelineLayout);
        if (mVertModule) dev.destroyShaderModule(mVertModule);
        if (mFragModule) dev.destroyShaderModule(mFragModule);
    }

    std::pair<ExecutionResult, ExecutionContext *> prepare(TaskInfo & taskInfo, Arguments & arguments) override {
        auto & submissionImpl = static_cast<SubmissionImpl &>(taskInfo.submission);

        auto a = arguments.castTo<GpuDraw::A>();
        if (!a) GN_UNLIKELY {
                GN_ERROR(sLogger)("GpuDrawVulkan::prepare: arguments is not GpuDraw::A");
                return std::make_pair(FAILED, nullptr);
            }

        auto taskContext = std::make_unique<DrawActionContextVulkan>();

        // prepare command buffer.
        auto & submissionContext     = SubmissionContextVulkan::get(submissionImpl, mGpu);
        taskContext->commandBufferId = submissionContext.commandBufferManager.prepare(CommandBufferManagerVulkan::GRAPHICS);
        if (!taskContext->commandBufferId) GN_UNLIKELY {
                GN_ERROR(sLogger)("GpuDrawVulkan::prepare: failed to prepare command buffer");
                return std::make_pair(FAILED, nullptr);
            }

        // No need to call render pass manager here, since we won't change render target here.

        return std::make_pair(PASSED, taskContext.release());
    }

    ExecutionResult execute(TaskInfo & taskInfo, Arguments & arguments, ExecutionContext * context) override {
        bool hasWarning = false;

        auto & submissionImpl = static_cast<SubmissionImpl &>(taskInfo.submission);

        auto a = arguments.castTo<GpuDraw::A>();
        if (!a) GN_UNLIKELY {
                GN_ERROR(sLogger)("GpuDrawVulkan::execute: arguments is not GpuDraw::A");
                return FAILED;
            }

        auto ctx = static_cast<DrawActionContextVulkan *>(context);
        if (!ctx) GN_UNLIKELY {
                GN_ERROR(sLogger)("GpuDrawVulkan::execute: context is not DrawActionContextVulkan");
                return FAILED;
            }

        // acquire command buffer.
        auto & sc = SubmissionContextVulkan::get(submissionImpl, mGpu);
        auto   cb = sc.commandBufferManager.execute(ctx->commandBufferId);
        if (!cb.queue || !cb.commandBuffer) GN_UNLIKELY {
                GN_ERROR(sLogger)("GpuDrawVulkan::execute: failed to acquire command buffer");
                return FAILED;
            }

        createPipelineIfNeeded();

        // Task 7.1: if vs/ps were provided but pipeline is missing (creation failed), fail the task.
        const bool requiredShaders = mCreateParams.vs.binary && mCreateParams.ps.binary;
        if (requiredShaders && !mPipeline) GN_UNLIKELY {
                GN_ERROR(sLogger)("GpuDrawVulkan::execute: vs/ps provided but pipeline not created, name='{}'", this->name.c_str());
                return FAILED;
            }

        // When pipeline is valid: set viewport/scissor from render target extent, then bind and draw (Task 3.3 / 6.4).
        if (mPipeline) {
            cb.commandBuffer.handle().bindPipeline(vk::PipelineBindPoint::eGraphics, mPipeline);
            // Mesh is optional; when no vertex buffer, use default 3 vertices (e.g. fullscreen triangle).
            uint32_t vertexCount   = 3;
            uint32_t instanceCount = 1;
            uint32_t firstVertex   = 0;
            uint32_t firstInstance = 0;
            if (!a->geometry.value.vertices.empty() && a->geometry.value.vertices[0].stride > 0) {
                vertexCount = (uint32_t) a->geometry.value.vertices[0].count();
            }
            if (vertexCount > 0) cb.commandBuffer.handle().draw(vertexCount, instanceCount, firstVertex, firstInstance);
        }

        // call into render pass to end render pass if needed. Must be called after all draw commands are queued in the
        // command buffer.
        auto rp = sc.renderPassManager.execute(taskInfo, cb.commandBuffer.handle());
        if (!rp) GN_UNLIKELY {
                GN_ERROR(sLogger)("GpuDrawVulkan::execute: failed to end render pass");
                return FAILED;
            }
        GN_ASSERT(rp->firstTaskIndex != taskInfo.index); // should never be the beginning of a render pass.

        // submit command buffer, if asked to do so.
        if (cb.submit) cb.queue->submit(rapid_vulkan::CommandQueue::SubmitParameters {.commandBuffers = {cb.commandBuffer}});

        return hasWarning ? WARNING : PASSED;
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