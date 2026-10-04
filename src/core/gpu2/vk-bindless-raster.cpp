#include "pch.h"
#include "vk-bindless-raster.h"
#include "vk-bindless-descriptor-heap.h"
#include "vk-format-utils.h"

static GN::Logger * sLogger = GN::getLogger("GN.gpu2.vk.bindless.raster");

namespace GN::gpu2 {

VkBindlessRaster::VkBindlessRaster(const StrA & name, AutoRef<GpuContextVulkan2> gpu, RasterTarget target, AutoRef<bindless::DescriptorHeap> heap,
                                   uint32_t heapSetIndex, vk::PipelineLayout pipelineLayout, vk::DescriptorPool passPool,
                                   std::vector<vk::DescriptorSet> passSets, size_t numberOfDrawsHint, uint32_t maxImmediateSize)
    : bindless::Raster(TYPE_INFO(), name), mGpu(std::move(gpu)), mRenderTarget(std::move(target)), mHeap(std::move(heap)), mHeapSetIndex(heapSetIndex),
      mPipelineLayout(pipelineLayout), mPassDescriptorPool(passPool), mPassDescriptorSets(std::move(passSets)),
      mStorage(std::make_unique<BindlessDrawStorage>(numberOfDrawsHint)) {
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

void VkBindlessRaster::retainCleanup(std::function<void()> cleanup) {
    if (cleanup) { mRetainedCleanups.push_back(std::move(cleanup)); }
}

AutoRef<GpuPayload> VkBindlessRaster::seal() {
    if (mSealed) GN_UNLIKELY {
            GN_ERROR(sLogger, "VkBindlessRaster::seal: raster is already sealed");
            return {};
        }
    mSealed = true;

    VkBindlessPayload::ConstructParameters cp;
    cp.gpu                = mGpu;
    cp.target             = std::move(mRenderTarget);
    cp.heap               = std::move(mHeap);
    cp.heapSetIndex       = mHeapSetIndex;
    cp.pipelineLayout     = mPipelineLayout;
    cp.storage            = std::move(mStorage);
    cp.immediateData      = std::move(mImmediateData);
    cp.retainedCleanups   = std::move(mRetainedCleanups);
    cp.passDescriptorPool = mPassDescriptorPool;
    cp.passDescriptorSets = std::move(mPassDescriptorSets);
    mPassDescriptorPool   = vk::DescriptorPool {};

    return AutoRef<GpuPayload>(new VkBindlessPayload(name, std::move(cp)));
}

AutoRef<bindless::Raster> createVkBindlessRaster(const StrA & name, const bindless::Raster::CreateParameters & cp) {
    AutoRef<GpuContextVulkan2> vkGpu(RuntimeType::cast<GpuContextVulkan2>(cp.gpu.get()));
    if (!vkGpu || !vkGpu->ready()) GN_UNLIKELY {
            GN_ERROR(sLogger, "createVkBindlessRaster: null or unready GpuContextVulkan2");
            return {};
        }

    // Fast conflict check: passResources must not collide with heapSetIndex
    if (cp.heap && cp.heapSetIndex < cp.passResources.size() && !cp.passResources[cp.heapSetIndex].empty()) GN_UNLIKELY {
            GN_ERROR(sLogger, "createVkBindlessRaster: passResources conflicts with heapSetIndex {}", cp.heapSetIndex);
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

    return AutoRef<bindless::Raster>(
        new VkBindlessRaster(name, vkGpu, *cp.target, cp.heap, cp.heapSetIndex, pl, passPool, passSets, cp.numberOfDrawsHint, cp.maxImmediateSize));
}

} // namespace GN::gpu2
