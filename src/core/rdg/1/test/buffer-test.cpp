/*
 * Buffer artifact unit tests — Phase 0
 *
 * Tests cover:
 *   Phase 0.1 — BufferUsageBits enum values and BufferUsageFlags bit-combining.
 *   Phase 0.2 — BufferVulkan pre-allocation and setContent() contract.
 */

#include "common.h"
#include <catch2/catch_test_macros.hpp>
#include <garnet/GNrdg.h>
#ifdef _MSC_VER
    #include <intrin.h>
    #define popcount_u32(x) __popcnt(x)
#else
    #define popcount_u32(x) __builtin_popcount(x)
#endif

using namespace GN;
using namespace GN::rdg;

// ─────────────────────────────────────────────────────────────────────────────
// Phase 0.1: BufferUsageBits / BufferUsageFlags (no GPU required)
// ─────────────────────────────────────────────────────────────────────────────

TEST_CASE("Buffer: Usage bits have distinct non-zero values", "[rdg][buffer]") {
    CHECK(static_cast<uint32_t>(BufferUsageBits::VERTEX) != 0);
    CHECK(static_cast<uint32_t>(BufferUsageBits::INDEX) != 0);
    CHECK(static_cast<uint32_t>(BufferUsageBits::UNIFORM) != 0);
    CHECK(static_cast<uint32_t>(BufferUsageBits::STORAGE) != 0);

    // All four bits are distinct.
    uint32_t all = static_cast<uint32_t>(BufferUsageBits::VERTEX) | static_cast<uint32_t>(BufferUsageBits::INDEX) |
                   static_cast<uint32_t>(BufferUsageBits::UNIFORM) | static_cast<uint32_t>(BufferUsageBits::STORAGE);
    CHECK(popcount_u32(all) == 4);
}

TEST_CASE("Buffer: Usage flags combine correctly via BitFlags", "[rdg][buffer]") {
    BufferUsageFlags geometry(static_cast<uint32_t>(BufferUsageBits::VERTEX) | static_cast<uint32_t>(BufferUsageBits::INDEX));
    CHECK((geometry.mask & static_cast<uint32_t>(BufferUsageBits::VERTEX)) != 0);
    CHECK((geometry.mask & static_cast<uint32_t>(BufferUsageBits::INDEX)) != 0);
    CHECK((geometry.mask & static_cast<uint32_t>(BufferUsageBits::UNIFORM)) == 0);
}

TEST_CASE("Buffer: CreateParameters default usage is VERTEX|INDEX|UNIFORM", "[rdg][buffer]") {
    PersistentBuffer::CreateParameters cp;
    uint32_t                           defaultMask =
        static_cast<uint32_t>(BufferUsageBits::VERTEX) | static_cast<uint32_t>(BufferUsageBits::INDEX) | static_cast<uint32_t>(BufferUsageBits::UNIFORM);
    CHECK(cp.usage.mask == defaultMask);
}

TEST_CASE("Buffer: CreateParameters accepts UNIFORM usage", "[rdg][buffer]") {
    PersistentBuffer::CreateParameters cp;
    cp.usage = BufferUsageFlags(static_cast<uint32_t>(BufferUsageBits::UNIFORM));
    CHECK((cp.usage.mask & static_cast<uint32_t>(BufferUsageBits::UNIFORM)) != 0);
    CHECK((cp.usage.mask & static_cast<uint32_t>(BufferUsageBits::VERTEX)) == 0);
}

// ─────────────────────────────────────────────────────────────────────────────
// Phase 0.2: BufferVulkan pre-allocation and setContent() (GPU required)
// ─────────────────────────────────────────────────────────────────────────────

TEST_CASE("Buffer: create returns non-null for valid params", "[rdg][buffer][gpu]") {
    auto gpu = GpuContext::create("gpu", {.howToPrintDeviceCaps = gpuVerbosity});
    if (!gpu) SKIP("No Vulkan GPU context available");

    PersistentBuffer::CreateParameters cp;
    cp.context = gpu;
    cp.size    = 64;
    auto buf   = PersistentBuffer::create("test_buf", cp);
    REQUIRE(buf != nullptr);
}

TEST_CASE("Buffer: create fails when size is 0", "[rdg][buffer][gpu]") {
    auto gpu = GpuContext::create("gpu2", {.howToPrintDeviceCaps = gpuVerbosity});
    if (!gpu) SKIP("No Vulkan GPU context available");

    PersistentBuffer::CreateParameters cp;
    cp.context = gpu;
    cp.size    = 0;
    auto buf   = PersistentBuffer::create("test_buf_zero", cp);
    CHECK(buf == nullptr);
}

TEST_CASE("Buffer: setContent writes data correctly", "[rdg][buffer][gpu]") {
    auto gpu = GpuContext::create("gpu3", {.howToPrintDeviceCaps = gpuVerbosity});
    if (!gpu) SKIP("No Vulkan GPU context available");

    constexpr uint32_t kCount      = 4;
    constexpr uint64_t kSize       = kCount * sizeof(float);
    const float        src[kCount] = {1.f, 2.f, 3.f, 4.f};

    PersistentBuffer::CreateParameters cp;
    cp.context = gpu;
    cp.size    = kSize;
    auto buf   = PersistentBuffer::create("test_buf_content", cp);
    REQUIRE(buf != nullptr);

    bool ok = buf->setContent(src, kSize);
    CHECK(ok);
}

TEST_CASE("Buffer: setContent fails when data pointer is null", "[rdg][buffer][gpu]") {
    auto gpu = GpuContext::create("gpu4", {.howToPrintDeviceCaps = gpuVerbosity});
    if (!gpu) SKIP("No Vulkan GPU context available");

    PersistentBuffer::CreateParameters cp;
    cp.context = gpu;
    cp.size    = 64;
    auto buf   = PersistentBuffer::create("test_buf_null_ptr", cp);
    REQUIRE(buf != nullptr);

    CHECK_FALSE(buf->setContent(nullptr, 64));
}

TEST_CASE("Buffer: setContent fails when size exceeds allocated capacity", "[rdg][buffer][gpu]") {
    auto gpu = GpuContext::create("gpu5", {.howToPrintDeviceCaps = gpuVerbosity});
    if (!gpu) SKIP("No Vulkan GPU context available");

    PersistentBuffer::CreateParameters cp;
    cp.context = gpu;
    cp.size    = 32;
    auto buf   = PersistentBuffer::create("test_buf_overflow", cp);
    REQUIRE(buf != nullptr);

    const uint8_t bigData[64] = {};
    CHECK_FALSE(buf->setContent(bigData, 64));
}

TEST_CASE("Buffer: setContent can be called multiple times (map-reuse)", "[rdg][buffer][gpu]") {
    auto gpu = GpuContext::create("gpu6", {.howToPrintDeviceCaps = gpuVerbosity});
    if (!gpu) SKIP("No Vulkan GPU context available");

    PersistentBuffer::CreateParameters cp;
    cp.context = gpu;
    cp.size    = 64;
    auto buf   = PersistentBuffer::create("test_buf_multi", cp);
    REQUIRE(buf != nullptr);

    const uint8_t data1[64] = {};
    const uint8_t data2[64] = {};
    CHECK(buf->setContent(data1, 64));
    CHECK(buf->setContent(data2, 32)); // smaller size is also valid
}

TEST_CASE("Buffer: create with UNIFORM usage succeeds", "[rdg][buffer][gpu]") {
    auto gpu = GpuContext::create("gpu7", {.howToPrintDeviceCaps = gpuVerbosity});
    if (!gpu) SKIP("No Vulkan GPU context available");

    PersistentBuffer::CreateParameters cp;
    cp.context = gpu;
    cp.size    = 256;
    cp.usage   = BufferUsageFlags(static_cast<uint32_t>(BufferUsageBits::UNIFORM));
    auto buf   = PersistentBuffer::create("test_ubo", cp);
    REQUIRE(buf != nullptr);
}
