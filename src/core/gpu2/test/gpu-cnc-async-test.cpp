// Tests for GpuCnC async upload/download: uploadBuffer / downloadBuffer / downloadImage.
// These exercise the staging-buffer machinery owned by the sealed payload and the std::future
// based read-back that resolves from GpuContext::pump() after the GPU fence signals.

#include <catch2/catch_test_macros.hpp>
#include "gpu2-test-helpers.h"

#include <cstring>
#include <future>
#include <vector>

using namespace GN;
using namespace GN::gpu2;

// ── uploadBuffer ──────────────────────────────────────────────────────────────────────

TEST_CASE("GPU2/CnC async: uploadBuffer copies content into device-local buffer", "[gpu2][cnc][async][gpu]") {
    auto gpu = makeGpu();
    if (!gpu) SKIP("No GPU context available");

    constexpr size_t   N    = 64;
    constexpr uint64_t SIZE = N * sizeof(uint32_t);

    std::vector<uint32_t> expected(N);
    for (size_t i = 0; i < N; ++i) expected[i] = (uint32_t) (i * 7 + 1);

    // Device-local destination (NOT mappable): upload must route through an internal staging buffer.
    auto dst = Buffer::create("dst", {.context = gpu, .size = SIZE});
    REQUIRE(dst);

    auto cnc = GpuCnC::create({.gpu = gpu});
    REQUIRE(cnc);
    cnc->uploadBuffer(dst, 0, ArrayProxy<const uint8_t>(reinterpret_cast<const uint8_t *>(expected.data()), SIZE));
    submitAndWait(gpu, "upload", cnc->seal());

    std::vector<uint8_t> raw = dst->readContent();
    REQUIRE(raw.size() == SIZE);
    const auto * result = reinterpret_cast<const uint32_t *>(raw.data());
    for (size_t i = 0; i < N; ++i) CHECK(result[i] == expected[i]);
}

TEST_CASE("GPU2/CnC async: uploadBuffer honors destination offset", "[gpu2][cnc][async][gpu]") {
    auto gpu = makeGpu();
    if (!gpu) SKIP("No GPU context available");

    constexpr uint64_t SIZE = 8 * sizeof(uint32_t);
    auto               dst  = Buffer::create("dst", {.context = gpu, .size = SIZE});
    REQUIRE(dst);

    // Zero the whole buffer first, then upload 4 words at word-offset 2.
    std::vector<uint32_t> zeros(8, 0u);
    uint32_t              payload[4] = {111, 222, 333, 444};

    auto cnc = GpuCnC::create({.gpu = gpu});
    REQUIRE(cnc);
    cnc->uploadBuffer(dst, 0, ArrayProxy<const uint8_t>(reinterpret_cast<const uint8_t *>(zeros.data()), SIZE));
    cnc->uploadBuffer(dst, 2 * sizeof(uint32_t), ArrayProxy<const uint8_t>(reinterpret_cast<const uint8_t *>(payload), sizeof(payload)));
    submitAndWait(gpu, "upload-offset", cnc->seal());

    std::vector<uint8_t> raw = dst->readContent();
    REQUIRE(raw.size() == SIZE);
    const auto * r = reinterpret_cast<const uint32_t *>(raw.data());
    CHECK(r[0] == 0u);
    CHECK(r[1] == 0u);
    CHECK(r[2] == 111u);
    CHECK(r[3] == 222u);
    CHECK(r[4] == 333u);
    CHECK(r[5] == 444u);
    CHECK(r[6] == 0u);
    CHECK(r[7] == 0u);
}

// ── downloadBuffer ────────────────────────────────────────────────────────────────────

TEST_CASE("GPU2/CnC async: downloadBuffer returns buffer content via future", "[gpu2][cnc][async][gpu]") {
    auto gpu = makeGpu();
    if (!gpu) SKIP("No GPU context available");

    constexpr size_t   N    = 32;
    constexpr uint64_t SIZE = N * sizeof(uint32_t);

    std::vector<uint32_t> expected(N);
    for (size_t i = 0; i < N; ++i) expected[i] = (uint32_t) (0xA0000000u + i);

    auto src = Buffer::create("src", {.context = gpu, .size = SIZE, .mappable = true});
    if (!src) SKIP("Mappable buffer unavailable");
    {
        auto m = src->map();
        REQUIRE_FALSE(m.empty());
        std::memcpy(m.data(), expected.data(), SIZE);
    }

    auto cnc = GpuCnC::create({.gpu = gpu});
    REQUIRE(cnc);
    auto future = cnc->downloadBuffer(src);
    submitAndWait(gpu, "download", cnc->seal());

    REQUIRE(future.valid());
    AutoRef<const Blob> blob = future.get();
    REQUIRE(blob);
    REQUIRE(blob->size() == SIZE);
    const auto * result = reinterpret_cast<const uint32_t *>(blob->data());
    for (size_t i = 0; i < N; ++i) CHECK(result[i] == expected[i]);
}

TEST_CASE("GPU2/CnC async: downloadBuffer honors offset and size", "[gpu2][cnc][async][gpu]") {
    auto gpu = makeGpu();
    if (!gpu) SKIP("No GPU context available");

    constexpr uint64_t SIZE = 8 * sizeof(uint32_t);
    auto               src  = Buffer::create("src", {.context = gpu, .size = SIZE, .mappable = true});
    if (!src) SKIP("Mappable buffer unavailable");
    {
        auto     m    = src->map();
        uint32_t v[8] = {10, 20, 30, 40, 50, 60, 70, 80};
        std::memcpy(m.data(), v, SIZE);
    }

    auto cnc = GpuCnC::create({.gpu = gpu});
    REQUIRE(cnc);
    auto future = cnc->downloadBuffer(src, 3 * sizeof(uint32_t), 2 * sizeof(uint32_t));
    submitAndWait(gpu, "download-range", cnc->seal());

    AutoRef<const Blob> blob = future.get();
    REQUIRE(blob);
    REQUIRE(blob->size() == 2 * sizeof(uint32_t));
    const auto * r = reinterpret_cast<const uint32_t *>(blob->data());
    CHECK(r[0] == 40u);
    CHECK(r[1] == 50u);
}

// ── Round trip: upload then download within one payload ────────────────────────────────

TEST_CASE("GPU2/CnC async: upload then download round-trips through device-local buffer", "[gpu2][cnc][async][gpu]") {
    auto gpu = makeGpu();
    if (!gpu) SKIP("No GPU context available");

    constexpr size_t   N    = 48;
    constexpr uint64_t SIZE = N * sizeof(uint32_t);

    std::vector<uint32_t> expected(N);
    for (size_t i = 0; i < N; ++i) expected[i] = (uint32_t) (i * i + 3);

    auto dst = Buffer::create("dst", {.context = gpu, .size = SIZE});
    REQUIRE(dst);

    auto cnc = GpuCnC::create({.gpu = gpu});
    REQUIRE(cnc);
    cnc->uploadBuffer(dst, 0, ArrayProxy<const uint8_t>(reinterpret_cast<const uint8_t *>(expected.data()), SIZE));
    auto future = cnc->downloadBuffer(dst); // depends on the upload; tracker must serialize the two transfers
    submitAndWait(gpu, "roundtrip", cnc->seal());

    AutoRef<const Blob> blob = future.get();
    REQUIRE(blob);
    REQUIRE(blob->size() == SIZE);
    const auto * result = reinterpret_cast<const uint32_t *>(blob->data());
    for (size_t i = 0; i < N; ++i) CHECK(result[i] == expected[i]);
}

// ── downloadImage ─────────────────────────────────────────────────────────────────────

TEST_CASE("GPU2/CnC async: downloadImage reads back texture pixels", "[gpu2][cnc][async][gpu]") {
    auto gpu = makeGpu();
    if (!gpu) SKIP("No GPU context available");

    constexpr uint32_t W = 4, H = 4;
    constexpr uint64_t imgBytes = W * H * 4;

    // Stage a known RGBA8 pattern and upload it into a texture.
    auto staging = Buffer::create("staging", {.context = gpu, .size = imgBytes, .mappable = true});
    if (!staging) SKIP("Mappable buffer unavailable");
    {
        auto   m = staging->map();
        auto * p = static_cast<uint8_t *>(m.data());
        for (uint32_t y = 0; y < H; ++y)
            for (uint32_t x = 0; x < W; ++x) {
                size_t off = (y * W + x) * 4;
                p[off + 0] = (uint8_t) (x * 10);
                p[off + 1] = (uint8_t) (y * 10);
                p[off + 2] = (uint8_t) (x + y);
                p[off + 3] = 255;
            }
    }

    auto tex = Texture::create(
        "tex", {.context = gpu, .descriptor = Texture::Descriptor {}.setFormat(gfx::img::PixelFormat::RGBA8()).setDimensions(W, H).setLevels(1)});
    if (!tex) SKIP("RGBA8 texture unavailable");

    GpuCnC::Region region;
    region.imageOffset = {0, 0, 0};
    region.imageExtent = {W, H, 1};

    auto up = GpuCnC::create({.gpu = gpu});
    REQUIRE(up);
    up->copyBufferToImage({.src = staging, .dst = tex, .regions = ArrayProxy<const GpuCnC::Region>(&region, 1)});
    submitAndWait(gpu, "img-upload", up->seal());

    // Download the texture back into a blob.
    auto down = GpuCnC::create({.gpu = gpu});
    REQUIRE(down);
    auto future = down->downloadImage(tex, ArrayProxy<const GpuCnC::Region>(&region, 1));
    submitAndWait(gpu, "img-download", down->seal());

    GpuCnC::TextureContent content = future.get();
    REQUIRE(content.blob);
    REQUIRE(content.blob->size() == imgBytes);
    REQUIRE(content.regions.size() == 1);
    CHECK(content.regions[0].bufferOffset == 0);

    const auto * px = reinterpret_cast<const uint8_t *>(content.blob->data());
    for (uint32_t y = 0; y < H; ++y)
        for (uint32_t x = 0; x < W; ++x) {
            size_t off = (y * W + x) * 4;
            CHECK(px[off + 0] == (uint8_t) (x * 10));
            CHECK(px[off + 1] == (uint8_t) (y * 10));
            CHECK(px[off + 2] == (uint8_t) (x + y));
            CHECK(px[off + 3] == 255);
        }
}

// ── Cancellation ──────────────────────────────────────────────────────────────────────

TEST_CASE("GPU2/CnC async: dropping payload without submit resolves downloadBuffer future to empty blob", "[gpu2][cnc][async][gpu]") {
    auto gpu = makeGpu();
    if (!gpu) SKIP("No GPU context available");

    auto src = Buffer::create("src", {.context = gpu, .size = 256, .mappable = true});
    if (!src) SKIP("Mappable buffer unavailable");

    std::future<AutoRef<const Blob>> future;
    {
        auto cnc = GpuCnC::create({.gpu = gpu});
        REQUIRE(cnc);
        future                      = cnc->downloadBuffer(src);
        AutoRef<GpuPayload> payload = cnc->seal();
        REQUIRE(payload);
        // payload + cnc go out of scope here without ever being submitted.
    }

    REQUIRE(future.valid());
    AutoRef<const Blob> blob = future.get(); // signaled, never throws
    CHECK(!blob);                            // canceled => empty blob
}

TEST_CASE("GPU2/CnC async: dropping payload without submit resolves downloadImage future to empty content", "[gpu2][cnc][async][gpu]") {
    auto gpu = makeGpu();
    if (!gpu) SKIP("No GPU context available");

    auto tex = Texture::create(
        "tex", {.context = gpu, .descriptor = Texture::Descriptor {}.setFormat(gfx::img::PixelFormat::RGBA8()).setDimensions(4, 4).setLevels(1)});
    if (!tex) SKIP("RGBA8 texture unavailable");

    GpuCnC::Region region;
    region.imageOffset = {0, 0, 0};
    region.imageExtent = {4, 4, 1};

    std::future<GpuCnC::TextureContent> future;
    {
        auto cnc = GpuCnC::create({.gpu = gpu});
        REQUIRE(cnc);
        future                      = cnc->downloadImage(tex, ArrayProxy<const GpuCnC::Region>(&region, 1));
        AutoRef<GpuPayload> payload = cnc->seal();
        REQUIRE(payload);
        // dropped without submission
    }

    REQUIRE(future.valid());
    GpuCnC::TextureContent content = future.get(); // signaled, never throws
    CHECK(!content.blob);                          // canceled => empty blob
    CHECK(content.regions.empty());                // ... and empty regions
}

TEST_CASE("GPU2/CnC async: tearing down the GpuContext resolves a pending download future to empty", "[gpu2][cnc][async][gpu]") {
    std::future<AutoRef<const Blob>>    bufFuture;
    std::future<GpuCnC::TextureContent> imgFuture;
    {
        auto gpu = makeGpu();
        if (!gpu) SKIP("No GPU context available");

        auto src = Buffer::create("src", {.context = gpu, .size = 256, .mappable = true});
        if (!src) SKIP("Mappable buffer unavailable");
        auto tex = Texture::create(
            "tex", {.context = gpu, .descriptor = Texture::Descriptor {}.setFormat(gfx::img::PixelFormat::RGBA8()).setDimensions(4, 4).setLevels(1)});
        if (!tex) SKIP("RGBA8 texture unavailable");

        GpuCnC::Region region;
        region.imageOffset = {0, 0, 0};
        region.imageExtent = {4, 4, 1};

        auto cnc = GpuCnC::create({.gpu = gpu});
        REQUIRE(cnc);
        bufFuture                   = cnc->downloadBuffer(src);
        imgFuture                   = cnc->downloadImage(tex, ArrayProxy<const GpuCnC::Region>(&region, 1));
        AutoRef<GpuPayload> payload = cnc->seal();
        REQUIRE(payload);
        // Everything (payload, cnc, resources, and finally the GpuContext) is destroyed at scope exit
        // while the futures are still outstanding and the work was never submitted.
    }

    REQUIRE(bufFuture.valid());
    REQUIRE(imgFuture.valid());
    CHECK(!bufFuture.get()); // empty blob
    GpuCnC::TextureContent content = imgFuture.get();
    CHECK(!content.blob);
    CHECK(content.regions.empty());
}

// ── Invalid arguments resolve immediately ──────────────────────────────────────────────

TEST_CASE("GPU2/CnC async: downloadBuffer with out-of-range offset resolves to empty blob", "[gpu2][cnc][async][gpu]") {
    auto gpu = makeGpu();
    if (!gpu) SKIP("No GPU context available");

    auto src = Buffer::create("src", {.context = gpu, .size = 64});
    REQUIRE(src);

    auto cnc = GpuCnC::create({.gpu = gpu});
    REQUIRE(cnc);
    auto future = cnc->downloadBuffer(src, /*offset=*/128); // beyond the 64-byte buffer
    REQUIRE(future.valid());
    AutoRef<const Blob> blob = future.get(); // resolved synchronously, no submit needed
    CHECK(!blob);
}
