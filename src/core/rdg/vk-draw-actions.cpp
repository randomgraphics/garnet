#include "pch.h"
#include "vk-draw-actions.h"
#include "vk-submission-context.h"
#include "vk-backbuffer.h"
#include "vk-texture.h"

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

        // Pipeline layout: one push constant range for vertex (e.g. PBR model + viewProj, 128 bytes).
        constexpr uint32_t           kPushConstantSize = 128;
        vk::PushConstantRange        pushRange {vk::ShaderStageFlagBits::eVertex, 0, kPushConstantSize};
        vk::PipelineLayoutCreateInfo layoutCi {{}, 0, nullptr, 1, &pushRange};
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

    ExecutionResult prepare(TaskInfo & taskInfo, Arguments & arguments) override {
        auto & submissionImpl = static_cast<SubmissionImpl &>(taskInfo.submission);

        auto a = arguments.castTo<GpuDraw::A>();
        GN_RDG_FAIL_ON_FALSE(a, "{} - arguments is not GpuDraw::A", taskInfo);

        // standard preparation.
        auto & submissionContext = submissionImpl.ensureSubmissionContext<SubmissionContextVulkan>(mGpu);
        GN_RDG_FAIL_ON_FAIL(submissionContext.commandBufferManager.prepare(taskInfo, CommandBufferManagerVulkan::GRAPHICS));
        // pass in an empty render target indicating we are not changing the render target.
        GN_RDG_FAIL_ON_FALSE(submissionContext.renderPassManager.prepareDraw(taskInfo, {}));

        // done
        return PASSED;
    }

    ExecutionResult execute(TaskInfo & taskInfo, Arguments & arguments) override {
        bool hasWarning = false;

        auto & submission = taskInfo.submission;

        auto a = arguments.castTo<GpuDraw::A>();
        GN_RDG_FAIL_ON_FALSE(a, "{} - arguments is not GpuDraw::A", taskInfo);

        // standard execution.
        auto & sc = submission.ensureSubmissionContext<SubmissionContextVulkan>(mGpu);
        auto   cb = sc.commandBufferManager.execute(taskInfo);
        GN_RDG_FAIL_ON_FALSE(cb.queue && cb.commandBuffer);

        createPipelineIfNeeded();

        // Task 7.1: if vs/ps were provided but pipeline is missing (creation failed), fail the task.
        const bool requiredShaders = mCreateParams.vs.binary && mCreateParams.ps.binary;
        if (requiredShaders && !mPipeline) GN_UNLIKELY {
                GN_ERROR(sLogger)("{} - vs/ps provided but pipeline not created", taskInfo);
                return FAILED;
            }

        // When pipeline is valid: set viewport/scissor from render target extent, then bind and draw (Task 3.3 / 6.4).
        if (mPipeline) {
            cb.commandBuffer.handle().bindPipeline(vk::PipelineBindPoint::eGraphics, mPipeline);
            if (!a->constants.empty()) {
                const auto size = static_cast<uint32_t>(a->constants.size());
                if (size <= 128)
                    cb.commandBuffer.handle().pushConstants(mPipelineLayout, vk::ShaderStageFlagBits::eVertex, 0, size, a->constants.data());
                else {
                    GN_ERROR(sLogger)("{} - inline constants size is too large, size={}", taskInfo, size);
                    return FAILED;
                }
            }
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
        GN_RDG_FAIL_ON_FAIL(rp.result);
        if (rp.needToEnd) GN_UNLIKELY {
                GN_VERBOSE(sLogger)("{} - ending render pass", taskInfo);
                cb.commandBuffer.handle().endRendering();
            }

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