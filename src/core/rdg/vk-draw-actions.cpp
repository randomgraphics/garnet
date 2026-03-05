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

static vk::Format toVkFormat(GpuGeometry::AttributeFormat f) {
    using F = GpuGeometry::AttributeFormat;
    switch (f) {
    case F::F32_1:
        return vk::Format::eR32Sfloat;
    case F::F32_2:
        return vk::Format::eR32G32Sfloat;
    case F::F32_3:
        return vk::Format::eR32G32B32Sfloat;
    case F::F32_4:
        return vk::Format::eR32G32B32A32Sfloat;
    case F::F16_1:
        return vk::Format::eR16Sfloat;
    case F::F16_2:
        return vk::Format::eR16G16Sfloat;
    case F::F16_3:
        return vk::Format::eR16G16B16Sfloat;
    case F::F16_4:
        return vk::Format::eR16G16B16A16Sfloat;
    case F::U32_1:
        return vk::Format::eR32Uint;
    case F::U32_2:
        return vk::Format::eR32G32Uint;
    case F::U32_3:
        return vk::Format::eR32G32B32Uint;
    case F::U32_4:
        return vk::Format::eR32G32B32A32Uint;
    case F::U16_1:
        return vk::Format::eR16Uint;
    case F::U16_2:
        return vk::Format::eR16G16Uint;
    case F::U16_3:
        return vk::Format::eR16G16B16Uint;
    case F::U16_4:
        return vk::Format::eR16G16B16A16Uint;
    case F::U8_1:
        return vk::Format::eR8Uint;
    case F::U8_2:
        return vk::Format::eR8G8Uint;
    case F::U8_3:
        return vk::Format::eR8G8B8Uint;
    case F::U8_4:
        return vk::Format::eR8G8B8A8Uint;
    case F::I32_1:
        return vk::Format::eR32Sint;
    case F::I32_2:
        return vk::Format::eR32G32Sint;
    case F::I32_3:
        return vk::Format::eR32G32B32Sint;
    case F::I32_4:
        return vk::Format::eR32G32B32A32Sint;
    case F::I16_1:
        return vk::Format::eR16Sint;
    case F::I16_2:
        return vk::Format::eR16G16Sint;
    case F::I16_3:
        return vk::Format::eR16G16B16Sint;
    case F::I16_4:
        return vk::Format::eR16G16B16A16Sint;
    case F::I8_1:
        return vk::Format::eR8Sint;
    case F::I8_2:
        return vk::Format::eR8G8Sint;
    case F::I8_3:
        return vk::Format::eR8G8B8Sint;
    case F::I8_4:
        return vk::Format::eR8G8B8A8Sint;
    default:
        return vk::Format::eR32G32B32Sfloat;
    }
}

class GpuDrawVulkan : public GpuDraw {
    AutoRef<GpuContextVulkan> mGpu;
    GpuDraw::CreateParameters mCreateParams {};
    vk::ShaderModule          mVertModule {};
    vk::ShaderModule          mFragModule {};
    vk::PipelineLayout        mPipelineLayout {};
    vk::Pipeline              mNoGeomPipeline {}; ///< pipeline for shader-generated vertices (no vertex input)
    vk::Pipeline              mPipeline {};
    GpuGeometry::VertexFormat mCachedVertexFormat {};
    uint32_t                  mCachedStride = 0;

    static vk::ShaderModule createShaderModule(vk::Device device, const void * code, size_t codeSize) {
        if (!code || codeSize == 0 || (codeSize % 4) != 0) return {};
        vk::ShaderModuleCreateInfo ci {};
        ci.codeSize = codeSize;
        ci.pCode    = reinterpret_cast<const uint32_t *>(code);
        return device.createShaderModule(ci);
    }

    void ensurePipelineLayout() {
        if (mPipelineLayout) return;
        const auto                   dev               = mGpu->device().handle();
        constexpr uint32_t           kPushConstantSize = 128;
        vk::PushConstantRange        pushRange {vk::ShaderStageFlagBits::eVertex, 0, kPushConstantSize};
        vk::PipelineLayoutCreateInfo layoutCi {{}, 0, nullptr, 1, &pushRange};
        mPipelineLayout = dev.createPipelineLayout(layoutCi);
        if (!mPipelineLayout) GN_UNLIKELY {
                GN_WARN(sLogger)("GpuDrawVulkan: createPipelineLayout failed, name='{}'", this->name.c_str());
            }
    }

    /// Build and cache the no-vertex-input pipeline for shader-generated vertices (e.g. gl_VertexIndex triangles).
    void ensureNoGeomPipeline() {
        if (mNoGeomPipeline) return;
        if (!mVertModule || !mFragModule) return;

        ensurePipelineLayout();
        if (!mPipelineLayout) return;

        const auto dev = mGpu->device().handle();

        const char *                      vertEntry = mCreateParams.vs.entry ? mCreateParams.vs.entry : "main";
        const char *                      fragEntry = mCreateParams.ps.entry ? mCreateParams.ps.entry : "main";
        vk::PipelineShaderStageCreateInfo stages[2] = {
            {vk::PipelineShaderStageCreateFlags(), vk::ShaderStageFlagBits::eVertex, mVertModule, vertEntry},
            {vk::PipelineShaderStageCreateFlags(), vk::ShaderStageFlagBits::eFragment, mFragModule, fragEntry},
        };

        vk::PipelineVertexInputStateCreateInfo   vertexInput {}; // no vertex input
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
        viewportState.viewportCount                        = 1;
        viewportState.scissorCount                         = 1;
        vk::DynamicState                   dynamicStates[] = {vk::DynamicState::eViewport, vk::DynamicState::eScissor};
        vk::PipelineDynamicStateCreateInfo dynamicState {{}, 2, dynamicStates};
        vk::PipelineRenderingCreateInfo    renderingCi {};
        const vk::Format                   colorFormats[] = {vk::Format::eB8G8R8A8Unorm};
        renderingCi.setColorAttachmentFormats(colorFormats);

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
        pipeCi.setRenderPass(nullptr);

        auto result = dev.createGraphicsPipeline(nullptr, pipeCi);
        if (result.result != vk::Result::eSuccess) GN_UNLIKELY {
                GN_ERROR(sLogger)("GpuDrawVulkan: createGraphicsPipeline (no-geom) failed, name='{}'", this->name.c_str());
                return;
            }
        mNoGeomPipeline = result.value;
    }

    /// Build and cache pipeline for the given geometry. Uses geometry.format and first vertex buffer stride.
    void ensurePipelineForGeometry(const GpuGeometry & geom) {
        if (!mVertModule || !mFragModule) return;
        if (geom.format.empty() || geom.vertices.empty() || geom.vertices[0].stride == 0) return;
        const uint32_t stride = static_cast<uint32_t>(geom.vertices[0].stride);
        if (mPipeline && mCachedVertexFormat == geom.format && mCachedStride == stride) return;

        const auto dev = mGpu->device().handle();
        if (mPipeline) {
            dev.destroyPipeline(mPipeline);
            mPipeline = nullptr;
        }

        ensurePipelineLayout();
        if (!mPipelineLayout) return;

        mCachedVertexFormat = geom.format;
        mCachedStride       = stride;

        const char *                      vertEntry = mCreateParams.vs.entry ? mCreateParams.vs.entry : "main";
        const char *                      fragEntry = mCreateParams.ps.entry ? mCreateParams.ps.entry : "main";
        vk::PipelineShaderStageCreateInfo stages[2] = {
            {vk::PipelineShaderStageCreateFlags(), vk::ShaderStageFlagBits::eVertex, mVertModule, vertEntry},
            {vk::PipelineShaderStageCreateFlags(), vk::ShaderStageFlagBits::eFragment, mFragModule, fragEntry},
        };

        vk::VertexInputBindingDescription              binding {0, stride, vk::VertexInputRate::eVertex};
        DynaArray<vk::VertexInputAttributeDescription> vkAttrs;
        vkAttrs.reserve(geom.format.attributes.size());
        for (const auto & a : geom.format.attributes) vkAttrs.append({a.location, 0, toVkFormat(a.format), a.offset});

        vk::PipelineVertexInputStateCreateInfo   vertexInput {{}, 1, &binding, static_cast<uint32_t>(vkAttrs.size()), vkAttrs.data()};
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
        viewportState.viewportCount                        = 1;
        viewportState.scissorCount                         = 1;
        vk::DynamicState                   dynamicStates[] = {vk::DynamicState::eViewport, vk::DynamicState::eScissor};
        vk::PipelineDynamicStateCreateInfo dynamicState {{}, 2, dynamicStates};
        vk::PipelineRenderingCreateInfo    renderingCi {};
        const vk::Format                   colorFormats[] = {vk::Format::eB8G8R8A8Unorm};
        renderingCi.setColorAttachmentFormats(colorFormats);

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
        pipeCi.setRenderPass(nullptr);

        auto result = dev.createGraphicsPipeline(nullptr, pipeCi);
        if (result.result != vk::Result::eSuccess) GN_UNLIKELY {
                GN_ERROR(sLogger)("GpuDrawVulkan: createGraphicsPipeline failed, name='{}'", this->name.c_str());
                mCachedVertexFormat.attributes.clear();
                mCachedStride = 0;
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
        if (mNoGeomPipeline) dev.destroyPipeline(mNoGeomPipeline);
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

        const GpuGeometry & geom        = a->geometry;
        const bool          hasGeometry = !geom.format.empty() && !geom.vertices.empty() && geom.vertices[0].stride > 0;

        vk::Pipeline activePipeline {};
        uint32_t     vertexCount = 0;
        if (hasGeometry) {
            ensurePipelineForGeometry(geom);
            if (!mPipeline) GN_UNLIKELY {
                    GN_ERROR(sLogger)("{} - geometry has vertex format but pipeline creation failed", taskInfo);
                    return FAILED;
                }
            activePipeline = mPipeline;
            vertexCount    = (uint32_t) geom.vertices[0].count();
        } else if (geom.vertexCount > 0) {
            ensureNoGeomPipeline();
            activePipeline = mNoGeomPipeline;
            vertexCount    = geom.vertexCount;
        }

        if (activePipeline) {
            cb.commandBuffer.handle().bindPipeline(vk::PipelineBindPoint::eGraphics, activePipeline);
            if (!a->constants.empty()) {
                const auto size = static_cast<uint32_t>(a->constants.size());
                if (size <= 128)
                    cb.commandBuffer.handle().pushConstants(mPipelineLayout, vk::ShaderStageFlagBits::eVertex, 0, size, a->constants.data());
                else {
                    GN_ERROR(sLogger)("{} - inline constants size is too large, size={}", taskInfo, size);
                    return FAILED;
                }
            }
            if (vertexCount > 0) cb.commandBuffer.handle().draw(vertexCount, 1, 0, 0);
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