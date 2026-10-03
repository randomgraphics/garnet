#if GN_BUILD_HAS_VULKAN

    #include "../vk-gpu-context.h"
    #include "../vk-texture.h"

    #include "bindless-test-vert.spv.h"
    #include "bindless-test-frag.spv.h"

    #include <catch2/catch_test_macros.hpp>
    #include <garnet/GNgpu2.h>
    #include "gpu2-test-helpers.h"

using namespace GN;
using namespace GN::gpu2;

static gfx::img::Image makeSolidImage(uint32_t w, uint32_t h, uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) {
    gfx::img::Extent3D extent;
    extent.set(w, h, 1);
    gfx::img::PlaneDesc planeDesc = gfx::img::PlaneDesc::make(gfx::img::PixelFormat::RGBA8(), extent);
    gfx::img::ImageDesc imageDesc = gfx::img::ImageDesc::make(planeDesc, 1, 1, 1);
    gfx::img::Image     image(imageDesc);
    auto *              p = (uint8_t *) image.data();
    for (size_t i = 0; i < image.size(); i += 4) {
        p[i + 0] = r;
        p[i + 1] = g;
        p[i + 2] = b;
        p[i + 3] = a;
    }
    return image;
}

static void checkPixels(const gfx::img::Image & image, uint8_t er, uint8_t eg, uint8_t eb, uint8_t ea = 255) {
    REQUIRE_FALSE(image.empty());
    auto pixels = image.plane().toRGBA8(image.data());
    REQUIRE_FALSE(pixels.empty());
    for (const auto & px : pixels) {
        CHECK(px.r == er);
        CHECK(px.g == eg);
        CHECK(px.b == eb);
        CHECK(px.a == ea);
    }
}

template<typename T>
static AutoRef<const Blob> makePushConstants(const T & val) {
    return AutoRef<const Blob>(new SimpleBlob<uint8_t>(sizeof(T), reinterpret_cast<const uint8_t *>(&val)));
}

TEST_CASE("bindless::Raster: creation validation and conflict checks", "[gpu2][bindless]") {
    auto gpu = makeGpu();
    if (!gpu) SKIP("No GPU context available");

    constexpr uint32_t W = 16, H = 16;
    auto targetTex = makeRgba8Tex(gpu, "rt", W, H);
    REQUIRE(targetTex);

    RasterTarget rt;
    rt.colorTargets.append(RasterTarget::ColorTarget(GpuResourceView(targetTex)));

    auto heap = bindless::DescriptorHeap::create("test-heap", {.gpu = gpu, .capacity = 16});
    REQUIRE(heap);

    // 1. Null GPU returns empty
    {
        auto r = bindless::Raster::create("null-gpu", {.gpu = nullptr, .target = &rt, .heap = heap});
        CHECK_FALSE(r);
    }

    // 2. Null or empty target returns empty
    {
        auto r = bindless::Raster::create("null-target", {.gpu = gpu, .target = nullptr, .heap = heap});
        CHECK_FALSE(r);

        RasterTarget emptyRt;
        auto r2 = bindless::Raster::create("empty-target", {.gpu = gpu, .target = &emptyRt, .heap = heap});
        CHECK_FALSE(r2);
    }

    // 3. Fast conflict check: passResources collides with heapSetIndex
    {
        GpuResourceTable conflictTable;
        conflictTable.resize(1);
        conflictTable[0].resize(1);
        conflictTable[0][0].append(GpuResourceView(targetTex));

        // heapSetIndex = 0 collides with conflictTable[0]
        auto r = bindless::Raster::create("conflict", {.gpu = gpu, .target = &rt, .heap = heap, .heapSetIndex = 0,
                                                       .passResources = conflictTable});
        CHECK_FALSE(r);

        // heapSetIndex = 1 does NOT collide with conflictTable[0]
        auto rCompat = bindless::Raster::create("compat", {.gpu = gpu, .target = &rt, .heap = heap, .heapSetIndex = 1,
                                                           .passResources = conflictTable});
        CHECK(rCompat);
    }

    // 4. Pure bindless configuration succeeds
    {
        auto r = bindless::Raster::create("pure-bindless", {.gpu = gpu, .target = &rt, .heap = heap,
                                                            .heapSetIndex = 0});
        CHECK(r);
        CHECK(r->target().colorTargets.size() == 1);
    }
}

TEST_CASE("bindless::Raster: recordDraw, retainResource, and seal lifecycle", "[gpu2][bindless]") {
    auto gpu = makeGpu();
    if (!gpu) SKIP("No GPU context available");

    constexpr uint32_t W = 16, H = 16;
    auto targetTex = makeRgba8Tex(gpu, "rt", W, H);
    REQUIRE(targetTex);

    RasterTarget rt;
    rt.colorTargets.append(RasterTarget::ColorTarget(GpuResourceView(targetTex)));

    auto heap = bindless::DescriptorHeap::create("test-heap", {.gpu = gpu, .capacity = 16});
    REQUIRE(heap);

    auto vs = makeShader(gpu, "bindless-vert", kBindlessTestVertSpv, sizeof(kBindlessTestVertSpv));
    auto ps = makeShader(gpu, "bindless-frag", kBindlessTestFragSpv, sizeof(kBindlessTestFragSpv));
    REQUIRE(vs);
    REQUIRE(ps);

    auto raster = bindless::Raster::create("recorder", {.gpu = gpu, .target = &rt, .heap = heap, .heapSetIndex = 0});
    REQUIRE(raster);

    // Retain dynamic resource
    auto dummyBuffer = Buffer::create("dummy", Buffer::CreateParameters {.context = gpu, .size = 64, .mappable = false});
    raster->retainResource(dummyBuffer);

    // Record draw call
    RasterGeometry emptyGeom {};
    emptyGeom.vertexCount = 3;
    uint32_t pushConstantIndex = 0;

    raster->recordDraw({.vs = vs, .ps = ps, .geometry = emptyGeom, .immediates = makePushConstants(pushConstantIndex)});

    // Seal into GpuPayload
    auto payload = raster->seal();
    REQUIRE(payload);

    // After seal: further recording or sealing must fail safely
    raster->recordDraw({.vs = vs, .ps = ps, .geometry = emptyGeom});
    CHECK_FALSE(raster->seal());
}

TEST_CASE("bindless::Raster: render textured quad via bindless heap with auto-restore invariant",
          "[gpu2][bindless][gpu]") {
    auto gpu = makeGpu();
    if (!gpu) SKIP("No GPU context available");

    constexpr uint32_t W = 8, H = 8;

    // 1. Create two textures: red and green
    auto texRed = makeRgba8Tex(gpu, "texRed", W, H);
    auto texGreen = makeRgba8Tex(gpu, "texGreen", W, H);
    REQUIRE(texRed);
    REQUIRE(texGreen);

    REQUIRE(texRed->setContent(makeSolidImage(W, H, 255, 0, 0, 255)));
    REQUIRE(texGreen->setContent(makeSolidImage(W, H, 0, 255, 0, 255)));

    // 2. Create descriptor heap and allocate both textures
    auto heap = bindless::DescriptorHeap::create("heap", {.gpu = gpu, .capacity = 16});
    REQUIRE(heap);

    uint32_t slotRed = heap->allocate(GpuResourceView(texRed));
    uint32_t slotGreen = heap->allocate(GpuResourceView(texGreen));
    REQUIRE(slotRed != bindless::INVALID_DESCRIPTOR_INDEX);
    REQUIRE(slotGreen != bindless::INVALID_DESCRIPTOR_INDEX);
    REQUIRE(slotRed != slotGreen);

    // 3. Create render target
    auto targetTex = makeRgba8Tex(gpu, "target", W, H);
    REQUIRE(targetTex);

    RasterTarget rt;
    rt.colorTargets.append(RasterTarget::ColorTarget(GpuResourceView(targetTex)));
    rt.setClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    auto vs = makeShader(gpu, "bindless-vert", kBindlessTestVertSpv, sizeof(kBindlessTestVertSpv));
    auto ps = makeShader(gpu, "bindless-frag", kBindlessTestFragSpv, sizeof(kBindlessTestFragSpv));
    REQUIRE(vs);
    REQUIRE(ps);

    // --- Pass A: Render using slotGreen (expecting solid green output) ---
    {
        auto raster = bindless::Raster::create("draw-green", {.gpu = gpu, .target = &rt, .heap = heap,
                                                              .heapSetIndex = 0, .pushConstantSize = 128});
        REQUIRE(raster);

        RasterGeometry geom {};
        geom.vertexCount = 3; // fullscreen triangle

        raster->recordDraw({.vs = vs, .ps = ps, .geometry = geom, .immediates = makePushConstants(slotGreen)});

        auto payload = raster->seal();
        REQUIRE(payload);

        submitAndWait(gpu, "submit-green", payload);
    }

    // Invariant check: after bindless pass ends, the render target must be automatically restored
    // to SHADER_READ_ONLY_OPTIMAL layout on GPU.
    {
        auto * vkTex = RuntimeType::cast<TextureVulkanBase>(targetTex.get());
        REQUIRE(vkTex);
        const auto & state = vkTex->getState();
        const auto * plane = state.get(0, 0, vk::ImageAspectFlagBits::eColor);
        REQUIRE(plane);
        CHECK(plane->layout == vk::ImageLayout::eShaderReadOnlyOptimal);
    }

    // Verify rendered pixels: green = (0, 255, 0, 255)
    {
        gfx::img::Image result = targetTex->readback();
        checkPixels(result, 0, 255, 0, 255);
    }

    // --- Pass B: Render using slotRed (expecting solid red output) ---
    {
        auto raster = bindless::Raster::create("draw-red", {.gpu = gpu, .target = &rt, .heap = heap,
                                                            .heapSetIndex = 0, .pushConstantSize = 128});
        REQUIRE(raster);

        RasterGeometry geom {};
        geom.vertexCount = 3;

        raster->recordDraw({.vs = vs, .ps = ps, .geometry = geom, .immediates = makePushConstants(slotRed)});

        auto payload = raster->seal();
        REQUIRE(payload);

        submitAndWait(gpu, "submit-red", payload);
    }

    // Verify rendered pixels: red = (255, 0, 0, 255)
    {
        gfx::img::Image result = targetTex->readback();
        checkPixels(result, 255, 0, 0, 255);
    }
}

#endif // GN_BUILD_HAS_VULKAN
