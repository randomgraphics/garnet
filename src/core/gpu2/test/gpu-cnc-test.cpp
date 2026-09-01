#include <catch2/catch_test_macros.hpp>
#include <garnet/GNgpu2.h>

#include <atomic>
#include <cstring>
#include <vector>

using namespace GN;
using namespace GN::gpu2;

// Helper: create a GPU context, skip the test if none is available.
static AutoRef<GpuContext> makeGpu() {
    return GpuContext::create("gpu", GpuContext::CreateParameters {.howToPrintDeviceCaps = GpuContext::Verbosity::SILENCE});
}

// Helper: submit a payload and spin on gpu->pump() until the fence fires.
static void submitAndWait(GpuContext * gpu, AutoRef<GpuPayload> payload) {
    std::atomic<bool> done = false;
    gpu->submit(GpuContext::SubmitParameters("wait").appendWork(payload).setOnComplete([&done] { done = true; }));
    while (!done) gpu->pump();
}

// ── GpuCnC creation ──────────────────────────────────────────────────────────────────

TEST_CASE("GPU2/CnC: create returns valid object", "[gpu2][cnc][gpu]") {
    auto gpu = makeGpu();
    if (!gpu) SKIP("No GPU context available");

    auto cnc = GpuCnC::create({.gpu = gpu});
    REQUIRE(cnc);
}

TEST_CASE("GPU2/CnC: create with null gpu returns empty", "[gpu2][cnc]") {
    auto cnc = GpuCnC::create({.gpu = {}});
    REQUIRE_FALSE(cnc);
}

// ── Seal / double-seal ────────────────────────────────────────────────────────────────

TEST_CASE("GPU2/CnC: empty seal returns valid payload", "[gpu2][cnc][gpu]") {
    auto gpu = makeGpu();
    if (!gpu) SKIP("No GPU context available");

    auto cnc = GpuCnC::create({.gpu = gpu});
    REQUIRE(cnc);

    AutoRef<GpuPayload> payload = cnc->seal();
    REQUIRE(payload);
}

TEST_CASE("GPU2/CnC: double seal returns empty", "[gpu2][cnc][gpu]") {
    auto gpu = makeGpu();
    if (!gpu) SKIP("No GPU context available");

    auto cnc = GpuCnC::create({.gpu = gpu});
    REQUIRE(cnc);

    cnc->seal();
    AutoRef<GpuPayload> second = cnc->seal();
    REQUIRE_FALSE(second);
}

// ── Buffer-to-buffer copy ─────────────────────────────────────────────────────────────

TEST_CASE("GPU2/CnC: copyBufferToBuffer copies data correctly", "[gpu2][cnc][gpu]") {
    auto gpu = makeGpu();
    if (!gpu) SKIP("No GPU context available");

    constexpr size_t   N    = 64;
    constexpr uint64_t SIZE = N * sizeof(uint32_t);

    // Source: mappable buffer filled with sequential values.
    auto src = Buffer::create("src", {.context = gpu, .size = SIZE, .mappable = true});
    if (!src) SKIP("Mappable buffer unavailable");

    {
        auto m = src->map();
        REQUIRE_FALSE(m.empty());
        auto * p = static_cast<uint32_t *>(m.data());
        for (size_t i = 0; i < N; ++i) p[i] = (uint32_t) i;
    }

    // Destination: device-local buffer.
    auto dst = Buffer::create("dst", {.context = gpu, .size = SIZE});
    REQUIRE(dst);

    auto cnc = GpuCnC::create({.gpu = gpu});
    REQUIRE(cnc);
    cnc->copyBufferToBuffer({.src = src, .dst = dst, .size = SIZE});
    submitAndWait(gpu.get(), cnc->seal());

    // Readback and verify.
    std::vector<uint8_t> raw = dst->readContent();
    REQUIRE(raw.size() == SIZE);
    const auto * result = reinterpret_cast<const uint32_t *>(raw.data());
    for (size_t i = 0; i < N; ++i) { CHECK(result[i] == (uint32_t) i); }
}

TEST_CASE("GPU2/CnC: copyBufferToBuffer with offsets", "[gpu2][cnc][gpu]") {
    auto gpu = makeGpu();
    if (!gpu) SKIP("No GPU context available");

    constexpr uint64_t FULL = 8 * sizeof(uint32_t);

    auto src = Buffer::create("src", {.context = gpu, .size = FULL, .mappable = true});
    if (!src) SKIP("Mappable buffer unavailable");

    {
        auto     m    = src->map();
        auto *   p    = static_cast<uint32_t *>(m.data());
        uint32_t v[8] = {10, 20, 30, 40, 50, 60, 70, 80};
        std::memcpy(p, v, FULL);
    }

    // Destination: same size. Copy only the middle 4 elements starting at word 2 in src → word 0 in dst.
    auto dst = Buffer::create("dst", {.context = gpu, .size = FULL});
    REQUIRE(dst);

    auto cnc = GpuCnC::create({.gpu = gpu});
    REQUIRE(cnc);
    cnc->copyBufferToBuffer({.src = src, .dst = dst, .srcOffset = 2 * sizeof(uint32_t), .dstOffset = 0, .size = 4 * sizeof(uint32_t)});
    submitAndWait(gpu.get(), cnc->seal());

    std::vector<uint8_t> raw = dst->readContent(0, 4 * sizeof(uint32_t));
    REQUIRE(raw.size() == 4 * sizeof(uint32_t));
    const auto * r = reinterpret_cast<const uint32_t *>(raw.data());
    CHECK(r[0] == 30u);
    CHECK(r[1] == 40u);
    CHECK(r[2] == 50u);
    CHECK(r[3] == 60u);
}

// ── Buffer-to-image copy ──────────────────────────────────────────────────────────────

TEST_CASE("GPU2/CnC: copyBufferToImage uploads pixel data", "[gpu2][cnc][gpu]") {
    auto gpu = makeGpu();
    if (!gpu) SKIP("No GPU context available");

    constexpr uint32_t W = 4, H = 4;

    // Fill staging buffer with a known RGBA8 pattern: each pixel encodes (x, y, 0, 255).
    constexpr uint64_t stagingSize = W * H * 4;
    auto               staging     = Buffer::create("staging", {.context = gpu, .size = stagingSize, .mappable = true});
    if (!staging) SKIP("Mappable buffer unavailable");

    {
        auto   m = staging->map();
        auto * p = static_cast<uint8_t *>(m.data());
        for (uint32_t y = 0; y < H; ++y) {
            for (uint32_t x = 0; x < W; ++x) {
                size_t off = (y * W + x) * 4;
                p[off + 0] = (uint8_t) x;
                p[off + 1] = (uint8_t) y;
                p[off + 2] = 0;
                p[off + 3] = 255;
            }
        }
    }

    auto tex = Texture::create("dst-tex", {.context = gpu, .descriptor = Texture::Descriptor {}.setFormat(gfx::img::PixelFormat::RGBA8()).setDimensions(W, H)});
    if (!tex) SKIP("RGBA8 texture unavailable");

    GpuCnC::Region region;
    region.imageOffset = {0, 0, 0};
    region.imageExtent = {W, H, 1};

    auto cnc = GpuCnC::create({.gpu = gpu});
    REQUIRE(cnc);
    cnc->copyBufferToImage({.src = staging, .dst = tex, .regions = ArrayView<const GpuCnC::Region>(&region, 1)});
    submitAndWait(gpu.get(), cnc->seal());

    // Transition the texture to shader-read so readback can sample it.
    // Readback internally handles any required layout transition.
    gfx::img::Image img = tex->readback();
    REQUIRE_FALSE(img.empty());
    REQUIRE(img.width() == W);
    REQUIRE(img.height() == H);

    std::vector<gfx::img::RGBA8> pixels = img.plane().toRGBA8(img.data());
    REQUIRE(pixels.size() == W * H);
    for (uint32_t y = 0; y < H; ++y) {
        for (uint32_t x = 0; x < W; ++x) {
            const auto & px = pixels[y * W + x];
            CHECK(px.r == x);
            CHECK(px.g == y);
            CHECK(px.b == 0u);
            CHECK(px.a == 255u);
        }
    }
}

// ── Compute with null shader ──────────────────────────────────────────────────────────

TEST_CASE("GPU2/CnC: compute with null shader logs error but does not crash", "[gpu2][cnc][gpu]") {
    auto gpu = makeGpu();
    if (!gpu) SKIP("No GPU context available");

    auto cnc = GpuCnC::create({.gpu = gpu});
    REQUIRE(cnc);

    // Record a dispatch with no compute shader: payload records error during record time, not crash.
    cnc->compute({});

    AutoRef<GpuPayload> payload = cnc->seal();
    REQUIRE(payload);

    // Submit must not crash; the null-shader dispatch is silently skipped.
    submitAndWait(gpu.get(), payload);
    SUCCEED("No crash with null compute shader");
}
