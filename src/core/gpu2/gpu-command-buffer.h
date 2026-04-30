#pragma once

/// Internal gpu2 header — not part of the public API surface.
#include <garnet/GNgpu2.h>

namespace GN::gfx::gpu2 {

/// Internal command buffer abstraction. Not part of the public API.
/// Public callers use GpuContext::submit() + GpuPayload instead.
struct GpuCommandBuffer : public RootEntity {
    GN_API GN_REGISTER_RUNTIME_TYPE(RootEntity);

    struct Payload;
    struct Fence;

    struct SubmitParameters {
        StrA                      name;
        ArrayContainer<Payload *> jobs;
    };

    struct CreateParameters {
        AutoRef<GpuContext> gpu;
    };

    static GN_API AutoRef<GpuCommandBuffer> create(const CreateParameters &);

    virtual Fence * submit(const SubmitParameters &) = 0;

protected:
    using RootEntity::RootEntity;
};

/// Shared implementation for command buffers that record against one \c GpuContext.
class GpuCommandBufferCommon : public GpuCommandBuffer {
public:
    GN_REGISTER_RUNTIME_TYPE(GpuCommandBuffer);

    AutoRef<GpuContext> gpuContext() const { return mGpu; }

protected:
    GpuCommandBufferCommon(const RuntimeType::TypeInfo & leafType, const StrA & entityName, AutoRef<GpuContext> gpu)
        : GpuCommandBuffer(leafType, entityName), mGpu(std::move(gpu)) {}

    AutoRef<GpuContext> mGpu;
};

} // namespace GN::gfx::gpu2
