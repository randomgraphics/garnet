#include <catch2/catch_test_macros.hpp>
#if GN_BUILD_HAS_VULKAN
    #include "../vk-gpu-context.h"
    #include "../vk-buffer.h"
#endif
#include <garnet/GNgpu2.h>

using namespace GN;
using namespace GN::gpu2;

#if GN_BUILD_HAS_VULKAN
TEST_CASE("GPU2: suballocated buffers map independent ranges", "[gpu2][buffer][vma][gpu]") {
    auto gpu = GpuContext::create("vma-mapping", {.debug = GpuContext::DebugMode::ENABLED, .howToPrintDeviceCaps = GpuContext::Verbosity::SILENCE});
    if (!gpu) SKIP("No GPU context available");
    auto vkGpu = RuntimeType::cast<GpuContextVulkan2>(gpu.get());
    REQUIRE(vkGpu);
    const auto * gi = vkGpu->vulkanDevice().gi();
    REQUIRE(gi->vmaAllocator);

    VmaTotalStatistics before {}, during {}, after {};
    vmaCalculateStatistics(gi->vmaAllocator, &before);
    std::vector<rv::Ref<rv::Buffer>>      buffers;
    std::vector<rv::Buffer::MappedResult> mappings;
    constexpr uint32_t                    count = 128;
    for (uint32_t i = 0; i < count; ++i) {
        auto buffer = rv::Ref<rv::Buffer>(new rv::Buffer(rv::Buffer::ConstructParameters {{"vma-range"}, gi, 256}.setStaging()));
        auto mapped = buffer->map({64, 64});
        REQUIRE(mapped.data);
        REQUIRE(mapped.offset == 64);
        REQUIRE(mapped.size == 64);
        std::memset(mapped.data, static_cast<int>(i), 64);
        buffers.push_back(buffer);
        mappings.push_back(mapped);
    }
    vmaCalculateStatistics(gi->vmaAllocator, &during);
    CHECK(during.total.statistics.allocationCount == before.total.statistics.allocationCount + count);
    // Independent Vulkan buffers must share a small number of device-memory blocks.
    CHECK(during.total.statistics.blockCount < before.total.statistics.blockCount + count / 2);
    for (uint32_t i = 0; i < count; ++i) {
        for (uint32_t j = 0; j < 64; ++j) REQUIRE(mappings[i].data[j] == i);
        buffers[i]->unmap();
        auto mapped = buffers[i]->map({});
        REQUIRE(mapped.data);
        for (uint32_t j = 64; j < 128; ++j) REQUIRE(mapped.data[j] == i);
        buffers[i]->unmap();
        // Releasing one allocation must not invalidate its still-mapped neighbors.
        buffers[i].clear();
    }
    vmaCalculateStatistics(gi->vmaAllocator, &after);
    CHECK(after.total.statistics.allocationCount == before.total.statistics.allocationCount);
}

TEST_CASE("GPU2: buffer destruction preserves outstanding native references", "[gpu2][buffer][gpu]") {
    auto gpu = GpuContext::create("buffer-lifetime", {.debug = GpuContext::DebugMode::ENABLED, .howToPrintDeviceCaps = GpuContext::Verbosity::SILENCE});
    if (!gpu) SKIP("No GPU context available");
    auto original = Buffer::create("retained-buffer", {.context = gpu, .size = 192, .mappable = true});
    REQUIRE(original);
    {
        auto mapped = original->map();
        REQUIRE_FALSE(mapped.empty());
        std::memset(mapped.data(), 0x5a, mapped.size());
    }
    auto native = RuntimeType::cast<BufferVulkan>(original.get())->rvBuffer();
    original.clear();
    // Command buffers retain native buffers after logical wrappers can disappear.
    // Allocating/releasing more wrappers must never alias that outstanding reference.
    for (unsigned i = 0; i < 64; ++i) {
        auto next = Buffer::create("temporary-buffer", {.context = gpu, .size = 192, .mappable = true});
        REQUIRE(next);
        CHECK(RuntimeType::cast<BufferVulkan>(next.get())->nativeBuffer() != native->handle());
        auto mapped = next->map();
        REQUIRE_FALSE(mapped.empty());
        std::memset(mapped.data(), int(i), mapped.size());
    }
    auto mapped = native->map({});
    REQUIRE(mapped.data);
    for (size_t i = 0; i < mapped.size; ++i) CHECK(mapped.data[i] == 0x5a);
    native->unmap();
}

TEST_CASE("GPU2: rapid-vulkan dedicated allocation fallback maps correctly", "[gpu2][buffer][vma][gpu]") {
    auto gpu = GpuContext::create("vma-fallback", {.debug = GpuContext::DebugMode::ENABLED, .howToPrintDeviceCaps = GpuContext::Verbosity::SILENCE});
    if (!gpu) SKIP("No GPU context available");
    auto vkGpu = RuntimeType::cast<GpuContextVulkan2>(gpu.get());
    REQUIRE(vkGpu);
    rv::Device::ConstructParameters params;
    params.instance           = vkGpu->vulkanDevice().gi()->instance;
    params.enableVmaAllocator = false;
    params.printVkInfo        = rv::Device::SILENCE;
    rv::Device device(params);
    REQUIRE_FALSE(device.gi()->vmaAllocator);
    rv::Buffer buffer(rv::Buffer::ConstructParameters {{"dedicated-range"}, device.gi(), 256}.setStaging());
    auto       mapped = buffer.map({64, 64});
    REQUIRE(mapped.data);
    std::memset(mapped.data, 0x5a, 64);
    buffer.unmap();
    mapped = buffer.map({});
    REQUIRE(mapped.data);
    for (uint32_t i = 64; i < 128; ++i) REQUIRE(mapped.data[i] == 0x5a);
    buffer.unmap();
}
#endif

static AutoRef<GpuContext> makeGpu() {
    return GpuContext::create("gpu", GpuContext::CreateParameters {.howToPrintDeviceCaps = GpuContext::Verbosity::SILENCE});
}

TEST_CASE("GPU2: Buffer setContent and readContent (device-local)", "[gpu2][buffer][gpu]") {
    auto gpu = makeGpu();
    if (!gpu) SKIP("No GPU context available");

    const std::vector<uint32_t> src = {1, 2, 3, 4, 5, 6, 7, 8};
    const size_t                sz  = src.size() * sizeof(uint32_t);

    auto buf = Buffer::create("test-buf", Buffer::CreateParameters {.context = gpu, .size = sz, .mappable = false});
    if (!buf) SKIP("Device-local buffer unavailable");

    REQUIRE(buf->setContent(ArrayView<const uint8_t>((const uint8_t *) src.data(), sz)));

    auto dst = buf->readContent();
    REQUIRE(dst.size() == sz);
    REQUIRE(std::memcmp(dst.data(), src.data(), sz) == 0);
}

TEST_CASE("GPU2: Buffer setContent with offset (device-local)", "[gpu2][buffer][gpu]") {
    auto gpu = makeGpu();
    if (!gpu) SKIP("No GPU context available");

    constexpr size_t kCount = 8;
    constexpr size_t kSz    = kCount * sizeof(uint32_t);

    auto buf = Buffer::create("test-buf-offset", Buffer::CreateParameters {.context = gpu, .size = kSz, .mappable = false});
    if (!buf) SKIP("Device-local buffer unavailable");

    // Zero-fill then write the second half
    const std::vector<uint32_t> zeros(kCount, 0);
    REQUIRE(buf->setContent(ArrayView<const uint8_t>((const uint8_t *) zeros.data(), kSz)));

    const std::vector<uint32_t> patch = {0xDEAD, 0xBEEF, 0xCAFE, 0xF00D};
    const size_t                half  = kSz / 2;
    REQUIRE(buf->setContent(ArrayView<const uint8_t>((const uint8_t *) patch.data(), half), half));

    auto dst = buf->readContent();
    REQUIRE(dst.size() == kSz);

    auto * u32 = (const uint32_t *) dst.data();
    for (size_t i = 0; i < kCount / 2; ++i) CHECK(u32[i] == 0u);
    for (size_t i = 0; i < patch.size(); ++i) CHECK(u32[kCount / 2 + i] == patch[i]);
}

TEST_CASE("GPU2: Buffer map, write, and readContent (mappable)", "[gpu2][buffer][gpu]") {
    auto gpu = makeGpu();
    if (!gpu) SKIP("No GPU context available");

    const std::vector<float> src = {1.f, 2.f, 3.f, 4.f};
    const size_t             sz  = src.size() * sizeof(float);

    auto buf = Buffer::create("test-mappable", Buffer::CreateParameters {.context = gpu, .size = sz, .mappable = true});
    if (!buf) SKIP("Mappable buffer unavailable");

    {
        auto mapped = buf->map();
        REQUIRE_FALSE(mapped.empty());
        REQUIRE(mapped.size() == sz);
        std::memcpy(mapped.data(), src.data(), sz);
    } // unmap on scope exit

    auto dst = buf->readContent();
    REQUIRE(dst.size() == sz);
    REQUIRE(std::memcmp(dst.data(), src.data(), sz) == 0);
}

TEST_CASE("GPU2: Buffer map non-mappable returns empty", "[gpu2][buffer]") {
    auto gpu = makeGpu();
    if (!gpu) SKIP("No GPU context available");

    auto buf = Buffer::create("non-mappable", Buffer::CreateParameters {.context = gpu, .size = 64, .mappable = false});
    if (!buf) SKIP("Buffer unavailable");

    auto mapped = buf->map();
    CHECK(mapped.empty());
}

TEST_CASE("GPU2: Buffer map already-mapped returns empty", "[gpu2][buffer][gpu]") {
    auto gpu = makeGpu();
    if (!gpu) SKIP("No GPU context available");

    auto buf = Buffer::create("double-map", Buffer::CreateParameters {.context = gpu, .size = 64, .mappable = true});
    if (!buf) SKIP("Mappable buffer unavailable");

    auto first = buf->map();
    REQUIRE_FALSE(first.empty());

    auto second = buf->map();
    CHECK(second.empty());
}

TEST_CASE("GPU2: Buffer create with zero size fails", "[gpu2][buffer]") {
    auto gpu = makeGpu();
    if (!gpu) SKIP("No GPU context available");

    auto buf = Buffer::create("zero-size", Buffer::CreateParameters {.context = gpu, .size = 0});
    CHECK_FALSE(buf);
}
