#if GN_BUILD_HAS_VULKAN

    #include "../vk-gpu-context.h"
    #include "bindless-cnc-test-comp.spv.h"

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

struct PushConstantData {
    uint32_t textureIndex;
    float    multiplier;
};

template<typename T>
static ArrayView<const uint8_t> makePushConstants(const T & val) {
    return ArrayView<const uint8_t>(reinterpret_cast<const uint8_t *>(&val), sizeof(T));
}

TEST_CASE("bindless::CnC: creation validation and conflict checks", "[gpu2][bindless][cnc]") {
    auto gpu = makeGpu();
    if (!gpu) SKIP("No GPU context available");

    auto heap = bindless::DescriptorHeap::create("test-heap", {.gpu = gpu, .capacity = 16});
    REQUIRE(heap);

    // 1. Null GPU returns empty
    {
        auto cnc = bindless::CnC::create("null-gpu", {.gpu = nullptr, .heap = heap});
        CHECK_FALSE(cnc);
    }

    // 2. Null heap returns empty
    {
        auto cnc = bindless::CnC::create("null-heap", {.gpu = gpu, .heap = nullptr});
        CHECK_FALSE(cnc);
    }

    // 3. Fast conflict check: passResources collides with heapSetIndex
    {
        auto dummyBuf = Buffer::create("dummy", Buffer::CreateParameters {.context = gpu, .size = 256});
        REQUIRE(dummyBuf);

        GpuResourceTable conflictTable;
        conflictTable.resize(1);
        conflictTable[0].resize(1);
        conflictTable[0][0].append(GpuResourceView(dummyBuf));

        // heapSetIndex = 0 collides with conflictTable[0]
        auto cncConflict = bindless::CnC::create("conflict", {.gpu = gpu, .heap = heap, .heapSetIndex = 0, .passResources = conflictTable});
        CHECK_FALSE(cncConflict);

        // heapSetIndex = 1 does NOT collide with conflictTable[0]
        auto cncCompat = bindless::CnC::create("compat", {.gpu = gpu, .heap = heap, .heapSetIndex = 1, .passResources = conflictTable});
        CHECK(cncCompat);
    }

    // 4. Pure bindless configuration succeeds
    {
        auto cnc = bindless::CnC::create("pure-bindless", {.gpu = gpu, .heap = heap, .heapSetIndex = 0});
        CHECK(cnc);
    }
}

TEST_CASE("bindless::CnC: retainResource, retainCleanup, and seal lifecycle", "[gpu2][bindless][cnc]") {
    auto gpu = makeGpu();
    if (!gpu) SKIP("No GPU context available");

    auto heap = bindless::DescriptorHeap::create("test-heap", {.gpu = gpu, .capacity = 16});
    REQUIRE(heap);

    auto cs = makeShader(gpu, "bindless-cnc-cs", kBindlessCncTestCompSpv, sizeof(kBindlessCncTestCompSpv));
    REQUIRE(cs);

    auto cnc = bindless::CnC::create("recorder", {.gpu = gpu, .heap = heap, .heapSetIndex = 0});
    REQUIRE(cnc);

    // Retain dynamic resource via AutoRef<Buffer>
    auto dummyBuffer = Buffer::create("dummy", Buffer::CreateParameters {.context = gpu, .size = 64});
    cnc->retainResource(dummyBuffer);

    // Retain generic arbitrary object (e.g. shared_ptr) and custom cleanup lambda
    auto               genericResource = std::make_shared<int>(42);
    std::weak_ptr<int> weakRef         = genericResource;
    cnc->retainResource(std::move(genericResource));

    bool cleanupCallbackFired = false;
    cnc->retainCleanup([&cleanupCallbackFired] { cleanupCallbackFired = true; });

    // Record a compute dispatch
    PushConstantData pc {0, 1.0f};
    cnc->recordCompute({.cs = cs, .x = 1, .y = 1, .z = 1, .immediates = makePushConstants(pc)});

    // Seal into GpuPayload
    auto payload = cnc->seal();
    REQUIRE(payload);

    // After seal: further recording or sealing must fail safely
    cnc->recordCompute({.cs = cs, .x = 1, .y = 1, .z = 1});
    CHECK_FALSE(cnc->seal());

    // Destroying the payload executes retained cleanups
    CHECK_FALSE(cleanupCallbackFired);
    CHECK_FALSE(weakRef.expired());
    payload.clear();
    CHECK(cleanupCallbackFired);
    CHECK(weakRef.expired());
}

TEST_CASE("bindless::CnC: sample bindless texture in compute and download results", "[gpu2][bindless][cnc][gpu]") {
    auto gpu = makeGpu();
    if (!gpu) SKIP("No GPU context available");

    constexpr uint32_t W = 8, H = 8;

    // 1. Create two textures: red (255, 0, 0, 255) and blue (0, 0, 255, 255)
    auto texRed  = makeRgba8Tex(gpu, "texRed", W, H);
    auto texBlue = makeRgba8Tex(gpu, "texBlue", W, H);
    REQUIRE(texRed);
    REQUIRE(texBlue);

    REQUIRE(texRed->setContent(makeSolidImage(W, H, 255, 0, 0, 255)));
    REQUIRE(texBlue->setContent(makeSolidImage(W, H, 0, 0, 255, 255)));

    // 2. Create descriptor heap and allocate both textures
    auto heap = bindless::DescriptorHeap::create("heap", {.gpu = gpu, .capacity = 16});
    REQUIRE(heap);

    uint32_t slotRed  = heap->allocate(GpuResourceView(texRed));
    uint32_t slotBlue = heap->allocate(GpuResourceView(texBlue));
    REQUIRE(slotRed != bindless::INVALID_DESCRIPTOR_INDEX);
    REQUIRE(slotBlue != bindless::INVALID_DESCRIPTOR_INDEX);
    REQUIRE(slotRed != slotBlue);

    // 3. Create storage output buffer for 16 float4 elements
    constexpr size_t bufferSize = 16 * sizeof(float) * 4;
    auto             outBuf     = Buffer::create("outBuf", Buffer::CreateParameters {.context = gpu, .size = bufferSize});
    REQUIRE(outBuf);

    GpuResourceTable passResources;
    passResources.resize(2); // Set 0 empty (heapSetIndex = 0), Set 1 has storage buffer
    passResources[1].resize(1);
    passResources[1][0].append(GpuResourceView(outBuf).setBufferViewType(GpuResourceView::BufferView::STORAGE));

    auto cs = makeShader(gpu, "bindless-cnc-cs", kBindlessCncTestCompSpv, sizeof(kBindlessCncTestCompSpv));
    REQUIRE(cs);

    // --- Pass A: Dispatch compute sampling slotRed with multiplier 2.0f ---
    {
        auto cnc = bindless::CnC::create("cnc-red", {.gpu = gpu, .heap = heap, .heapSetIndex = 0, .passResources = passResources, .maxImmediateSize = 128});
        REQUIRE(cnc);

        PushConstantData pc {slotRed, 2.0f};
        cnc->recordCompute({.cs = cs, .x = 1, .y = 1, .z = 1, .immediates = makePushConstants(pc)});

        auto downloadFuture = cnc->recordDownloadBuffer(outBuf, 0, bufferSize);

        auto payload = cnc->seal();
        REQUIRE(payload);

        submitAndWait(gpu, "submit-red", payload);

        REQUIRE(downloadFuture.valid());
        auto blob = downloadFuture.get();
        REQUIRE(blob);
        REQUIRE(blob->size() == bufferSize);

        const float * floats = reinterpret_cast<const float *>(blob->data());
        for (int i = 0; i < 16; ++i) {
            // Red sampled: (1.0, 0.0, 0.0, 1.0) * 2.0f = (2.0f, 0.0f, 0.0f, 2.0f)
            CHECK(std::abs(floats[i * 4 + 0] - 2.0f) < 0.02f);
            CHECK(std::abs(floats[i * 4 + 1] - 0.0f) < 0.02f);
            CHECK(std::abs(floats[i * 4 + 2] - 0.0f) < 0.02f);
            CHECK(std::abs(floats[i * 4 + 3] - 2.0f) < 0.02f);
        }
    }

    // --- Pass B: Dispatch compute sampling slotBlue with multiplier 0.5f ---
    {
        auto cnc = bindless::CnC::create("cnc-blue", {.gpu = gpu, .heap = heap, .heapSetIndex = 0, .passResources = passResources, .maxImmediateSize = 128});
        REQUIRE(cnc);

        PushConstantData pc {slotBlue, 0.5f};
        cnc->recordCompute({.cs = cs, .x = 1, .y = 1, .z = 1, .immediates = makePushConstants(pc)});

        auto downloadFuture = cnc->recordDownloadBuffer(outBuf, 0, bufferSize);

        auto payload = cnc->seal();
        REQUIRE(payload);

        submitAndWait(gpu, "submit-blue", payload);

        REQUIRE(downloadFuture.valid());
        auto blob = downloadFuture.get();
        REQUIRE(blob);
        REQUIRE(blob->size() == bufferSize);

        const float * floats = reinterpret_cast<const float *>(blob->data());
        for (int i = 0; i < 16; ++i) {
            // Blue sampled: (0.0, 0.0, 1.0, 1.0) * 0.5f = (0.0f, 0.0f, 0.5f, 0.5f)
            CHECK(std::abs(floats[i * 4 + 0] - 0.0f) < 0.02f);
            CHECK(std::abs(floats[i * 4 + 1] - 0.0f) < 0.02f);
            CHECK(std::abs(floats[i * 4 + 2] - 0.5f) < 0.02f);
            CHECK(std::abs(floats[i * 4 + 3] - 0.5f) < 0.02f);
        }
    }
}

TEST_CASE("bindless::CnC: interleaved compute and buffer copy", "[gpu2][bindless][cnc][gpu]") {
    auto gpu = makeGpu();
    if (!gpu) SKIP("No GPU context available");

    constexpr uint32_t W = 8, H = 8;
    auto               texGreen = makeRgba8Tex(gpu, "texGreen", W, H);
    REQUIRE(texGreen);
    REQUIRE(texGreen->setContent(makeSolidImage(W, H, 0, 255, 0, 255)));

    auto heap = bindless::DescriptorHeap::create("heap", {.gpu = gpu, .capacity = 16});
    REQUIRE(heap);
    uint32_t slotGreen = heap->allocate(GpuResourceView(texGreen));
    REQUIRE(slotGreen != bindless::INVALID_DESCRIPTOR_INDEX);

    constexpr size_t bufferSize = 16 * sizeof(float) * 4;
    auto             computeBuf = Buffer::create("computeBuf", Buffer::CreateParameters {.context = gpu, .size = bufferSize});
    auto             copyDstBuf = Buffer::create("copyDstBuf", Buffer::CreateParameters {.context = gpu, .size = bufferSize});
    REQUIRE(computeBuf);
    REQUIRE(copyDstBuf);

    GpuResourceTable passResources;
    passResources.resize(2);
    passResources[1].resize(1);
    passResources[1][0].append(GpuResourceView(computeBuf).setBufferViewType(GpuResourceView::BufferView::STORAGE));

    auto cs = makeShader(gpu, "bindless-cnc-cs", kBindlessCncTestCompSpv, sizeof(kBindlessCncTestCompSpv));
    REQUIRE(cs);

    auto cnc = bindless::CnC::create("interleaved", {.gpu = gpu, .heap = heap, .heapSetIndex = 0, .passResources = passResources, .maxImmediateSize = 128});
    REQUIRE(cnc);

    PushConstantData pc {slotGreen, 1.0f};
    cnc->recordCompute({.cs = cs, .x = 1, .y = 1, .z = 1, .immediates = makePushConstants(pc)});

    // Copy compute output to copyDstBuf within the same CnC pass
    cnc->recordCopyBufferToBuffer({.src = computeBuf, .dst = copyDstBuf, .srcOffset = 0, .dstOffset = 0, .size = bufferSize});

    // Download copyDstBuf
    auto downloadFuture = cnc->recordDownloadBuffer(copyDstBuf, 0, bufferSize);

    auto payload = cnc->seal();
    REQUIRE(payload);

    submitAndWait(gpu, "submit-interleaved", payload);

    REQUIRE(downloadFuture.valid());
    auto blob = downloadFuture.get();
    REQUIRE(blob);
    REQUIRE(blob->size() == bufferSize);

    const float * floats = reinterpret_cast<const float *>(blob->data());
    for (int i = 0; i < 16; ++i) {
        CHECK(std::abs(floats[i * 4 + 0] - 0.0f) < 0.02f);
        CHECK(std::abs(floats[i * 4 + 1] - 1.0f) < 0.02f);
        CHECK(std::abs(floats[i * 4 + 2] - 0.0f) < 0.02f);
        CHECK(std::abs(floats[i * 4 + 3] - 1.0f) < 0.02f);
    }
}

TEST_CASE("bindless::CnC: high-throughput dispatch recording stress test", "[gpu2][bindless][cnc]") {
    auto gpu = makeGpu();
    if (!gpu) SKIP("No GPU context available");

    auto heap = bindless::DescriptorHeap::create("stress-heap", {.gpu = gpu, .capacity = 16});
    REQUIRE(heap);

    auto cs = makeShader(gpu, "bindless-cnc-cs", kBindlessCncTestCompSpv, sizeof(kBindlessCncTestCompSpv));
    REQUIRE(cs);

    auto cnc = bindless::CnC::create("stress-cnc", {.gpu = gpu, .heap = heap, .heapSetIndex = 0, .maxImmediateSize = 128});
    REQUIRE(cnc);

    constexpr size_t kNumDispatches = 10000;
    for (uint32_t i = 0; i < kNumDispatches; ++i) {
        PushConstantData pc {i % 16, static_cast<float>(i)};
        cnc->recordCompute({.cs = cs, .x = 1, .y = 1, .z = 1, .immediates = makePushConstants(pc)});
    }

    auto payload = cnc->seal();
    REQUIRE(payload);
}

#endif // GN_BUILD_HAS_VULKAN
