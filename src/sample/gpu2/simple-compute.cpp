/// simple-compute: demonstrates bindless CnC compute dispatch.
///
/// Fills a GPU storage buffer with the values [0, 1, 2, ..., N-1], dispatches
/// a compute shader that adds 10.0 to each element, then reads back the buffer
/// and verifies the result.
#include <garnet/GNgpu2.h>

#include <atomic>
#include <cstdio>
#include <cstring>
#include <vector>

// SPIR-V blob generated from shaders/simple-compute.comp by CMake script.
#include "simple-compute-comp.spv.h"

using namespace GN;
using namespace GN::gpu2;

struct PushConstants {
    uint32_t count;
    float    increment;
    uint32_t dataIndex;
};

static bool submitAndWait(GpuContext * gpu, AutoRef<GpuPayload> payload) {
    std::atomic<bool> done = false;
    gpu->submit(GpuContext::SubmitParameters("compute").appendWork(payload).setOnComplete([&done] { done = true; }));
    while (!done) gpu->pump();
    return true;
}

int main() {
    auto gpu = GpuContext::create("gpu", GpuContext::CreateParameters {});
    if (!gpu) {
        std::fprintf(stderr, "Failed to create GPU context\n");
        return -1;
    }

    constexpr size_t   COUNT     = 256;
    constexpr float    INCREMENT = 10.0f;
    constexpr uint64_t BYTE_SIZE = COUNT * sizeof(float);

    // Device-local storage buffer; use setContent for initial upload.
    auto buf = Buffer::create("data", {.context = gpu, .size = BYTE_SIZE});
    if (!buf) {
        std::fprintf(stderr, "Failed to create storage buffer\n");
        return -1;
    }
    {
        std::vector<float> initData(COUNT);
        for (size_t i = 0; i < COUNT; ++i) initData[i] = (float) i;
        if (!buf->setContent({reinterpret_cast<const uint8_t *>(initData.data()), BYTE_SIZE})) {
            std::fprintf(stderr, "Failed to upload initial data\n");
            return -1;
        }
    }

    // Compile compute shader from the embedded SPIR-V blob.
    auto cs =
        GpuShader::create({.context = gpu, .name = "simple-compute", .binary = kSimpleComputeCompSpv, .size = sizeof(kSimpleComputeCompSpv), .entry = "main"});
    if (!cs) {
        std::fprintf(stderr, "Failed to create compute shader\n");
        return -1;
    }

    // The shader resolves this storage buffer through the descriptor heap.
    auto heap = bindless::DescriptorHeap::create("simple-compute-heap", {.gpu = gpu, .capacity = 1});
    if (!heap) {
        std::fprintf(stderr, "Failed to create bindless descriptor heap\n");
        return -1;
    }
    const auto bufferView = GpuResourceView {buf}.setBufferViewType(GpuResourceView::BufferView::STORAGE);
    const auto dataIndex  = heap->allocate(bindless::DescriptorHeap::STORAGE_BUFFER, bufferView);
    if (dataIndex == bindless::DescriptorHeap::INVALID_DESCRIPTOR_INDEX) {
        std::fprintf(stderr, "Failed to allocate storage-buffer descriptor\n");
        return -1;
    }

    // Declare the writable resource for pass-level hazard tracking (set 1).
    GpuResourceTable resources;
    resources.resize(2);
    resources[1].resize(1);
    resources[1][0].append(bufferView);

    // Dispatch: one thread per element, in groups of 64.
    constexpr uint32_t GROUPS = (COUNT + 63) / 64;

    PushConstants pc {(uint32_t) COUNT, INCREMENT, dataIndex.slot};

    auto cnc = bindless::CnC::create("simple-compute", {.gpu = gpu, .heap = heap, .passResources = resources});
    if (!cnc) {
        std::fprintf(stderr, "Failed to create bindless CnC\n");
        return -1;
    }
    bindless::CnC::ComputeParameters cp;
    cp.cs         = cs;
    cp.immediates = {reinterpret_cast<const uint8_t *>(&pc), sizeof(pc)};
    cp.x          = GROUPS;
    cnc->recordCompute(cp);
    submitAndWait(gpu.get(), cnc->seal());

    // Read back and verify.
    std::vector<uint8_t> raw = buf->readContent();
    if (raw.size() != BYTE_SIZE) {
        std::fprintf(stderr, "Readback size mismatch: got %zu, expected %zu\n", raw.size(), (size_t) BYTE_SIZE);
        return -1;
    }
    const float * result = reinterpret_cast<const float *>(raw.data());
    for (size_t i = 0; i < COUNT; ++i) {
        float expected = (float) i + INCREMENT;
        if (std::abs(result[i] - expected) > 1e-4f) {
            std::fprintf(stderr, "Data mismatch at index %zu: got %f, expected %f\n", i, result[i], expected);
            return -1;
        }
    }

    std::fprintf(stdout, "simple-compute: PASSED — %zu floats incremented by %.1f and verified.\n", COUNT, (double) INCREMENT);
    return 0;
}
