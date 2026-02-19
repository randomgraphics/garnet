#include "vk-render-pass.h"
#include "vk-texture.h"
#include "vk-resource-tracker.h"
#include "vk-backbuffer.h"

namespace GN::rdg {

static GN::Logger * sLogger = GN::getLogger("GN.rdg");

RenderPassManagerVulkan::RenderPassManagerVulkan(const ConstructParameters & params): SubmissionImpl::Context(TYPE), mGpu(params.gpu) {}

RenderPassManagerVulkan::~RenderPassManagerVulkan() {}

// static vk::Format pixelFormatToVk(gfx::img::PixelFormat pf) {
//     if (pf == gfx::img::PixelFormat::RGBA_8_8_8_8_UNORM() || pf == gfx::img::PixelFormat::RGBA8()) return vk::Format::eR8G8B8A8Unorm;
//     if (pf == gfx::img::PixelFormat::BGRA_8_8_8_8_UNORM() || pf == gfx::img::PixelFormat::BGRA8()) return vk::Format::eB8G8R8A8Unorm;
//     if (pf == gfx::img::PixelFormat::RGBA_8_8_8_8_SRGB()) return vk::Format::eR8G8B8A8Srgb;
//     return vk::Format::eR8G8B8A8Unorm;
// }

// bool RenderPassManagerVulkan::getOrCreateRpFb(const RenderTarget & renderTarget) {
//     if (!mGpu) GN_UNLIKELY return false;
//     const auto & gi = mGpu->globalInfo();

//     for (vk::ImageView v : mCurrentImageViews) gi.device.destroyImageView(v, gi.allocator);
//     mCurrentImageViews.clear();
//     mCurrentRenderPass.reset();
//     if (mCurrentFramebuffer) {
//         gi.device.destroyFramebuffer(mCurrentFramebuffer, gi.allocator);
//         mCurrentFramebuffer = VK_NULL_HANDLE;
//     }

//     std::vector<vk::Format> colorFormats;
//     vk::Format              depthFormat = vk::Format::eUndefined;
//     uint32_t                width = 1, height = 1;

//     for (const auto & color : renderTarget.colors) {
//         auto * tex = std::get_if<AutoRef<Texture>>(&color.target);
//         if (!tex || !tex->get()) GN_UNLIKELY continue;
//         auto * texVk = tex->get()->castTo<TextureVulkan>();
//         if (!texVk || !texVk->handle()) GN_UNLIKELY continue;
//         vk::Image  image = texVk->handle();
//         vk::Format format = pixelFormatToVk(texVk->descriptor().format);
//         colorFormats.push_back(format);
//         const auto & d = texVk->descriptor();
//         if (width == 1 && height == 1) {
//             width  = d.width > 0 ? d.width : 1;
//             height = d.height > 0 ? d.height : 1;
//         }
//         vk::ImageSubresourceRange range(vk::ImageAspectFlagBits::eColor, color.subresourceIndex.mip, 1, color.subresourceIndex.face, 1);
//         vk::ImageView view = gi.device.createImageView(vk::ImageViewCreateInfo({}, image, vk::ImageViewType::e2D, format).setSubresourceRange(range),
//         gi.allocator); if (!view) GN_UNLIKELY return false; mCurrentImageViews.push_back(view);
//     }

//     if (colorFormats.empty()) GN_UNLIKELY return false;

//     auto rpParams = rapid_vulkan::RenderPass::ConstructParameters {{"rdg render pass"}, &gi}.simple(colorFormats, depthFormat, true, true);
//     mCurrentRenderPass.reset(new rapid_vulkan::RenderPass(rpParams));
//     if (!mCurrentRenderPass->handle()) GN_UNLIKELY return false;

//     vk::FramebufferCreateInfo fbCi({}, mCurrentRenderPass->handle());
//     fbCi.setAttachments(mCurrentImageViews).setWidth((uint32_t) width).setHeight((uint32_t) height).setLayers(1);
//     mCurrentFramebuffer = gi.device.createFramebuffer(fbCi, gi.allocator);
//     if (!mCurrentFramebuffer) GN_UNLIKELY return false;

//     mCurrentExtent = vk::Extent2D(width, height);
//     return true;
// }

const rapid_vulkan::Image * getColorTargetImage(const RenderTarget::ColorTarget & color, [[maybe_unused]] size_t stage, SubmissionImpl & submission) {
    const rapid_vulkan::Image * image = nullptr;
    if (color.target.index() == 0) {
        // this color target is a texture.
        auto tex = std::get<0>(color.target).castTo<TextureVulkan>().get();
        if (!tex) GN_UNLIKELY {
                GN_ERROR(sLogger)("RenderPassManagerVulkan::execute: render target texture is not set for stage {}", stage);
                return {};
            }
        image = tex->image();
        if (!image) GN_UNLIKELY {
                GN_ERROR(sLogger)("RenderPassManagerVulkan::execute: the render target texture for stage {} is not properly initialized.", stage);
                return {};
            }
    } else {
        // this color target is a backbuffer.
        auto bb = std::get<1>(color.target).castTo<BackbufferVulkan>().get();
        if (!bb) {
            GN_ERROR(sLogger)("RenderPassManagerVulkan::execute: render target backbuffer is not set for stage {}", stage);
            return {};
        }
        auto ctx = submission.getExecutionContext<FrameExecutionContextVulkan>();
        if (!ctx) GN_UNLIKELY {
                GN_ERROR(sLogger)
                ("RenderPassManagerVulkan::execute: FrameExecutionContextVulkan not found. This is most likely due to rendering to a unprepared backbuffer.");
                return {};
            }
        auto frameIter = ctx->bb2frame.find(bb->sequence);
        if (frameIter == ctx->bb2frame.end()) GN_UNLIKELY {
                GN_ERROR(sLogger)
                ("RenderPassManagerVulkan::execute: can't fine the frame for backbuffer. This is most likely due to rendering to a unprepared backbuffer.");
                return {};
            }
        image = frameIter->second->backbuffer->image.get();
        if (!image) GN_UNLIKELY {
                GN_ERROR(sLogger)("RenderPassManagerVulkan::execute: the backbuffer is not properly initialized.", stage);
                return {};
            }
    }
    GN_ASSERT(image);
    return image;
}

static vk::ImageView getColorTargetImageView(const RenderTarget::ColorTarget & color, [[maybe_unused]] size_t stage, SubmissionImpl & submission) {
    auto image = getColorTargetImage(color, stage, submission);
    if (!image) GN_UNLIKELY return {};
    auto viewParams = rapid_vulkan::Image::GetViewParameters()
                          .setType(vk::ImageViewType::e2D)
                          .setFormat(image->desc().format)
                          .setRange(vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eColor, color.subresourceIndex.mip, 1, color.subresourceIndex.face, 1));
    return image->getView(viewParams);
}

static vk::Format getDepthViewFormat(vk::Format format) {
    if (format == vk::Format::eD16Unorm) return vk::Format::eD16Unorm;
    if (format == vk::Format::eD32Sfloat) return vk::Format::eD32Sfloat;
    if (format == vk::Format::eD24UnormS8Uint) return vk::Format::eX8D24UnormPack32;
    if (format == vk::Format::eD32SfloatS8Uint) return vk::Format::eD32Sfloat;
    return vk::Format::eUndefined;
}

static vk::Format getStencilViewFormat(vk::Format format) {
    if (format == vk::Format::eD16UnormS8Uint) return vk::Format::eS8Uint;
    if (format == vk::Format::eD24UnormS8Uint) return vk::Format::eS8Uint;
    if (format == vk::Format::eD32SfloatS8Uint) return vk::Format::eS8Uint;
    return vk::Format::eUndefined;
}

static vk::ImageView getDepthTargetImageView(const RenderTarget::DepthStencil & depthStencil) {
    auto depth = depthStencil.target.castTo<TextureVulkan>().get();
    if (!depth) GN_UNLIKELY return {};

    auto image = depth->image();
    if (!image) GN_UNLIKELY {
            GN_ERROR(sLogger)("RenderPassManagerVulkan::execute: the depth target texture is not properly initialized.");
            return {};
        }

    auto depthFormat = getDepthViewFormat(image->desc().format);
    if (depthFormat == vk::Format::eUndefined) GN_UNLIKELY {
            GN_ERROR(sLogger)("RenderPassManagerVulkan::execute: supported depth texture format: {}.", (uint32_t) image->desc().format);
            return {};
        }

    auto depthViewParams =
        rapid_vulkan::Image::GetViewParameters()
            .setType(vk::ImageViewType::e2D)
            .setFormat(depthFormat)
            .setRange(vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eDepth, depthStencil.subresourceIndex.mip, 1, depthStencil.subresourceIndex.face, 1));
    auto view = image->getView(depthViewParams);
    if (!view) GN_UNLIKELY {
            GN_ERROR(sLogger)("RenderPassManagerVulkan::execute: can't create depth view for depth target texture.");
            return {};
        }

    // done
    return view;
}

static vk::ImageView getStencilTargetImageView(const RenderTarget::DepthStencil & depthStencil) {
    auto depth = depthStencil.target.castTo<TextureVulkan>().get();
    if (!depth) GN_UNLIKELY return {};

    auto image = depth->image();
    if (!image) GN_UNLIKELY {
            GN_ERROR(sLogger)("RenderPassManagerVulkan::execute: the depth target texture is not properly initialized.");
            return {};
        }

    auto stencilFormat = getStencilViewFormat(image->desc().format);
    if (stencilFormat == vk::Format::eUndefined) {
        // the depth texture's format does not have a stencil component. This is NOT an error.
        return {};
    }

    // setup stencil attachment, only if the depth texture's format has a stencil component.
    auto stencilViewParams = rapid_vulkan::Image::GetViewParameters()
                                 .setType(vk::ImageViewType::e2D)
                                 .setFormat(stencilFormat)
                                 .setRange(vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eStencil, depthStencil.subresourceIndex.mip, 1,
                                                                     depthStencil.subresourceIndex.face, 1));
    auto stencilView = image->getView(stencilViewParams);
    if (!stencilView) GN_UNLIKELY {
            GN_ERROR(sLogger)("RenderPassManagerVulkan::execute: can't create stencil view for depth target texture.");
            return {};
        }

    // done.
    return stencilView;
}

bool RenderPassManagerVulkan::prepare(TaskInfo & taskInfo, const RenderTarget & renderTarget) {
    bool changed = !mPrevRenderTarget || *mPrevRenderTarget != renderTarget;
    if (changed) {
        GN_INFO(sLogger)("RenderPassManagerVulkan::prepare: render target changed, starting a new render pass.");
        mRenderPasses.push_back(RenderPass {.firstTaskIndex = taskInfo.index, .lastTaskIndex = taskInfo.index});
    } else {
        mRenderPasses.back().lastTaskIndex = taskInfo.index;
        GN_INFO(sLogger)("RenderPassManagerVulkan::prepare: render target not changed, continuing the current render pass.");
    }
    return true;
}

const RenderPassManagerVulkan::RenderPass * RenderPassManagerVulkan::execute(TaskInfo & taskInfo, const RenderTarget & renderTarget,
                                                                             [[maybe_unused]] const ResourceTrackerVulkan & rt,
                                                                             vk::CommandBuffer                              commandBuffer) {
    // Find the render pass that the task belongs to.
    GN_INFO(sLogger)("RenderPassManagerVulkan::execute: {} render pass(es) found.", mRenderPasses.size());
    RenderPass * rp = nullptr;
    for (size_t i = 0; i < mRenderPasses.size(); i++) {
        if (mRenderPasses[i].firstTaskIndex <= taskInfo.index && mRenderPasses[i].lastTaskIndex >= taskInfo.index) { rp = &mRenderPasses[i]; }
    }
    if (!rp) {
        GN_ERROR(sLogger)("Workflow '{}' task '{}' doesn't seem to be part of any render pass.", taskInfo.workflow, taskInfo.task);
        return nullptr;
    }

    // If this is the first task of the render pass, start a new render pass.
    if (rp->firstTaskIndex == taskInfo.index) {
        auto renderInfo = vk::RenderingInfo().setLayerCount(1);

        // setup color attachments.
        std::vector<vk::RenderingAttachmentInfo> colorAttachments;
        colorAttachments.reserve(renderTarget.colors.size());
        for (size_t i = 0; i < renderTarget.colors.size(); i++) {
            const auto & color = renderTarget.colors[i];
            auto         view  = getColorTargetImageView(color, i, static_cast<SubmissionImpl &>(taskInfo.submission));
            if (!view) GN_UNLIKELY {
                    GN_ERROR(sLogger)("RenderPassManagerVulkan::execute: can't create view for render target texture for stage {}.", i);
                    return nullptr;
                }
            colorAttachments.push_back(
                vk::RenderingAttachmentInfo()
                    .setImageView(view)
                    .setLoadOp(vk::AttachmentLoadOp::eClear)
                    .setImageLayout(vk::ImageLayout::eUndefined)
                    .setClearValue(vk::ClearValue(vk::ClearColorValue(0.0f, 0.0f, 0.0f, 1.0f)))); // TODO: get clear value from ClearRenderTarget action.
        }
        renderInfo.setColorAttachments(colorAttachments);

        // setup depth attachment
        vk::RenderingAttachmentInfo depthAttachment, stencilAttachment;
        auto                        depthView = getDepthTargetImageView(renderTarget.depthStencil);
        if (depthView) {
            depthAttachment.setImageView(depthView)
                .setLoadOp(vk::AttachmentLoadOp::eClear)
                .setImageLayout(vk::ImageLayout::eUndefined)
                .setClearValue(vk::ClearValue(vk::ClearDepthStencilValue(1.0f, 0))); // TODO: get clear value from ClearRenderTarget action.
            renderInfo.setPDepthAttachment(&depthAttachment);
        }

        // setup stencil attachment
        auto stencilView = getStencilTargetImageView(renderTarget.depthStencil);
        if (stencilView) {
            stencilAttachment.setImageView(stencilView)
                .setLoadOp(vk::AttachmentLoadOp::eClear)
                .setImageLayout(vk::ImageLayout::eUndefined)
                .setClearValue(vk::ClearValue(vk::ClearDepthStencilValue(0, 0))); // TODO: get clear value from ClearRenderTarget action.
            renderInfo.setPStencilAttachment(&stencilAttachment);
        }

        // start a new dynamic render pass.
        commandBuffer.beginRendering(renderInfo);
    }
    // done
    return rp;
}

} // namespace GN::rdg