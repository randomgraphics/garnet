#include "pch.h"
#include "vk-render-pass.h"
#include "vk-texture.h"
#include "vk-backbuffer.h"

namespace GN::rdg {

namespace {

GN::Logger * sLogger = GN::getLogger("GN.rdg");

// static vk::Format pixelFormatToVk(gfx::img::PixelFormat pf) {
//     if (pf == gfx::img::PixelFormat::RGBA_8_8_8_8_UNORM() || pf == gfx::img::PixelFormat::RGBA8()) return vk::Format::eR8G8B8A8Unorm;
//     if (pf == gfx::img::PixelFormat::BGRA_8_8_8_8_UNORM() || pf == gfx::img::PixelFormat::BGRA8()) return vk::Format::eB8G8R8A8Unorm;
//     if (pf == gfx::img::PixelFormat::RGBA_8_8_8_8_SRGB()) return vk::Format::eR8G8B8A8Srgb;
//     return vk::Format::eR8G8B8A8Unorm;
// }

std::pair<const rapid_vulkan::Image *, vk::Extent2D> getColorTargetImage(const GpuImageView & color, [[maybe_unused]] size_t stage) {
    const rapid_vulkan::Image * image  = nullptr;
    vk::Extent2D                extent = {0, 0};
    if (color.image.index() == 0) {
        // this color target is a texture.
        auto tex = std::get<0>(color.image).castTo<TextureVulkan>().get();
        if (!tex) GN_UNLIKELY {
                GN_ERROR(sLogger)("RenderPassManagerVulkan::execute: render target texture is not set for stage {}", stage);
                return {};
            }
        image = tex->image();
        if (!image) GN_UNLIKELY {
                GN_ERROR(sLogger)("RenderPassManagerVulkan::execute: the render target texture for stage {} is not properly initialized.", stage);
                return {};
            }
        auto dim      = tex->dimensions(color.subresourceIndex.mip);
        extent.width  = dim.width;
        extent.height = dim.height;
    } else {
        // this color target is a backbuffer.
        auto bb = std::get<1>(color.image).castTo<BackbufferVulkan>().get();
        if (!bb) {
            GN_ERROR(sLogger)("RenderPassManagerVulkan::execute: render target backbuffer is not set for stage {}", stage);
            return {};
        }
        image = bb->backBufferImage();
        if (!image) GN_UNLIKELY {
                GN_ERROR(sLogger)("RenderPassManagerVulkan::execute: the backbuffer is not prepared yet or not properly initialized.");
                return {};
            }
        extent.width  = bb->descriptor().width;
        extent.height = bb->descriptor().height;
    }
    GN_ASSERT(image);
    return {image, extent};
}

constexpr TextureVulkan::ImageState COLOR_ATTACHMENT_STATE {
    vk::ImageLayout::eColorAttachmentOptimal,
    vk::AccessFlagBits::eColorAttachmentWrite,
    vk::PipelineStageFlagBits::eColorAttachmentOutput,
};
constexpr TextureVulkan::ImageStateTransitionFlags DISCARD_CONTENT {.discardContent = true};

std::pair<vk::ImageView, vk::Extent2D> getColorTargetImageView(const GpuImageView & color, size_t stage, rapid_vulkan::Barrier * barrier = nullptr) {
    auto [image, baseExtent] = getColorTargetImage(color, stage);
    if (!image) GN_UNLIKELY return {nullptr, {0, 0}};

    if (color.image.index() == 0) {
        auto * tex = std::get<0>(color.image).castTo<TextureVulkan>().get();
        if (tex && tex->trackImageState(color.subresourceIndex.mip, 1, color.subresourceIndex.face, 1, COLOR_ATTACHMENT_STATE, DISCARD_CONTENT)) {
            const auto * st = tex->getImageState(color.subresourceIndex.mip, color.subresourceIndex.face);
            if (st) {
                vk::ImageSubresourceRange range(vk::ImageAspectFlagBits::eColor, color.subresourceIndex.mip, 1, color.subresourceIndex.face, 1);
                barrier->i(image->handle(), st->prev.access, st->curr.access, st->prev.layout, st->curr.layout, range);
            }
        }
    } else {
        auto * bb = std::get<1>(color.image).castTo<BackbufferVulkan>().get();
        if (bb && bb->trackImageState(COLOR_ATTACHMENT_STATE, DISCARD_CONTENT)) {
            const auto &              st = bb->getImageState();
            vk::ImageSubresourceRange range(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1);
            barrier->i(image->handle(), st.prev.access, st.curr.access, st.prev.layout, st.curr.layout, range);
        }
    }

    auto viewParams = rapid_vulkan::Image::GetViewParameters()
                          .setType(vk::ImageViewType::e2D)
                          .setFormat(image->desc().format)
                          .setRange(vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eColor, color.subresourceIndex.mip, 1, color.subresourceIndex.face, 1));
    auto mipExtent = rapid_vulkan::getMipLevelExtent(vk::Extent3D(baseExtent.width, baseExtent.height, 1), color.subresourceIndex.mip);
    return {image->getView(viewParams), vk::Extent2D(mipExtent.width, mipExtent.height)};
}

vk::Format getDepthViewFormat(vk::Format format) {
    if (format == vk::Format::eD16Unorm) return vk::Format::eD16Unorm;
    if (format == vk::Format::eD32Sfloat) return vk::Format::eD32Sfloat;
    if (format == vk::Format::eD24UnormS8Uint) return vk::Format::eX8D24UnormPack32;
    if (format == vk::Format::eD32SfloatS8Uint) return vk::Format::eD32Sfloat;
    return vk::Format::eUndefined;
}

vk::Format getStencilViewFormat(vk::Format format) {
    if (format == vk::Format::eD16UnormS8Uint) return vk::Format::eS8Uint;
    if (format == vk::Format::eD24UnormS8Uint) return vk::Format::eS8Uint;
    if (format == vk::Format::eD32SfloatS8Uint) return vk::Format::eS8Uint;
    return vk::Format::eUndefined;
}

vk::ImageView getDepthTargetImageView(const RenderTarget::DepthStencil & depthStencil) {
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

vk::ImageView getStencilTargetImageView(const RenderTarget::DepthStencil & depthStencil) {
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

void trackRenderTargetState(const RenderTarget & renderTarget) {
    // track the state of the color targets.
    for (size_t i = 0; i < renderTarget.colors.size(); i++) {
        const auto & color = renderTarget.colors[i].target;
        if (0 == color.image.index()) {
            // this color target is a texture.
            auto tex = std::get<0>(color.image).castTo<TextureVulkan>().get();
            if (tex)
                tex->trackImageState(color.subresourceIndex.mip, 1, color.subresourceIndex.face, 1,
                                     {vk::ImageLayout::eColorAttachmentOptimal,
                                      vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite,
                                      vk::PipelineStageFlagBits::eColorAttachmentOutput});
        } else {
            // this color target is a backbuffer.
            auto bb = std::get<1>(color.image).castTo<BackbufferVulkan>().get();
            if (bb)
                bb->trackImageState({vk::ImageLayout::eColorAttachmentOptimal,
                                     vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite,
                                     vk::PipelineStageFlagBits::eColorAttachmentOutput});
        }
    }

    // track the state of the depth stencil target.
    auto depth = renderTarget.depthStencil.target.castTo<TextureVulkan>().get();
    if (depth)
        depth->trackImageState(renderTarget.depthStencil.subresourceIndex.mip, 1, renderTarget.depthStencil.subresourceIndex.face, 1,
                               {vk::ImageLayout::eDepthStencilAttachmentOptimal,
                                vk::AccessFlagBits::eDepthStencilAttachmentRead | vk::AccessFlagBits::eDepthStencilAttachmentWrite,
                                vk::PipelineStageFlagBits::eEarlyFragmentTests | vk::PipelineStageFlagBits::eLateFragmentTests});
}

inline bool areSameRenderTarget(const AutoRef<RenderTarget> & a, const AutoRef<RenderTarget> & b) {
    if (a == b) return true;    // same object, or both empty.
    if (!a || !b) return false; // one is empty, the other is not.
    return *a == *b;
}

} // namespace

bool RenderPassManagerVulkan::prepareDraw(TaskInfo & taskInfo, AutoRef<RenderTarget> renderTarget) {
    auto & entry = mEntries[taskInfo.index];
    entry.draw   = std::move(renderTarget);
    GN_ASSERT(entry.isDraw());
    GN_ASSERT(!entry.isPresent());
    return true;
}

bool RenderPassManagerVulkan::preparePresent(TaskInfo & taskInfo, AutoRef<Backbuffer> backbuffer) {
    if (!backbuffer) GN_UNLIKELY {
            GN_ERROR(sLogger)("{} - can't present empty backbuffer", taskInfo);
            return false;
        }
    auto & entry  = mEntries[taskInfo.index];
    entry.present = std::move(backbuffer);
    GN_ASSERT(entry.isPresent());
    GN_ASSERT(!entry.isDraw());
    return true;
}

auto RenderPassManagerVulkan::execute(TaskInfo & taskInfo, vk::CommandBuffer commandBuffer) -> RenderPassExecutionResult {
    // Find the entry that the task belongs to.
    auto iter = mEntries.find(taskInfo.index);
    if (iter == mEntries.end()) GN_UNLIKELY {
            GN_ERROR(sLogger)("{} - entry not found", taskInfo);
            return {};
        }

    auto & entry = iter->second;

    if (entry.isDraw()) {
        // we need to start a new render pass if one of the following is true:
        // 1. this is the first task in the render pass.
        // 2. the previous task is a present action (while the current task is a draw)
        // 3. the previous task is a draw or clear action with a different render target.
        bool needtoBegin = iter == mEntries.begin() || std::prev(iter)->second.present || std::prev(iter)->second.draw != entry.draw;
        if (needtoBegin && !beginRenderPass(*entry.draw, commandBuffer)) return {};

        /// We need to end the render pass if one of the following is true:
        /// 1. this is the last task in the render pass.
        /// 2. the next task is drawing to different render target or is present action.
        bool needToEnd = iter == std::prev(mEntries.end()) || std::next(iter)->second.draw != entry.draw;
        return {Action::ExecutionResult::PASSED, needToEnd};
    } else {
        // this is a present action. currently do nothing, just return.
        return {Action::ExecutionResult::PASSED};
    }
}

bool RenderPassManagerVulkan::beginRenderPass(const RenderTarget & renderTarget, vk::CommandBuffer commandBuffer) {
    GN_VERBOSE(sLogger)("begin render pass for render target: {}.", renderTarget.name);

    // setup render info.
    auto renderInfo = vk::RenderingInfo().setLayerCount(1);

    // Barrier for layout transitions; only entries with actual state changes are added (trackImageState returns true).
    rapid_vulkan::Barrier layoutBarrier;
    layoutBarrier.s(vk::PipelineStageFlagBits::eTopOfPipe, vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests);

    // setup color attachments (and add layout barriers when trackImageState indicates a transition is needed).
    vk::Rect2D                               renderArea(vk::Offset2D(0, 0), vk::Extent2D(~0u, ~0u));
    std::vector<vk::RenderingAttachmentInfo> colorAttachments;
    colorAttachments.reserve(renderTarget.colors.size());
    for (size_t i = 0; i < renderTarget.colors.size(); i++) {
        const auto & color = renderTarget.colors[i];
        auto [view, dim]   = getColorTargetImageView(color.target, i, &layoutBarrier);
        if (!view) GN_UNLIKELY {
                GN_ERROR(sLogger)("can't create view for render target texture for stage {}.", i);
                return false;
            }
        const auto & c          = color.clearColor;
        auto         clearValue = vk::ClearColorValue(c.f4[0], c.f4[1], c.f4[2], c.f4[3]);
        colorAttachments.push_back(vk::RenderingAttachmentInfo()
                                       .setImageView(view)
                                       .setLoadOp(vk::AttachmentLoadOp::eClear)
                                       .setImageLayout(vk::ImageLayout::eColorAttachmentOptimal)
                                       .setClearValue(vk::ClearValue(clearValue)));

        renderArea.extent.width  = std::min(dim.width, renderArea.extent.width);
        renderArea.extent.height = std::min(dim.height, renderArea.extent.height);
    }
    renderInfo.setColorAttachments(colorAttachments);

    // setup depth attachment
    vk::RenderingAttachmentInfo depthAttachment, stencilAttachment;
    auto                        depthView = getDepthTargetImageView(renderTarget.depthStencil);
    if (depthView) {
        depthAttachment.setImageView(depthView)
            .setLoadOp(vk::AttachmentLoadOp::eClear)
            .setImageLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal)
            .setClearValue(vk::ClearValue(vk::ClearDepthStencilValue(1.0f, 0))); // TODO: get clear value from ClearRenderTarget action.
        renderInfo.setPDepthAttachment(&depthAttachment);
        auto dim = renderTarget.depthStencil.target.castTo<TextureVulkan>().get()->dimensions(renderTarget.depthStencil.subresourceIndex.mip);
        // .extent.width  = std::min(dim.width, renderArea.extent.width);
        renderArea.extent.height = std::min(dim.height, renderArea.extent.height);
    }

    // setup stencil attachment
    auto stencilView = getStencilTargetImageView(renderTarget.depthStencil);
    if (stencilView) {
        stencilAttachment.setImageView(stencilView)
            .setLoadOp(vk::AttachmentLoadOp::eClear)
            .setImageLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal)
            .setClearValue(vk::ClearValue(vk::ClearDepthStencilValue(1.0f, 0))); // TODO: get clear value from ClearRenderTarget action.
        renderInfo.setPStencilAttachment(&stencilAttachment);
        auto dim                 = renderTarget.depthStencil.target.castTo<TextureVulkan>().get()->dimensions(renderTarget.depthStencil.subresourceIndex.mip);
        renderArea.extent.width  = std::min(dim.width, renderArea.extent.width);
        renderArea.extent.height = std::min(dim.height, renderArea.extent.height);
    }

    // setup render area.
    GN_VERBOSE(sLogger)("render pass area: (width={}, height={}).", renderArea.extent.width, renderArea.extent.height);
    renderInfo.setRenderArea(renderArea);

    // Add depth/stencil layout transition when present (color transitions added in getColorTargetImageView when needed).
    if (depthView || stencilView) {
        auto depthTex = renderTarget.depthStencil.target.castTo<TextureVulkan>().get();
        auto depthImg = depthTex->image();
        GN_ASSERT(depthImg);

        vk::ImageAspectFlags aspect = vk::ImageAspectFlagBits::eNone;
        if (depthView) aspect |= vk::ImageAspectFlagBits::eDepth;
        if (stencilView) aspect |= vk::ImageAspectFlagBits::eStencil;
        vk::AccessFlags           access = vk::AccessFlagBits::eDepthStencilAttachmentRead | vk::AccessFlagBits::eDepthStencilAttachmentWrite;
        vk::ImageSubresourceRange range(aspect, renderTarget.depthStencil.subresourceIndex.mip, 1, renderTarget.depthStencil.subresourceIndex.face, 1);
        layoutBarrier.i(depthImg->handle(), {}, access, vk::ImageLayout::eUndefined, vk::ImageLayout::eDepthStencilAttachmentOptimal, range);
    }

    // transfer image layout via barrier.
    layoutBarrier.cmdWrite(commandBuffer);

    // track the updated render target states
    trackRenderTargetState(renderTarget);

    // start a new dynamic render pass.
    commandBuffer.beginRendering(renderInfo);

    // setup viewport
    auto viewWidth  = FLT_MAX == renderTarget.viewport.width ? (float) renderArea.extent.width : renderTarget.viewport.width;
    auto viewHeight = FLT_MAX == renderTarget.viewport.height ? (float) renderArea.extent.height : renderTarget.viewport.height;
    auto viewport   = vk::Viewport(renderTarget.viewport.x, renderTarget.viewport.y, viewWidth, viewHeight, 0, 1);
    commandBuffer.setViewport(0, 1, &viewport);

    // setup scissor.
    auto scissorWidth  = (~0u) == renderTarget.scissorRect.width ? (uint32_t) std::ceil(viewWidth) : renderTarget.scissorRect.width;
    auto scissorHeight = (~0u) == renderTarget.scissorRect.height ? (uint32_t) std::ceil(viewHeight) : renderTarget.scissorRect.height;
    auto scissor       = vk::Rect2D(vk::Offset2D(renderTarget.scissorRect.x, renderTarget.scissorRect.y), vk::Extent2D(scissorWidth, scissorHeight));
    commandBuffer.setScissor(0, 1, &scissor);

    return true;
}

} // namespace GN::rdg