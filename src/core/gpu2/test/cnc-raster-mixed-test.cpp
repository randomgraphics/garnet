#if GN_BUILD_HAS_VULKAN

    // Include the Vulkan context header first — it pulls in rapid-vulkan before any other TU can
    // do so (ODR risk enforced by a #error guard in vk-gpu-context.h).
    #include "../vk-gpu-context.h"
    #include "mixed-cnc-raster-comp.spv.h"
    #include "rtt-fullscreen-vert.spv.h"
    #include "bindless-test-frag.spv.h"

    #include <catch2/catch_test_macros.hpp>
    #include <garnet/GNgpu2.h>

using namespace GN;
using namespace GN::gpu2;

    #include "gpu2-test-helpers.h"

// ---------------------------------------------------------------------------
// Test: bindless CnC upload → bindless raster sample → bindless compute copy → readback
// Verifies cross-payload barrier correctness across three payload types
// in a single submission batch.
// ---------------------------------------------------------------------------
TEST_CASE("GPU2/CnC+Raster: upload->render->compute copy->readback", "[gpu2][cnc][raster][gpu]") {
    auto gpu = makeGpu();
    if (!gpu) SKIP("No GPU context available");

    constexpr uint32_t W = 8, H = 8;
    constexpr uint32_t PIXEL_COUNT = W * H;

    // Create all three textures
    auto sourceTex    = makeRgba8Tex(gpu, "source", W, H); // upload destination
    auto renderTarget = makeRgba8Tex(gpu, "rt", W, H);     // raster color attachment
    auto outputTex    = makeRgba8Tex(gpu, "output", W, H); // compute output
    if (!sourceTex || !renderTarget || !outputTex) SKIP("RGBA8 textures unavailable");

    // Staging buffer: solid red (R=255, G=0, B=0, A=255) for every pixel.
    auto staging = Buffer::create("staging", {.context = gpu, .size = PIXEL_COUNT * 4u, .mappable = true});
    if (!staging) SKIP("Mappable buffer unavailable");
    {
        auto   m = staging->map();
        auto * p = static_cast<uint8_t *>(m.data());
        for (uint32_t i = 0; i < PIXEL_COUNT; ++i) {
            // solid red: R=255, G=0, B=0, A=255
            p[i * 4 + 0] = 255;
            p[i * 4 + 1] = 0;
            p[i * 4 + 2] = 0;
            p[i * 4 + 3] = 255;
        }
    }

    auto heap = bindless::DescriptorHeap::create("mixed-heap", {.gpu = gpu, .capacity = 16});
    REQUIRE(heap);
    auto sampler = Sampler::create("mixed-sampler", {.context = gpu});
    REQUIRE(sampler);
    REQUIRE(heap->allocate(bindless::DescriptorHeap::SAMPLER, GpuResourceView {sampler}).slot == 0);
    const auto sampledSourceIndex = heap->allocate(bindless::DescriptorHeap::SAMPLED_TEXTURE, GpuResourceView {sourceTex});
    REQUIRE(sampledSourceIndex != bindless::DescriptorHeap::INVALID_DESCRIPTOR_INDEX);

    // ── Payload 1: heap-free bindless CnC upload staging → sourceTex ────
    bindless::CnC::Region region;
    region.imageOffset = {0, 0, 0};
    region.imageExtent = {W, H, 1};

    auto cnc1 = bindless::CnC::create("mixed-upload", {.gpu = gpu});
    REQUIRE(cnc1);
    {
        auto mapped = staging->map();
        cnc1->recordUploadImage(sourceTex, {(const uint8_t *) mapped.data(), mapped.size()}, ArrayView<const bindless::CnC::Region>(&region, 1));
    }
    AutoRef<GpuPayload> p1 = cnc1->seal();

    // ── Payload 2: bindless Raster — sample sourceTex → renderTarget ────
    auto vs = makeShader(gpu, "rtt-vert", kRttFullscreenVertSpv, sizeof(kRttFullscreenVertSpv));
    auto ps = makeShader(gpu, "rtt-frag", kBindlessTestFragSpv, sizeof(kBindlessTestFragSpv));
    REQUIRE(vs);
    REQUIRE(ps);

    GpuResourceView rtView;
    rtView.resource = renderTarget;

    RasterTarget rt;
    rt.colorTargets.append(RasterTarget::ColorTarget(rtView));
    rt.setClearColor(0.0f, 0.0f, 0.0f);

    auto raster = bindless::Raster::create("cnc-raster", {.gpu = gpu, .target = &rt, .heap = heap});
    REQUIRE(raster);

    RasterGeometry dpGeometry;
    dpGeometry.vertexCount = 3;
    const uint32_t sampledSourceSlot = sampledSourceIndex.slot;

    raster->recordDraw({.vs = vs, .ps = ps, .geometry = dpGeometry, .immediates = {reinterpret_cast<const uint8_t *>(&sampledSourceSlot), sizeof(sampledSourceSlot)}});
    AutoRef<GpuPayload> p2 = raster->seal();

    // ── Payload 3: bindless CnC compute — copy renderTarget → outputTex ─
    auto cs = makeShader(gpu, "mixed-comp", kMixedCncRasterCompSpv, sizeof(kMixedCncRasterCompSpv));
    REQUIRE(cs);

    // Storage-image descriptor indices select renderTarget (input) and outputTex (output) in set 0.
    GpuResourceView inputView;
    inputView.resource = renderTarget;
    inputView.setImageViewType(GpuResourceView::ImageView::STORAGE);

    GpuResourceView outputView;
    outputView.resource = outputTex;
    outputView.setImageViewType(GpuResourceView::ImageView::STORAGE);

    const auto inputIndex  = heap->allocate(bindless::DescriptorHeap::STORAGE_TEXTURE, inputView);
    const auto outputIndex = heap->allocate(bindless::DescriptorHeap::STORAGE_TEXTURE, outputView);
    REQUIRE(inputIndex != bindless::DescriptorHeap::INVALID_DESCRIPTOR_INDEX);
    REQUIRE(outputIndex != bindless::DescriptorHeap::INVALID_DESCRIPTOR_INDEX);

    // Set 1 pass resources declare storage-image hazards to the state tracker. The shader accesses
    // these same images through descriptor indices in the bindless heap at set 0.
    GpuResourceTable computeResources;
    computeResources.resize(2);
    computeResources[1].resize(2);
    computeResources[1][0].append(inputView);
    computeResources[1][1].append(outputView);

    auto cnc2 = bindless::CnC::create("mixed-compute", {.gpu = gpu, .heap = heap, .heapSetIndex = 0, .passResources = computeResources});
    REQUIRE(cnc2);
    struct ComputeIndices {
        uint32_t input;
        uint32_t output;
    } indices {inputIndex.slot, outputIndex.slot};
    cnc2->recordCompute({
        .cs         = cs,
        // Dispatch enough groups to cover W×H pixels with 8×8 local size
        .x = (W + 7) / 8,
        .y = (H + 7) / 8,
        .immediates = {reinterpret_cast<const uint8_t *>(&indices), sizeof(indices)},
    });
    AutoRef<GpuPayload> p3 = cnc2->seal();

    // Keep heap, descriptors, shader inputs and all three payloads alive through the ordered submit.
    // ── Submit all three payloads in one batch ────────────────────────────
    submitAndWait(gpu, "mixed-cnc-raster", p1, p2, p3);

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
