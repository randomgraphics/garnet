#include "pch.h"
#include "vk-bindless-cnc-payload.h"
#include "vk-bindless-descriptor-heap.h"
#include "vk-bindless-compute-pso-cache.h"
#include "vk-buffer.h"
#include "vk-texture.h"

static GN::Logger * sLogger = GN::getLogger("GN.gpu2.vk.bindless.cnc.payload");

namespace GN::gpu2 {

VkBindlessCncPayload::VkBindlessCncPayload(const StrA & name, ConstructParameters params)
    : GpuPayloadVulkan(name), mUploadStorage(std::move(params.uploadStorage)), mGpu(std::move(params.gpu)), mHeap(std::move(params.heap)),
      mHeapSetIndex(params.heapSetIndex), mPipelineLayout(params.pipelineLayout), mOps(std::move(params.ops)), mImmediateData(std::move(params.immediateData)),
      mRetainedCleanups(std::move(params.retainedCleanups)), mPassDescriptorPool(params.passDescriptorPool),
      mPassDescriptorSets(std::move(params.passDescriptorSets)), mPassResources(std::move(params.passResources)) {}

VkBindlessCncPayload::~VkBindlessCncPayload() {
    for (auto & cleanup : mRetainedCleanups) {
        if (cleanup) cleanup();
    }
    mRetainedCleanups.clear();

    if (mGpu && mGpu->ready() && mPassDescriptorPool) {
        mGpu->vulkanDevice().handle().destroyDescriptorPool(mPassDescriptorPool);
        mPassDescriptorPool = vk::DescriptorPool {};
    }
}

void VkBindlessCncPayload::onGpuComplete() {
    for (auto & op : mOps) {
        std::visit(
            [&](auto & o) {
                using T = std::decay_t<decltype(o)>;
                if constexpr (std::is_same_v<T, StoredUploadBuffer>) {
                    o.staging.clear();
                } else if constexpr (std::is_same_v<T, StoredBufferToImage>) {
                    o.src.clear();
                } else if constexpr (std::is_same_v<T, StoredDownloadBuffer>) {
                    resolveDownloadBuffer(o);
                } else if constexpr (std::is_same_v<T, StoredDownloadImage>) {
                    resolveDownloadImage(o);
                }
            },
            op);
    }

    for (auto & cleanup : mRetainedCleanups) {
        if (cleanup) cleanup();
    }
    mRetainedCleanups.clear();
}

void VkBindlessCncPayload::recordForVulkanSubmit(const RecordContext & ctx) {
    mUploadStorage->unmapForSubmit();
    if (!ctx.dev || ctx.cmd.empty()) return;

    vk::CommandBuffer vkcb = ctx.cmd.handle();

    // 1. Register pass resources with state tracker if present
    if (ctx.batchTracker) {
        auto & tracker = *ctx.batchTracker;
        // Uploads to the built-in material SSBO must be visible before compute reads.
        if (auto * heap = RuntimeType::cast<VkBindlessDescriptorHeap>(mHeap.get())) {
            tracker.addStorageBuffer(RuntimeType::cast<BufferVulkan>(heap->materialBuffer().get()), false, vk::PipelineStageFlagBits::eComputeShader);
        }
        for (size_t setIdx = 0; setIdx < mPassResources.size(); ++setIdx) {
            const auto & set = mPassResources[setIdx];
            for (size_t bindIdx = 0; bindIdx < set.size(); ++bindIdx) {
                const auto & slot = set[bindIdx];
                for (const auto & view : slot) {
                    if (view.empty()) continue;
                    if (view.isTexture()) {
                        auto * tex = RuntimeType::cast<TextureVulkanBase>(view.texture().get());
                        if (!tex) continue;
                        if (view.imageView.type == GpuResourceView::ImageView::STORAGE) {
                            tracker.addStorageTexture(tex, view, vk::PipelineStageFlagBits::eComputeShader);
                        } else {
                            tracker.addSampledTexture(tex, view, vk::PipelineStageFlagBits::eComputeShader);
                        }
                    } else if (view.isBuffer()) {
                        auto * buf = RuntimeType::cast<BufferVulkan>(view.buffer().get());
                        if (!buf) continue;
                        if (view.bufferView.type == GpuResourceView::BufferView::STORAGE) {
                            tracker.addStorageBuffer(buf, /*write=*/true, vk::PipelineStageFlagBits::eComputeShader);
                        } else {
                            tracker.addUniformBuffer(buf, vk::PipelineStageFlagBits::eComputeShader);
                        }
                    }
                }
            }
        }
        tracker.emitPrePassBarriers(vkcb);
    }

    // 2. Bind descriptor heap once for compute
    if (mHeap && mPipelineLayout) {
        auto * vkHeap = RuntimeType::cast<VkBindlessDescriptorHeap>(mHeap.get());
        if (vkHeap && vkHeap->nativeDescriptorSet()) {
            auto heapSet = vkHeap->nativeDescriptorSet();
            vkcb.bindDescriptorSets(vk::PipelineBindPoint::eCompute, mPipelineLayout, mHeapSetIndex, 1, &heapSet, 0, nullptr);
        }
    }

    // 3. Bind pass descriptor sets if present
    for (size_t s = 0; s < mPassDescriptorSets.size(); ++s) {
        if (s != mHeapSetIndex && mPassDescriptorSets[s]) {
            vkcb.bindDescriptorSets(vk::PipelineBindPoint::eCompute, mPipelineLayout, static_cast<uint32_t>(s), 1, &mPassDescriptorSets[s], 0, nullptr);
        }
    }

    // 4. Record operations directly to Vulkan command buffer
    vk::Pipeline                                                 activePipeline {};
    bool                                                         hasComputeWrite  = false;
    bool                                                         hasTransferWrite = false;
    std::vector<BufferVulkan *>                                  writtenBuffers;
    std::vector<std::pair<TextureVulkanBase *, GpuResourceView>> writtenTextures;

    auto addWrittenBuffer = [&](BufferVulkan * b) {
        if (b && std::find(writtenBuffers.begin(), writtenBuffers.end(), b) == writtenBuffers.end()) { writtenBuffers.push_back(b); }
    };
    auto addWrittenTexture = [&](TextureVulkanBase * t, const GpuResourceView & v) {
        if (!t) return;
        for (const auto & [existingT, existingV] : writtenTextures) {
            if (existingT == t) return;
        }
        writtenTextures.push_back({t, v});
    };

    for (const auto & op : mOps) {
        std::visit(
            [&](const auto & o) {
                using T = std::decay_t<decltype(o)>;
                if constexpr (std::is_same_v<T, StoredBindlessCompute>) {
                    auto * csVk = RuntimeType::cast<GpuShaderVulkan>(o.cs.get());
                    if (!csVk || !csVk->rvShader()) return;

                    if (hasTransferWrite && ctx.batchTracker) {
                        ctx.batchTracker->restoreBuffersToReadReady(writtenBuffers, vkcb);
                        for (const auto & [t, v] : writtenTextures) { ctx.batchTracker->restoreAttachmentToShaderReadOnly(t, v, vkcb); }
                        hasTransferWrite = false;
                    }

                    if (hasComputeWrite) {
                        vk::MemoryBarrier mb;
                        mb.setSrcAccessMask(vk::AccessFlagBits::eShaderWrite)
                            .setDstAccessMask(vk::AccessFlagBits::eShaderRead | vk::AccessFlagBits::eShaderWrite);
                        vkcb.pipelineBarrier(vk::PipelineStageFlagBits::eComputeShader, vk::PipelineStageFlagBits::eComputeShader, {}, 1, &mb, 0, nullptr, 0,
                                             nullptr);
                    }

                    vk::Pipeline pipe = mGpu->bindlessComputePsoCache().getOrCreate(mPipelineLayout, csVk);
                    if (!pipe) return;

                    if (pipe != activePipeline) {
                        vkcb.bindPipeline(vk::PipelineBindPoint::eCompute, pipe);
                        activePipeline = pipe;
                    }

                    if (o.immediateSize > 0 && mPipelineLayout && (o.immediateOffset + o.immediateSize <= mImmediateData.size())) {
                        vkcb.pushConstants(mPipelineLayout, vk::ShaderStageFlagBits::eCompute, 0, o.immediateSize, mImmediateData.data() + o.immediateOffset);
                    }

                    vkcb.dispatch(o.x, o.y, o.z);
                    hasComputeWrite = true;
                } else if (ctx.batchTracker) {
                    auto & tracker = *ctx.batchTracker;
                    if (hasComputeWrite) {
                        vk::MemoryBarrier mb;
                        mb.setSrcAccessMask(vk::AccessFlagBits::eShaderWrite)
                            .setDstAccessMask(vk::AccessFlagBits::eTransferRead | vk::AccessFlagBits::eTransferWrite);
                        vkcb.pipelineBarrier(vk::PipelineStageFlagBits::eComputeShader, vk::PipelineStageFlagBits::eTransfer, {}, 1, &mb, 0, nullptr, 0,
                                             nullptr);
                        hasComputeWrite = false;
                    }

                    if constexpr (std::is_same_v<T, StoredBufferToBuffer>) {
                        auto * srcVk = RuntimeType::cast<BufferVulkan>(o.src.get());
                        auto * dstVk = RuntimeType::cast<BufferVulkan>(o.dst.get());
                        if (srcVk && srcVk == dstVk) GN_UNLIKELY {
                                GN_ERROR(sLogger, "VkBindlessCncPayload: copyBufferToBuffer: src and dst are the same buffer");
                                return;
                            }
                        emitBufferCopy(srcVk, dstVk, o.srcOffset, o.dstOffset, o.size, vkcb, tracker);
                        addWrittenBuffer(dstVk);
                        addWrittenBuffer(srcVk);
                        hasTransferWrite = true;
                    } else if constexpr (std::is_same_v<T, StoredBufferToImage>) {
                        recordBufToImg(o, vkcb, tracker);
                        auto * dstVk = RuntimeType::cast<TextureVulkanBase>(o.dst.get());
                        addWrittenTexture(dstVk, GpuResourceView {});
                        hasTransferWrite = true;
                    } else if constexpr (std::is_same_v<T, StoredImageToImage>) {
                        recordImageCopy(o, vkcb, tracker);
                        addWrittenTexture(RuntimeType::cast<TextureVulkanBase>(o.src.get()), GpuResourceView {});
                        addWrittenTexture(RuntimeType::cast<TextureVulkanBase>(o.dst.get()), GpuResourceView {});
                        hasTransferWrite = true;
                    } else if constexpr (std::is_same_v<T, StoredUploadBuffer>) {
                        auto * dstVk = RuntimeType::cast<BufferVulkan>(o.dst.get());
                        emitBufferCopy(RuntimeType::cast<BufferVulkan>(o.staging.get()), dstVk, o.srcOffset, o.dstOffset, o.size, vkcb, tracker);
                        addWrittenBuffer(dstVk);
                        hasTransferWrite = true;
                    } else if constexpr (std::is_same_v<T, StoredDownloadBuffer>) {
                        emitBufferCopy(RuntimeType::cast<BufferVulkan>(o.src.get()), RuntimeType::cast<BufferVulkan>(o.staging.get()), o.srcOffset, 0, o.size,
                                       vkcb, tracker);
                        addWrittenBuffer(RuntimeType::cast<BufferVulkan>(o.src.get()));
                    } else if constexpr (std::is_same_v<T, StoredDownloadImage>) {
                        recordDownloadImage(o, vkcb, tracker);
                        auto * srcVk = RuntimeType::cast<TextureVulkanBase>(o.src.get());
                        addWrittenTexture(srcVk, GpuResourceView {});
                        hasTransferWrite = true;
                    }
                }
            },
            op);
    }

    // 5. Automated invariant: restore written resources back to fully readable state
    if (hasComputeWrite) {
        for (size_t setIdx = 0; setIdx < mPassResources.size(); ++setIdx) {
            const auto & set = mPassResources[setIdx];
            for (size_t bindIdx = 0; bindIdx < set.size(); ++bindIdx) {
                const auto & slot = set[bindIdx];
                for (const auto & view : slot) {
                    if (view.empty()) continue;
                    if (view.isBuffer() && view.bufferView.type == GpuResourceView::BufferView::STORAGE) {
                        auto * buf = RuntimeType::cast<BufferVulkan>(view.buffer().get());
                        addWrittenBuffer(buf);
                    } else if (view.isTexture() && view.imageView.type == GpuResourceView::ImageView::STORAGE) {
                        auto * tex = RuntimeType::cast<TextureVulkanBase>(view.texture().get());
                        addWrittenTexture(tex, view);
                    }
                }
            }
        }
    }

    if (ctx.batchTracker) {
        auto & tracker = *ctx.batchTracker;
        if (!writtenBuffers.empty()) { tracker.restoreBuffersToReadReady(writtenBuffers, vkcb); }
        for (const auto & [tex, view] : writtenTextures) { tracker.restoreAttachmentToShaderReadOnly(tex, view, vkcb); }
    }

    if (hasComputeWrite) {
        const auto        readReady = BufferStateVulkan::READ_READY();
        vk::MemoryBarrier mb;
        mb.setSrcAccessMask(vk::AccessFlagBits::eShaderWrite).setDstAccessMask(readReady.access);
        vkcb.pipelineBarrier(vk::PipelineStageFlagBits::eComputeShader, readReady.stages, {}, 1, &mb, 0, nullptr, 0, nullptr);
    }
}

} // namespace GN::gpu2
