#if GN_BUILD_HAS_VULKAN

    // Include the Vulkan context header first — it pulls in rapid-vulkan before any other TU can
    // do so (ODR risk enforced by a #error guard in vk-gpu-context.h).
    #include "../vk-gpu-context.h"

    // Generated SPIR-V headers are produced by the CMake custom command in
    // src/test/internal/CMakeLists.txt and land in the binary dir on the include path.
    #include "rtt-fullscreen-vert.spv.h"
    #include "rtt-fullscreen-frag.spv.h"
    #include "rtt-mrt-frag.spv.h"

    #include <catch2/catch_test_macros.hpp>
    #include <garnet/GNgpu2.h>
    #include <atomic>

using namespace GN;
using namespace GN::gpu2;

// ---------------------------------------------------------------------------
// Helpers shared across test cases
// ---------------------------------------------------------------------------

static AutoRef<GpuContext> makeGpu() {
    return GpuContext::create("gpu", {.howToPrintDeviceCaps = GpuContext::Verbosity::SILENCE, .debug = GpuContext::DebugMode::ENABLED});
}

static AutoRef<Texture> makeRgba8Tex(const AutoRef<GpuContext> & gpu, const char * name, uint32_t w, uint32_t h) {
    // Single mip level: Vulkan requires render-attachment image views to have levelCount=1,
    // and sampling a solid-color texture needs no mip chain.
    return Texture::create(name,
                           {.context = gpu, .descriptor = Texture::Descriptor {}.setFormat(gfx::img::PixelFormat::RGBA8()).setDimensions(w, h).setLevels(1)});
}

template<typename... Payloads>
static void submitAndWait(const AutoRef<GpuContext> & gpu, Payloads &&... payloads) {
    std::atomic<bool> done = false;
    auto              sp   = GpuContext::SubmitParameters("rtt-test");
    (sp.appendWork(payloads), ...);
    sp.setOnComplete([&done] { done = true; });
    gpu->submit(sp);
    while (!done) gpu->pump();
}

// Create a GpuShader from one of the pre-compiled SPIR-V blobs embedded in the .spv.h headers.
// `binary` is the uint32_t array; `byteSize` is sizeof(that array).
static AutoRef<GpuShader> makeShader(const AutoRef<GpuContext> & gpu, const char * name, const void * binary, size_t byteSize) {
    return GpuShader::create({.context = gpu, .name = name, .binary = binary, .size = byteSize});
}

// ---------------------------------------------------------------------------
// Tests
// ---------------------------------------------------------------------------

// Pass 1: clear a small offscreen texture to solid red via GpuRaster (no draw calls — clear only).
// Pass 2: sample that texture with a fullscreen triangle shader and write the result to a second
//         offscreen texture ("backbuffer").
// Both passes submitted in one GpuContext::submit() call so the cross-payload state tracker
// handles the transition from COLOR_ATTACHMENT to SHADER_READ_ONLY between them.
// After GPU completion, verify every pixel of the backbuffer matches the intended red.
TEST_CASE("GPU2 RTT: render solid color to texture then sample to backbuffer", "[gpu2][rtt][gpu]") {
    auto gpu = makeGpu();
    if (!gpu) SKIP("No GPU context available");

    constexpr uint32_t W = 8, H = 8;

    auto rtt        = makeRgba8Tex(gpu, "rtt", W, H);
    auto backbuffer = makeRgba8Tex(gpu, "backbuffer", W, H);
    if (!rtt || !backbuffer) SKIP("RGBA8 textures unavailable");

    // --- Pass 1: clear rtt to solid red ---
    GpuResourceView rttView;
    rttView.resource = rtt;
    RasterTarget rt1;
    rt1.colorTargets.append(RasterTarget::ColorTarget {.target = rttView});
    rt1.setClearColor(1.0f, 0.0f, 0.0f); // red
    auto raster1 = GpuRaster::create({.gpu = gpu, .target = rt1});
    REQUIRE(raster1);
    AutoRef<GpuPayload> p1 = raster1->seal();

    // --- Pass 2: sample rtt into backbuffer via fullscreen shader ---
    auto vs = makeShader(gpu, "rtt-vert", kRttFullscreenVertSpv, sizeof(kRttFullscreenVertSpv));
    auto ps = makeShader(gpu, "rtt-frag", kRttFullscreenFragSpv, sizeof(kRttFullscreenFragSpv));
    REQUIRE(vs);
    REQUIRE(ps);

    GpuResourceView bbView;
    bbView.resource = backbuffer;
    RasterTarget rt2;
    rt2.colorTargets.append(RasterTarget::ColorTarget {.target = bbView});
    rt2.setClearColor(0.0f, 0.0f, 0.0f); // black clear, overwritten by the sample draw
    auto raster2 = GpuRaster::create({.gpu = gpu, .target = rt2});
    REQUIRE(raster2);

    // Bind rtt at set=0, binding=0 — matches the fragment shader's sampler2D declaration.
    GpuResourceTable resources;
    resources.resize(1);
    resources[0].resize(1);
    resources[0][0].resize(1);
    resources[0][0][0] = rttView;

    GpuRaster::DrawParameters dp;
    dp.vs                   = vs;
    dp.ps                   = ps;
    dp.geometry.vertexCount = 3; // fullscreen triangle from gl_VertexIndex, no vertex buffer
    dp.resources            = resources;
    raster2->draw(dp);
    AutoRef<GpuPayload> p2 = raster2->seal();

    // Submit both passes as one batch. The batch tracker transitions rtt from
    // COLOR_ATTACHMENT_OPTIMAL (after p1) to SHADER_READ_ONLY_OPTIMAL (for p2).
    submitAndWait(gpu, p1, p2);

    // Verify every pixel of the backbuffer is red (R=255, G=0, B=0, A=255).
    gfx::img::Image image = backbuffer->readback();
    REQUIRE_FALSE(image.empty());
    REQUIRE(image.width() == W);
    REQUIRE(image.height() == H);

    auto pixels = image.plane().toRGBA8(image.data());
    REQUIRE(pixels.size() == (size_t) (W * H));
    for (size_t i = 0; i < pixels.size(); ++i) {
        const auto & px = pixels[i];
        INFO("pixel[" << i << "] = (" << (int) px.r << "," << (int) px.g << "," << (int) px.b << "," << (int) px.a << ")");
        CHECK(px.r == 255u);
        CHECK(px.g == 0u);
        CHECK(px.b == 0u);
        CHECK(px.a == 255u);
    }
}

// Variant: use green as the render color, to confirm the pipeline is not accidentally
// hardcoding red rather than sampling the texture.
TEST_CASE("GPU2 RTT: green render color propagates through texture sample", "[gpu2][rtt][gpu]") {
    auto gpu = makeGpu();
    if (!gpu) SKIP("No GPU context available");

    constexpr uint32_t W = 8, H = 8;

    auto rtt        = makeRgba8Tex(gpu, "rtt-g", W, H);
    auto backbuffer = makeRgba8Tex(gpu, "bb-g", W, H);
    if (!rtt || !backbuffer) SKIP("RGBA8 textures unavailable");

    GpuResourceView rttView;
    rttView.resource = rtt;

    // Pass 1: clear rtt to green.
    RasterTarget rt1;
    rt1.colorTargets.append(RasterTarget::ColorTarget {.target = rttView});
    rt1.setClearColor(0.0f, 1.0f, 0.0f);
    auto                raster1 = GpuRaster::create({.gpu = gpu, .target = rt1});
    AutoRef<GpuPayload> p1      = raster1->seal();

    // Pass 2: sample rtt into backbuffer.
    auto vs = makeShader(gpu, "rtt-vert", kRttFullscreenVertSpv, sizeof(kRttFullscreenVertSpv));
    auto ps = makeShader(gpu, "rtt-frag", kRttFullscreenFragSpv, sizeof(kRttFullscreenFragSpv));
    REQUIRE(vs);
    REQUIRE(ps);

    GpuResourceView bbView;
    bbView.resource = backbuffer;
    RasterTarget rt2;
    rt2.colorTargets.append(RasterTarget::ColorTarget {.target = bbView});
    rt2.setClearColor(0.0f, 0.0f, 0.0f);
    auto raster2 = GpuRaster::create({.gpu = gpu, .target = rt2});

    GpuResourceTable resources;
    resources.resize(1);
    resources[0].resize(1);
    resources[0][0].resize(1);
    resources[0][0][0] = rttView;

    GpuRaster::DrawParameters dp;
    dp.vs                   = vs;
    dp.ps                   = ps;
    dp.geometry.vertexCount = 3;
    dp.resources            = resources;
    raster2->draw(dp);
    AutoRef<GpuPayload> p2 = raster2->seal();

    submitAndWait(gpu, p1, p2);

    gfx::img::Image image = backbuffer->readback();
    REQUIRE_FALSE(image.empty());
    auto pixels = image.plane().toRGBA8(image.data());
    REQUIRE(pixels.size() == (size_t) (W * H));
    for (size_t i = 0; i < pixels.size(); ++i) {
        const auto & px = pixels[i];
        INFO("pixel[" << i << "] = (" << (int) px.r << "," << (int) px.g << "," << (int) px.b << "," << (int) px.a << ")");
        CHECK(px.r == 0u);
        CHECK(px.g == 255u);
        CHECK(px.b == 0u);
        CHECK(px.a == 255u);
    }
}

// Render to two color attachments in one pass: location 0 receives blue, location 1 receives red.
// Verifies that MRT output routing works correctly end-to-end.
TEST_CASE("GPU2 RTT: MRT — render blue and red to two color targets simultaneously", "[gpu2][rtt][gpu]") {
    auto gpu = makeGpu();
    if (!gpu) SKIP("No GPU context available");

    constexpr uint32_t W = 8, H = 8;

    auto tex0 = makeRgba8Tex(gpu, "mrt-0", W, H); // receives blue (location 0)
    auto tex1 = makeRgba8Tex(gpu, "mrt-1", W, H); // receives red  (location 1)
    if (!tex0 || !tex1) SKIP("RGBA8 textures unavailable");

    auto vs = makeShader(gpu, "rtt-vert", kRttFullscreenVertSpv, sizeof(kRttFullscreenVertSpv));
    auto ps = makeShader(gpu, "rtt-mrt-frag", kRttMrtFragSpv, sizeof(kRttMrtFragSpv));
    REQUIRE(vs);
    REQUIRE(ps);

    GpuResourceView view0, view1;
    view0.resource = tex0;
    view1.resource = tex1;

    RasterTarget rt;
    rt.colorTargets.append(RasterTarget::ColorTarget {.target = view0}); // location 0 → blue
    rt.colorTargets.append(RasterTarget::ColorTarget {.target = view1}); // location 1 → red
    rt.setClearColor(0.0f, 0.0f, 0.0f);

    auto raster = GpuRaster::create({.gpu = gpu, .target = rt});
    REQUIRE(raster);

    GpuRaster::DrawParameters dp;
    dp.vs                   = vs;
    dp.ps                   = ps;
    dp.geometry.vertexCount = 3;
    raster->draw(dp);
    AutoRef<GpuPayload> p = raster->seal();

    submitAndWait(gpu, p);

    {
        gfx::img::Image img    = tex0->readback();
        auto            pixels = img.plane().toRGBA8(img.data());
        REQUIRE(pixels.size() == (size_t) (W * H));
        for (size_t i = 0; i < pixels.size(); ++i) {
            const auto & px = pixels[i];
            INFO("tex0 pixel[" << i << "] = (" << (int) px.r << "," << (int) px.g << "," << (int) px.b << "," << (int) px.a << ")");
            CHECK(px.r == 0u);
            CHECK(px.g == 0u);
            CHECK(px.b == 255u);
            CHECK(px.a == 255u);
        }
    }
    {
        gfx::img::Image img    = tex1->readback();
        auto            pixels = img.plane().toRGBA8(img.data());
        REQUIRE(pixels.size() == (size_t) (W * H));
        for (size_t i = 0; i < pixels.size(); ++i) {
            const auto & px = pixels[i];
            INFO("tex1 pixel[" << i << "] = (" << (int) px.r << "," << (int) px.g << "," << (int) px.b << "," << (int) px.a << ")");
            CHECK(px.r == 255u);
            CHECK(px.g == 0u);
            CHECK(px.b == 0u);
            CHECK(px.a == 255u);
        }
    }
}

#endif // GN_BUILD_HAS_VULKAN
