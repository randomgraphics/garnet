#include "pch.h"
#include "vk-bindless-cnc.h"
#include "vk-bindless-cnc-payload.h"
#include "vk-bindless-descriptor-heap.h"
#include "vk-bindless-pipeline-layout.h"
#include "vk-sampler.h"

static GN::Logger * sLogger = GN::getLogger("GN.gpu2.vk.bindless.cnc");

namespace GN::gpu2 {

bool buildBindlessPassDescriptorSets(GpuContextVulkan2 & gpu, const GpuResourceTable & passResources, uint32_t heapSetIndex, vk::ShaderStageFlags stageFlags,
                                     vk::DescriptorPool & outPool, std::vector<vk::DescriptorSet> & outSets) {
    if (passResources.empty()) return true;

    auto vkDev = gpu.vulkanDevice().handle();

    uint32_t numCombinedSamplers = 0;
    uint32_t numStorageImages    = 0;
    uint32_t numUniformBuffers   = 0;
    uint32_t numStorageBuffers   = 0;

    for (size_t s = 0; s < passResources.size(); ++s) {
        if (s == heapSetIndex || passResources[s].empty()) continue;
        for (size_t b = 0; b < passResources[s].size(); ++b) {
            const auto & slot = passResources[s][b];
            for (const auto & v : slot) {
                if (v.empty()) continue;
                if (v.isTexture()) {
                    if (v.imageView.type == GpuResourceView::ImageView::STORAGE) {
                        numStorageImages++;
                    } else {
                        numCombinedSamplers++;
                    }
                } else if (v.isBuffer()) {
                    if (v.bufferView.type == GpuResourceView::BufferView::STORAGE) {
                        numStorageBuffers++;
                    } else {
                        numUniformBuffers++;
                    }
                }
            }
        }
    }

    if (numCombinedSamplers == 0 && numStorageImages == 0 && numUniformBuffers == 0 && numStorageBuffers == 0) { return true; }

    std::vector<vk::DescriptorPoolSize> poolSizes;
    if (numCombinedSamplers > 0) poolSizes.push_back({vk::DescriptorType::eCombinedImageSampler, numCombinedSamplers});
    if (numStorageImages > 0) poolSizes.push_back({vk::DescriptorType::eStorageImage, numStorageImages});
    if (numUniformBuffers > 0) poolSizes.push_back({vk::DescriptorType::eUniformBuffer, numUniformBuffers});
    if (numStorageBuffers > 0) poolSizes.push_back({vk::DescriptorType::eStorageBuffer, numStorageBuffers});

    vk::DescriptorPoolCreateInfo poolInfo;
    poolInfo.setMaxSets(static_cast<uint32_t>(passResources.size())).setPoolSizes(poolSizes);

    try {
        outPool = vkDev.createDescriptorPool(poolInfo);
    } catch (const std::exception & e) {
        GN_ERROR(sLogger, "buildPassDescriptorSets: createDescriptorPool failed: {}", e.what());
        return false;
    }

    outSets.resize(passResources.size());

    for (size_t s = 0; s < passResources.size(); ++s) {
        if (s == heapSetIndex || passResources[s].empty()) continue;

        std::vector<vk::DescriptorSetLayoutBinding> bindings;
        for (size_t b = 0; b < passResources[s].size(); ++b) {
            const auto & slot = passResources[s][b];
            if (slot.empty()) continue;

            vk::DescriptorSetLayoutBinding bind;
            bind.setBinding(static_cast<uint32_t>(b)).setDescriptorCount(static_cast<uint32_t>(slot.size())).setStageFlags(stageFlags);

            if (slot[0].isTexture()) {
                bind.setDescriptorType(slot[0].imageView.type == GpuResourceView::ImageView::STORAGE ? vk::DescriptorType::eStorageImage
                                                                                                     : vk::DescriptorType::eCombinedImageSampler);
            } else if (slot[0].isBuffer()) {
                bind.setDescriptorType(slot[0].bufferView.type == GpuResourceView::BufferView::STORAGE ? vk::DescriptorType::eStorageBuffer
                                                                                                       : vk::DescriptorType::eUniformBuffer);
            }
            bindings.push_back(bind);
        }

        if (bindings.empty()) continue;

        vk::DescriptorSetLayoutCreateInfo layoutInfo;
        layoutInfo.setBindings(bindings);
        vk::DescriptorSetLayout setlayout = vkDev.createDescriptorSetLayout(layoutInfo);

        vk::DescriptorSetAllocateInfo allocInfo;
        allocInfo.setDescriptorPool(outPool).setSetLayouts(setlayout);
        vk::DescriptorSet descSet = vkDev.allocateDescriptorSets(allocInfo)[0];
        outSets[s]                = descSet;

        // Populate writes
        std::vector<vk::WriteDescriptorSet>                writes;
        std::vector<std::vector<vk::DescriptorImageInfo>>  imageInfos;
        std::vector<std::vector<vk::DescriptorBufferInfo>> bufferInfos;

        for (size_t b = 0; b < passResources[s].size(); ++b) {
            const auto & slot = passResources[s][b];
            if (slot.empty()) continue;

            if (slot[0].isTexture()) {
                auto & curImageInfos = imageInfos.emplace_back();
                for (const auto & v : slot) {
                    auto * tex = RuntimeType::cast<TextureVulkanBase>(v.texture().get());
                    if (!tex) continue;
                    vk::DescriptorImageInfo ii;
                    ii.imageView = tex->nativeView(v.imageView);
                    if (v.imageView.type == GpuResourceView::ImageView::STORAGE) {
                        ii.imageLayout = vk::ImageLayout::eGeneral;
                    } else {
                        ii.imageLayout = shaderReadOnlyLayout(tex->descriptor().format);
                        if (v.combinedTextureSampler) {
                            auto * samp = RuntimeType::cast<SamplerVulkan>(v.combinedTextureSampler.get());
                            ii.sampler  = samp ? samp->nativeSampler() : gpu.defaultLinearSampler();
                        } else {
                            ii.sampler = gpu.defaultLinearSampler();
                        }
                    }
                    curImageInfos.push_back(ii);
                }
                vk::WriteDescriptorSet w;
                w.setDstSet(descSet)
                    .setDstBinding(static_cast<uint32_t>(b))
                    .setDescriptorType(slot[0].imageView.type == GpuResourceView::ImageView::STORAGE ? vk::DescriptorType::eStorageImage
                                                                                                     : vk::DescriptorType::eCombinedImageSampler)
                    .setImageInfo(curImageInfos);
                writes.push_back(w);
            } else if (slot[0].isBuffer()) {
                auto & curBufferInfos = bufferInfos.emplace_back();
                for (const auto & v : slot) {
                    auto * buf = RuntimeType::cast<BufferVulkan>(v.buffer().get());
                    if (!buf) continue;
                    vk::DescriptorBufferInfo bi;
                    bi.buffer = buf->nativeBuffer();
                    bi.offset = v.bufferView.offset;
                    bi.range  = v.bufferView.size ? v.bufferView.size : VK_WHOLE_SIZE;
                    curBufferInfos.push_back(bi);
                }
                vk::WriteDescriptorSet w;
                w.setDstSet(descSet)
                    .setDstBinding(static_cast<uint32_t>(b))
                    .setDescriptorType(slot[0].bufferView.type == GpuResourceView::BufferView::STORAGE ? vk::DescriptorType::eStorageBuffer
                                                                                                       : vk::DescriptorType::eUniformBuffer)
                    .setBufferInfo(curBufferInfos);
                writes.push_back(w);
            }
        }

        if (!writes.empty()) { vkDev.updateDescriptorSets(writes, {}); }

        vkDev.destroyDescriptorSetLayout(setlayout);
    }

    return true;
}

// The helper is shared with bindless raster creation so both pass types bind identical resources.

VkBindlessCnC::VkBindlessCnC(const StrA & name, AutoRef<GpuContextVulkan2> gpu, AutoRef<bindless::DescriptorHeap> heap, uint32_t heapSetIndex,
                             vk::PipelineLayout pipelineLayout, vk::DescriptorPool passPool, std::vector<vk::DescriptorSet> passSets,
                             GpuResourceTable passResources, size_t opCountHint)
    : bindless::CnC(TYPE_INFO(), name), mGpu(std::move(gpu)), mHeap(std::move(heap)), mHeapSetIndex(heapSetIndex), mPipelineLayout(pipelineLayout),
      mPassDescriptorPool(passPool), mPassDescriptorSets(std::move(passSets)), mPassResources(std::move(passResources)) {
    mOps.reserve(opCountHint);
}

VkBindlessCnC::~VkBindlessCnC() {
    if (!mSealed && mPassDescriptorPool && mGpu && mGpu->ready()) {
        mGpu->vulkanDevice().handle().destroyDescriptorPool(mPassDescriptorPool);
        mPassDescriptorPool = vk::DescriptorPool {};
    }
}

void VkBindlessCnC::recordCompute(const ComputeParameters & params) {
    if (mSealed) GN_UNLIKELY {
            GN_ERROR(sLogger, "VkBindlessCnC::recordCompute: cannot record compute into a sealed CnC recorder");
            return;
        }
    if (!params.cs) GN_UNLIKELY {
            GN_ERROR(sLogger, "VkBindlessCnC::recordCompute: compute shader is required");
            return;
        }

    StoredBindlessCompute op;
    op.cs = params.cs;
    op.x  = params.x;
    op.y  = params.y;
    op.z  = params.z;

    if (!params.immediates.empty()) {
        op.immediateOffset = static_cast<uint32_t>(mImmediateData.size());
        op.immediateSize   = static_cast<uint32_t>(params.immediates.size());
        mImmediateData.insert(mImmediateData.end(), params.immediates.begin(), params.immediates.end());
    }

    mOps.emplace_back(std::move(op));
}

void VkBindlessCnC::recordCopyBuffer(const BufferToBuffer & p) {
    if (mSealed) GN_UNLIKELY {
            GN_ERROR(sLogger, "VkBindlessCnC::recordCopyBuffer: already sealed");
            return;
        }
    mOps.emplace_back(StoredBufferToBuffer {p.src, p.dst, p.srcOffset, p.dstOffset, p.size});
}

void VkBindlessCnC::recordUploadBuffer(AutoRef<Buffer> dst, uint64_t offset, ArrayView<const uint8_t> content) {
    if (mSealed) GN_UNLIKELY {
            GN_ERROR(sLogger, "VkBindlessCnC::recordUploadBuffer: already sealed");
            return;
        }
    if (!dst || content.empty()) GN_UNLIKELY {
            GN_ERROR(sLogger, "VkBindlessCnC::recordUploadBuffer: null destination or empty content");
            return;
        }
    const uint64_t size  = content.size();
    auto           slice = mUploadStorage->copy(mGpu, name, content);
    if (!slice.buffer) GN_UNLIKELY {
            GN_ERROR(sLogger, "CNC::recordUploadBuffer: staging allocation failed");
            return;
        }
    StoredUploadBuffer op;
    op.staging   = std::move(slice.buffer);
    op.dst       = std::move(dst);
    op.dstOffset = offset;
    op.size      = size;
    op.srcOffset = slice.offset;
    mOps.emplace_back(std::move(op));
}

std::future<AutoRef<const Blob>> VkBindlessCnC::recordDownloadBuffer(AutoRef<Buffer> src, uint64_t offset, uint64_t size) {
    DownloadResult<AutoRef<const Blob>> result;
    auto                                future = result.future();

    auto * srcVk = RuntimeType::cast<BufferVulkan>(src.get());
    if (mSealed || !srcVk) GN_UNLIKELY {
            GN_ERROR(sLogger, "VkBindlessCnC::recordDownloadBuffer: {}", mSealed ? "already sealed" : "null/invalid source buffer");
            return future;
        }

    const uint64_t bufSize = srcVk->bufferSize();
    if (offset > bufSize) GN_UNLIKELY {
            GN_ERROR(sLogger, "VkBindlessCnC::recordDownloadBuffer: offset {} exceeds buffer size {}", offset, bufSize);
            return future;
        }
    if (size == uint64_t(~0)) size = bufSize - offset;
    if (offset + size > bufSize) GN_UNLIKELY {
            GN_ERROR(sLogger, "VkBindlessCnC::recordDownloadBuffer: range [{}, {}) exceeds buffer size {}", offset, offset + size, bufSize);
            return future;
        }
    if (size == 0) return future;

    auto staging = Buffer::create(name + "/download_stg", {.context = mGpu, .size = size, .mappable = true});
    if (!staging) GN_UNLIKELY {
            GN_ERROR(sLogger, "VkBindlessCnC::recordDownloadBuffer: staging buffer allocation failed");
            return future;
        }

    StoredDownloadBuffer op;
    op.src       = std::move(src);
    op.staging   = std::move(staging);
    op.srcOffset = offset;
    op.size      = size;
    op.result    = std::move(result);
    mOps.emplace_back(std::move(op));
    return future;
}

void VkBindlessCnC::recordUploadImage(AutoRef<Texture> dst, ArrayView<const uint8_t> content, ArrayView<const Region> regions) {
    if (mSealed || !validImageUpload(dst.get(), content, regions)) GN_UNLIKELY {
            GN_ERROR(sLogger, "CNC::recordUploadImage: sealed recorder or empty upload");
            return;
        }
    StoredBufferToImage op;
    op.dst = dst;
    for (auto region : regions) {
        auto slice = mUploadStorage->copyImage(mGpu, name, content, dst->descriptor(), region);
        if (!slice.buffer) GN_UNLIKELY {
                GN_ERROR(sLogger, "CNC::recordUploadImage: staging allocation failed");
                return;
            }
        if (op.src && op.src.get() != slice.buffer.get()) {
            mOps.emplace_back(std::move(op));
            op     = StoredBufferToImage {};
            op.dst = dst;
        }
        op.src                 = std::move(slice.buffer);
        region.dataOffset      = slice.offset;
        region.rowPitchBytes   = 0;
        region.slicePitchBytes = 0;
        op.regions.append(region);
    }
    mOps.emplace_back(std::move(op));
}

void VkBindlessCnC::recordCopyImage(const ImageToImage & p) {
    if (mSealed || !validImageCopy(p)) GN_UNLIKELY {
            GN_ERROR(sLogger, "CNC::recordCopyImage: sealed recorder or invalid copy resources");
            return;
        }
    StoredImageToImage op;
    op.src = p.src;
    op.dst = p.dst;
    for (const auto & r : p.regions) op.regions.append(r);
    mOps.emplace_back(std::move(op));
}

std::future<GpuCnC::TextureContent> VkBindlessCnC::recordDownloadImage(AutoRef<Texture> src, ArrayView<const Region> regions) {
    DownloadResult<GpuCnC::TextureContent> result;
    auto                                   future = result.future();

    auto * srcVk = RuntimeType::cast<TextureVulkanBase>(src.get());
    if (mSealed || !srcVk || regions.empty()) GN_UNLIKELY {
            GN_ERROR(sLogger, "VkBindlessCnC::recordDownloadImage: {}",
                     mSealed ? "already sealed" : (!srcVk ? "null/invalid source texture" : "no regions specified"));
            return future;
        }

    if (srcVk->descriptor().samples != 1 || !singleCopyAspect(srcVk->descriptor())) GN_UNLIKELY {
            GN_ERROR(sLogger, "CNC::recordDownloadImage: image downloads require a single-sample, single-aspect format");
            return future;
        }
    for (const auto & region : regions) {
        if (!validImageRegion(srcVk->descriptor(), region.mip, region.face, region.imageOffset, region.imageExtent)) GN_UNLIKELY {
                GN_ERROR(sLogger, "CNC::recordDownloadImage: invalid image region");
                return future;
            }
    }

    const auto     fmt = srcVk->descriptor().format;
    const auto &   ld  = fmt.layoutDesc();
    const uint32_t bw  = ld.blockWidth ? ld.blockWidth : 1;
    const uint32_t bh  = ld.blockHeight ? ld.blockHeight : 1;
    const uint32_t bb  = fmt.bytesPerBlock() ? fmt.bytesPerBlock() : 1;

    auto lcm4 = [](uint64_t n) -> uint64_t {
        uint64_t a = 4, b = n;
        while (b) {
            uint64_t t = a % b;
            a          = b;
            b          = t;
        }
        return (4 / a) * n;
    };
    const uint64_t align = lcm4(bb);

    DynaArray<GpuCnC::Region> packed;
    uint64_t                  cursor = 0;
    for (const auto & r : regions) {
        const uint32_t w        = r.imageExtent.x ? r.imageExtent.x : 1;
        const uint32_t h        = r.imageExtent.y ? r.imageExtent.y : 1;
        const uint32_t d        = r.imageExtent.z ? r.imageExtent.z : 1;
        const uint64_t blocksX  = (w + bw - 1) / bw;
        const uint64_t blocksY  = (h + bh - 1) / bh;
        const uint64_t regBytes = blocksX * blocksY * d * bb;

        cursor = ((cursor + align - 1) / align) * align;

        GpuCnC::Region pr  = r;
        pr.dataOffset      = cursor;
        pr.rowPitchBytes   = blocksX * bb;
        pr.slicePitchBytes = blocksX * blocksY * bb;
        packed.append(pr);

        cursor += regBytes;
    }

    if (cursor == 0) GN_UNLIKELY {
            GN_ERROR(sLogger, "VkBindlessCnC::recordDownloadImage: regions describe zero bytes");
            return future;
        }

    auto staging = Buffer::create(name + "/download_img_stg", {.context = mGpu, .size = cursor, .mappable = true});
    if (!staging) GN_UNLIKELY {
            GN_ERROR(sLogger, "VkBindlessCnC::recordDownloadImage: staging buffer allocation failed");
            return future;
        }

    StoredDownloadImage op;
    op.src     = std::move(src);
    op.staging = std::move(staging);
    op.regions = std::move(packed);
    op.result  = std::move(result);
    mOps.emplace_back(std::move(op));
    return future;
}

void VkBindlessCnC::addCleanupCallback(std::function<void()> cleanup) {
    if (cleanup) { mRetainedCleanups.push_back(std::move(cleanup)); }
}

AutoRef<GpuPayload> VkBindlessCnC::seal() {
    if (mSealed) GN_UNLIKELY {
            GN_ERROR(sLogger, "VkBindlessCnC::seal: recorder is already sealed");
            return {};
        }
    mSealed = true;

    VkBindlessCncPayload::ConstructParameters cp;
    cp.gpu                = mGpu;
    cp.heap               = std::move(mHeap);
    cp.heapSetIndex       = mHeapSetIndex;
    cp.pipelineLayout     = mPipelineLayout;
    cp.uploadStorage      = std::move(mUploadStorage);
    cp.ops                = std::move(mOps);
    cp.immediateData      = std::move(mImmediateData);
    cp.retainedCleanups   = std::move(mRetainedCleanups);
    cp.passDescriptorPool = mPassDescriptorPool;
    cp.passDescriptorSets = std::move(mPassDescriptorSets);
    cp.passResources      = std::move(mPassResources);
    mPassDescriptorPool   = vk::DescriptorPool {};

    return AutoRef<GpuPayload>(new VkBindlessCncPayload(name, std::move(cp)));
}

AutoRef<bindless::CnC> createVkBindlessCnc(const StrA & name, const bindless::CnC::CreateParameters & cp) {
    AutoRef<GpuContextVulkan2> vkGpu(RuntimeType::cast<GpuContextVulkan2>(cp.gpu.get()));
    if (!vkGpu || !vkGpu->ready()) GN_UNLIKELY {
            GN_ERROR(sLogger, "createVkBindlessCnc: null or unready GpuContextVulkan2");
            return {};
        }

    if (!cp.heap) GN_UNLIKELY {
            GN_ERROR(sLogger, "createVkBindlessCnc: null DescriptorHeap");
            return {};
        }

    // Fast conflict check: passResources must not collide with heapSetIndex
    if (cp.heapSetIndex < cp.passResources.size() && !cp.passResources[cp.heapSetIndex].empty()) GN_UNLIKELY {
            GN_ERROR(sLogger, "createVkBindlessCnc: passResources conflicts with heapSetIndex {}", cp.heapSetIndex);
            return {};
        }

    auto *             vkHeap = RuntimeType::cast<VkBindlessDescriptorHeap>(cp.heap.get());
    vk::PipelineLayout pl     = vkGpu->bindlessPipelineLayoutCache().getOrCreateCompute(cp, vkHeap);
    if (!pl) GN_UNLIKELY {
            GN_ERROR(sLogger, "createVkBindlessCnc: failed to obtain VkPipelineLayout for compute");
            return {};
        }

    vk::DescriptorPool             passPool {};
    std::vector<vk::DescriptorSet> passSets;

    if (!buildBindlessPassDescriptorSets(*vkGpu, cp.passResources, cp.heapSetIndex, vk::ShaderStageFlagBits::eCompute, passPool, passSets)) {
        GN_ERROR(sLogger, "createVkBindlessCnc: failed to build pass descriptor sets");
        return {};
    }

    return AutoRef<bindless::CnC>(new VkBindlessCnC(name, vkGpu, cp.heap, cp.heapSetIndex, pl, passPool, passSets, cp.passResources, cp.opCountHint));
}

} // namespace GN::gpu2
