#pragma once

#include "vk-gpu-context.h"

namespace GN::gpu2 {

class GpuResourceStateTrackerVulkan;

/// Vulkan backing for gpu2's opaque acceleration-structure resource.
class VkAccelerationStructureVulkan final : public AccelerationStructure {
public:
    GN_REGISTER_RUNTIME_TYPE(AccelerationStructure);

    VkAccelerationStructureVulkan(const StrA & entityName, AutoRef<GpuContextVulkan2> gpu);
    ~VkAccelerationStructureVulkan() override;

    bool init(const BuildParameters & params, CnC & recorder, AutoRef<Buffer> & scratchBuffer);
    bool recordBuild(vk::CommandBuffer commandBuffer, GpuResourceStateTrackerVulkan & tracker, const AutoRef<Buffer> & scratchBuffer);
    void releaseBuildInputs();

    AutoRef<GpuContext>          gpu() const override;
    vk::AccelerationStructureKHR nativeHandle() const { return mTopLevel.handle; }
    vk::DeviceAddress            deviceAddress() const { return mTopLevel.address; }
    bool                         buildSubmitted() const { return mBuildSubmitted; }
    bool                         buildEncoded() const { return mBuildEncoded; }
    void                         markBuildSubmitted() {
        if (mBuildEncoded) mBuildSubmitted = true;
    }

private:
    struct NativeStructure {
        rv::Ref<rv::Buffer>          storage;
        vk::AccelerationStructureKHR handle {};
        vk::DeviceAddress            address = 0;
    };

    bool createNativeStructure(vk::AccelerationStructureTypeKHR type, uint64_t size, NativeStructure & out);
    void reset();

    AutoRef<GpuContextVulkan2>  mGpu;
    NativeStructure             mBottomLevel;
    NativeStructure             mTopLevel;
    DynaArray<TriangleGeometry> mGeometries;
    AutoRef<Buffer>             mTransformBuffer;
    AutoRef<Buffer>             mInstanceBuffer;
    uint64_t                    mScratchSize      = 0;
    uint64_t                    mScratchAlignment = 1;
    bool                        mPreferFastTrace  = true;
    bool                        mBuildRecorded    = false;
    bool                        mBuildEncoded     = false;
    bool                        mBuildSubmitted   = false;
};

AutoRef<AccelerationStructure> createAccelerationStructureVulkan(const StrA & name, AutoRef<GpuContextVulkan2> gpu,
                                                                 const AccelerationStructure::BuildParameters & params, CnC & recorder,
                                                                 AutoRef<Buffer> & scratchBuffer);

} // namespace GN::gpu2
