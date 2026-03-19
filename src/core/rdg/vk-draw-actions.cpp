#include "pch.h"
#include "vk-draw-actions.h"
#include "vk-submission-context.h"
#include "vk-backbuffer.h"
#include "vk-buffer-state.h"
#include "vk-command-buffer.h"
#include "vk-texture.h"
#include "vk-format-utils.h"
#include "vk-transient-buffer.h"
#include "vk-pso-factory.h"
#include <rapid-vulkan/rapid-vulkan.h>

namespace GN::rdg {

static GN::Logger * sLogger = getLogger("GN.rdg");

// =====================================================================================================================
// ClearRenderTargetVulkan
// =====================================================================================================================

class ClearRenderTargetVulkan : public ClearRenderTarget {
    AutoRef<GpuContextVulkan> mGpu;

public:
    ClearRenderTargetVulkan(const StrA & name, AutoRef<GpuContextVulkan> gpu): ClearRenderTarget(TYPE_INFO(), name), mGpu(gpu) {}

    ExecutionResult prepare(TaskInfo & taskInfo, Arguments & arguments) override {
        auto & submission = taskInfo.submission;

        auto a = RuntimeType::cast<ClearRenderTarget::A>(arguments);
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

        auto a = RuntimeType::cast<ClearRenderTarget::A>(arguments);
        GN_RDG_FAIL_ON_FALSE(a, "{} - arguments is not ClearRenderTarget::A", taskInfo);

        // standard execution
        auto & sc = submission.ensureSubmissionContext<SubmissionContextVulkan>(mGpu);
        auto   cb = sc.commandBufferManager.execute(taskInfo);
        GN_RDG_FAIL_ON_FALSE(cb, "{} - failed to get command buffer", taskInfo);
        auto rp = sc.renderPassManager.execute(taskInfo, &cb);
        GN_RDG_FAIL_ON_FAIL(rp);

        // done
        return PASSED;
    }
};

AutoRef<ClearRenderTarget> createVulkanClearRenderTarget(const StrA & name, const ClearRenderTarget::CreateParameters & params) {
    auto gpu = params.gpu.staticCastTo<GpuContextVulkan>();
    if (!gpu) GN_UNLIKELY {
            GN_ERROR(sLogger)("createVulkanClearRenderTarget: gpu is empty, name='{}'", name);
            return {};
        }
    return AutoRef<ClearRenderTarget>(new ClearRenderTargetVulkan(name, gpu));
}

// =====================================================================================================================
// GpuDrawVulkan
// =====================================================================================================================

namespace {

/// Get VkImageView for a GpuResourceView that is a texture. Returns VK_NULL_HANDLE on failure.
vk::ImageView getTextureImageView(const GpuResourceView & view) {
    if (!view.isTexture()) return {};
    auto tex = view.texture().staticCastTo<TextureVulkan>();
    if (!tex || !tex->image()) return {};
    const auto & iv     = view.imageView;
    uint32_t     mips   = (iv.range.e.numMipLevels == (uint32_t) -1) ? 1 : iv.range.e.numMipLevels;
    uint32_t     layers = (iv.range.e.numArrayLayers == (uint32_t) -1) ? 1 : iv.range.e.numArrayLayers;
    vk::Format   format = (iv.format != gfx::img::PixelFormat::UNKNOWN()) ? pixelFormatToVkFormat(iv.format) : pixelFormatToVkFormat(tex->descriptor().format);
    auto         params = rapid_vulkan::Image::GetViewParameters()
                              .setType(vk::ImageViewType::e2D)
                              .setFormat(format)
                              .setRange(vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eColor, iv.range.i.mip, mips, iv.range.i.face, layers));
    return tex->image()->getView(params);
}

} // namespace

class GpuDrawVulkan : public GpuDraw {
    AutoRef<GpuContextVulkan>                        mGpu;
    GpuDraw::CreateParameters                        mCreateParams {};
    mutable rapid_vulkan::Ref<rapid_vulkan::Sampler> mDefaultSampler {};

    rapid_vulkan::Sampler * ensureDefaultSampler() const {
        if (mDefaultSampler) return mDefaultSampler.get();
        const rapid_vulkan::GlobalInfo * gi = mGpu->device().gi();
        if (!gi) return nullptr;
        rapid_vulkan::Sampler::ConstructParameters cp;
        cp.gi = gi;
        cp.setLinear();
        mDefaultSampler.reset(new rapid_vulkan::Sampler(cp));
        return mDefaultSampler.get();
    }

public:
    GpuDrawVulkan(const StrA & name, AutoRef<GpuContextVulkan> gpu, const GpuDraw::CreateParameters & params)
        : GpuDraw(TYPE_INFO(), name), mGpu(gpu), mCreateParams(params) {}

    ExecutionResult prepare(TaskInfo & taskInfo, Arguments & arguments) override {
        auto & submission = taskInfo.submission;

        auto a = RuntimeType::cast<GpuDraw::A>(arguments);
        GN_RDG_FAIL_ON_FALSE(a, "{} - arguments is not GpuDraw::A", taskInfo);

        // standard preparation.
        auto & submissionContext = submission.ensureSubmissionContext<SubmissionContextVulkan>(mGpu);
        GN_RDG_FAIL_ON_FAIL(submissionContext.commandBufferManager.prepare(taskInfo, CommandBufferManagerVulkan::GRAPHICS));
        GN_RDG_FAIL_ON_FAIL(submissionContext.renderPassManager.prepareDraw(taskInfo, a->renderTarget));
        GN_RDG_FAIL_ON_FAIL(submissionContext.renderPassManager.registerDrawTextureTransitions(taskInfo, a->resources));
        GN_RDG_FAIL_ON_FAIL(submissionContext.renderPassManager.registerDrawBufferTransitions(taskInfo, a->resources, a->geometry));

        return PASSED;
    }

    ExecutionResult execute(TaskInfo & taskInfo, Arguments & arguments) override {
        auto & submission = taskInfo.submission;

        auto a = RuntimeType::cast<GpuDraw::A>(arguments);
        GN_RDG_FAIL_ON_FALSE(a, "{} - arguments is not GpuDraw::A", taskInfo);

        const auto size = static_cast<uint32_t>(a->immediates.size());
        if (size > 128) GN_UNLIKELY {
                GN_ERROR(sLogger)("{} - inline constants size is too large (max = 128 bytes), size={}", taskInfo, size);
                return FAILED;
            }

        auto & sc = submission.ensureSubmissionContext<SubmissionContextVulkan>(mGpu);
        auto   cb = sc.commandBufferManager.execute(taskInfo);
        GN_RDG_FAIL_ON_FALSE(cb, "{} - failed to get command buffer", taskInfo);
        auto rp = sc.renderPassManager.execute(taskInfo, &cb);
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

        // Resource state transitions are done in the render pass manager at beginRenderPass (registered in prepare pass).

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

        // Bind shader resources from 3-D GpuResourceTable; Drawable/DrawPack manage layout and descriptors.
        for (size_t setIdx = 0; setIdx < a->resources.size(); ++setIdx) {
            const auto & set = a->resources[setIdx];
            for (size_t bindingIdx = 0; bindingIdx < set.size(); ++bindingIdx) {
                const auto & views = set[bindingIdx];
                if (views.empty()) continue;
                // Buffers: convert to rapid_vulkan BufferView and bind.
                if (views[0].isBuffer()) {
                    std::vector<rapid_vulkan::BufferView> rvViews;
                    rvViews.reserve(views.size());
                    for (const auto & v : views) {
                        if (!v.artifact || !v.isBuffer()) continue;
                        auto ref = BufferUtils::toRapid(v.buffer());
                        if (!ref) continue;
                        vk::DeviceSize sz = v.bufferView.size;
                        if (sz == 0) sz = vk::DeviceSize(-1);
                        rvViews.push_back(rapid_vulkan::BufferView {ref, v.bufferView.offset, sz});
                    }
                    if (!rvViews.empty()) {
                        drawable.b(rapid_vulkan::DescriptorIdentifier(static_cast<uint32_t>(setIdx), static_cast<uint32_t>(bindingIdx)), rvViews);
                    }
                }
                // Image/sampler: bind as combined image sampler for material textures.
                if (views[0].isImage()) {
                    rapid_vulkan::Sampler * sampler = ensureDefaultSampler();
                    if (!sampler) continue;
                    std::vector<rapid_vulkan::ImageSampler> imgSamplers;
                    imgSamplers.reserve(views.size());
                    for (const auto & v : views) {
                        if (!v.artifact || !v.isImage()) continue;
                        vk::ImageView iv = getTextureImageView(v);
                        if (!iv) continue;
                        rapid_vulkan::ImageSampler is;
                        is.view    = iv;
                        is.layout  = vk::ImageLayout::eShaderReadOnlyOptimal;
                        is.sampler = sampler;
                        imgSamplers.push_back(is);
                    }
                    if (!imgSamplers.empty()) {
                        drawable.t(rapid_vulkan::DescriptorIdentifier(static_cast<uint32_t>(setIdx), static_cast<uint32_t>(bindingIdx)), imgSamplers);
                    }
                }
            }
        }

        if (!geom.vertices.empty() && geom.vertices[0].buffer) {
            auto ref = BufferUtils::toRapid(geom.vertices[0].buffer);
            if (ref) GN_LIKELY {
                    const rapid_vulkan::BufferView view {ref.get(), geom.vertices[0].offset, vk::DeviceSize(-1)};
                    drawable.v(vk::ArrayProxy<const rapid_vulkan::BufferView>(1, &view));
                }
        }

        if (geom.indexCount > 0 && geom.indices.buffer) {
            auto ref = BufferUtils::toRapid(geom.indices.buffer);
            if (ref) GN_LIKELY {
                    const rapid_vulkan::BufferView  view {ref.get(), geom.indices.offset, vk::DeviceSize(-1)};
                    const vk::IndexType             indexType = (geom.indices.stride == 4) ? vk::IndexType::eUint32 : vk::IndexType::eUint16;
                    drawable.i(view, indexType);
                }
        }

        rapid_vulkan::GraphicsPipeline::DrawParameters drawParams {};
        if (geom.indexCount > 0 && geom.indices.buffer) {
            drawParams.setIndexed(geom.indexCount).setInstance(1, 0);
        } else {
            drawParams.setNonIndexed(geom.vertexCount, 0).setInstance(1, 0);
        }
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

AutoRef<GpuDraw> createVulkanGpuDraw(const StrA & name, const GpuDraw::CreateParameters & params) {
    auto gpu = params.context.staticCastTo<GpuContextVulkan>();
    if (!gpu) GN_UNLIKELY {
            GN_ERROR(sLogger)("createVulkanGpuDraw: gpu is empty, name='{}'", name);
            return {};
        }
    return AutoRef<GpuDraw>(new GpuDrawVulkan(name, gpu, params));
}

} // namespace GN::rdg