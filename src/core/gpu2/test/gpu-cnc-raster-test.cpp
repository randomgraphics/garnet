#if GN_BUILD_HAS_VULKAN

// Include the Vulkan context header first — it pulls in rapid-vulkan before any other TU can
// do so (ODR risk enforced by a #error guard in vk-gpu-context.h).
#include "../vk-gpu-context.h"
#include "mixed-cnc-raster-comp.spv.h"
#include "rtt-fullscreen-vert.spv.h"
#include "rtt-fullscreen-frag.spv.h"

#include <catch2/catch_test_macros.hpp>
#include <garnet/GNgpu2.h>
#include <atomic>
#include <cstring>
#include <vector>

using namespace GN;
using namespace GN::gpu2;

static AutoRef<GpuContext> makeGpu() {
    return GpuContext::create("gpu", {.howToPrintDeviceCaps = GpuContext::Verbosity::SILENCE, .debug = GpuContext::DebugMode::ENABLED});
}

static AutoRef<Texture> makeRgba8Tex(const AutoRef<GpuContext> & gpu, const char * name, uint32_t w, uint32_t h) {
    return Texture::create(name, {.context = gpu, .descriptor = Texture::Descriptor {}.setFormat(gfx::img::PixelFormat::RGBA8()).setDimensions(w, h).setLevels(1)});
}

static AutoRef<GpuShader> makeShader(const AutoRef<GpuContext> & gpu, const char * name, const void * binary, size_t byteSize) {
    return GpuShader::create({.context = gpu, .name = name, .binary = binary, .size = byteSize});
}

template<typename... Payloads>
static void submitAndWait(const AutoRef<GpuContext> & gpu, Payloads &&... payloads) {
    std::atomic<bool> done = false;
    auto              sp   = GpuContext::SubmitParameters("mixed-cnc-raster");
    (sp.appendWork(payloads), ...);
    sp.setOnComplete([&done] { done = true; });
    gpu->submit(sp);
    while (!done) gpu->pump();
}

// ---------------------------------------------------------------------------
// Test: CnC upload → raster sample → compute copy → readback
// Verifies cross-payload barrier correctness across three payload types
// in a single submission batch.
// ---------------------------------------------------------------------------
TEST_CASE("GPU2/CnC+Raster: upload->render->compute copy->readback", "[gpu2][cnc][raster][gpu]") {
    auto gpu = makeGpu();
    if (!gpu) SKIP("No GPU context available");

    constexpr uint32_t W            = 8, H = 8;
    constexpr uint32_t PIXEL_COUNT  = W * H;
    constexpr uint32_t BYTES_PER_PIXEL = 4;
    constexpr uint64_t SIZE         = PIXEL_COUNT * BYTES_PER_PIXEL;

    // Create all three textures
    auto sourceTex    = makeRgba8Tex(gpu, "source", W, H); // upload destination
    auto renderTarget = makeRgba8Tex(gpu, "rt",     W, H); // raster color attachment
    auto outputTex    = makeRgba8Tex(gpu, "output", W, H); // compute output
    if (!sourceTex || !renderTarget || !outputTex) SKIP("RGBA8 textures unavailable");

    // Staging buffer: solid red (R=255, G=0, B=0, A=255) for every pixel.
    auto staging = Buffer::create("staging", {.context = gpu, .size = SIZE, .mappable = true});
    if (!staging) SKIP("Mappable buffer unavailable");
    {
        auto   m = staging->map();
        auto * p = static_cast<uint8_t *>(m.data());
        for (uint32_t i = 0; i < PIXEL_COUNT; ++i) {
            p[i * 4 + 0] = 255; // R
            p[i * 4 + 1] = 0;   // G
            p[i * 4 + 2] = 0;   // B
            p[i * 4 + 3] = 255; // A
        }
    }

    // ── Payload 1: CnC upload staging → sourceTex ────────────────────────
    GpuCnC::Region region;
    region.imageSubresource = {};
    region.imageOffset      = {0, 0, 0};
    region.imageExtent      = {W, H, 1};

    auto cnc1 = GpuCnC::create({.gpu = gpu});
    REQUIRE(cnc1);
    cnc1->copyBufferToImage({.src = staging, .dst = sourceTex, .regions = ArrayProxy<GpuCnC::Region>(&region, 1)});
    AutoRef<GpuPayload> p1 = cnc1->seal();

    // ── Payload 2: Raster — sample sourceTex → renderTarget ──────────────
    auto vs = makeShader(gpu, "rtt-vert", kRttFullscreenVertSpv, sizeof(kRttFullscreenVertSpv));
    auto ps = makeShader(gpu, "rtt-frag", kRttFullscreenFragSpv, sizeof(kRttFullscreenFragSpv));
    REQUIRE(vs);
    REQUIRE(ps);

    GpuResourceView sourceView;
    sourceView.resource = sourceTex;

    GpuResourceView rtView;
    rtView.resource = renderTarget;

    RasterTarget rt;
    rt.colorTargets.append(RasterTarget::ColorTarget {.target = rtView});
    rt.setClearColor(0.0f, 0.0f, 0.0f);

    auto raster = GpuRaster::create({.gpu = gpu, .target = rt});
    REQUIRE(raster);

    // Bind sourceTex at set=0, binding=0 (matches rtt-fullscreen.frag's sampler2D)
    GpuResourceTable rasterResources;
    rasterResources.resize(1);
    rasterResources[0].resize(1);
    rasterResources[0][0].resize(1);
    rasterResources[0][0][0] = sourceView;

    GpuRaster::DrawParameters dp;
    dp.vs                   = vs;
    dp.ps                   = ps;
    dp.geometry.vertexCount = 3;
    dp.resources            = rasterResources;
    raster->draw(dp);
    AutoRef<GpuPayload> p2 = raster->seal();

    // ── Payload 3: CnC compute — copy renderTarget → outputTex ───────────
    auto cs = makeShader(gpu, "mixed-comp", kMixedCncRasterCompSpv, sizeof(kMixedCncRasterCompSpv));
    REQUIRE(cs);

    // set=0, binding=0: renderTarget as storage image (input)
    // set=0, binding=1: outputTex as storage image (output)
    GpuResourceView inputView;
    inputView.resource = renderTarget;
    inputView.setImageViewType(GpuResourceView::ImageView::STORAGE);

    GpuResourceView outputView;
    outputView.resource = outputTex;
    outputView.setImageViewType(GpuResourceView::ImageView::STORAGE);

    GpuResourceTable computeResources;
    computeResources.resize(1);
    computeResources[0].resize(2);
    computeResources[0][0].resize(1);
    computeResources[0][1].resize(1);
    computeResources[0][0][0] = inputView;
    computeResources[0][1][0] = outputView;

    auto cnc2 = GpuCnC::create({.gpu = gpu});
    REQUIRE(cnc2);
    cnc2->compute({
        .cs        = cs,
        .resources = computeResources,
        // Dispatch enough groups to cover W×H pixels with 8×8 local size
        .x = (W + 7) / 8,
        .y = (H + 7) / 8,
    });
    AutoRef<GpuPayload> p3 = cnc2->seal();

    // ── Submit all three payloads in one batch ────────────────────────────
    submitAndWait(gpu, p1, p2, p3);

    // ── Verify: outputTex should be solid red ────────────────────────────
    gfx::img::Image image = outputTex->readback();
    REQUIRE_FALSE(image.empty());
    REQUIRE(image.width() == W);
    REQUIRE(image.height() == H);

    auto pixels = image.plane().toRGBA8(image.data());
    REQUIRE(pixels.size() == (size_t) PIXEL_COUNT);
    for (size_t i = 0; i < pixels.size(); ++i) {
        const auto & px = pixels[i];
        INFO("pixel[" << i << "] = (" << (int) px.r << "," << (int) px.g << "," << (int) px.b << "," << (int) px.a << ")");
        CHECK(px.r == 255u);
        CHECK(px.g == 0u);
        CHECK(px.b == 0u);
        CHECK(px.a == 255u);
    }
}

#endif // GN_BUILD_HAS_VULKAN
