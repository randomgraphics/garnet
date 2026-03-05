#include "pch.h"
#include "vk-buffer.h"

static GN::Logger * sLogger = GN::getLogger("GN.rdg");

namespace GN::rdg {

bool BufferVulkan::setContent(const void * data, uint64_t size) {
    if (!data || !size) GN_UNLIKELY {
            GN_ERROR(sLogger)("BufferVulkan::setContent: data or size is null/zero, name='{}'", name);
            return false;
        }
    auto & gi = mGpu->globalInfo();
    rapid_vulkan::Buffer::ConstructParameters cp;
    cp.name   = name.c_str();
    cp.gi     = &gi;
    cp.size   = size;
    cp.usage  = vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eIndexBuffer;
    cp.memory = vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent;
    mVkBuffer = rapid_vulkan::Ref<rapid_vulkan::Buffer>::make(cp);
    if (!mVkBuffer || !mVkBuffer->desc().handle) GN_UNLIKELY {
            GN_ERROR(sLogger)("BufferVulkan::setContent: failed to create VkBuffer, name='{}'", name);
            mVkBuffer = {};
            return false;
        }
    rapid_vulkan::Buffer::Map<uint8_t> mapped(*mVkBuffer);
    if (!mapped) GN_UNLIKELY {
            GN_ERROR(sLogger)("BufferVulkan::setContent: failed to map VkBuffer, name='{}'", name);
            mVkBuffer = {};
            return false;
        }
    memcpy(mapped.data, data, size);
    mSize = size;
    return true;
}

AutoRef<Buffer> createVulkanBuffer(ArtifactDatabase & db, const StrA & name, const Buffer::CreateParameters & params) {
    auto gpu = params.context.castTo<GpuContextVulkan>();
    if (!gpu) GN_UNLIKELY {
            GN_ERROR(sLogger)("createVulkanBuffer: gpu is not Vulkan, name='{}'", name);
            return {};
        }
    if (!params.size) GN_UNLIKELY {
            GN_ERROR(sLogger)("createVulkanBuffer: size is 0, name='{}'", name);
            return {};
        }
    auto * p = new BufferVulkan(db, name, gpu);
    if (!p->sequence) GN_UNLIKELY {
            GN_ERROR(sLogger)("createVulkanBuffer: duplicate type+name, name='{}'", name);
            delete p;
            return {};
        }
    return AutoRef<Buffer>(p);
}

} // namespace GN::rdg
