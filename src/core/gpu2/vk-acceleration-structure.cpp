#include "pch.h"

#include "vk-acceleration-structure.h"

#include "vk-buffer.h"
#include "vk-gpu-resource-state-tracker.h"

static GN::Logger * sLogger = GN::getLogger("GN.gpu2.vk.acceleration-structure");

namespace GN::gpu2 {

VkAccelerationStructureVulkan::VkAccelerationStructureVulkan(const StrA & entityName, AutoRef<GpuContextVulkan2> gpu):
    AccelerationStructure(TYPE_INFO(), entityName), mGpu(std::move(gpu)) {}

VkAccelerationStructureVulkan::~VkAccelerationStructureVulkan() {
    reset();
}

AutoRef<GpuContext> VkAccelerationStructureVulkan::gpu() const {
    return mGpu;
}

bool VkAccelerationStructureVulkan::createNativeStructure(vk::AccelerationStructureTypeKHR type, uint64_t size, NativeStructure & out) {
    const auto & device = mGpu->vulkanDevice();
    rv::Buffer::ConstructParameters bufferParams;
    bufferParams.name = name.c_str();
    bufferParams.gi = device.gi();
    bufferParams.size = size;
    bufferParams.usage = vk::BufferUsageFlagBits::eAccelerationStructureStorageKHR | vk::BufferUsageFlagBits::eShaderDeviceAddress;
    bufferParams.memory = vk::MemoryPropertyFlagBits::eDeviceLocal;
    out.storage = rv::Ref<rv::Buffer>(new rv::Buffer(bufferParams));
    if (!out.storage || !out.storage->handle()) return false;

    vk::AccelerationStructureCreateInfoKHR createInfo;
    createInfo.buffer = out.storage->handle();
    createInfo.size = size;
    createInfo.type = type;
    out.handle = device.handle().createAccelerationStructureKHR(createInfo);
    if (!out.handle) return false;

    vk::AccelerationStructureDeviceAddressInfoKHR addressInfo;
    addressInfo.accelerationStructure = out.handle;
    out.address = device.handle().getAccelerationStructureAddressKHR(addressInfo);
    return out.address != 0;
}

bool VkAccelerationStructureVulkan::init(const BuildParameters & params, CnC & recorder, AutoRef<Buffer> & scratchBuffer) {
    reset();
    if (!mGpu || !mGpu->ready() || !mGpu->caps().accelerationStructures || params.geometries.empty()) return false;
    mPreferFastTrace = params.preference == BuildPreference::FAST_TRACE;

    auto * physicalDevice = &mGpu->vulkanDevice().gi()->physical;
    vk::PhysicalDeviceAccelerationStructurePropertiesKHR asProperties;
    vk::PhysicalDeviceProperties2 properties;
    properties.pNext = &asProperties;
    physicalDevice->getProperties2(&properties);
    mScratchAlignment = std::max<uint64_t>(1, asProperties.minAccelerationStructureScratchOffsetAlignment);

    std::vector<vk::TransformMatrixKHR> transforms;
    std::vector<vk::AccelerationStructureGeometryKHR> bottomGeometries;
    std::vector<vk::AccelerationStructureBuildRangeInfoKHR> bottomRanges;
    std::vector<uint32_t> primitiveCounts;
    transforms.reserve(params.geometries.size());
    bottomGeometries.reserve(params.geometries.size());
    bottomRanges.reserve(params.geometries.size());
    primitiveCounts.reserve(params.geometries.size());

    for (const TriangleGeometry & source : params.geometries) {
        auto * vertex = RuntimeType::cast<BufferVulkan>(source.vertexBuffer.get());
        auto * index = RuntimeType::cast<BufferVulkan>(source.indexBuffer.get());
        if (!vertex || vertex->context() != mGpu.get() || !vertex->isAccelerationStructureBuildInput() || !vertex->gpuAddress() || source.vertexCount < 3 ||
            source.vertexStride < source.positionOffset + sizeof(float) * 3 || source.indexCount % 3 != 0 ||
            (source.indexFormat == IndexFormat::NONE && (source.indexBuffer || source.indexCount != 0)) ||
            (source.indexFormat != IndexFormat::NONE && (!index || index->context() != mGpu.get() || !index->isAccelerationStructureBuildInput() || !index->gpuAddress() || source.indexCount < 3)))
        {
            GN_ERROR(sLogger, "AccelerationStructure build has invalid or non-build-input geometry, resource='{}'", name);
            reset();
            return false;
        }

        const uint64_t vertexEnd = source.vertexOffset + uint64_t(source.vertexCount - 1) * source.vertexStride + source.positionOffset + sizeof(float) * 3;
        if (vertexEnd > vertex->size()) {
            GN_ERROR(sLogger, "AccelerationStructure vertex range exceeds buffer, resource='{}'", name);
            reset();
            return false;
        }
        uint64_t indexElementSize = 0;
        switch (source.indexFormat) {
        case IndexFormat::NONE: break;
        case IndexFormat::UINT16: indexElementSize = sizeof(uint16_t); break;
        case IndexFormat::UINT32: indexElementSize = sizeof(uint32_t); break;
        }
        if (index && source.indexOffset + uint64_t(source.indexCount) * indexElementSize > index->size()) {
            GN_ERROR(sLogger, "AccelerationStructure index range exceeds buffer, resource='{}'", name);
            reset();
            return false;
        }

        vk::TransformMatrixKHR transform;
        for (uint32_t row = 0; row < 3; ++row) {
            const Vector4f & values = source.objectToWorld.rows[row];
            transform.matrix[row][0] = values.x;
            transform.matrix[row][1] = values.y;
            transform.matrix[row][2] = values.z;
            transform.matrix[row][3] = values.w;
        }
        transforms.push_back(transform);

        vk::AccelerationStructureGeometryTrianglesDataKHR triangles;
        triangles.vertexFormat = vk::Format::eR32G32B32Sfloat;
        triangles.vertexData.deviceAddress = vertex->gpuAddress() + source.vertexOffset + source.positionOffset;
        triangles.vertexStride = source.vertexStride;
        triangles.maxVertex = source.vertexCount - 1;
        triangles.transformData.deviceAddress = 0;
        if (index) {
            triangles.indexData.deviceAddress = index->gpuAddress() + source.indexOffset;
            triangles.indexType = source.indexFormat == IndexFormat::UINT16 ? vk::IndexType::eUint16 : vk::IndexType::eUint32;
        } else {
            triangles.indexType = vk::IndexType::eNoneKHR;
        }

        vk::AccelerationStructureGeometryKHR geometry;
        geometry.geometryType = vk::GeometryTypeKHR::eTriangles;
        geometry.flags = vk::GeometryFlagBitsKHR::eOpaque;
        geometry.geometry.triangles = triangles;
        bottomGeometries.push_back(geometry);
        const uint32_t primitiveCount = source.indexFormat == IndexFormat::NONE ? source.vertexCount / 3 : source.indexCount / 3;
        bottomRanges.push_back({primitiveCount, 0, 0, 0});
        primitiveCounts.push_back(primitiveCount);
        mGeometries.append(source);
    }

    const uint64_t transformBytes = transforms.size() * sizeof(vk::TransformMatrixKHR);
    mTransformBuffer = Buffer::create(name + "/transforms", {.context = mGpu, .size = transformBytes, .accelerationStructureBuildInput = true});
    if (!mTransformBuffer || !mTransformBuffer->gpuAddress()) {
        reset();
        return false;
    }
    recorder.recordUploadBuffer(mTransformBuffer, 0, {reinterpret_cast<const uint8_t *>(transforms.data()), transforms.size() * sizeof(vk::TransformMatrixKHR)});
    for (size_t i = 0; i < bottomGeometries.size(); ++i) {
        bottomGeometries[i].geometry.triangles.transformData.deviceAddress = mTransformBuffer->gpuAddress() + i * sizeof(vk::TransformMatrixKHR);
    }

    const bool preferFastTrace = params.preference == BuildPreference::FAST_TRACE;
    const vk::BuildAccelerationStructureFlagsKHR buildFlags = preferFastTrace ? vk::BuildAccelerationStructureFlagBitsKHR::ePreferFastTrace :
                                                                                 vk::BuildAccelerationStructureFlagBitsKHR::ePreferFastBuild;
    vk::AccelerationStructureBuildGeometryInfoKHR bottomInfo;
    bottomInfo.type = vk::AccelerationStructureTypeKHR::eBottomLevel;
    bottomInfo.flags = buildFlags;
    bottomInfo.mode = vk::BuildAccelerationStructureModeKHR::eBuild;
    bottomInfo.geometryCount = static_cast<uint32_t>(bottomGeometries.size());
    bottomInfo.pGeometries = bottomGeometries.data();
    const auto bottomSizes = mGpu->vulkanDevice().handle().getAccelerationStructureBuildSizesKHR(vk::AccelerationStructureBuildTypeKHR::eDevice, bottomInfo, primitiveCounts);
    if (!createNativeStructure(vk::AccelerationStructureTypeKHR::eBottomLevel, bottomSizes.accelerationStructureSize, mBottomLevel)) {
        reset();
        return false;
    }
    bottomInfo.dstAccelerationStructure = mBottomLevel.handle;
    mScratchSize = bottomSizes.buildScratchSize;

    vk::AccelerationStructureInstanceKHR instance;
    for (uint32_t row = 0; row < 3; ++row) {
        instance.transform.matrix[row][row] = 1.0f;
    }
    instance.mask = 0xff;
    instance.flags = static_cast<uint8_t>(vk::GeometryInstanceFlagBitsKHR::eTriangleFacingCullDisable);
    instance.accelerationStructureReference = mBottomLevel.address;
    mInstanceBuffer = Buffer::create(name + "/instance", {.context = mGpu, .size = sizeof(instance), .accelerationStructureBuildInput = true});
    if (!mInstanceBuffer || !mInstanceBuffer->gpuAddress()) {
        reset();
        return false;
    }
    recorder.recordUploadBuffer(mInstanceBuffer, 0, {reinterpret_cast<const uint8_t *>(&instance), sizeof(instance)});

    vk::AccelerationStructureGeometryInstancesDataKHR instances;
    instances.data.deviceAddress = mInstanceBuffer->gpuAddress();
    vk::AccelerationStructureGeometryKHR topGeometry;
    topGeometry.geometryType = vk::GeometryTypeKHR::eInstances;
    topGeometry.geometry.instances = instances;
    vk::AccelerationStructureBuildGeometryInfoKHR topInfo;
    topInfo.type = vk::AccelerationStructureTypeKHR::eTopLevel;
    topInfo.flags = buildFlags;
    topInfo.mode = vk::BuildAccelerationStructureModeKHR::eBuild;
    topInfo.geometryCount = 1;
    topInfo.pGeometries = &topGeometry;
    const std::array<uint32_t, 1> topPrimitiveCounts = {1};
    const auto topSizes = mGpu->vulkanDevice().handle().getAccelerationStructureBuildSizesKHR(vk::AccelerationStructureBuildTypeKHR::eDevice, topInfo, topPrimitiveCounts);
    if (!createNativeStructure(vk::AccelerationStructureTypeKHR::eTopLevel, topSizes.accelerationStructureSize, mTopLevel)) {
        reset();
        return false;
    }
    topInfo.dstAccelerationStructure = mTopLevel.handle;
    mScratchSize = std::max(mScratchSize, topSizes.buildScratchSize);

    if (mScratchSize > std::numeric_limits<uint64_t>::max() - mScratchAlignment) {
        reset();
        return false;
    }
    scratchBuffer = Buffer::create(name + "/scratch", {.context = mGpu, .size = mScratchSize + mScratchAlignment});
    if (!scratchBuffer || !scratchBuffer->gpuAddress()) {
        reset();
        return false;
    }
    mBuildRecorded = true;
    return true;
}

bool VkAccelerationStructureVulkan::recordBuild(vk::CommandBuffer commandBuffer, GpuResourceStateTrackerVulkan & tracker, const AutoRef<Buffer> & scratchBuffer) {
    if (!mBuildRecorded || !commandBuffer || !scratchBuffer) return false;
    if (!tracker.addAccelerationStructure(this, true, vk::PipelineStageFlagBits::eAccelerationStructureBuildKHR)) return false;
    if (!tracker.addAccelerationStructureBuildInput(RuntimeType::cast<BufferVulkan>(mTransformBuffer.get()))) return false;
    if (!tracker.addAccelerationStructureBuildInput(RuntimeType::cast<BufferVulkan>(mInstanceBuffer.get()))) return false;
    for (const TriangleGeometry & geometry : mGeometries) {
        if (!tracker.addAccelerationStructureBuildInput(RuntimeType::cast<BufferVulkan>(geometry.vertexBuffer.get()))) return false;
        if (geometry.indexBuffer && !tracker.addAccelerationStructureBuildInput(RuntimeType::cast<BufferVulkan>(geometry.indexBuffer.get()))) return false;
    }
    tracker.emitPrePassBarriers(commandBuffer);

    std::vector<vk::AccelerationStructureGeometryKHR> bottomGeometries;
    std::vector<vk::AccelerationStructureBuildRangeInfoKHR> ranges;
    std::vector<uint32_t> primitiveCounts;
    bottomGeometries.reserve(mGeometries.size());
    ranges.reserve(mGeometries.size());
    primitiveCounts.reserve(mGeometries.size());
    for (size_t i = 0; i < mGeometries.size(); ++i) {
        const TriangleGeometry & source = mGeometries[i];
        auto * vertex = RuntimeType::cast<BufferVulkan>(source.vertexBuffer.get());
        auto * index = RuntimeType::cast<BufferVulkan>(source.indexBuffer.get());
        vk::AccelerationStructureGeometryTrianglesDataKHR triangles;
        triangles.vertexFormat = vk::Format::eR32G32B32Sfloat;
        triangles.vertexData.deviceAddress = vertex->gpuAddress() + source.vertexOffset + source.positionOffset;
        triangles.vertexStride = source.vertexStride;
        triangles.maxVertex = source.vertexCount - 1;
        triangles.transformData.deviceAddress = mTransformBuffer->gpuAddress() + i * sizeof(vk::TransformMatrixKHR);
        if (index) {
            triangles.indexData.deviceAddress = index->gpuAddress() + source.indexOffset;
            triangles.indexType = source.indexFormat == IndexFormat::UINT16 ? vk::IndexType::eUint16 : vk::IndexType::eUint32;
        } else {
            triangles.indexType = vk::IndexType::eNoneKHR;
        }
        vk::AccelerationStructureGeometryKHR geometry;
        geometry.geometryType = vk::GeometryTypeKHR::eTriangles;
        geometry.flags = vk::GeometryFlagBitsKHR::eOpaque;
        geometry.geometry.triangles = triangles;
        bottomGeometries.push_back(geometry);
        const uint32_t primitiveCount = source.indexFormat == IndexFormat::NONE ? source.vertexCount / 3 : source.indexCount / 3;
        ranges.push_back({primitiveCount, 0, 0, 0});
        primitiveCounts.push_back(primitiveCount);
    }

    const uint64_t scratchAddress = scratchBuffer->gpuAddress();
    const uint64_t alignedScratchAddress = (scratchAddress + mScratchAlignment - 1) / mScratchAlignment * mScratchAlignment;
    vk::AccelerationStructureBuildGeometryInfoKHR bottomInfo;
    bottomInfo.type = vk::AccelerationStructureTypeKHR::eBottomLevel;
    bottomInfo.flags = mPreferFastTrace ? vk::BuildAccelerationStructureFlagBitsKHR::ePreferFastTrace : vk::BuildAccelerationStructureFlagBitsKHR::ePreferFastBuild;
    bottomInfo.mode = vk::BuildAccelerationStructureModeKHR::eBuild;
    bottomInfo.dstAccelerationStructure = mBottomLevel.handle;
    bottomInfo.geometryCount = static_cast<uint32_t>(bottomGeometries.size());
    bottomInfo.pGeometries = bottomGeometries.data();
    bottomInfo.scratchData.deviceAddress = alignedScratchAddress;
    std::vector<const vk::AccelerationStructureBuildRangeInfoKHR *> rangePointers = {ranges.data()};
    commandBuffer.buildAccelerationStructuresKHR(1, &bottomInfo, rangePointers.data());

    vk::MemoryBarrier bottomToTop;
    bottomToTop.srcAccessMask = vk::AccessFlagBits::eAccelerationStructureWriteKHR;
    bottomToTop.dstAccessMask = vk::AccessFlagBits::eAccelerationStructureReadKHR;
    commandBuffer.pipelineBarrier(vk::PipelineStageFlagBits::eAccelerationStructureBuildKHR, vk::PipelineStageFlagBits::eAccelerationStructureBuildKHR, {}, 1, &bottomToTop,
                                  0, nullptr, 0, nullptr);

    vk::AccelerationStructureGeometryInstancesDataKHR instanceData;
    instanceData.data.deviceAddress = mInstanceBuffer->gpuAddress();
    vk::AccelerationStructureGeometryKHR topGeometry;
    topGeometry.geometryType = vk::GeometryTypeKHR::eInstances;
    topGeometry.geometry.instances = instanceData;
    vk::AccelerationStructureBuildGeometryInfoKHR topInfo;
    topInfo.type = vk::AccelerationStructureTypeKHR::eTopLevel;
    topInfo.flags = mPreferFastTrace ? vk::BuildAccelerationStructureFlagBitsKHR::ePreferFastTrace : vk::BuildAccelerationStructureFlagBitsKHR::ePreferFastBuild;
    topInfo.mode = vk::BuildAccelerationStructureModeKHR::eBuild;
    topInfo.dstAccelerationStructure = mTopLevel.handle;
    topInfo.geometryCount = 1;
    topInfo.pGeometries = &topGeometry;
    topInfo.scratchData.deviceAddress = alignedScratchAddress;
    const vk::AccelerationStructureBuildRangeInfoKHR topRange {1, 0, 0, 0};
    const vk::AccelerationStructureBuildRangeInfoKHR * topRangePtr = &topRange;
    commandBuffer.buildAccelerationStructuresKHR(1, &topInfo, &topRangePtr);
    tracker.restoreAccelerationStructureToRead(this, commandBuffer);
    mBuildEncoded = true;
    return true;
}

void VkAccelerationStructureVulkan::releaseBuildInputs() {
    mTransformBuffer.clear();
    mInstanceBuffer.clear();
    mGeometries.clear();
}

void VkAccelerationStructureVulkan::reset() {
    if (mGpu && mGpu->ready()) {
        const auto device = mGpu->vulkanDevice().handle();
        if (mTopLevel.handle) device.destroyAccelerationStructureKHR(mTopLevel.handle);
        if (mBottomLevel.handle) device.destroyAccelerationStructureKHR(mBottomLevel.handle);
    }
    mTopLevel = {};
    mBottomLevel = {};
    mScratchSize = 0;
    mPreferFastTrace = true;
    mBuildRecorded = false;
    mBuildEncoded = false;
    mBuildSubmitted = false;
    releaseBuildInputs();
}

AutoRef<AccelerationStructure> createAccelerationStructureVulkan(const StrA & name, AutoRef<GpuContextVulkan2> gpu,
                                                                 const AccelerationStructure::BuildParameters & params, CnC & recorder,
                                                                 AutoRef<Buffer> & scratchBuffer) {
    if (!gpu || !gpu->caps().accelerationStructures) {
        GN_ERROR(sLogger, "Cannot build acceleration structure '{}': device does not support acceleration structures", name);
        return {};
    }
    auto resource = std::make_unique<VkAccelerationStructureVulkan>(name, std::move(gpu));
    if (!resource->init(params, recorder, scratchBuffer)) return {};
    return AutoRef<AccelerationStructure>(resource.release());
}

} // namespace GN::gpu2
