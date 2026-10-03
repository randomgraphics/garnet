#include <catch2/catch_test_macros.hpp>
#if GN_BUILD_HAS_VULKAN
    #include "../vk-gpu-context.h"
    #include "../vk-transient-buffer.h"
#endif
#include <garnet/GNgpu2.h>

using namespace GN;
using namespace GN::gpu2;

#if GN_BUILD_HAS_VULKAN
TEST_CASE("GPU2: TransientArena allocates aligned suballocations with GPU addresses", "[gpu2][transient][gpu]") {
    auto gpu = GpuContext::create("transient-test", {.debug = GpuContext::DebugMode::ENABLED, .howToPrintDeviceCaps = GpuContext::Verbosity::SILENCE});
    if (!gpu) SKIP("No GPU context available");

    auto arena = AutoRef<TransientArenaVulkan>(new TransientArenaVulkan("test-arena", {.context = gpu, .suggestedArenaSize = 1024 * 1024}));
    REQUIRE(arena);

    // Allocate first transient buffer (48 bytes, 16-byte aligned)
    auto b1 = arena->allocate(48, 16, "mat1");
    REQUIRE(b1);
    CHECK(b1->bufferOffset() == 0);
    CHECK(b1->gpuAddress() != 0);

    // Write content
    std::vector<uint8_t> data1(48, 0xAB);
    CHECK(b1->setContent(data1));
    auto read1 = b1->readContent();
    CHECK(read1 == data1);

    // Allocate second transient buffer (80 bytes, 64-byte aligned)
    auto b2 = arena->allocate(80, 64, "mat2");
    REQUIRE(b2);
    CHECK(b2->bufferOffset() >= 48);
    CHECK((b2->bufferOffset() % 64) == 0);
    CHECK(b2->gpuAddress() == b1->gpuAddress() + b2->bufferOffset());

    std::vector<uint8_t> data2(80, 0xCD);
    CHECK(b2->setContent(data2));
    auto read2 = b2->readContent();
    CHECK(read2 == data2);

    // Verify b1 was not corrupted
    CHECK(b1->readContent() == data1);

    // Test map()
    {
        auto mapped = b1->map();
        REQUIRE(mapped.data());
        CHECK(mapped.size() == 48);
        CHECK(static_cast<const uint8_t *>(mapped.data())[0] == 0xAB);
    }
}

TEST_CASE("GPU2: TransientArena recycles backing buffers when suballocations are released", "[gpu2][transient][gpu]") {
    auto gpu = GpuContext::create("transient-recycle", {.debug = GpuContext::DebugMode::ENABLED, .howToPrintDeviceCaps = GpuContext::Verbosity::SILENCE});
    if (!gpu) SKIP("No GPU context available");

    auto arena = AutoRef<TransientArenaVulkan>(new TransientArenaVulkan("recycle-arena", {.context = gpu, .suggestedArenaSize = 1024 * 1024}));
    REQUIRE(arena);

    {
        auto b1 = arena->allocate(128, 64, "temp1");
        auto b2 = arena->allocate(256, 64, "temp2");
        REQUIRE(b1);
        REQUIRE(b2);
        CHECK(arena->numBackingBuffers() == 1);
        CHECK(arena->backingLiveCount(0) == 2);
    }

    // Both b1 and b2 went out of scope; liveCount should now be 0.
    CHECK(arena->backingLiveCount(0) == 0);

    // Reset marks unused backing buffers as recycled
    arena->reset();

    // Next allocation should reuse the same backing buffer from offset 0
    auto b3 = arena->allocate(64, 16, "reused");
    REQUIRE(b3);
    CHECK(arena->numBackingBuffers() == 1); // No new backing buffer allocated
    CHECK(b3->bufferOffset() == 0);
    CHECK(arena->backingLiveCount(0) == 1);
}

TEST_CASE("GPU2: Buffer::create with transient=true creates device-addressable buffer", "[gpu2][transient][gpu]") {
    auto gpu = GpuContext::create("transient-factory-test", {.debug = GpuContext::DebugMode::ENABLED, .howToPrintDeviceCaps = GpuContext::Verbosity::SILENCE});
    if (!gpu) SKIP("No GPU context available");

    auto buf = Buffer::create("transient-buf", {.context = gpu, .size = 64, .transient = true});
    REQUIRE(buf);
    CHECK(buf->gpuAddress() != 0);

    std::vector<uint8_t> data(64, 0xEF);
    CHECK(buf->setContent(data));
    CHECK(buf->readContent() == data);
}
#endif
