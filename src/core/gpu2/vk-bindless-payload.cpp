#include "pch.h"
#include "vk-bindless-payload.h"
#include "vk-bindless-descriptor-heap.h"
#include "vk-bindless-pso-cache.h"
#include "vk-format-utils.h"
#include "vk-buffer.h"
#include "vk-texture.h"

static GN::Logger * sLogger = GN::getLogger("GN.gpu2.vk.bindless.payload");

namespace GN::gpu2 {

VkBindlessPayload::VkBindlessPayload(const StrA & name, ConstructParameters params)
    : GpuPayloadVulkan(name), mGpu(std::move(params.gpu)), mRenderTarget(std::move(params.target)), mHeap(std::move(params.heap)),
      mHeapSetIndex(params.heapSetIndex), mPipelineLayout(params.pipelineLayout), mDraws(std::move(params.draws)),
      mImmediateData(std::move(params.immediateData)), mRetainedCleanups(std::move(params.retainedCleanups)), mPassDescriptorPool(params.passDescriptorPool),
      mPassDescriptorSets(std::move(params.passDescriptorSets)) {}

VkBindlessPayload::~VkBindlessPayload() {
    for (auto & cleanup : mRetainedCleanups) {
        if (cleanup) cleanup();
    }
    mRetainedCleanups.clear();
    if (mGpu && mGpu->ready() && mPassDescriptorPool) {
        mGpu->vulkanDevice().handle().destroyDescriptorPool(mPassDescriptorPool);
        mPassDescriptorPool = vk::DescriptorPool {};
    }
}

void VkBindlessPayload::onGpuComplete() {
    for (auto & cleanup : mRetainedCleanups) {
        if (cleanup) cleanup();
    }
    mRetainedCleanups.clear();
}

void VkBindlessPayload::recordForVulkanSubmit(const RecordContext & ctx) {
    if (!ctx.dev || ctx.cmd.empty()) return;

    vk::CommandBuffer vkcb     = ctx.cmd.handle();
    auto &            psoCache = mGpu->bindlessPsoCache();

    // 1. Pre-pass layout transitions for render targets
    if (ctx.batchTracker) {
        ctx.batchTracker->addRasterTarget(mRenderTarget);
        ctx.batchTracker->emitPrePassBarriers(vkcb);
    } else {
        // Fallback barrier when no batch tracker is active
        std::vector<vk::ImageMemoryBarrier> preBarriers;
        for (const auto & ct : mRenderTarget.colorTargets) {
            if (ct.target.texture) {
                auto * tex = RuntimeType::cast<TextureVulkanBase>(ct.target.texture.get());
                if (tex && tex->nativeImage()) {
                    vk::ImageMemoryBarrier b;
                    b.setOldLayout(vk::ImageLayout::eUndefined)
                        .setNewLayout(vk::ImageLayout::eColorAttachmentOptimal)
                        .setImage(tex->nativeImage())
                        .setSubresourceRange({vk::ImageAspectFlagBits::eColor, ct.target.mip, 1, ct.target.face, 1})
                        .setSrcAccessMask({})
                        .setDstAccessMask(vk::AccessFlagBits::eColorAttachmentWrite)
                        .setSrcQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
                        .setDstQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED);
                    preBarriers.push_back(b);
                }
            }
        }
        if (mRenderTarget.depthStencilTarget.texture) {
            auto * dTex = RuntimeType::cast<TextureVulkanBase>(mRenderTarget.depthStencilTarget.texture.get());
            if (dTex && dTex->nativeImage()) {
                vk::ImageMemoryBarrier b;
                b.setOldLayout(vk::ImageLayout::eUndefined)
                    .setNewLayout(vk::ImageLayout::eDepthAttachmentOptimal)
                    .setImage(dTex->nativeImage())
                    .setSubresourceRange({vk::ImageAspectFlagBits::eDepth, mRenderTarget.depthStencilTarget.mip, 1, mRenderTarget.depthStencilTarget.face, 1})
                    .setSrcAccessMask({})
                    .setDstAccessMask(vk::AccessFlagBits::eDepthStencilAttachmentWrite)
                    .setSrcQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
                    .setDstQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED);
                preBarriers.push_back(b);
            }
        }
        if (!preBarriers.empty()) {
            vkcb.pipelineBarrier(vk::PipelineStageFlagBits::eTopOfPipe,
                                 vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests, {}, 0, nullptr, 0, nullptr,
                                 static_cast<uint32_t>(preBarriers.size()), preBarriers.data());
        }
    }

    // 2. Resolve render target attachments and begin dynamic rendering
    vk::Extent2D ext(~0u, ~0u);
    PassFormats  formats;

    const auto &        cc = mRenderTarget.clearColor;
    vk::ClearColorValue clearCv(std::array<float, 4> {cc.f4[0], cc.f4[1], cc.f4[2], cc.f4[3]});

    std::vector<vk::RenderingAttachmentInfo> colorAtts;
    colorAtts.reserve(mRenderTarget.colorTargets.size());
    formats.colors.reserve(mRenderTarget.colorTargets.size());

    for (size_t i = 0; i < mRenderTarget.colorTargets.size(); ++i) {
        vk::Image             img {};
        vk::ImageView         view {};
        vk::Extent2D          attExt {};
        vk::Format            fmt    = vk::Format::eUndefined;
        const GpuResourceView ctView = mRenderTarget.colorTargets[i].view();
        if (!resolveColorAttachment(ctView, &img, &view, &attExt, &fmt)) { continue; }
        ext.width  = std::min(ext.width, attExt.width);
        ext.height = std::min(ext.height, attExt.height);
        formats.colors.push_back(fmt);

        vk::RenderingAttachmentInfo att;
        att.setImageView(view)
            .setImageLayout(vk::ImageLayout::eColorAttachmentOptimal)
            .setLoadOp(vk::AttachmentLoadOp::eClear)
            .setStoreOp(vk::AttachmentStoreOp::eStore)
            .setClearValue(vk::ClearValue(clearCv));
        colorAtts.push_back(att);
    }

    vk::RenderingAttachmentInfo depthAtt;
    bool                        hasDepth = false;
    vk::Image                   depthImg {};
    const GpuResourceView       dst = mRenderTarget.depthStencilTarget.view();
    if (dst.isTexture() && dst.texture()) {
        vk::Extent2D depthExtent;
        if (attachmentExtent(dst, depthExtent)) {
            auto *        depthTex  = RuntimeType::cast<TextureVulkanBase>(dst.texture().get());
            vk::ImageView depthView = depthTex ? depthTex->nativeView(dst.imageView) : vk::ImageView {};
            if (depthTex && depthView) {
                depthImg                  = depthTex->nativeImage();
                gfx::img::PixelFormat dpf = dst.imageView.format;
                if (dpf == gfx::img::PixelFormat::UNKNOWN()) dpf = depthTex->descriptor().format;
                formats.depth = pixelFormatToVkFormat(dpf);

                depthAtt.setImageView(depthView)
                    .setImageLayout(vk::ImageLayout::eDepthAttachmentOptimal)
                    .setLoadOp(vk::AttachmentLoadOp::eClear)
                    .setStoreOp(vk::AttachmentStoreOp::eStore)
                    .setClearValue(vk::ClearValue(vk::ClearDepthStencilValue(mRenderTarget.clearDepth, mRenderTarget.clearStencil)));
                ext.width  = std::min(ext.width, depthExtent.width);
                ext.height = std::min(ext.height, depthExtent.height);
                hasDepth   = true;
            }
        }
    }

    if (ext.width == ~0u || ext.height == ~0u) ext = vk::Extent2D(1, 1);

    vk::RenderingInfo ri;
    ri.setRenderArea(vk::Rect2D(vk::Offset2D(0, 0), ext)).setLayerCount(1).setColorAttachments(colorAtts);
    if (hasDepth) ri.setPDepthAttachment(&depthAtt);

    vkcb.beginRendering(ri);

    // 3. Bind global descriptor heap once at heapSetIndex
    if (mHeap && mPipelineLayout) {
        auto * vkHeap = RuntimeType::cast<VkBindlessDescriptorHeap>(mHeap.get());
        if (vkHeap && vkHeap->nativeDescriptorSet()) {
            auto heapSet = vkHeap->nativeDescriptorSet();
            vkcb.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, mPipelineLayout, mHeapSetIndex, 1, &heapSet, 0, nullptr);
        }
    }

    // Bind pass resources sets if present
    for (size_t s = 0; s < mPassDescriptorSets.size(); ++s) {
        if (s != mHeapSetIndex && mPassDescriptorSets[s]) {
            vkcb.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, mPipelineLayout, static_cast<uint32_t>(s), 1, &mPassDescriptorSets[s], 0, nullptr);
        }
    }

    // 4. Record draws directly to Vulkan command buffer
    vk::Pipeline activePipeline {};

    for (const auto & d : mDraws) {
        if (d.geometry.vertexCount == 0 && d.geometry.indexCount == 0) continue;

        auto * vsVk = RuntimeType::cast<GpuShaderVulkan>(d.vs.get());
        auto * hsVk = RuntimeType::cast<GpuShaderVulkan>(d.hs.get());
        auto * dsVk = RuntimeType::cast<GpuShaderVulkan>(d.ds.get());
        auto * gsVk = RuntimeType::cast<GpuShaderVulkan>(d.gs.get());
        auto * psVk = RuntimeType::cast<GpuShaderVulkan>(d.ps.get());
        if (!vsVk || !vsVk->rvShader()) continue;

        // Viewport and Scissor
        vk::Viewport vp =
            d.mergedState.viewport ? rsViewportToVk(*d.mergedState.viewport, ext) : vk::Viewport(0.0f, 0.0f, (float) ext.width, (float) ext.height, 0.0f, 1.0f);
        vkcb.setViewport(0, 1, &vp);

        vk::Rect2D sc = d.mergedState.scissorRect ? rsScissorToVk(*d.mergedState.scissorRect, ext) : vk::Rect2D(vk::Offset2D(0, 0), ext);
        vkcb.setScissor(0, 1, &sc);

        // Get-or-create graphics pipeline
        vk::Pipeline pipe = psoCache.getOrCreate(mPipelineLayout, vsVk, hsVk, dsVk, gsVk, psVk, d.mergedState, d.geometry, formats, mRenderTarget.colorTargets);
        if (!pipe) continue;

        if (pipe != activePipeline) {
            vkcb.bindPipeline(vk::PipelineBindPoint::eGraphics, pipe);
            activePipeline = pipe;
        }

        // Push constants / immediates
        if (d.immediateSize > 0 && mPipelineLayout && (d.immediateOffset + d.immediateSize <= mImmediateData.size())) {
            vkcb.pushConstants(mPipelineLayout, vk::ShaderStageFlagBits::eAllGraphics, 0, d.immediateSize, mImmediateData.data() + d.immediateOffset);
        }

        // Vertex buffer bindings
        if (!d.geometry.vertices.empty()) {
            std::vector<vk::Buffer>     vkBuffers;
            std::vector<vk::DeviceSize> vkOffsets;
            vkBuffers.reserve(d.geometry.vertices.size());
            vkOffsets.reserve(d.geometry.vertices.size());
            for (const auto & vb : d.geometry.vertices) {
                auto * bVk = RuntimeType::cast<BufferVulkan>(vb.buffer.get());
                vkBuffers.push_back(bVk ? bVk->nativeBuffer() : vk::Buffer {});
                vkOffsets.push_back(vb.offset);
            }
            vkcb.bindVertexBuffers(0, static_cast<uint32_t>(vkBuffers.size()), vkBuffers.data(), vkOffsets.data());
        }

        // Draw indexed or non-indexed
        if (d.geometry.indexCount > 0 && d.geometry.indices.buffer) {
            auto * ibVk = RuntimeType::cast<BufferVulkan>(d.geometry.indices.buffer.get());
            if (ibVk && ibVk->nativeBuffer()) {
                vk::IndexType it = (d.geometry.indices.stride == 4) ? vk::IndexType::eUint32 : vk::IndexType::eUint16;
                vkcb.bindIndexBuffer(ibVk->nativeBuffer(), d.geometry.indices.offset, it);
                vkcb.drawIndexed(d.geometry.indexCount, 1, 0, 0, 0);
            }
        } else if (d.geometry.vertexCount > 0) {
            vkcb.draw(d.geometry.vertexCount, 1, 0, 0);
        }
    }

    vkcb.endRendering();

    // 5. Automated invariant: restore attachments to SHADER_READ_ONLY_OPTIMAL with pipeline barrier
    for (const auto & ct : mRenderTarget.colorTargets) {
        if (!ct.target.texture) continue;
        auto * tex = RuntimeType::cast<TextureVulkanBase>(ct.target.texture.get());
        if (!tex || !tex->nativeImage()) continue;

        if (ctx.batchTracker) {
            ctx.batchTracker->restoreAttachmentToShaderReadOnly(tex, ct.view(), vkcb);
        } else {
            vk::ImageMemoryBarrier b;
            b.setOldLayout(vk::ImageLayout::eColorAttachmentOptimal)
                .setNewLayout(vk::ImageLayout::eShaderReadOnlyOptimal)
                .setImage(tex->nativeImage())
                .setSubresourceRange({vk::ImageAspectFlagBits::eColor, ct.target.mip, 1, ct.target.face, 1})
                .setSrcAccessMask(vk::AccessFlagBits::eColorAttachmentWrite)
                .setDstAccessMask(vk::AccessFlagBits::eShaderRead)
                .setSrcQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
                .setDstQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED);
            vkcb.pipelineBarrier(vk::PipelineStageFlagBits::eColorAttachmentOutput,
                                 vk::PipelineStageFlagBits::eFragmentShader | vk::PipelineStageFlagBits::eComputeShader, {}, 0, nullptr, 0, nullptr, 1, &b);
        }
    }

    if (hasDepth && depthImg) {
        auto * dTex = RuntimeType::cast<TextureVulkanBase>(mRenderTarget.depthStencilTarget.texture.get());
        if (dTex) {
            if (ctx.batchTracker) {
                ctx.batchTracker->restoreAttachmentToShaderReadOnly(dTex, mRenderTarget.depthStencilTarget.view(), vkcb);
            } else {
                vk::ImageMemoryBarrier b;
                b.setOldLayout(vk::ImageLayout::eDepthAttachmentOptimal)
                    .setNewLayout(vk::ImageLayout::eShaderReadOnlyOptimal)
                    .setImage(depthImg)
                    .setSubresourceRange({vk::ImageAspectFlagBits::eDepth, mRenderTarget.depthStencilTarget.mip, 1, mRenderTarget.depthStencilTarget.face, 1})
                    .setSrcAccessMask(vk::AccessFlagBits::eDepthStencilAttachmentWrite)
                    .setDstAccessMask(vk::AccessFlagBits::eShaderRead)
                    .setSrcQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
                    .setDstQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED);
                vkcb.pipelineBarrier(vk::PipelineStageFlagBits::eLateFragmentTests,
                                     vk::PipelineStageFlagBits::eFragmentShader | vk::PipelineStageFlagBits::eComputeShader, {}, 0, nullptr, 0, nullptr, 1, &b);
            }
        }
    }
}

} // namespace GN::gpu2
