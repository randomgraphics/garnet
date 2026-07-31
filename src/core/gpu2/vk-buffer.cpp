#include "pch.h"
#include "vk-buffer.h"

static GN::Logger * sLogger = GN::getLogger("GN.gpu2.vk");

namespace GN::gpu2 {

// -----------------------------------------------------------------------------
// BufferVulkan
// -----------------------------------------------------------------------------

BufferVulkan::~BufferVulkan() { reset(); }

bool BufferVulkan::init(const Buffer::CreateParameters & params) {
    reset();
    if (!params.context) {
        GN_ERROR(sLogger, "BufferVulkan::init: context is null, name='{}'", name);
        return false;
    }
    if (params.size == 0) {
        GN_ERROR(sLogger, "BufferVulkan::init: size must be > 0, name='{}'", name);
        return false;
    }
    mGpu = params.context.staticCastTo<GpuContextVulkan2>();
    if (!mGpu || !mGpu->ready()) {
        GN_ERROR(sLogger, "BufferVulkan::init: invalid GpuContext, name='{}'", name);
        return false;
    }
    const rv::Device & dev = mGpu->vulkanDevice();
    if (!dev.gi()) {
        GN_ERROR(sLogger, "BufferVulkan::init: invalid Vulkan device, name='{}'", name);
        return false;
    }

    mSize     = params.size;
    mMappable = params.mappable;

    // All buffers get the full usage set so they can serve any role without recreation.
    // Mappable buffers live in host-visible coherent memory; device-local buffers use staging for uploads.
    constexpr vk::BufferUsageFlags kAllUsages = vk::BufferUsageFlagBits::eTransferSrc | vk::BufferUsageFlagBits::eTransferDst |
                                                vk::BufferUsageFlagBits::eUniformBuffer | vk::BufferUsageFlagBits::eStorageBuffer |
                                                vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eIndexBuffer;

    rv::Buffer::ConstructParameters cp;
    cp.gi    = dev.gi();
    cp.size  = params.size;
    cp.usage = kAllUsages;
    cp.memory =
        params.mappable ? (vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent) : vk::MemoryPropertyFlagBits::eDeviceLocal;

    mRvBuffer = rv::Ref<rv::Buffer>(new rv::Buffer(cp));
    if (!mRvBuffer || !mRvBuffer->handle()) {
        GN_ERROR(sLogger, "BufferVulkan::init: VkBuffer creation failed, name='{}'", name);
        reset();
        return false;
    }
    rv::setVkHandleName(dev.gi()->device, mRvBuffer->handle(), name.c_str());

    gpuState = BufferStateVulkan::UNDEFINED();
    return true;
}

Buffer::Mapped BufferVulkan::map() {
    if (!mRvBuffer) {
        GN_ERROR(sLogger, "BufferVulkan::map: buffer not initialized, name='{}'", name);
        return {};
    }
    if (!mMappable) {
        GN_ERROR(sLogger, "BufferVulkan::map: buffer is not mappable, name='{}'", name);
        return {};
    }
    if (mIsMapped) {
        GN_ERROR(sLogger, "BufferVulkan::map: buffer already mapped, name='{}'", name);
        return {};
    }
    auto mapped = mRvBuffer->map({});
    if (!mapped.data) {
        GN_ERROR(sLogger, "BufferVulkan::map: vkMapMemory failed, name='{}'", name);
        return {};
    }
    mIsMapped = true;
    return Mapped(this, mapped.data, (size_t) mapped.size);
}

void BufferVulkan::unmap(const Mapped &) {
    if (mRvBuffer && mIsMapped) {
        mRvBuffer->unmap();
        mIsMapped       = false;
        gpuState.access = vk::AccessFlagBits::eHostWrite;
        gpuState.stages = vk::PipelineStageFlagBits::eHost;
    }
}

bool BufferVulkan::setContent(ArrayProxy<const uint8_t> data, size_t offset) {
    if (!mRvBuffer) {
        GN_ERROR(sLogger, "BufferVulkan::setContent: buffer not initialized, name='{}'", name);
        return false;
    }
    if (data.empty()) return true;
    if (!mGpu || !mGpu->ready()) return false;
    const rv::Device & dev = mGpu->vulkanDevice();
    rv::CommandQueue * gq  = dev.graphics();
    if (!gq) {
        GN_ERROR(sLogger, "BufferVulkan::setContent: no graphics queue, name='{}'", name);
        return false;
    }
    mGpu->waitForIdle();
    rv::Buffer::SetContentParameters sc;
    sc.setQueue(*gq).setData(data.data(), data.size()).setOffset((vk::DeviceSize) offset);
    mRvBuffer->setContent(sc);
    gpuState.access = vk::AccessFlagBits::eTransferWrite;
    gpuState.stages = vk::PipelineStageFlagBits::eTransfer;
    return true;
}

std::vector<uint8_t> BufferVulkan::readContent(size_t offset, size_t size) const {
    if (!mRvBuffer) {
        GN_ERROR(sLogger, "BufferVulkan::readContent: buffer not initialized, name='{}'", name);
        return {};
    }
    if (!mGpu || !mGpu->ready()) return {};
    const rv::Device & dev = mGpu->vulkanDevice();
    rv::CommandQueue * gq  = dev.graphics();
    if (!gq) {
        GN_ERROR(sLogger, "BufferVulkan::readContent: no graphics queue, name='{}'", name);
        return {};
    }
    mGpu->waitForIdle();
    rv::Buffer::ReadParameters rp;
    rp.setQueue(*gq).setRange((vk::DeviceSize) offset, size == (size_t) -1 ? vk::DeviceSize(-1) : (vk::DeviceSize) size);
    auto result     = mRvBuffer->readContent(rp);
    gpuState.access = vk::AccessFlagBits::eTransferRead;
    gpuState.stages = vk::PipelineStageFlagBits::eTransfer;
    return result;
}

void BufferVulkan::reset() {
    if (mIsMapped && mRvBuffer) {
        mRvBuffer->unmap();
        mIsMapped = false;
    }
    mRvBuffer.clear();
    mSize     = 0;
    mMappable = false;
    mGpu.clear();
    gpuState = BufferStateVulkan::UNDEFINED();
}

// -----------------------------------------------------------------------------
// Factory
// -----------------------------------------------------------------------------

AutoRef<Buffer> createBufferVulkan2(const StrA & entityName, const Buffer::CreateParameters & params) {
    auto p = new BufferVulkan(entityName);
    if (!p->init(params)) {
        delete p;
        return {};
    }
    return AutoRef<Buffer>(p);
}

} // namespace GN::gpu2
