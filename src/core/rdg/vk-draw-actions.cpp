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
    AutoRef<GpuContextVulkan>     mGpu;
    GenericDraw::CreateParameters mCreateParams {};
    vk::ShaderModule              mVertModule {};
    vk::ShaderModule              mFragModule {};
    vk::PipelineLayout            mPipelineLayout {};
    vk::Pipeline                  mPipeline {}; // null until pipeline creation (Phase 6 / Task 6.3)

    static vk::ShaderModule createShaderModule(vk::Device device, const Blob * blob) {
        if (!blob || blob->size() == 0 || (blob->size() % 4) != 0) return {};
        vk::ShaderModuleCreateInfo ci {};
        ci.codeSize = blob->size();
        ci.pCode    = reinterpret_cast<const uint32_t *>(blob->data());
        return device.createShaderModule(ci);
    }

    void createPipelineIfNeeded() {
        if (mPipeline || !mVertModule || !mFragModule) return;
        const auto dev = mGpu->device().handle();

        // Pipeline layout: no descriptor sets, no push constants (Task 6.2)
        vk::PipelineLayoutCreateInfo layoutCi {};
        mPipelineLayout = dev.createPipelineLayout(layoutCi);
        if (!mPipelineLayout) GN_UNLIKELY {
                GN_WARN(sLogger)("GenericDrawVulkan: createPipelineLayout failed, name='{}'", name);
                return;
            }

        const char * vertEntry = (mCreateParams.vs && !mCreateParams.vs->entryPoint.empty()) ? mCreateParams.vs->entryPoint.c_str() : "main";
        const char * fragEntry = (mCreateParams.ps && !mCreateParams.ps->entryPoint.empty()) ? mCreateParams.ps->entryPoint.c_str() : "main";

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
                GN_ERROR(sLogger)("GenericDrawVulkan: createGraphicsPipeline failed, name='{}'", name);
                dev.destroyPipelineLayout(mPipelineLayout);
                mPipelineLayout = nullptr;
                return;
            }
        mPipeline = result.value;
    }

public:
    GenericDrawVulkan(ArtifactDatabase & db, const StrA & name, AutoRef<GpuContextVulkan> gpu, const GenericDraw::CreateParameters & params)
        : GenericDraw(db, TYPE, name), mGpu(gpu), mCreateParams(params) {
        const auto dev = mGpu->device().handle();
        if (params.vs && params.vs->shaderBinary) mVertModule = createShaderModule(dev, params.vs->shaderBinary.get());
        if (params.ps && params.ps->shaderBinary) mFragModule = createShaderModule(dev, params.ps->shaderBinary.get());
        bool vsFail = (params.vs && params.vs->shaderBinary && !mVertModule);
        bool psFail = (params.ps && params.ps->shaderBinary && !mFragModule);
        if (vsFail || psFail) { GN_WARN(sLogger)("GenericDrawVulkan: failed to create one or more shader modules, name='{}'", name); }
    }

    ~GenericDrawVulkan() override {
        const auto dev = mGpu->device().handle();
        if (mPipeline) dev.destroyPipeline(mPipeline);
        if (mPipelineLayout) dev.destroyPipelineLayout(mPipelineLayout);
        if (mVertModule) dev.destroyShaderModule(mVertModule);
        if (mFragModule) dev.destroyShaderModule(mFragModule);
    }

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

        createPipelineIfNeeded();

        // Task 7.1: if vs/ps were provided but pipeline is missing (creation failed), fail the task.
        const bool requiredShaders = mCreateParams.vs && mCreateParams.ps && mCreateParams.vs->shaderBinary && mCreateParams.ps->shaderBinary;
        if (requiredShaders && !mPipeline) GN_UNLIKELY {
                GN_ERROR(sLogger)("GenericDrawVulkan::execute: vs/ps provided but pipeline not created, name='{}'", name);
                return FAILED;
            }

        // When pipeline is valid: set viewport/scissor from render target extent, then bind and draw (Task 3.3 / 6.4).
        if (mPipeline) {
            uint32_t extentW = 0, extentH = 0;
            if (renderTarget->colors.size() > 0) {
                const auto & c0 = renderTarget->colors[0];
                if (c0.target.index() == 0) {
                    auto tex = std::get<0>(c0.target).castTo<TextureVulkan>().get();
                    if (tex) {
                        auto dim = tex->dimensions(c0.subresourceIndex.mip);
                        extentW  = dim.width;
                        extentH  = dim.height;
                    }
                } else {
                    auto bb = std::get<1>(c0.target).castTo<BackbufferVulkan>().get();
                    if (bb) {
                        extentW = bb->descriptor().width;
                        extentH = bb->descriptor().height;
                    }
                }
            }
            if (extentW > 0 && extentH > 0) {
                vk::Viewport vp {0.0f, 0.0f, (float) extentW, (float) extentH, 0.0f, 1.0f};
                cb.commandBuffer.handle().setViewport(0, vp);
                cb.commandBuffer.handle().setScissor(0, vk::Rect2D {{0, 0}, {extentW, extentH}});
            }
            cb.commandBuffer.handle().bindPipeline(vk::PipelineBindPoint::eGraphics, mPipeline);
            const auto *   dp            = a->drawParams.get();
            const uint32_t vertexCount   = dp ? dp->vertexCount : 0;
            const uint32_t instanceCount = dp ? dp->instanceCount : 1;
            const uint32_t firstVertex   = dp ? dp->firstVertex : 0;
            const uint32_t firstInstance = dp ? dp->firstInstance : 0;
            if (vertexCount > 0) cb.commandBuffer.handle().draw(vertexCount, instanceCount, firstVertex, firstInstance);
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