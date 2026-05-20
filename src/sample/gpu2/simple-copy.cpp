/// simple-copy: demonstrates GpuCnC buffer-to-buffer copy.
///
/// Creates two GPU buffers, fills the source with sequential float values,
/// copies it to the destination via GpuCnC::copyBufferToBuffer(), then reads
/// back the destination and verifies the data.
#include <garnet/GNgpu2.h>

#include <atomic>
#include <cstdio>
#include <cstring>
#include <vector>

using namespace GN;
using namespace GN::gpu2;

static bool submitAndWait(GpuContext * gpu, AutoRef<GpuPayload> payload) {
    std::atomic<bool> done = false;
    gpu->submit(GpuContext::SubmitParameters("copy").appendWork(payload).setOnComplete([&done] { done = true; }));
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
    constexpr uint64_t BYTE_SIZE = COUNT * sizeof(float);

    // Source: CPU-visible (mappable) buffer.
    auto src = Buffer::create("src", {.context = gpu, .size = BYTE_SIZE, .mappable = true});
    if (!src) {
        std::fprintf(stderr, "Failed to create source buffer\n");
        return -1;
    }
    {
        auto m = src->map();
        if (m.empty()) {
            std::fprintf(stderr, "Failed to map source buffer\n");
            return -1;
        }
        auto * p = static_cast<float *>(m.data());
        for (size_t i = 0; i < COUNT; ++i) p[i] = (float) i;
    }

    // Destination: device-local buffer.
    auto dst = Buffer::create("dst", {.context = gpu, .size = BYTE_SIZE});
    if (!dst) {
        std::fprintf(stderr, "Failed to create destination buffer\n");
        return -1;
    }

    // Record and submit the copy.
    auto cnc = GpuCnC::create({.gpu = gpu});
    if (!cnc) {
        std::fprintf(stderr, "Failed to create GpuCnC\n");
        return -1;
    }
    cnc->copyBufferToBuffer({.src = src, .dst = dst, .size = BYTE_SIZE});
    submitAndWait(gpu.get(), cnc->seal());

    // Read back and verify.
    std::vector<uint8_t> raw = dst->readContent();
    if (raw.size() != BYTE_SIZE) {
        std::fprintf(stderr, "Readback size mismatch: got %zu, expected %zu\n", raw.size(), (size_t) BYTE_SIZE);
        return -1;
    }
    const float * result = reinterpret_cast<const float *>(raw.data());
    for (size_t i = 0; i < COUNT; ++i) {
        if (result[i] != (float) i) {
            std::fprintf(stderr, "Data mismatch at index %zu: got %f, expected %f\n", i, result[i], (float) i);
            return -1;
        }
    }

    std::fprintf(stdout, "simple-copy: PASSED — %zu floats copied and verified.\n", COUNT);
    return 0;
}
