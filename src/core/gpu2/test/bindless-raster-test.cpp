#if GN_BUILD_HAS_VULKAN

    #include "../vk-gpu-context.h"
    #include "../vk-bindless-payload.h"
    #include "../vk-texture.h"

    #include "bindless-test-vert.spv.h"
    #include "bindless-test-frag.spv.h"

    #include <catch2/catch_test_macros.hpp>
    #include <garnet/GNgpu2.h>
    #include "gpu2-test-helpers.h"

using namespace GN;
using namespace GN::gpu2;

namespace {
class CountingDrawResource final : public std::pmr::memory_resource {
public:
    size_t allocations = 0;

private:
    void * do_allocate(size_t bytes, size_t alignment) override {
        ++allocations;
        return std::pmr::new_delete_resource()->allocate(bytes, alignment);
    }
    void do_deallocate(void * p, size_t bytes, size_t alignment) override { std::pmr::new_delete_resource()->deallocate(p, bytes, alignment); }
    bool do_is_equal(const std::pmr::memory_resource & other) const noexcept override { return this == &other; }
};
} // namespace

TEST_CASE("bindless::Raster: preallocated geometry backing and fallback", "[gpu2][bindless][allocation]") {
    CountingDrawResource upstream;
    auto                 storage = std::make_unique<BindlessDrawStorage>(100, &upstream);
    RasterGeometry       source;
    source.format.attributes.resize(64);
    source.vertices.resize(8);
    source.vertices[0].offset = 123;
    bindless::Raster::DrawParameters params {.geometry = source};
    for (size_t i = 0; i < 100; ++i) storage->draws.emplace_back(params, RasterState {}, &storage->pool, 0, 0);
    CHECK(upstream.allocations == 0);

    const auto * vertices       = storage->draws.front().geometry.vertices.data();
    auto         payloadStorage = std::move(storage);
    CHECK(payloadStorage->draws.front().geometry.vertices.data() == vertices);
    // Growing after recording must preserve the original backing and allow upstream fallback.
    payloadStorage->draws.reserve(1000);
    for (size_t i = 100; i < 1000; ++i) payloadStorage->draws.emplace_back(params, RasterState {}, &payloadStorage->pool, 0, 0);
    CHECK(upstream.allocations > 0);
    CHECK(payloadStorage->draws.front().geometry.vertices.data() == vertices);
    CHECK(payloadStorage->draws.front().geometry.vertices[0].offset == 123);
    CHECK(payloadStorage->draws.back().geometry.format.attributes.size() == 64);
    CHECK(payloadStorage->draws.back().geometry.vertices.size() == 8);
}

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
static ArrayView<const uint8_t> makePushConstants(const T & val) {
    return ArrayView<const uint8_t>(reinterpret_cast<const uint8_t *>(&val), sizeof(T));
}

TEST_CASE("bindless::Raster: creation validation and conflict checks", "[gpu2][bindless]") {
    auto gpu = makeGpu();
    if (!gpu) SKIP("No GPU context available");

    constexpr uint32_t W = 16, H = 16;
    auto               targetTex = makeRgba8Tex(gpu, "rt", W, H);
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
        auto         r2 = bindless::Raster::create("empty-target", {.gpu = gpu, .target = &emptyRt, .heap = heap});
        CHECK_FALSE(r2);
    }

    // 3. Fast conflict check: passResources collides with heapSetIndex
    {
        GpuResourceTable conflictTable;
        conflictTable.resize(1);
        conflictTable[0].resize(1);
        conflictTable[0][0].append(GpuResourceView(targetTex));

        // heapSetIndex = 0 collides with conflictTable[0]: creation fails
        auto r = bindless::Raster::create("conflict", {.gpu = gpu, .target = &rt, .heap = heap, .heapSetIndex = 0, .passResources = conflictTable});
        CHECK_FALSE(r);

        // heapSetIndex = 1 does NOT collide with conflictTable[0]: creation succeeds
        auto rCompat = bindless::Raster::create("compat", {.gpu = gpu, .target = &rt, .heap = heap, .heapSetIndex = 1, .passResources = conflictTable});
        CHECK(rCompat);
    }

    // 4. Pure bindless configuration succeeds
    {
        auto r = bindless::Raster::create("pure-bindless", {.gpu = gpu, .target = &rt, .heap = heap, .heapSetIndex = 0});
        CHECK(r);
    }
}

TEST_CASE("bindless::Raster: recordDraw, retainResource, and seal lifecycle", "[gpu2][bindless]") {
    auto gpu = makeGpu();
    if (!gpu) SKIP("No GPU context available");

    constexpr uint32_t W = 16, H = 16;
    auto               targetTex = makeRgba8Tex(gpu, "rt", W, H);
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

    // Retain dynamic resource via AutoRef<Buffer>
    auto dummyBuffer = Buffer::create("dummy", Buffer::CreateParameters {.context = gpu, .size = 64, .mappable = false});
    raster->retainResource(dummyBuffer);

    // Retain generic arbitrary object (e.g. shared_ptr) and custom cleanup lambda
    auto               genericResource = std::make_shared<int>(42);
    std::weak_ptr<int> weakRef         = genericResource;
    raster->retainResource(std::move(genericResource));

    bool cleanupCallbackFired = false;
    raster->addCleanupCallback([&cleanupCallbackFired] { cleanupCallbackFired = true; });

    // Record draw call
    RasterGeometry emptyGeom {};
    emptyGeom.vertexCount      = 3;
    uint32_t pushConstantIndex = 0;

    raster->recordDraw({.vs = vs, .ps = ps, .geometry = emptyGeom, .immediates = makePushConstants(pushConstantIndex)});

    // Seal into GpuPayload
    auto payload = raster->seal();
    REQUIRE(payload);

    // After seal: further recording or sealing must fail safely
    raster->recordDraw({.vs = vs, .ps = ps, .geometry = emptyGeom});
    CHECK_FALSE(raster->seal());

    // Destroying the payload (or GPU completion) executes retained cleanups
    CHECK_FALSE(cleanupCallbackFired);
    CHECK_FALSE(weakRef.expired());
    payload.clear();
    CHECK(cleanupCallbackFired);
    CHECK(weakRef.expired());
}

TEST_CASE("bindless::Raster: render textured quad via bindless heap with auto-restore invariant", "[gpu2][bindless][gpu]") {
    auto gpu = makeGpu();
    if (!gpu) SKIP("No GPU context available");

    constexpr uint32_t W = 8, H = 8;

    // 1. Create two textures: red and green
    auto texRed   = makeRgba8Tex(gpu, "texRed", W, H);
    auto texGreen = makeRgba8Tex(gpu, "texGreen", W, H);
    REQUIRE(texRed);
    REQUIRE(texGreen);

    REQUIRE(texRed->setContent(makeSolidImage(W, H, 255, 0, 0, 255)));
    REQUIRE(texGreen->setContent(makeSolidImage(W, H, 0, 255, 0, 255)));

    // 2. Create descriptor heap and allocate both textures
    auto heap = bindless::DescriptorHeap::create("heap", {.gpu = gpu, .capacity = 16});
    REQUIRE(heap);
    auto sharedSampler = Sampler::create("shared-sampler", {.context = gpu});
    REQUIRE(sharedSampler);
    REQUIRE(heap->allocate(bindless::DescriptorHeap::SAMPLER, GpuResourceView(sharedSampler)).slot == 0);

    auto slotRed   = heap->allocate(bindless::DescriptorHeap::SAMPLED_TEXTURE, GpuResourceView(texRed));
    auto slotGreen = heap->allocate(bindless::DescriptorHeap::SAMPLED_TEXTURE, GpuResourceView(texGreen));
    REQUIRE(slotRed != bindless::DescriptorHeap::INVALID_DESCRIPTOR_INDEX);
    REQUIRE(slotGreen != bindless::DescriptorHeap::INVALID_DESCRIPTOR_INDEX);
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
        auto raster = bindless::Raster::create("draw-green", {.gpu = gpu, .target = &rt, .heap = heap, .heapSetIndex = 0, .maxImmediateSize = 128});
        REQUIRE(raster);

        RasterGeometry geom {};
        geom.vertexCount = 3; // fullscreen triangle

        raster->recordDraw({.vs = vs, .ps = ps, .geometry = geom, .immediates = makePushConstants(slotGreen.slot)});

        auto payload = raster->seal();
        REQUIRE(payload);

        submitAndWait(gpu, "submit-green", payload);
    }

    // Verify rendered pixels: green = (0, 255, 0, 255)
    {
        gfx::img::Image result = targetTex->readback();
        checkPixels(result, 0, 255, 0, 255);
    }

    // --- Pass B: Render using slotRed (expecting solid red output) ---
    {
        auto raster = bindless::Raster::create("draw-red", {.gpu = gpu, .target = &rt, .heap = heap, .heapSetIndex = 0, .maxImmediateSize = 128});
        REQUIRE(raster);

        RasterGeometry geom {};
        geom.vertexCount = 3;

        raster->recordDraw({.vs = vs, .ps = ps, .geometry = geom, .immediates = makePushConstants(slotRed.slot)});

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

TEST_CASE("GpuContext::caps: maxImmediateSize and maxBindlessSampledImages", "[gpu2]") {
    auto gpu = makeGpu();
    if (!gpu) SKIP("No GPU context available");

    auto c = gpu->caps();
    CHECK(c.maxImmediateSize >= 128);
    CHECK(c.maxBindlessSampledImages >= 16);
}

#endif // GN_BUILD_HAS_VULKAN
