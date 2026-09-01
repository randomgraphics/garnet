#pragma once

#include <garnet/GNgpu2.h>
#include "vk-gpu-context.h"
#include "vk-buffer-state.h"

namespace GN::gpu2 {

class BufferVulkan final : public Buffer {
public:
    GN_REGISTER_RUNTIME_TYPE(Buffer);

    explicit BufferVulkan(const StrA & entityName): Buffer(TYPE_INFO(), entityName) {}
    ~BufferVulkan() override;

    bool init(const CreateParameters & params);

    Mapped               map() override;
    bool                 setContent(ArrayView<const uint8_t> data, size_t offset = 0) override;
    std::vector<uint8_t> readContent(size_t offset = 0, size_t size = (size_t) -1) const override;

    vk::Buffer          nativeBuffer() const { return mRvBuffer ? mRvBuffer->handle() : vk::Buffer {}; }
    rv::Ref<rv::Buffer> rvBuffer() const { return mRvBuffer; }
    uint64_t            bufferSize() const { return mSize; }

    mutable BufferStateVulkan gpuState {};

protected:
    void unmap(const Mapped &) override;

private:
    AutoRef<GpuContextVulkan2> mGpu;
    rv::Ref<rv::Buffer>        mRvBuffer {};
    uint64_t                   mSize     = 0;
    bool                       mMappable = false;
    bool                       mIsMapped = false;

    void reset();
};

AutoRef<Buffer> createBufferVulkan2(const StrA & name, const Buffer::CreateParameters & params);

} // namespace GN::gpu2
