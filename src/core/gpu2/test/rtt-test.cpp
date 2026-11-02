#if GN_BUILD_HAS_VULKAN

    // Include the Vulkan context header first — it pulls in rapid-vulkan before any other TU can
    // do so (ODR risk enforced by a #error guard in vk-gpu-context.h).
    #include "../vk-gpu-context.h"

    // Generated SPIR-V headers are produced by the CMake custom command in
    // src/test/internal/CMakeLists.txt and land in the binary dir on the include path.
    #include "rtt-fullscreen-vert.spv.h"
    #include "rtt-mrt-frag.spv.h"
    #include "bindless-test-frag.spv.h"

    #include <catch2/catch_test_macros.hpp>
    #include <garnet/GNgpu2.h>
    #include <atomic>

using namespace GN;
using namespace GN::gpu2;

    #include "gpu2-test-helpers.h"

template<typename T>
static ArrayView<const uint8_t> makePushConstants(const T & value) {
    return {(const uint8_t *) &value, sizeof(value)};
}

static AutoRef<bindless::DescriptorHeap> makeRasterHeap(const AutoRef<GpuContext> & gpu) {
    return bindless::DescriptorHeap::create("rtt-test-heap", {.gpu = gpu, .capacity = 16});
}

static bool addSharedSampler(const AutoRef<GpuContext> & gpu, const AutoRef<bindless::DescriptorHeap> & heap) {
    auto sampler = Sampler::create("rtt-test-sampler", {.context = gpu});
    if (!sampler) return false;
    return heap->allocate(bindless::DescriptorHeap::SAMPLER, GpuResourceView {sampler}).slot == 0;
}

// ---------------------------------------------------------------------------
// Tests
// ---------------------------------------------------------------------------

// Pass 1: clear a small offscreen texture to solid red via bindless::Raster (no draw calls — clear only).
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
    auto heap = makeRasterHeap(gpu);
    REQUIRE(heap);
    REQUIRE(addSharedSampler(gpu, heap));
    const auto textureIndex = heap->allocate(bindless::DescriptorHeap::SAMPLED_TEXTURE, GpuResourceView {rtt});
    REQUIRE(textureIndex != bindless::DescriptorHeap::INVALID_DESCRIPTOR_INDEX);

    // --- Pass 1: clear rtt to solid red ---
    GpuResourceView rttView;
    rttView.resource = rtt;
    RasterTarget rt1;
    rt1.colorTargets.append(RasterTarget::ColorTarget(rttView));
    rt1.setClearColor(1.0f, 0.0f, 0.0f); // red
    auto raster1 = bindless::Raster::create("rtt-source", {.gpu = gpu, .target = &rt1, .heap = heap});
    REQUIRE(raster1);
    AutoRef<GpuPayload> p1 = raster1->seal();

    // --- Pass 2: sample rtt into backbuffer via fullscreen shader ---
    auto vs = makeShader(gpu, "rtt-vert", kRttFullscreenVertSpv, sizeof(kRttFullscreenVertSpv));
    auto ps = makeShader(gpu, "rtt-frag", kBindlessTestFragSpv, sizeof(kBindlessTestFragSpv));
    REQUIRE(vs);
    REQUIRE(ps);

    GpuResourceView bbView;
    bbView.resource = backbuffer;
    RasterTarget rt2;
    rt2.colorTargets.append(RasterTarget::ColorTarget(bbView));
    rt2.setClearColor(0.0f, 0.0f, 0.0f); // black clear, overwritten by the sample draw
    auto raster2 = bindless::Raster::create("rtt-backbuffer", {.gpu = gpu, .target = &rt2, .heap = heap});
    REQUIRE(raster2);

    RasterGeometry dpGeometry;
    dpGeometry.vertexCount = 3; // fullscreen triangle from gl_VertexIndex, no vertex buffer

    raster2->recordDraw({.vs = vs, .ps = ps, .geometry = dpGeometry, .immediates = makePushConstants(textureIndex.slot)});
    AutoRef<GpuPayload> p2 = raster2->seal();

    // Submit both passes as one batch. The batch tracker transitions rtt from
    // COLOR_ATTACHMENT_OPTIMAL (after p1) to SHADER_READ_ONLY_OPTIMAL (for p2).
    submitAndWait(gpu, "rtt-test", p1, p2);

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
    auto heap = makeRasterHeap(gpu);
    REQUIRE(heap);
    REQUIRE(addSharedSampler(gpu, heap));
    const auto textureIndex = heap->allocate(bindless::DescriptorHeap::SAMPLED_TEXTURE, GpuResourceView {rtt});
    REQUIRE(textureIndex != bindless::DescriptorHeap::INVALID_DESCRIPTOR_INDEX);

    GpuResourceView rttView;
    rttView.resource = rtt;

    // Pass 1: clear rtt to green.
    RasterTarget rt1;
    rt1.colorTargets.append(RasterTarget::ColorTarget(rttView));
    rt1.setClearColor(0.0f, 1.0f, 0.0f);
    auto                raster1 = bindless::Raster::create("rtt-green-source", {.gpu = gpu, .target = &rt1, .heap = heap});
    AutoRef<GpuPayload> p1      = raster1->seal();

    // Pass 2: sample rtt into backbuffer.
    auto vs = makeShader(gpu, "rtt-vert", kRttFullscreenVertSpv, sizeof(kRttFullscreenVertSpv));
    auto ps = makeShader(gpu, "rtt-frag", kBindlessTestFragSpv, sizeof(kBindlessTestFragSpv));
    REQUIRE(vs);
    REQUIRE(ps);

    GpuResourceView bbView;
    bbView.resource = backbuffer;
    RasterTarget rt2;
    rt2.colorTargets.append(RasterTarget::ColorTarget(bbView));
    rt2.setClearColor(0.0f, 0.0f, 0.0f);
    auto raster2 = bindless::Raster::create("rtt-green-backbuffer", {.gpu = gpu, .target = &rt2, .heap = heap});

    RasterGeometry            dpGeometry;
    dpGeometry.vertexCount = 3;
    raster2->recordDraw({.vs = vs, .ps = ps, .geometry = dpGeometry, .immediates = makePushConstants(textureIndex.slot)});
    AutoRef<GpuPayload> p2 = raster2->seal();

    submitAndWait(gpu, "rtt-test", p1, p2);

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
    rt.colorTargets.append(RasterTarget::ColorTarget(view0)); // location 0 → blue
    rt.colorTargets.append(RasterTarget::ColorTarget(view1)); // location 1 → red
    rt.setClearColor(0.0f, 0.0f, 0.0f);

    auto heap = makeRasterHeap(gpu);
    REQUIRE(heap);
    auto raster = bindless::Raster::create("rtt-mrt", {.gpu = gpu, .target = &rt, .heap = heap});
    REQUIRE(raster);

    RasterGeometry dpGeometry;
    dpGeometry.vertexCount = 3;
    bindless::Raster::DrawParameters dp {.geometry = dpGeometry};
    dp.vs                  = vs;
    dp.ps                  = ps;
    raster->recordDraw(dp);
    AutoRef<GpuPayload> p = raster->seal();

    submitAndWait(gpu, "rtt-test", p);

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

TEST_CASE("GPU2 RTT: borrowed draw inputs are snapshotted before returning", "[gpu2][raster][gpu]") {
    auto gpu = makeGpu();
    if (!gpu) SKIP("No GPU context available");
    auto output = makeRgba8Tex(gpu, "snapshot-output", 12, 4);
    REQUIRE(output);
    AutoRef<GpuPayload> payload;
    {
        // The sealed payload must retain resources after all caller-owned descriptions disappear.
        auto red   = makeRgba8Tex(gpu, "snapshot-red", 1, 1);
        auto green = makeRgba8Tex(gpu, "snapshot-green", 1, 1);
        REQUIRE(red);
        REQUIRE(green);
        GpuResourceView redView, greenView, outputView;
        redView.resource    = red;
        greenView.resource  = green;
        outputView.resource = output;
        RasterTarget redTarget, greenTarget, target;
        redTarget.setColorTarget(0, redView).setClearColor(1, 0, 0, 1);
        greenTarget.setColorTarget(0, greenView).setClearColor(0, 1, 0, 1);
        auto heap = makeRasterHeap(gpu);
        REQUIRE(heap);
        REQUIRE(addSharedSampler(gpu, heap));
        const auto redIndex = heap->allocate(bindless::DescriptorHeap::SAMPLED_TEXTURE, redView);
        const auto greenIndex = heap->allocate(bindless::DescriptorHeap::SAMPLED_TEXTURE, greenView);
        REQUIRE(redIndex != bindless::DescriptorHeap::INVALID_DESCRIPTOR_INDEX);
        REQUIRE(greenIndex != bindless::DescriptorHeap::INVALID_DESCRIPTOR_INDEX);
        auto clearRed   = bindless::Raster::create("snapshot-clear-red", {.gpu = gpu, .target = &redTarget, .heap = heap});
        auto clearGreen = bindless::Raster::create("snapshot-clear-green", {.gpu = gpu, .target = &greenTarget, .heap = heap});
        REQUIRE(clearRed);
        REQUIRE(clearGreen);
        submitAndWait(gpu, "snapshot-init", clearRed->seal(), clearGreen->seal());
        target.setColorTarget(0, outputView).setClearColor(0, 0, 0, 1);
        auto raster = bindless::Raster::create("snapshot-draws", {.gpu = gpu, .target = &target, .heap = heap});
        REQUIRE(raster);
        RasterGeometry geometry;
        geometry.vertexCount = 3;
        bindless::Raster::DrawParameters draw {.geometry = geometry};
        draw.vs = makeShader(gpu, "snapshot-vs", kRttFullscreenVertSpv, sizeof(kRttFullscreenVertSpv));
        draw.ps = makeShader(gpu, "snapshot-ps", kBindlessTestFragSpv, sizeof(kBindlessTestFragSpv));
        REQUIRE(draw.vs);
        REQUIRE(draw.ps);
        draw.states.cullMode    = RasterState::CULL_NONE;
        draw.states.scissorRect = RasterState::ScissorRect {0, 0, 4, 4};
        uint32_t textureIndex = redIndex.slot;
        draw.immediates = makePushConstants(textureIndex);
        raster->recordDraw(draw);
        textureIndex = greenIndex.slot;
        draw.immediates = makePushConstants(textureIndex);
        geometry.vertexCount    = 0;
        draw.states.scissorRect = RasterState::ScissorRect {4, 0, 4, 4};
        raster->recordDraw(draw);
        geometry.vertexCount    = 3;
        draw.states.scissorRect = RasterState::ScissorRect {8, 0, 4, 4};
        raster->recordDraw(draw);
        geometry.vertexCount = 0;
        payload = raster->seal();
    }
    REQUIRE(payload);
    submitAndWait(gpu, "snapshot-render", payload);
    auto image = output->readback();
    REQUIRE_FALSE(image.empty());
    auto pixels = image.plane().toRGBA8(image.data());
    REQUIRE(pixels.size() == 48);
    for (size_t i = 0; i < pixels.size(); ++i) {
        CAPTURE(i);
        CHECK(pixels[i].r == (i % 12 < 4 ? 255 : 0));
        CHECK(pixels[i].g == (i % 12 >= 8 ? 255 : 0));
        CHECK(pixels[i].b == 0);
    }
}

#endif // GN_BUILD_HAS_VULKAN
