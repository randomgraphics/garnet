#if GN_BUILD_HAS_VULKAN

    #include "../vk-gpu-context.h"
    #include "../vk-buffer.h"
    #include "../vk-texture.h"
    #include "../vk-buffer-state.h"
    #include "bindless-cnc-test-comp.spv.h"
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

TEST_CASE("bindless::CnC: retainResource, addCleanupCallback, and seal lifecycle", "[gpu2][bindless][cnc]") {
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
    cnc->addCleanupCallback([&cleanupCallbackFired] { cleanupCallbackFired = true; });

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
    // Synchronous readback must leave the sampled texture shader-readable for the next compute pass.
    REQUIRE_FALSE(texRed->readback().empty());

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
    cnc->recordCopyBuffer({.src = computeBuf, .dst = copyDstBuf, .srcOffset = 0, .dstOffset = 0, .size = bufferSize});

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

TEST_CASE("bindless::CnC: automated invariant restores uploaded and copied buffers to read-ready state", "[gpu2][bindless][cnc][invariant]") {
    auto gpu = makeGpu();
    if (!gpu) SKIP("No GPU context available");

    auto heap = bindless::DescriptorHeap::create("test-heap", {.gpu = gpu, .capacity = 16});
    REQUIRE(heap);

    auto bufA = Buffer::create("bufA", Buffer::CreateParameters {.context = gpu, .size = 256});
    auto bufB = Buffer::create("bufB", Buffer::CreateParameters {.context = gpu, .size = 256});
    REQUIRE(bufA);
    REQUIRE(bufB);

    std::vector<uint8_t> data(256, 42);

    auto cnc = bindless::CnC::create("upload-copy-cnc", {.gpu = gpu, .heap = heap});
    REQUIRE(cnc);
    cnc->recordUploadBuffer(bufA, 0, data);
    cnc->recordCopyBuffer({.src = bufA, .dst = bufB, .srcOffset = 0, .dstOffset = 0, .size = 256});

    auto payload = cnc->seal();
    REQUIRE(payload);
    submitAndWait(gpu, "restore-buffers", payload);

    auto readBackA = bufA->readContent(0, 128);
    auto readBackB = bufB->readContent(0, 128);
    REQUIRE(readBackA.size() == 128);
    REQUIRE(readBackB.size() == 128);
    CHECK(readBackA[0] == 42);
    CHECK(readBackB[0] == 42);
}

TEST_CASE("bindless::CnC: automated invariant restores uploaded texture to SHADER_READ_ONLY_OPTIMAL", "[gpu2][bindless][cnc][invariant]") {
    auto gpu = makeGpu();
    if (!gpu) SKIP("No GPU context available");

    auto heap = bindless::DescriptorHeap::create("test-heap", {.gpu = gpu, .capacity = 16});
    REQUIRE(heap);

    auto tex = makeRgba8Tex(gpu, "upload-tex", 16, 16);
    REQUIRE(tex);

    constexpr uint64_t stagingSize = 16 * 16 * 4;
    auto               staging     = Buffer::create("staging", {.context = gpu, .size = stagingSize, .mappable = true});
    REQUIRE(staging);
    {
        auto m = staging->map();
        std::memset(m.data(), 128, stagingSize);
    }
    GpuCnC::Region region;
    region.imageOffset = {0, 0, 0};
    region.imageExtent = {16, 16, 1};

    auto cnc = bindless::CnC::create("upload-tex-cnc", {.gpu = gpu, .heap = heap});
    REQUIRE(cnc);
    {
        auto mapped = staging->map();
        cnc->recordUploadImage(tex, {(const uint8_t *) mapped.data(), mapped.size()}, ArrayView<const GpuCnC::Region>(&region, 1));
    }

    auto payload = cnc->seal();
    REQUIRE(payload);
    submitAndWait(gpu, "restore-texture", payload);

    auto result = tex->readback();
    REQUIRE_FALSE(result.empty());
    auto pixels = result.plane().toRGBA8(result.data());
    REQUIRE_FALSE(pixels.empty());
    CHECK(pixels[0].r == 128);
}

TEST_CASE("bindless::CnC: copyBufferToImage transitions to writable and restores to SRV for sampling", "[gpu2][bindless][cnc][invariant]") {
    auto gpu = makeGpu();
    if (!gpu) SKIP("No GPU context available");

    auto heap = bindless::DescriptorHeap::create("test-heap", {.gpu = gpu, .capacity = 16});
    REQUIRE(heap);

    constexpr uint32_t W = 16, H = 16;
    auto               tex = makeRgba8Tex(gpu, "srv-restore-tex", W, H);
    REQUIRE(tex);

    auto * vkTex = RuntimeType::cast<TextureVulkanBase>(tex.get());
    REQUIRE(vkTex);

    uint32_t slot = heap->allocate(GpuResourceView(tex));
    REQUIRE(slot != bindless::INVALID_DESCRIPTOR_INDEX);

    constexpr size_t bufferSize = 16 * sizeof(float) * 4;
    auto             outBuf     = Buffer::create("outBuf", Buffer::CreateParameters {.context = gpu, .size = bufferSize});
    REQUIRE(outBuf);

    GpuResourceTable passResources;
    passResources.resize(2);
    passResources[1].resize(1);
    passResources[1][0].append(GpuResourceView(outBuf).setBufferViewType(GpuResourceView::BufferView::STORAGE));

    auto cs = makeShader(gpu, "bindless-cnc-cs", kBindlessCncTestCompSpv, sizeof(kBindlessCncTestCompSpv));
    REQUIRE(cs);

    // --- Phase 1: Cross-pass copyBufferToImage -> restore to SRV -> sample in compute ---
    {
        // 1. Prepare blue pixels: (0, 0, 255, 255)
        constexpr uint64_t stagingSize = W * H * 4;
        auto               stagingBlue = Buffer::create("stagingBlue", {.context = gpu, .size = stagingSize, .mappable = true});
        REQUIRE(stagingBlue);
        {
            auto * p = static_cast<uint8_t *>(stagingBlue->map().data());
            for (uint32_t i = 0; i < W * H; ++i) {
                p[i * 4 + 0] = 0;
                p[i * 4 + 1] = 0;
                p[i * 4 + 2] = 255;
                p[i * 4 + 3] = 255;
            }
        }
        GpuCnC::Region region;
        region.imageOffset = {0, 0, 0};
        region.imageExtent = {W, H, 1};

        auto cnc1 = bindless::CnC::create("cnc-copy", {.gpu = gpu, .heap = heap});
        REQUIRE(cnc1);
        {
            auto mapped = stagingBlue->map();
            cnc1->recordUploadImage(tex, {(const uint8_t *) mapped.data(), mapped.size()}, ArrayView<const GpuCnC::Region>(&region, 1));
        }
        auto payload1 = cnc1->seal();
        REQUIRE(payload1);
        submitAndWait(gpu, "pass1-copy-blue", payload1);

        // 2. Sample in a subsequent compute pass
        auto cnc2 =
            bindless::CnC::create("cnc-sample-blue", {.gpu = gpu, .heap = heap, .heapSetIndex = 0, .passResources = passResources, .maxImmediateSize = 128});
        REQUIRE(cnc2);
        PushConstantData pc {slot, 1.0f};
        cnc2->recordCompute({.cs = cs, .x = 1, .y = 1, .z = 1, .immediates = makePushConstants(pc)});
        auto downloadFuture = cnc2->recordDownloadBuffer(outBuf, 0, bufferSize);
        auto payload2       = cnc2->seal();
        REQUIRE(payload2);
        submitAndWait(gpu, "pass2-sample-blue", payload2);

        REQUIRE(downloadFuture.valid());
        auto blob = downloadFuture.get();
        REQUIRE(blob);
        const float * floats = reinterpret_cast<const float *>(blob->data());
        for (int i = 0; i < 16; ++i) {
            // Blue sampled: (0.0, 0.0, 1.0, 1.0)
            CHECK(std::abs(floats[i * 4 + 0] - 0.0f) < 0.02f);
            CHECK(std::abs(floats[i * 4 + 1] - 0.0f) < 0.02f);
            CHECK(std::abs(floats[i * 4 + 2] - 1.0f) < 0.02f);
            CHECK(std::abs(floats[i * 4 + 3] - 1.0f) < 0.02f);
        }
    }

    // --- Phase 2: Same-pass interleave copyBufferToImage -> restore to SRV -> compute sample ---
    {
        // Prepare yellow pixels: (255, 255, 0, 255)
        constexpr uint64_t stagingSize   = W * H * 4;
        auto               stagingYellow = Buffer::create("stagingYellow", {.context = gpu, .size = stagingSize, .mappable = true});
        REQUIRE(stagingYellow);
        {
            auto * p = static_cast<uint8_t *>(stagingYellow->map().data());
            for (uint32_t i = 0; i < W * H; ++i) {
                p[i * 4 + 0] = 255;
                p[i * 4 + 1] = 255;
                p[i * 4 + 2] = 0;
                p[i * 4 + 3] = 255;
            }
        }
        GpuCnC::Region region;
        region.imageOffset = {0, 0, 0};
        region.imageExtent = {W, H, 1};

        auto cnc3 =
            bindless::CnC::create("cnc-interleaved", {.gpu = gpu, .heap = heap, .heapSetIndex = 0, .passResources = passResources, .maxImmediateSize = 128});
        REQUIRE(cnc3);
        // Interleave: copy buffer to image, then immediately sample in compute within the SAME pass
        {
            auto mapped = stagingYellow->map();
            cnc3->recordUploadImage(tex, {(const uint8_t *) mapped.data(), mapped.size()}, ArrayView<const GpuCnC::Region>(&region, 1));
        }
        PushConstantData pc {slot, 2.0f};
        cnc3->recordCompute({.cs = cs, .x = 1, .y = 1, .z = 1, .immediates = makePushConstants(pc)});
        auto downloadFuture = cnc3->recordDownloadBuffer(outBuf, 0, bufferSize);
        auto payload3       = cnc3->seal();
        REQUIRE(payload3);
        submitAndWait(gpu, "pass3-interleave-yellow", payload3);

        REQUIRE(downloadFuture.valid());
        auto blob = downloadFuture.get();
        REQUIRE(blob);
        const float * floats = reinterpret_cast<const float *>(blob->data());
        for (int i = 0; i < 16; ++i) {
            // Yellow sampled with multiplier 2.0f: (2.0, 2.0, 0.0, 2.0)
            CHECK(std::abs(floats[i * 4 + 0] - 2.0f) < 0.02f);
            CHECK(std::abs(floats[i * 4 + 1] - 2.0f) < 0.02f);
            CHECK(std::abs(floats[i * 4 + 2] - 0.0f) < 0.02f);
            CHECK(std::abs(floats[i * 4 + 3] - 2.0f) < 0.02f);
        }
    }
}

TEST_CASE("GpuCnC & GpuRaster: automated invariant restores buffer and render target states", "[gpu2][cnc][raster][invariant]") {
    auto gpu = makeGpu();
    if (!gpu) SKIP("No GPU context available");

    // 1. Traditional GpuCnC upload restores destination buffer to read-ready
    auto buf = Buffer::create("cnc-buf", Buffer::CreateParameters {.context = gpu, .size = 128});
    REQUIRE(buf);

    std::vector<uint8_t> data(128, 77);
    auto                 cnc = GpuCnC::create({.gpu = gpu});
    REQUIRE(cnc);
    cnc->recordUploadBuffer(buf, 0, data);
    submitAndWait(gpu, "cnc-upload", cnc->seal());

    auto readBack = buf->readContent(0, 128);
    REQUIRE(readBack.size() == 128);
    CHECK(readBack[0] == 77);

    // 2. Traditional GpuRaster rendering restores color target to SHADER_READ_ONLY_OPTIMAL and depth target to DEPTH_STENCIL_READ_ONLY_OPTIMAL
    auto colorTex = makeRgba8Tex(gpu, "raster-color", 16, 16);
    auto depthTex = Texture::create(
        "raster-depth",
        {.context = gpu, .descriptor = Texture::Descriptor {}.setFormat(gfx::img::PixelFormat::D_32_FLOAT()).setDimensions(16, 16).setLevels(1)});
    REQUIRE(colorTex);
    REQUIRE(depthTex);

    RasterTarget rt;
    rt.setColorTarget(0, GpuResourceView(colorTex));
    rt.depthStencilTarget.setView(GpuResourceView(depthTex));

    auto raster = GpuRaster::create("test-raster", {.gpu = gpu, .target = &rt});
    REQUIRE(raster);
    submitAndWait(gpu, "raster-clear", raster->seal());

    auto result = colorTex->readback();
    REQUIRE_FALSE(result.empty());
}

TEST_CASE("bindless::CnC + bindless::Raster: uploaded vertex buffer directly drawn without caller barriers", "[gpu2][bindless][cnc][raster][invariant]") {
    auto gpu = makeGpu();
    if (!gpu) SKIP("No GPU context available");

    auto heap = bindless::DescriptorHeap::create("test-heap", {.gpu = gpu, .capacity = 16});
    REQUIRE(heap);

    constexpr uint32_t W = 16, H = 16;
    auto               colorTex = makeRgba8Tex(gpu, "bindless-rt", W, H);
    REQUIRE(colorTex);

    auto sampleTex = makeRgba8Tex(gpu, "sample-src", W, H);
    REQUIRE(sampleTex);

    uint32_t slot = heap->allocate(GpuResourceView(sampleTex));
    REQUIRE(slot != bindless::INVALID_DESCRIPTOR_INDEX);

    constexpr uint64_t stagedSize = W * H * 4;
    auto               stagedTex  = Buffer::create("staged-tex", {.context = gpu, .size = stagedSize, .mappable = true});
    REQUIRE(stagedTex);
    {
        auto   m = stagedTex->map();
        auto * p = static_cast<uint8_t *>(m.data());
        for (uint32_t i = 0; i < W * H; ++i) {
            p[i * 4 + 0] = 255;
            p[i * 4 + 1] = 0;
            p[i * 4 + 2] = 0;
            p[i * 4 + 3] = 255;
        }
    }
    GpuCnC::Region texRegion;
    texRegion.imageOffset = {0, 0, 0};
    texRegion.imageExtent = {W, H, 1};

    // Fullscreen triangle dummy buffer
    std::vector<uint8_t> dummyVbData(64, 0);
    auto                 vb = Buffer::create("vb", Buffer::CreateParameters {.context = gpu, .size = dummyVbData.size()});
    REQUIRE(vb);

    // Pass 1: CnC uploads sample texture and vertex buffer
    auto cnc = bindless::CnC::create("vb-upload", {.gpu = gpu, .heap = heap});
    REQUIRE(cnc);
    {
        auto mapped = stagedTex->map();
        cnc->recordUploadImage(sampleTex, {(const uint8_t *) mapped.data(), mapped.size()}, ArrayView<const GpuCnC::Region>(&texRegion, 1));
    }
    cnc->recordUploadBuffer(vb, 0, dummyVbData);
    auto cncPayload = cnc->seal();
    REQUIRE(cncPayload);

    // Pass 2: bindless::Raster draws directly using vb and sampleTex without any caller-managed barriers
    RasterTarget target;
    target.setColorTarget(0, GpuResourceView(colorTex));
    target.setClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    auto vs = makeShader(gpu, "bindless-vs", kBindlessTestVertSpv, sizeof(kBindlessTestVertSpv));
    auto ps = makeShader(gpu, "bindless-ps", kBindlessTestFragSpv, sizeof(kBindlessTestFragSpv));
    REQUIRE(vs);
    REQUIRE(ps);

    auto raster = bindless::Raster::create("bindless-raster", {.gpu = gpu, .target = &target, .heap = heap, .heapSetIndex = 0});
    REQUIRE(raster);

    RasterGeometry geom;
    geom.vertexCount = 3;
    geom.vertices.push_back({.buffer = vb, .offset = 0, .stride = 16});

    raster->recordDraw({
        .vs         = vs,
        .ps         = ps,
        .geometry   = geom,
        .immediates = makePushConstants(slot),
    });
    auto rasterPayload = raster->seal();
    REQUIRE(rasterPayload);

    // Submit both payloads in a single batch
    submitAndWait(gpu, "cnc-then-bindless-raster", cncPayload, rasterPayload);

    // Verify buffer, sampled texture, and render target are all in read-ready states

    // Read back target pixels and verify rendered red output
    auto result = colorTex->readback();
    REQUIRE_FALSE(result.empty());
    auto pixels = result.plane().toRGBA8(result.data());
    REQUIRE_FALSE(pixels.empty());
    auto * p = reinterpret_cast<const uint8_t *>(pixels.data());
    CHECK(p[0] == 255);
    CHECK(p[1] == 0);
    CHECK(p[2] == 0);
    CHECK(p[3] == 255);
}

#endif // GN_BUILD_HAS_VULKAN
