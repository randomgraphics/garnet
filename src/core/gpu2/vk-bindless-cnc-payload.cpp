#include "pch.h"
#include "vk-bindless-cnc-payload.h"
#include "vk-bindless-descriptor-heap.h"
#include "vk-bindless-compute-pso-cache.h"
#include "vk-buffer.h"
#include "vk-texture.h"

static GN::Logger * sLogger = GN::getLogger("GN.gpu2.vk.bindless.cnc.payload");

namespace GN::gpu2 {

VkBindlessCncPayload::VkBindlessCncPayload(const StrA & name, ConstructParameters params)
    : GpuPayloadVulkan(name), mGpu(std::move(params.gpu)), mHeap(std::move(params.heap)),
      mHeapSetIndex(params.heapSetIndex), mPipelineLayout(params.pipelineLayout),
      mOps(std::move(params.ops)), mImmediateData(std::move(params.immediateData)),
      mRetainedCleanups(std::move(params.retainedCleanups)),
      mPassDescriptorPool(params.passDescriptorPool),
      mPassDescriptorSets(std::move(params.passDescriptorSets)),
      mPassResources(std::move(params.passResources)) {}

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
    if (!ctx.dev || ctx.cmd.empty()) return;

    vk::CommandBuffer vkcb = ctx.cmd.handle();

    // 1. Register pass resources with state tracker if present
    if (ctx.batchTracker) {
        auto & tracker = *ctx.batchTracker;
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
    vk::Pipeline activePipeline {};
    bool         hasComputeWrite = false;

    for (const auto & op : mOps) {
        std::visit(
            [&](const auto & o) {
                using T = std::decay_t<decltype(o)>;
                if constexpr (std::is_same_v<T, StoredBindlessCompute>) {
                    auto * csVk = RuntimeType::cast<GpuShaderVulkan>(o.cs.get());
                    if (!csVk || !csVk->rvShader()) return;

                    if (hasComputeWrite) {
                        vk::MemoryBarrier mb;
                        mb.setSrcAccessMask(vk::AccessFlagBits::eShaderWrite)
                          .setDstAccessMask(vk::AccessFlagBits::eShaderRead | vk::AccessFlagBits::eShaderWrite);
                        vkcb.pipelineBarrier(vk::PipelineStageFlagBits::eComputeShader, vk::PipelineStageFlagBits::eComputeShader,
                                             {}, 1, &mb, 0, nullptr, 0, nullptr);
                    }

                    vk::Pipeline pipe = mGpu->bindlessComputePsoCache().getOrCreate(mPipelineLayout, csVk);
                    if (!pipe) return;

                    if (pipe != activePipeline) {
                        vkcb.bindPipeline(vk::PipelineBindPoint::eCompute, pipe);
                        activePipeline = pipe;
                    }

                    if (o.immediateSize > 0 && mPipelineLayout && (o.immediateOffset + o.immediateSize <= mImmediateData.size())) {
                        vkcb.pushConstants(mPipelineLayout, vk::ShaderStageFlagBits::eCompute, 0, o.immediateSize,
                                           mImmediateData.data() + o.immediateOffset);
                    }

                    vkcb.dispatch(o.x, o.y, o.z);
                    hasComputeWrite = true;
                } else if (ctx.batchTracker) {
                    auto & tracker = *ctx.batchTracker;
                    if (hasComputeWrite) {
                        vk::MemoryBarrier mb;
                        mb.setSrcAccessMask(vk::AccessFlagBits::eShaderWrite)
                          .setDstAccessMask(vk::AccessFlagBits::eTransferRead | vk::AccessFlagBits::eTransferWrite);
                        vkcb.pipelineBarrier(vk::PipelineStageFlagBits::eComputeShader, vk::PipelineStageFlagBits::eTransfer,
                                             {}, 1, &mb, 0, nullptr, 0, nullptr);
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
                    } else if constexpr (std::is_same_v<T, StoredBufferToImage>) {
                        recordBufToImg(o, vkcb, tracker);
                    } else if constexpr (std::is_same_v<T, StoredUploadBuffer>) {
                        emitBufferCopy(RuntimeType::cast<BufferVulkan>(o.staging.get()), RuntimeType::cast<BufferVulkan>(o.dst.get()), 0, o.dstOffset, o.size, vkcb, tracker);
                    } else if constexpr (std::is_same_v<T, StoredDownloadBuffer>) {
                        emitBufferCopy(RuntimeType::cast<BufferVulkan>(o.src.get()), RuntimeType::cast<BufferVulkan>(o.staging.get()), o.srcOffset, 0, o.size, vkcb, tracker);
                    } else if constexpr (std::is_same_v<T, StoredDownloadImage>) {
                        recordDownloadImage(o, vkcb, tracker);
                    }
                }
            },
            op);
    }
}

} // namespace GN::gpu2
