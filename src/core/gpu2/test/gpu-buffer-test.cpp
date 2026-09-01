#include <catch2/catch_test_macros.hpp>
#include <garnet/GNgpu2.h>

using namespace GN;
using namespace GN::gpu2;

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
