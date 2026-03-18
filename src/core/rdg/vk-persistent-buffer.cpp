#include "pch.h"
#include "vk-persistent-buffer.h"

static GN::Logger * sLogger = GN::getLogger("GN.rdg");

namespace GN::rdg {

vk::BufferUsageFlags bufferUsageToVk(BufferUsageFlags usage) {
    vk::BufferUsageFlags result;
    const uint32_t       m = usage.mask;
    if (m & static_cast<uint32_t>(BufferUsageBits::VERTEX)) result |= vk::BufferUsageFlagBits::eVertexBuffer;
    if (m & static_cast<uint32_t>(BufferUsageBits::INDEX)) result |= vk::BufferUsageFlagBits::eIndexBuffer;
    if (m & static_cast<uint32_t>(BufferUsageBits::UNIFORM)) result |= vk::BufferUsageFlagBits::eUniformBuffer;
    if (m & static_cast<uint32_t>(BufferUsageBits::STORAGE)) result |= vk::BufferUsageFlagBits::eStorageBuffer;
    return result;
}

bool PersistentBufferVulkan::allocate(const CreateParameters & params) {
    auto &                                    gi = mGpu->globalInfo();
    rapid_vulkan::Buffer::ConstructParameters cp;
    cp.name   = name.c_str();
    cp.gi     = &gi;
    cp.size   = params.size;
    cp.usage  = bufferUsageToVk(params.usage) | vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eTransferSrc;
    cp.memory = vk::MemoryPropertyFlagBits::eDeviceLocal;
    mVkBuffer = rapid_vulkan::Ref<rapid_vulkan::Buffer>::make(cp);
    if (!mVkBuffer || !mVkBuffer->desc().handle) GN_UNLIKELY {
            GN_ERROR(sLogger)("BufferVulkan::allocate: failed to create VkBuffer, name='{}'", name);
            mVkBuffer = {};
            return false;
        }
    mAllocatedSize = params.size;
    mHandle        = mVkBuffer->desc().handle;
    return true;
}

bool PersistentBufferVulkan::setContent(const void * data, uint64_t size) {
    if (!data) GN_UNLIKELY {
            GN_ERROR(sLogger)("PersistentBufferVulkan::setContent: data is null");
            return false;
        }
    if (size > mAllocatedSize) GN_UNLIKELY {
            GN_ERROR(sLogger)("PersistentBufferVulkan::setContent: size {} exceeds allocated {}", size, mAllocatedSize);
            return false;
        }
    if (!mVkBuffer) return false;
    auto * q = mGpu->device().graphics();
    if (!q) return false;
    mVkBuffer->setContent(rapid_vulkan::Buffer::SetContentParameters {}.setQueue(*q).setData(data, size));
    return true;
}

AutoRef<Blob> PersistentBufferVulkan::readback() const {
    if (!mVkBuffer) return {};
    auto * q = mGpu->device().graphics();
    if (!q) return {};
    AutoRef<Blob> result;
    auto  params = rapid_vulkan::Buffer::ReadParametersWithCallback {};
    params.setQueue(*q).setRange(0, mAllocatedSize);
    params.callback = [&result](const void * data, size_t size) {
        if (data && size > 0) result = referenceTo(new SimpleBlob<uint8_t>(size, static_cast<const uint8_t *>(data)));
    };
    mVkBuffer->readContentWithCallback(params);
    return result;
}

AutoRef<PersistentBuffer> createVulkanPersistentBuffer(const StrA & name, const PersistentBuffer::CreateParameters & params) {
    auto gpu = params.context.staticCastTo<GpuContextVulkan>();
    if (!gpu) GN_UNLIKELY {
            GN_ERROR(sLogger)("createVulkanBuffer: gpu is not Vulkan, name='{}'", name);
            return {};
        }
    if (!params.size) GN_UNLIKELY {
            GN_ERROR(sLogger)("createVulkanBuffer: size is 0, name='{}'", name);
            return {};
        }
    auto * p = new PersistentBufferVulkan(name, std::move(gpu));
    if (!p->allocate(params)) GN_UNLIKELY {
            delete p;
            return {};
        }
    return AutoRef<PersistentBuffer>(p);
}

} // namespace GN::rdg
