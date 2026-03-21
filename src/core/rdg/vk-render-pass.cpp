#include "pch.h"
#include "vk-render-pass.h"
#include "vk-backbuffer.h"
#include "vk-texture.h"
#include "vk-buffer-state.h"
#include "vk-transient-buffer.h"
#include <garnet/rdg/artifacts.h>

namespace GN::rdg {

namespace {

GN::Logger * sLogger = GN::getLogger("GN.rdg");

// static vk::Format pixelFormatToVk(gfx::img::PixelFormat pf) {
//     if (pf == gfx::img::PixelFormat::RGBA_8_8_8_8_UNORM() || pf == gfx::img::PixelFormat::RGBA8()) return vk::Format::eR8G8B8A8Unorm;
//     if (pf == gfx::img::PixelFormat::BGRA_8_8_8_8_UNORM() || pf == gfx::img::PixelFormat::BGRA8()) return vk::Format::eB8G8R8A8Unorm;
//     if (pf == gfx::img::PixelFormat::RGBA_8_8_8_8_SRGB()) return vk::Format::eR8G8B8A8Srgb;
//     return vk::Format::eR8G8B8A8Unorm;
// }

std::pair<const rapid_vulkan::Image *, vk::Extent2D> getColorTargetImage(const GpuResourceView & color, [[maybe_unused]] size_t stage) {
    const rapid_vulkan::Image * image  = nullptr;
    vk::Extent2D                extent = {0, 0};
    if (auto tex = color.texture().staticCastTo<TextureVulkan>().get()) {
        image = tex->image();
        if (!image) GN_UNLIKELY {
                GN_ERROR(sLogger)("RenderPassManagerVulkan::execute: the render target texture for stage {} is not properly initialized.", stage);
                return {};
            }
        const auto & si  = color.imageView.range.i;
        auto         dim = tex->dimensions(si.mip);
        extent.width     = dim.width;
        extent.height    = dim.height;
    } else if (auto bb = color.backbuffer().staticCastTo<BackbufferVulkan>().get()) {
        image = bb->backBufferImage();
        if (!image) GN_UNLIKELY {
                GN_ERROR(sLogger)("RenderPassManagerVulkan::execute: the backbuffer is not prepared yet or not properly initialized.");
                return {};
            }
        extent.width  = bb->descriptor().width;
        extent.height = bb->descriptor().height;
    } else {
        GN_ERROR(sLogger)("RenderPassManagerVulkan::execute: color target is not a texture or backbuffer for stage {}", stage);
        return {};
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

static constexpr TextureVulkan::ImageState DEPTH_STENCIL_ATTACHMENT_STATE {
    vk::ImageLayout::eDepthStencilAttachmentOptimal,
    vk::AccessFlagBits::eDepthStencilAttachmentRead | vk::AccessFlagBits::eDepthStencilAttachmentWrite,
    vk::PipelineStageFlagBits::eEarlyFragmentTests | vk::PipelineStageFlagBits::eLateFragmentTests,
};

std::pair<vk::ImageView, vk::Extent2D> getColorTargetImageView(const GpuResourceView & color, size_t stage, rapid_vulkan::Barrier * barrier,
                                                               CommandBufferManagerVulkan::CommandBuffer * rdgCb) {
    auto [image, baseExtent] = getColorTargetImage(color, stage);
    if (!image) GN_UNLIKELY return {nullptr, {0, 0}};

    const auto & si = color.imageView.range.i;
    if (auto tex = color.texture().staticCastTo<TextureVulkan>().get()) {
        if (tex && rdgCb) {
            GpuResourceView::SubresourceRange range;
            range.i.mip            = si.mip;
            range.i.face           = si.face;
            range.e.numMipLevels   = 1;
            range.e.numArrayLayers = 1;
            if (rdgCb->transitionTexture(tex, range, COLOR_ATTACHMENT_STATE, DISCARD_CONTENT) && barrier) {
                const auto * st = rdgCb->getTextureState(tex, si.mip, si.face);
                if (st) {
                    vk::ImageSubresourceRange vkRange(vk::ImageAspectFlagBits::eColor, si.mip, 1, si.face, 1);
                    barrier->i(image->handle(), st->prev.access, st->curr.access, st->prev.layout, st->curr.layout, vkRange);
                }
            }
        } else if (tex && !rdgCb && tex->trackImageState(si.mip, 1, si.face, 1, COLOR_ATTACHMENT_STATE, DISCARD_CONTENT) && barrier) {
            const auto * st = tex->getImageState(si.mip, si.face);
            if (st) {
                vk::ImageSubresourceRange vkRange(vk::ImageAspectFlagBits::eColor, si.mip, 1, si.face, 1);
                barrier->i(image->handle(), st->prev.access, st->curr.access, st->prev.layout, st->curr.layout, vkRange);
            }
        }
    } else if (auto bb = color.backbuffer().staticCastTo<BackbufferVulkan>().get()) {
        if (bb && rdgCb) {
            if (rdgCb->transitionBackbuffer(bb, COLOR_ATTACHMENT_STATE, DISCARD_CONTENT) && barrier) {
                const auto * st = rdgCb->getBackbufferState(bb);
                if (st) {
                    vk::ImageSubresourceRange range(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1);
                    barrier->i(image->handle(), st->prev.access, st->curr.access, st->prev.layout, st->curr.layout, range);
                }
            }
        } else if (bb && !rdgCb && bb->trackImageState(COLOR_ATTACHMENT_STATE, DISCARD_CONTENT) && barrier) {
            const auto &              st = bb->getImageState();
            vk::ImageSubresourceRange range(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1);
            barrier->i(image->handle(), st.prev.access, st.curr.access, st.prev.layout, st.curr.layout, range);
        }
    }

    auto viewParams = rapid_vulkan::Image::GetViewParameters()
                          .setType(vk::ImageViewType::e2D)
                          .setFormat(image->desc().format)
                          .setRange(vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eColor, si.mip, 1, si.face, 1));
    auto mipExtent  = rapid_vulkan::getMipLevelExtent(vk::Extent3D(baseExtent.width, baseExtent.height, 1), si.mip);
    return {image->getView(viewParams), vk::Extent2D(mipExtent.width, mipExtent.height)};
}

/// True if the format supports depth/stencil attachment per device capabilities (optimal tiling).
static bool formatSupportsDepthStencilAttachment(vk::PhysicalDevice physical, vk::Format format) {
    vk::FormatProperties props = physical.getFormatProperties(format);
    return (props.optimalTilingFeatures & vk::FormatFeatureFlagBits::eDepthStencilAttachment) != vk::FormatFeatureFlags();
}

/// True if the format has a stencil component (combined depth-stencil). When true, use the same image view for both depth and stencil attachments.
static bool formatHasStencil(vk::Format format) {
    return format == vk::Format::eD16UnormS8Uint || format == vk::Format::eD24UnormS8Uint || format == vk::Format::eD32SfloatS8Uint;
}

vk::ImageView getDepthTargetImageView(vk::PhysicalDevice physical, const GpuResourceView & depthStencilTarget) {
    auto depth = depthStencilTarget.texture().staticCastTo<TextureVulkan>().get();
    if (!depth) GN_UNLIKELY return {};

    auto image = depth->image();
    if (!image) GN_UNLIKELY {
            GN_ERROR(sLogger)("RenderPassManagerVulkan::execute: the depth target texture is not properly initialized.");
            return {};
        }

    if (!formatSupportsDepthStencilAttachment(physical, image->desc().format)) GN_UNLIKELY {
            GN_ERROR(sLogger)
            ("RenderPassManagerVulkan::execute: format does not support depth/stencil attachment: {}.", rapid_vulkan::vkFormat2String(image->desc().format));
            return {};
        }

    const auto & si = depthStencilTarget.imageView.range.i;
    auto         aspect =
        formatHasStencil(image->desc().format) ? (vk::ImageAspectFlagBits::eDepth | vk::ImageAspectFlagBits::eStencil) : vk::ImageAspectFlagBits::eDepth;
    auto depthViewParams = rapid_vulkan::Image::GetViewParameters()
                               .setType(vk::ImageViewType::e2D)
                               .setFormat(image->desc().format)
                               .setRange(vk::ImageSubresourceRange(aspect, si.mip, 1, si.face, 1));
    auto view            = image->getView(depthViewParams);
    if (!view) GN_UNLIKELY {
            GN_ERROR(sLogger)("RenderPassManagerVulkan::execute: can't create depth view for depth target texture.");
            return {};
        }

    return view;
}

/// For combined depth-stencil formats returns the same view as depth (use one view for both attachments). Otherwise returns null.
vk::ImageView getStencilTargetImageView(const GpuResourceView & depthStencilTarget, vk::ImageView depthView) {
    auto depth = depthStencilTarget.texture().staticCastTo<TextureVulkan>().get();
    if (!depth) return {};
    auto image = depth->image();
    if (!image) return {};
    if (!formatHasStencil(image->desc().format)) return {};
    return depthView; // same view for both depth and stencil
}

bool belongToSameRenderPass(const AutoRef<RenderTarget> & a, const AutoRef<RenderTarget> & b) {
    if (a == b) return true;    // same render target pointer.
    if (!a || !b) return false; // one is empty, the other is not.
    // Ignore depth/stencil/viewport/scissor state, only check color/depth-stencil attachments and pass-level clear/rect state.
    // clang-format off
    return a->colors == b->colors
        && a->depthStencilTarget == b->depthStencilTarget
        && a->clearDepth == b->clearDepth
        && a->clearStencil == b->clearStencil;
    // clang-format on
}

constexpr TextureVulkan::ImageState SHADER_READ_STATE {
    vk::ImageLayout::eShaderReadOnlyOptimal,
    vk::AccessFlagBits::eShaderRead,
    vk::PipelineStageFlagBits::eVertexShader | vk::PipelineStageFlagBits::eFragmentShader,
};
constexpr vk::AccessFlags        SHADER_READ_ACCESS  = vk::AccessFlagBits::eShaderRead;
constexpr vk::PipelineStageFlags SHADER_READ_STAGES  = vk::PipelineStageFlagBits::eVertexShader | vk::PipelineStageFlagBits::eFragmentShader;
constexpr vk::AccessFlags        VERTEX_INPUT_ACCESS = vk::AccessFlagBits::eVertexAttributeRead;
constexpr vk::PipelineStageFlags VERTEX_INPUT_STAGES = vk::PipelineStageFlagBits::eVertexInput;

static std::pair<vk::DeviceSize, vk::DeviceSize> getBufferBarrierRange(Buffer * buf, uint64_t viewOffset, uint64_t viewSize) {
    if (!buf) return {0, 0};
    vk::DeviceSize offset = viewOffset;
    if (auto * t = dynamic_cast<TransientBufferVulkan *>(buf)) offset += t->offset();
    vk::DeviceSize size = (viewSize == 0) ? vk::DeviceSize(VK_WHOLE_SIZE) : vk::DeviceSize(viewSize);
    return {offset, size};
}

} // namespace

Action::ExecutionResult RenderPassManagerVulkan::prepareDraw(TaskInfo & taskInfo, AutoRef<RenderTarget> renderTarget) {
    // New submission: reset execute counter when we start building the entry list.
    if (mEntries.empty()) mCurrentExecuteIndex = 0;

    // Validate the render target.
    if (!renderTarget) {
        if (mEntries.empty()) GN_UNLIKELY {
                GN_ERROR(sLogger)("{} - empty render target is not allowed on the first draw task.", taskInfo);
                return Action::FAILED;
            }
        const Entry & prev = mEntries.back();
        if (!prev.draw) GN_UNLIKELY {
                GN_ERROR(sLogger)("{} - empty render target is not allowed, when the previous action is presenting backbuffer.", taskInfo);
                return Action::FAILED;
            }
        renderTarget = prev.draw;
    }
    GN_ASSERT(renderTarget);

    const size_t entryIndex   = mEntries.size();
    bool         needNewPass  = entryIndex == 0 || !mEntries.back().isDraw() || !belongToSameRenderPass(mEntries.back().draw, renderTarget);
    int32_t      passBeginIdx = needNewPass ? static_cast<int32_t>(entryIndex) : mEntries.back().renderPassBeginIndex;

    mEntries.push_back(Entry {});
    Entry & e              = mEntries.back();
    e.taskIndex            = taskInfo.index;
    e.draw                 = std::move(renderTarget);
    e.renderPassBeginIndex = passBeginIdx;
    GN_ASSERT(e.isDraw());
    GN_ASSERT(!e.isPresent());
    return Action::PASSED;
}

Action::ExecutionResult RenderPassManagerVulkan::preparePresent(TaskInfo & taskInfo, AutoRef<Backbuffer> backbuffer) {
    if (!backbuffer) GN_UNLIKELY {
            GN_ERROR(sLogger)("{} - can't present empty backbuffer", taskInfo);
            return Action::FAILED;
        }

    if (mEntries.empty()) mCurrentExecuteIndex = 0;

    AutoRef<RenderTarget> drawTarget;
    if (!mEntries.empty()) {
        drawTarget = mEntries.back().draw;
        if (drawTarget) {
            const auto & colors = drawTarget->colors;
            for (size_t i = 0; i < colors.size(); i++) {
                if (backbuffer == colors[i].target.backbuffer()) {
                    drawTarget = {};
                    break;
                }
            }
        }
    }

    mEntries.push_back(Entry {});
    Entry & e              = mEntries.back();
    e.taskIndex            = taskInfo.index;
    e.present              = std::move(backbuffer);
    e.draw                 = std::move(drawTarget);
    e.renderPassBeginIndex = -1; // present is not part of a render pass
    GN_ASSERT(!e.isDraw());
    GN_ASSERT(e.isPresent());
    return Action::PASSED;
}

Action::ExecutionResult RenderPassManagerVulkan::registerDrawTextureTransitions(TaskInfo & taskInfo, const GpuShaderAction::GpuResourceTable & resources) {
    if (mEntries.empty()) GN_UNLIKELY {
            GN_ERROR(sLogger)("{} - registerDrawTextureTransitions called without preceding prepareDraw", taskInfo);
            return Action::FAILED;
        }
    Entry & passEntry = mEntries[mEntries.back().renderPassBeginIndex];

    for (size_t setIdx = 0; setIdx < resources.size(); ++setIdx) {
        const auto & set = resources[setIdx];
        for (size_t bindingIdx = 0; bindingIdx < set.size(); ++bindingIdx) {
            const auto & views = set[bindingIdx];
            if (views.empty() || !views[0].isImage()) continue;
            for (const auto & v : views) {
                if (!v.artifact || !v.isTexture()) continue;
                auto tex = v.texture().staticCastTo<TextureVulkan>().get();
                if (!tex || !tex->image()) continue;
                GpuResourceView::SubresourceRange range = v.imageView.range;
                if (range.e.numMipLevels == (uint32_t) -1) range.e.numMipLevels = tex->descriptor().levels - range.i.mip;
                if (range.e.numArrayLayers == (uint32_t) -1) range.e.numArrayLayers = tex->descriptor().faces - range.i.face;
                if (range.e.numMipLevels == 0 || range.e.numArrayLayers == 0) continue;

                for (uint32_t mip = range.i.mip; mip < range.i.mip + range.e.numMipLevels; ++mip) {
                    for (uint32_t face = range.i.face; face < range.i.face + range.e.numArrayLayers; ++face) {
                        TextureTransitionKey key {tex, mip, face};
                        auto                 it = passEntry.textureTransitions.find(key);
                        if (it != passEntry.textureTransitions.end()) {
                            if (it->second != SHADER_READ_STATE) {
                                GN_ERROR(sLogger)
                                ("{} - resource state conflict: texture '{}' (mip={}, face={}) already registered in this render pass with different state "
                                 "(layout=0x{:x}, access=0x{:x}, stages=0x{:x}); draw requires shader-read. Resolve by using consistent state for the resource.",
                                 taskInfo, tex->name.c_str(), mip, face, static_cast<uint32_t>(it->second.layout), static_cast<uint32_t>(it->second.access),
                                 static_cast<uint32_t>(it->second.stages));
                                return Action::FAILED;
                            }
                            continue;
                        }
                        passEntry.textureTransitions[key] = SHADER_READ_STATE;
                    }
                }
            }
        }
    }
    return Action::PASSED;
}

Action::ExecutionResult RenderPassManagerVulkan::registerDrawBufferTransitions(TaskInfo & taskInfo, const GpuShaderAction::GpuResourceTable & resources,
                                                                               const GpuDraw::GpuGeometry & geom) {
    if (mEntries.empty()) GN_UNLIKELY {
            GN_ERROR(sLogger)("{} - registerDrawBufferTransitions called without preceding prepareDraw", taskInfo);
            return Action::FAILED;
        }
    Entry & passEntry = mEntries[mEntries.back().renderPassBeginIndex];

    auto registerBuffer = [&](Buffer * buf, vk::DeviceSize offset, vk::DeviceSize size, vk::AccessFlags access, vk::PipelineStageFlags stage) -> bool {
        if (!buf) return true;
        BufferTransitionKey key {buf, offset, size};
        BufferState         wanted {access, stage};
        auto                it = passEntry.bufferTransitions.find(key);
        if (it != passEntry.bufferTransitions.end()) {
            if (it->second != wanted) {
                GN_ERROR(sLogger)
                ("{} - resource state conflict: buffer (offset={}, size={}) already registered in this render pass with different state "
                 "(access=0x{:x}, stage=0x{:x}); draw requires (access=0x{:x}, stage=0x{:x}). Resolve by using consistent state for the resource.",
                 taskInfo, offset, size, static_cast<uint32_t>(it->second.access), static_cast<uint32_t>(it->second.stage), static_cast<uint32_t>(access),
                 static_cast<uint32_t>(stage));
                return false;
            }
            return true;
        }
        passEntry.bufferTransitions[key] = wanted;
        return true;
    };

    for (size_t setIdx = 0; setIdx < resources.size(); ++setIdx) {
        const auto & set = resources[setIdx];
        for (size_t bindingIdx = 0; bindingIdx < set.size(); ++bindingIdx) {
            const auto & views = set[bindingIdx];
            if (views.empty() || !views[0].isBuffer()) continue;
            for (const auto & v : views) {
                if (!v.artifact || !v.isBuffer()) continue;
                Buffer * buf = v.buffer().get();
                if (!buf) continue;
                auto [offset, size] = getBufferBarrierRange(buf, v.bufferView.offset, v.bufferView.size);
                if (!registerBuffer(buf, offset, size, SHADER_READ_ACCESS, SHADER_READ_STAGES)) return Action::FAILED;
            }
        }
    }

    if (!geom.vertices.empty() && geom.vertices[0].buffer) {
        Buffer * buf = geom.vertices[0].buffer.get();
        if (buf) {
            auto [offset, size] = getBufferBarrierRange(buf, geom.vertices[0].offset, 0);
            if (!registerBuffer(buf, offset, size, VERTEX_INPUT_ACCESS, VERTEX_INPUT_STAGES)) return Action::FAILED;
        }
    }
    return Action::PASSED;
}

auto RenderPassManagerVulkan::execute(TaskInfo & taskInfo, CommandBufferManagerVulkan::CommandBuffer * cb) -> RenderPass {
    if (mCurrentExecuteIndex >= mEntries.size()) GN_UNLIKELY {
            GN_ERROR(sLogger)("{} - execute index {} out of range (entries size {})", taskInfo, mCurrentExecuteIndex, mEntries.size());
            return {taskInfo, Action::ExecutionResult::FAILED, {}, nullptr};
        }

    Entry &      entry = mEntries[mCurrentExecuteIndex];
    const size_t index = mCurrentExecuteIndex;
    mCurrentExecuteIndex++;

    if (entry.taskIndex != taskInfo.index) GN_UNLIKELY {
            GN_ERROR(sLogger)("{} - entry task index mismatch: entry has {}, expected {}", taskInfo, entry.taskIndex, taskInfo.index);
            return {taskInfo, Action::ExecutionResult::FAILED, {}, nullptr};
        }

    vk::CommandBuffer vkCB;
    if (cb && *cb) vkCB = cb->commandBuffer().handle();

    if (entry.isPresent()) { return {taskInfo, Action::ExecutionResult::PASSED, vk::CommandBuffer {}, nullptr}; }

    // Draw entry: need to begin if this entry starts the pass (renderPassBeginIndex == our index).
    bool needToBegin = (entry.renderPassBeginIndex == static_cast<int32_t>(index));

    // Integrity: non-empty transition maps must only appear on the render-pass-begin entry.
    if ((!entry.textureTransitions.empty() || !entry.bufferTransitions.empty()) && entry.renderPassBeginIndex != static_cast<int32_t>(index)) GN_UNLIKELY {
            GN_ERROR(sLogger)("{} - integrity: draw resource transitions must be on render pass begin entry only (index={}, passBegin={})", taskInfo, index,
                              entry.renderPassBeginIndex);
            return {taskInfo, Action::ExecutionResult::FAILED, {}, nullptr};
        }

    if (needToBegin && !beginRenderPass(*entry.draw, vkCB, cb, &entry)) return {taskInfo, Action::ExecutionResult::FAILED, {}, nullptr};

    // Need to end render pass if: last entry; or next is a draw with different pass; or next is present of *this* render target's backbuffer.
    bool needToEnd = (index + 1 >= mEntries.size());
    if (!needToEnd) {
        const Entry & next = mEntries[index + 1];
        needToEnd          = (next.isDraw() && (next.renderPassBeginIndex != entry.renderPassBeginIndex)) || (next.isPresent() && !next.draw);
    }

    const RenderTarget * drawTarget = entry.draw.get();
    return {taskInfo, Action::ExecutionResult::PASSED, needToEnd ? vkCB : vk::CommandBuffer {}, drawTarget};
}

bool RenderPassManagerVulkan::beginRenderPass(const RenderTarget & renderTarget, vk::CommandBuffer vkCommandBuffer,
                                              CommandBufferManagerVulkan::CommandBuffer * rdgCommandBuffer, const Entry * drawResourceTransitions) {
    GN_VERBOSE(sLogger)("begin render pass for render target: {}.", renderTarget.name);

    // Emit barriers for all draw resources registered in prepare pass (textures -> shader read, buffers -> shader read / vertex input).
    if (drawResourceTransitions && rdgCommandBuffer &&
        (!drawResourceTransitions->textureTransitions.empty() || !drawResourceTransitions->bufferTransitions.empty())) {
        rapid_vulkan::Barrier  drawBarrier;
        vk::PipelineStageFlags combinedSrcStage {};
        vk::PipelineStageFlags combinedDstStage {};
        for (const auto & [key, state] : drawResourceTransitions->textureTransitions) {
            if (!key.tex || !key.tex->image()) continue;
            GpuResourceView::SubresourceRange range;
            range.i.mip            = key.mip;
            range.i.face           = key.face;
            range.e.numMipLevels   = 1;
            range.e.numArrayLayers = 1;
            if (rdgCommandBuffer->transitionTexture(key.tex, range, state)) {
                const auto * st = rdgCommandBuffer->getTextureState(key.tex, key.mip, key.face);
                if (st) {
                    combinedSrcStage |= st->prev.stages;
                    combinedDstStage |= st->curr.stages;
                    vk::ImageSubresourceRange vkRange(vk::ImageAspectFlagBits::eColor, key.mip, 1, key.face, 1);
                    drawBarrier.i(key.tex->image()->handle(), st->prev.access, st->curr.access, st->prev.layout, st->curr.layout, vkRange);
                }
            }
        }
        for (const auto & [key, state] : drawResourceTransitions->bufferTransitions) {
            if (!key.buffer) continue;
            if (rdgCommandBuffer->transitionBuffer(key.buffer, key.offset, key.size, state.access, state.stage)) {
                const auto * st = rdgCommandBuffer->getBufferState(key.buffer);
                if (st) {
                    combinedSrcStage |= st->prev.stage;
                    combinedDstStage |= st->curr.stage;
                    vk::Buffer handle = BufferUtils::getHandle(key.buffer);
                    if (handle) drawBarrier.b(handle, st->prev.access, st->curr.access, key.offset, key.size);
                }
            }
        }
        if (combinedSrcStage) {
            drawBarrier.s(combinedSrcStage, combinedDstStage);
            drawBarrier.cmdWrite(vkCommandBuffer);
        }
    }

    // setup render info.
    auto renderInfo = vk::RenderingInfo().setLayerCount(1);

    // Barrier for layout transitions; only entries with actual state changes are added (transitionTexture returns true).
    rapid_vulkan::Barrier layoutBarrier;
    layoutBarrier.s(vk::PipelineStageFlagBits::eTopOfPipe, vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests);

    // setup color attachments (and add layout barriers when trackImageState indicates a transition is needed).
    vk::Rect2D                               renderArea(vk::Offset2D(0, 0), vk::Extent2D(~0u, ~0u));
    std::vector<vk::RenderingAttachmentInfo> colorAttachments;
    colorAttachments.reserve(renderTarget.colors.size());
    for (size_t i = 0; i < renderTarget.colors.size(); i++) {
        const auto & color = renderTarget.colors[i];
        auto [view, dim]   = getColorTargetImageView(color.target, i, &layoutBarrier, rdgCommandBuffer);
        if (!view) GN_UNLIKELY {
                GN_ERROR(sLogger)("can't create view for render target texture for stage {}.", i);
                return false;
            }
        const auto & c          = renderTarget.clearColor;
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

    // setup depth attachment (and stencil when combined format: same view for both)
    const auto &                depthStencilTarget = renderTarget.depthStencilTarget;
    vk::PhysicalDevice          physical           = mGpu->globalInfo().physical;
    vk::RenderingAttachmentInfo depthAttachment, stencilAttachment;
    auto                        depthView = getDepthTargetImageView(physical, depthStencilTarget);
    if (depthView && (renderTarget.depthState.testEnabled() || renderTarget.depthState.writeEnabled())) {
        depthAttachment.setImageView(depthView)
            .setLoadOp(vk::AttachmentLoadOp::eClear)
            .setImageLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal)
            .setClearValue(vk::ClearValue(vk::ClearDepthStencilValue(renderTarget.clearDepth, (uint32_t) renderTarget.clearStencil)));
        renderInfo.setPDepthAttachment(&depthAttachment);
        auto stencilView = getStencilTargetImageView(depthStencilTarget, depthView);
        if (stencilView && renderTarget.stencilState.enabled()) {
            stencilAttachment.setImageView(stencilView)
                .setLoadOp(vk::AttachmentLoadOp::eClear)
                .setImageLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal)
                .setClearValue(vk::ClearValue(vk::ClearDepthStencilValue(renderTarget.clearDepth, (uint32_t) renderTarget.clearStencil)));
            renderInfo.setPStencilAttachment(&stencilAttachment);
        }
        auto depthTex = depthStencilTarget.texture().staticCastTo<TextureVulkan>().get();
        if (depthTex) {
            const auto & si          = depthStencilTarget.imageView.range.i;
            auto         dim         = depthTex->dimensions(si.mip);
            renderArea.extent.width  = std::min(dim.width, renderArea.extent.width);
            renderArea.extent.height = std::min(dim.height, renderArea.extent.height);
        }
    }

    // setup render area.
    GN_VERBOSE(sLogger)("render pass area: (width={}, height={}).", renderArea.extent.width, renderArea.extent.height);
    renderInfo.setRenderArea(renderArea);

    // Add depth/stencil layout transition when present (color transitions added in getColorTargetImageView when needed).
    if (depthView && depthStencilTarget.artifact) {
        auto depthTex = depthStencilTarget.texture().staticCastTo<TextureVulkan>().get();
        if (depthTex) {
            auto depthImg = depthTex->image();
            GN_ASSERT(depthImg);
            vk::ImageAspectFlags aspect = vk::ImageAspectFlagBits::eDepth;
            if (formatHasStencil(depthImg->desc().format)) aspect |= vk::ImageAspectFlagBits::eStencil;
            const auto & si = depthStencilTarget.imageView.range.i;
            if (rdgCommandBuffer) {
                GpuResourceView::SubresourceRange range;
                range.i.mip            = si.mip;
                range.i.face           = si.face;
                range.e.numMipLevels   = 1;
                range.e.numArrayLayers = 1;
                if (rdgCommandBuffer->transitionTexture(depthTex, range, DEPTH_STENCIL_ATTACHMENT_STATE)) {
                    const auto * st = rdgCommandBuffer->getTextureState(depthTex, si.mip, si.face);
                    if (st) {
                        vk::ImageSubresourceRange vkRange(aspect, si.mip, 1, si.face, 1);
                        layoutBarrier.i(depthImg->handle(), st->prev.access, st->curr.access, st->prev.layout, st->curr.layout, vkRange);
                    }
                }
            } else {
                vk::ImageSubresourceRange vkRange(aspect, si.mip, 1, si.face, 1);
                layoutBarrier.i(depthImg->handle(), {}, vk::AccessFlagBits::eDepthStencilAttachmentRead | vk::AccessFlagBits::eDepthStencilAttachmentWrite,
                                vk::ImageLayout::eUndefined, vk::ImageLayout::eDepthStencilAttachmentOptimal, vkRange);
            }
        }
    }

    // transfer image layout via barrier.
    layoutBarrier.cmdWrite(vkCommandBuffer);

    // start a new dynamic render pass.
    vkCommandBuffer.beginRendering(renderInfo);

    // setup viewport
    auto viewWidth  = FLT_MAX == renderTarget.viewport.width ? (float) renderArea.extent.width : renderTarget.viewport.width;
    auto viewHeight = FLT_MAX == renderTarget.viewport.height ? (float) renderArea.extent.height : renderTarget.viewport.height;
    auto viewport   = vk::Viewport(renderTarget.viewport.x, renderTarget.viewport.y, viewWidth, viewHeight, 0, 1);
    vkCommandBuffer.setViewport(0, 1, &viewport);

    // setup scissor.
    auto scissorWidth  = (~0u) == renderTarget.scissorRect.width ? (uint32_t) std::ceil(viewWidth) : renderTarget.scissorRect.width;
    auto scissorHeight = (~0u) == renderTarget.scissorRect.height ? (uint32_t) std::ceil(viewHeight) : renderTarget.scissorRect.height;
    auto scissor       = vk::Rect2D(vk::Offset2D(renderTarget.scissorRect.x, renderTarget.scissorRect.y), vk::Extent2D(scissorWidth, scissorHeight));
    vkCommandBuffer.setScissor(0, 1, &scissor);

    return true;
}

} // namespace GN::rdg