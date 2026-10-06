#include "pch.h"
#include "vk-bindless-raster.h"
#include "vk-bindless-cnc.h"
#include "vk-bindless-descriptor-heap.h"
#include "vk-format-utils.h"

static GN::Logger * sLogger = GN::getLogger("GN.gpu2.vk.bindless.raster");

namespace GN::gpu2 {

static inline bool sameResources(const GpuResourceTable & a, const GpuResourceTable & b) {
    if (a.size() != b.size()) return false;
    for (size_t s = 0; s < a.size(); ++s) {
        if (a[s].size() != b[s].size()) return false;
        for (size_t slot = 0; slot < a[s].size(); ++slot) {
            if (a[s][slot].size() != b[s][slot].size()) return false;
            for (size_t i = 0; i < a[s][slot].size(); ++i) {
                if (a[s][slot][i] != b[s][slot][i]) return false;
            }
        }
    }
    return true;
}

VkBindlessRaster::VkBindlessRaster(const StrA & name, AutoRef<GpuContextVulkan2> gpu, RasterTarget target, AutoRef<bindless::DescriptorHeap> heap,
                                   uint32_t heapSetIndex, vk::PipelineLayout pipelineLayout, vk::DescriptorPool passPool,
                                   std::vector<vk::DescriptorSet> passSets, GpuResourceTable passResources, size_t numberOfDrawsHint, uint32_t maxImmediateSize)
    : bindless::Raster(TYPE_INFO(), name), mGpu(std::move(gpu)), mRenderTarget(std::move(target)), mHeap(std::move(heap)), mHeapSetIndex(heapSetIndex),
      mPipelineLayout(pipelineLayout), mPassDescriptorPool(passPool), mPassDescriptorSets(std::move(passSets)), mPassResources(std::move(passResources)),
      mMaxImmediateSize(maxImmediateSize), mStorage(std::make_unique<BindlessDrawStorage>(numberOfDrawsHint)) {
    if (maxImmediateSize && numberOfDrawsHint > mImmediateData.max_size() / maxImmediateSize)
        throw std::length_error("Bindless immediate storage size overflow");
    mImmediateData.reserve(numberOfDrawsHint * maxImmediateSize);
}

VkBindlessRaster::~VkBindlessRaster() {
    if (!mSealed && mPassDescriptorPool && mGpu && mGpu->ready()) {
        mGpu->vulkanDevice().handle().destroyDescriptorPool(mPassDescriptorPool);
        mPassDescriptorPool = vk::DescriptorPool {};
    }
}

void VkBindlessRaster::recordDraw(const DrawParameters & params) {
    if (mSealed) GN_UNLIKELY {
            GN_ERROR(sLogger, "VkBindlessRaster::recordDraw: cannot record draws into a sealed raster");
            return;
        }
    if (!params.vs) GN_UNLIKELY {
            GN_ERROR(sLogger, "VkBindlessRaster::recordDraw: vertex shader is required");
            return;
        }

    RasterState mergedState = mRenderTarget.states;
    mergeRenderState(mergedState, params.states);

    uint32_t immOffset = 0;
    uint32_t immSize   = 0;
    if (!params.immediates.empty()) {
        immOffset = static_cast<uint32_t>(mImmediateData.size());
        immSize   = static_cast<uint32_t>(params.immediates.size());
        mImmediateData.insert(mImmediateData.end(), params.immediates.begin(), params.immediates.end());
    }

    mStorage->draws.emplace_back(params, mergedState, &mStorage->pool, immOffset, immSize);
}

void VkBindlessRaster::recordBindBasedDraw(const DrawParameters & params, const GpuResourceTable & drawResources) {
    if (mSealed) GN_UNLIKELY {
            GN_ERROR(sLogger, "VkBindlessRaster::recordBindBasedDraw: cannot record draws into a sealed raster");
            return;
        }
    if (!params.vs) GN_UNLIKELY {
            GN_ERROR(sLogger, "VkBindlessRaster::recordBindBasedDraw: vertex shader is required");
            return;
        }

    if (mHeap && mHeapSetIndex < drawResources.size() && !drawResources[mHeapSetIndex].empty()) GN_UNLIKELY {
            GN_ERROR(sLogger, "VkBindlessRaster::recordBindBasedDraw: drawResources conflicts with descriptor heap set {}", mHeapSetIndex);
            return;
        }

    if (drawResources.empty()) {
        recordDraw(params);
        return;
    }

    RasterState mergedState = mRenderTarget.states;
    mergeRenderState(mergedState, params.states);

    uint32_t immOffset = 0;
    uint32_t immSize   = 0;
    if (!params.immediates.empty()) {
        immOffset = static_cast<uint32_t>(mImmediateData.size());
        immSize   = static_cast<uint32_t>(params.immediates.size());
        mImmediateData.insert(mImmediateData.end(), params.immediates.begin(), params.immediates.end());
    }

    uint32_t boundIdx = ~0u;
    if (mLastBoundResourceIndex < mBoundResourceTables.size() && sameResources(mBoundResourceTables[mLastBoundResourceIndex], drawResources)) {
        boundIdx = mLastBoundResourceIndex;
    } else {
        for (size_t i = 0; i < mBoundResourceTables.size(); ++i) {
            if (sameResources(mBoundResourceTables[i], drawResources)) {
                boundIdx = static_cast<uint32_t>(i);
                break;
            }
        }
        if (boundIdx == ~0u) {
            boundIdx = static_cast<uint32_t>(mBoundResourceTables.size());
            mBoundResourceTables.push_back(drawResources);
        }
        mLastBoundResourceIndex = boundIdx;
    }

    mStorage->draws.emplace_back(params, mergedState, &mStorage->pool, immOffset, immSize, boundIdx);
}

void VkBindlessRaster::addCleanupCallback(std::function<void()> cleanup) {
    if (cleanup) { mRetainedCleanups.push_back(std::move(cleanup)); }
}

AutoRef<GpuPayload> VkBindlessRaster::seal() {
    if (mSealed) GN_UNLIKELY {
            GN_ERROR(sLogger, "VkBindlessRaster::seal: raster is already sealed");
            return {};
        }
    mSealed = true;

    std::vector<BoundResourceEntry> boundResources;
    if (!mBoundResourceTables.empty() && mGpu && mGpu->ready()) {
        auto * vkHeap = RuntimeType::cast<VkBindlessDescriptorHeap>(mHeap.get());
        boundResources.resize(mBoundResourceTables.size());
        for (size_t i = 0; i < mBoundResourceTables.size(); ++i) {
            const auto & t     = mBoundResourceTables[i];
            auto &       entry = boundResources[i];

            if (!buildBindlessPassDescriptorSets(*mGpu, t, mHeapSetIndex, vk::ShaderStageFlagBits::eAllGraphics, entry.descriptorPool, entry.descriptorSets)) {
                GN_ERROR(sLogger, "VkBindlessRaster::seal: failed to build descriptor sets for bound draw resources");
            }

            GpuResourceTable combined = mPassResources;
            for (size_t s = 0; s < t.size(); ++s) {
                if (s != mHeapSetIndex && !t[s].empty()) {
                    if (s >= combined.size()) combined.resize(s + 1);
                    combined[s] = t[s];
                }
            }

            entry.pipelineLayout =
                mGpu->bindlessPipelineLayoutCache().getOrCreate(mHeapSetIndex, mMaxImmediateSize, combined, vkHeap, vk::ShaderStageFlagBits::eAllGraphics);
        }
    }

    VkBindlessPayload::ConstructParameters cp;
    cp.gpu                 = mGpu;
    cp.target              = std::move(mRenderTarget);
    cp.heap                = std::move(mHeap);
    cp.heapSetIndex        = mHeapSetIndex;
    cp.pipelineLayout      = mPipelineLayout;
    cp.storage             = std::move(mStorage);
    cp.immediateData       = std::move(mImmediateData);
    cp.retainedCleanups    = std::move(mRetainedCleanups);
    cp.passDescriptorPool  = mPassDescriptorPool;
    cp.passDescriptorSets  = std::move(mPassDescriptorSets);
    cp.boundResources      = std::move(boundResources);
    cp.boundResourceTables = std::move(mBoundResourceTables);
    mPassDescriptorPool    = vk::DescriptorPool {};

    return AutoRef<GpuPayload>(new VkBindlessPayload(name, std::move(cp)));
}

AutoRef<bindless::Raster> createVkBindlessRaster(const StrA & name, const bindless::Raster::CreateParameters & cp) {
    AutoRef<GpuContextVulkan2> vkGpu(RuntimeType::cast<GpuContextVulkan2>(cp.gpu.get()));
    if (!vkGpu || !vkGpu->ready()) GN_UNLIKELY {
            GN_ERROR(sLogger, "createVkBindlessRaster: null or unready GpuContextVulkan2");
            return {};
        }

    // The descriptor heap and related data will take over the entire specified set (heapSetIndex).
    // Fast conflict check: if pass resources conflict with the heap (stay in the same set as where the descriptor heap is), creation will fail.
    if (cp.heap && cp.heapSetIndex < cp.passResources.size() && !cp.passResources[cp.heapSetIndex].empty()) GN_UNLIKELY {
            GN_ERROR(sLogger, "createVkBindlessRaster: passResources conflicts with descriptor heap set {}", cp.heapSetIndex);
            return {};
        }

    if (!cp.target || cp.target->empty()) GN_UNLIKELY {
            GN_ERROR(sLogger, "createVkBindlessRaster: invalid or empty RasterTarget");
            return {};
        }

    auto *             vkHeap = RuntimeType::cast<VkBindlessDescriptorHeap>(cp.heap.get());
    vk::PipelineLayout pl     = vkGpu->bindlessPipelineLayoutCache().getOrCreate(cp, vkHeap);
    if (!pl) GN_UNLIKELY {
            GN_ERROR(sLogger, "createVkBindlessRaster: failed to obtain VkPipelineLayout");
            return {};
        }

    vk::DescriptorPool             passPool {};
    std::vector<vk::DescriptorSet> passSets;

    if (!buildBindlessPassDescriptorSets(*vkGpu, cp.passResources, cp.heapSetIndex, vk::ShaderStageFlagBits::eAllGraphics, passPool, passSets)) {
        GN_ERROR(sLogger, "createVkBindlessRaster: failed to build pass descriptor sets");
        return {};
    }

    return AutoRef<bindless::Raster>(new VkBindlessRaster(name, vkGpu, *cp.target, cp.heap, cp.heapSetIndex, pl, passPool, passSets, cp.passResources,
                                                          cp.numberOfDrawsHint, cp.maxImmediateSize));
}

} // namespace GN::gpu2
