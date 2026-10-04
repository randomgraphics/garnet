#if GN_BUILD_HAS_VULKAN

    #include "../vk-gpu-context.h"
    #include "../vk-cnc-common.h"
    #include <catch2/catch_test_macros.hpp>
    #include <garnet/GNgpu2.h>
    #include "gpu2-test-helpers.h"

using namespace GN;
using namespace GN::gpu2;

TEST_CASE("CNC upload storage reuses mapped buffers and preserves slices across growth", "[gpu2][cnc][upload-storage]") {
    auto gpu = makeGpu();
    if (!gpu) SKIP("No GPU context available");
    CncUploadStorage storage;
    const uint8_t a[] = {1, 2, 3, 4}, b[] = {5, 6, 7, 8};
    auto first = storage.copy(gpu, "arena", {a, sizeof(a)});
    auto second = storage.copy(gpu, "arena", {b, sizeof(b)});
    REQUIRE(first.buffer);
    REQUIRE(second.buffer);
    CHECK(first.buffer.get() == second.buffer.get());
    CHECK(second.offset >= first.offset + sizeof(a));
    CHECK(second.offset % 256 == 0);
    std::vector<uint8_t> large(1024 * 1024 + 16, 73);
    auto grown = storage.copy(gpu, "arena", large);
    REQUIRE(grown.buffer);
    CHECK(grown.buffer.get() != first.buffer.get());
    storage.unmapForSubmit();
    // Mapping succeeds only if the arena released its persistent mapping. Slices still own their bytes.
    auto mapped = first.buffer->map();
    REQUIRE(mapped.data());
    CHECK(std::memcmp((const uint8_t *) mapped.data() + first.offset, a, sizeof(a)) == 0);
    CHECK(std::memcmp((const uint8_t *) mapped.data() + second.offset, b, sizeof(b)) == 0);
}

TEST_CASE("CNC image uploads snapshot CPU data and image copies preserve regions", "[gpu2][cnc][image][gpu]") {
    auto gpu = makeGpu();
    if (!gpu) SKIP("No GPU context available");
    auto source = makeRgba8Tex(gpu, "image.source", 8, 8);
    auto destination = makeRgba8Tex(gpu, "image.destination", 8, 8);
    REQUIRE(source);
    REQUIRE(destination);
    auto buffer = Buffer::create("upload.buffer", {.context = gpu, .size = 32 * 16});
    REQUIRE(buffer);

    auto exercise = [&](auto recorder) {
        REQUIRE(recorder);
        uint8_t bytes[16];
        for (uint32_t i = 0; i < 32; ++i) {
            std::memset(bytes, (int) i, sizeof(bytes));
            recorder->recordUploadBuffer(buffer, i * sizeof(bytes), {bytes, sizeof(bytes)});
        }
        // Nonzero CPU offset and padded rows verify both user layout and arena suballocation offsets.
        std::vector<uint8_t> pixels(16 + 10 * 8 * 4, 0);
        for (uint32_t y = 0; y < 8; ++y) for (uint32_t x = 0; x < 8; ++x) {
            const auto offset = 16 + (y * 10 + x) * 4;
            pixels[offset] = (uint8_t) x;
            pixels[offset + 1] = (uint8_t) y;
            pixels[offset + 3] = 255;
        }
        GpuCnC::Region upload;
        upload.dataOffset = 16;
        upload.rowPitchBytes = 40;
        upload.imageExtent = {8, 8, 1};
        recorder->recordUploadImage(source, pixels, {&upload, 1});
        std::fill(pixels.begin(), pixels.end(), uint8_t(99));
        std::vector<uint8_t> zeros(8 * 8 * 4, 0);
        GpuCnC::Region whole;
        whole.imageExtent = {8, 8, 1};
        recorder->recordUploadImage(destination, zeros, {&whole, 1});
        GpuCnC::ImageCopyRegion copy;
        copy.srcOffset = {2, 1, 0};
        copy.dstOffset = {1, 2, 0};
        copy.extent = {4, 3, 1};
        recorder->recordCopyImage({.src = source, .dst = destination, .regions = {&copy, 1}});
        auto imageFuture = recorder->recordDownloadImage(destination, {&whole, 1});
        auto bufferFuture = recorder->recordDownloadBuffer(buffer);
        auto payload = recorder->seal();
        REQUIRE(payload);
        recorder.clear();
        submitAndWait(gpu, "image.transfers", payload);
        auto image = imageFuture.get();
        REQUIRE(image.blob);
        REQUIRE(image.blob->size() == 8 * 8 * 4);
        const auto * result = (const uint8_t *) image.blob->data();
        for (uint32_t y = 0; y < 8; ++y) for (uint32_t x = 0; x < 8; ++x) {
            const bool copied = x >= 1 && x < 5 && y >= 2 && y < 5;
            CHECK(result[(y * 8 + x) * 4] == (copied ? x + 1 : 0));
            CHECK(result[(y * 8 + x) * 4 + 1] == (copied ? y - 1 : 0));
            CHECK(result[(y * 8 + x) * 4 + 3] == (copied ? 255 : 0));
        }
        auto bufferContent = bufferFuture.get();
        REQUIRE(bufferContent);
        REQUIRE(bufferContent->size() == 32 * 16);
        const auto * values = (const uint8_t *) bufferContent->data();
        for (uint32_t i = 0; i < 32 * 16; ++i) CHECK(values[i] == i / 16);
        // Readback uses another tracked transfer, then subsequent uploads and consumers must still work.
        auto readback = destination->readback();
        REQUIRE_FALSE(readback.empty());
        CHECK(((const uint8_t *) readback.data())[(2 * 8 + 1) * 4] == 2);
    };
    SECTION("ordinary CNC") { exercise(GpuCnC::create({.gpu = gpu})); }
    SECTION("bindless CNC") {
        auto heap = bindless::DescriptorHeap::create("image.heap", {.gpu = gpu, .capacity = 16});
        REQUIRE(heap);
        exercise(bindless::CnC::create("image.cnc", {.gpu = gpu, .heap = heap, .opCountHint = 40}));
    }
}

TEST_CASE("CNC CPU-image upload and synchronous transfers preserve all faces and mips", "[gpu2][cnc][image][invariant]") {
    auto gpu = makeGpu();
    if (!gpu) SKIP("No GPU context available");
    gfx::img::Extent3D extent;
    extent.set(8, 8, 1);
    const auto plane = gfx::img::PlaneDesc::make(gfx::img::PixelFormat::RGBA8(), extent);
    gfx::img::Image image(gfx::img::ImageDesc::make(plane, 1, 2, 2));
    for (uint32_t face = 0; face < 2; ++face) for (uint32_t mip = 0; mip < 2; ++mip) {
        const gfx::img::PlaneCoord coord {0, face, mip};
        const auto & plane = image.plane(coord);
        std::memset(image.at(coord), (int) (10 + face * 25 + mip * 3), plane.size);
    }
    auto desc = Texture::Descriptor {}.setFormat(image.format()).setDimensions(8, 8).setFaces(2).setLevels(2);
    auto texture = Texture::create("image.multi", {.context = gpu, .descriptor = desc});
    REQUIRE(texture);
    auto recorder = GpuCnC::create({.gpu = gpu});
    REQUIRE(recorder);
    recorder->recordUploadImage(texture, image);
    submitAndWait(gpu, "upload.cpu-image", recorder->seal());
    auto first = texture->readback();
    REQUIRE_FALSE(first.empty());
    CHECK(first.desc().faces == 2);
    CHECK(first.desc().levels == 2);
    CHECK(first.size() == image.size());
    CHECK(std::memcmp(first.data(), image.data(), image.size()) == 0);
    std::memset(image.data(), 37, image.size());
    REQUIRE(texture->setContent(image));
    auto second = texture->readback();
    REQUIRE(second.size() == image.size());
    CHECK(std::memcmp(second.data(), image.data(), image.size()) == 0);
    auto destination = Texture::create("image.copy-multi", {.context = gpu, .descriptor = desc});
    REQUIRE(destination);
    REQUIRE(texture->setContent(first));
    auto copier = GpuCnC::create({.gpu = gpu});
    REQUIRE(copier);
    DynaArray<GpuCnC::ImageCopyRegion> copies;
    for (uint32_t face = 0; face < 2; ++face) for (uint32_t mip = 0; mip < 2; ++mip) {
        GpuCnC::ImageCopyRegion region;
        region.srcFace = region.dstFace = face;
        region.srcMip = region.dstMip = mip;
        region.extent = {8u >> mip, 8u >> mip, 1};
        copies.append(region);
    }
    copier->recordCopyImage({.src = texture, .dst = destination, .regions = copies});
    submitAndWait(gpu, "copy.faces-and-mips", copier->seal());
    auto copied = destination->readback();
    REQUIRE(copied.size() == image.size());
    CHECK(std::memcmp(copied.data(), first.data(), image.size()) == 0);
}

TEST_CASE("CNC repacks padded RGB8 block rows and depth slices directly into upload storage", "[gpu2][cnc][upload-storage]") {
    auto gpu = makeGpu();
    if (!gpu) SKIP("No GPU context available");
    auto desc = Texture::Descriptor {}.setFormat(gfx::img::PixelFormat::RGB_8_8_8_UNORM()).setDimensions(1, 10, 2).setLevels(1);
    std::vector<uint8_t> pixels(1 + 43 + 9 * 4 + 3, 200);
    for (uint32_t z = 0; z < 2; ++z) for (uint32_t y = 0; y < 10; ++y) {
        auto * p = pixels.data() + 1 + z * 43 + y * 4;
        p[0] = (uint8_t) y;
        p[1] = (uint8_t) z;
        p[2] = 77;
    }
    GpuCnC::Region region;
    region.imageExtent = {1, 10, 2};
    region.dataOffset = 1;
    region.rowPitchBytes = 4;
    region.slicePitchBytes = 43;
    CncUploadStorage storage;
    auto slice = storage.copyImage(gpu, "rgb8", pixels, desc, region);
    REQUIRE(slice.buffer);
    storage.unmapForSubmit();
    auto mapped = slice.buffer->map();
    REQUIRE(mapped.data());
    const auto * result = (const uint8_t *) mapped.data() + slice.offset;
    for (uint32_t z = 0; z < 2; ++z) for (uint32_t y = 0; y < 10; ++y) {
        CHECK(result[(z * 10 + y) * 3] == y);
        CHECK(result[(z * 10 + y) * 3 + 1] == z);
        CHECK(result[(z * 10 + y) * 3 + 2] == 77);
    }
}

TEST_CASE("CNC compressed uploads accept arbitrary padding and image copies reach partial edge blocks", "[gpu2][cnc][image][compressed]") {
    auto gpu = makeGpu();
    if (!gpu) SKIP("No GPU context available");
    auto * vkGpu = RuntimeType::cast<GpuContextVulkan2>(gpu.get());
    REQUIRE(vkGpu);
    const auto format = gfx::img::PixelFormat::BC1_UNORM();
    const auto properties = vkGpu->vulkanDevice().gi()->physical.getFormatProperties(pixelFormatToVkFormat(format));
    if (!(properties.optimalTilingFeatures & vk::FormatFeatureFlagBits::eSampledImage)) SKIP("BC1 textures unsupported");
    auto desc = Texture::Descriptor {}.setFormat(format).setDimensions(10, 9).setLevels(1);
    auto source = Texture::create("compressed.source", {.context = gpu, .descriptor = desc});
    auto destination = Texture::create("compressed.destination", {.context = gpu, .descriptor = desc});
    REQUIRE(source);
    REQUIRE(destination);
    auto exercise = [&](auto recorder) {
        REQUIRE(recorder);
        // Three 8-byte blocks per row, with three padding bytes: 27 is not divisible by the block size.
        std::vector<uint8_t> pixels(1 + 27 * 2 + 24, 200);
        for (uint32_t y = 0; y < 3; ++y) for (uint32_t x = 0; x < 24; ++x) pixels[1 + y * 27 + x] = (uint8_t) (y * 24 + x);
        GpuCnC::Region region;
        region.imageExtent = {10, 9, 1};
        region.dataOffset = 1;
        region.rowPitchBytes = 27;
        recorder->recordUploadImage(source, pixels, {&region, 1});
        std::vector<uint8_t> zero(72, 0);
        region.dataOffset = 0;
        region.rowPitchBytes = 0;
        recorder->recordUploadImage(destination, zero, {&region, 1});
        GpuCnC::ImageCopyRegion copy;
        copy.srcOffset = {4, 4, 0};
        copy.dstOffset = {4, 4, 0};
        copy.extent = {6, 5, 1};
        recorder->recordCopyImage({.src = source, .dst = destination, .regions = {&copy, 1}});
        auto future = recorder->recordDownloadImage(destination, {&region, 1});
        submitAndWait(gpu, "compressed.copy", recorder->seal());
        auto content = future.get();
        REQUIRE(content.blob);
        REQUIRE(content.blob->size() == 72);
        REQUIRE(content.regions.size() == 1);
        CHECK(content.regions[0].rowPitchBytes == 24);
        const auto * result = (const uint8_t *) content.blob->data();
        for (uint32_t y = 0; y < 3; ++y) for (uint32_t x = 0; x < 24; ++x)
            CHECK(result[y * 24 + x] == ((y >= 1 && x >= 8) ? y * 24 + x : 0));
        auto image = destination->readback();
        REQUIRE_FALSE(image.empty());
        CHECK(image.format() == format);
    };
    SECTION("ordinary CNC") { exercise(GpuCnC::create({.gpu = gpu})); }
    SECTION("bindless CNC") {
        auto heap = bindless::DescriptorHeap::create("compressed.heap", {.gpu = gpu, .capacity = 16});
        REQUIRE(heap);
        exercise(bindless::CnC::create("compressed.cnc", {.gpu = gpu, .heap = heap}));
    }
}

TEST_CASE("Synchronous depth uploads and readbacks retain the shader-readable invariant", "[gpu2][cnc][image][invariant]") {
    auto gpu = makeGpu();
    if (!gpu) SKIP("No GPU context available");
    auto * vkGpu = RuntimeType::cast<GpuContextVulkan2>(gpu.get());
    REQUIRE(vkGpu);
    const auto format = gfx::img::PixelFormat::D_32_FLOAT();
    const auto properties = vkGpu->vulkanDevice().gi()->physical.getFormatProperties(pixelFormatToVkFormat(format));
    if (!(properties.optimalTilingFeatures & vk::FormatFeatureFlagBits::eSampledImage)) SKIP("D32 textures unsupported");
    gfx::img::Extent3D extent;
    extent.set(4, 4, 1);
    const auto plane = gfx::img::PlaneDesc::make(format, extent);
    gfx::img::Image image(gfx::img::ImageDesc::make(plane));
    auto * pixels = (float *) image.data();
    for (uint32_t i = 0; i < 16; ++i) pixels[i] = (float) i / 16;
    const auto desc = Texture::Descriptor {}.setFormat(format).setDimensions(4, 4).setLevels(1);
    auto source = Texture::create("depth.source", {.context = gpu, .descriptor = desc});
    auto destination = Texture::create("depth.copy", {.context = gpu, .descriptor = desc});
    REQUIRE(source);
    REQUIRE(destination);
    REQUIRE(source->setContent(image));
    auto read = source->readback();
    REQUIRE(read.size() == image.size());
    CHECK(std::memcmp(read.data(), image.data(), image.size()) == 0);
    auto recorder = GpuCnC::create({.gpu = gpu});
    REQUIRE(recorder);
    GpuCnC::ImageCopyRegion region;
    region.extent = {4, 4, 1};
    recorder->recordCopyImage({.src = source, .dst = destination, .regions = {&region, 1}});
    submitAndWait(gpu, "copy.depth", recorder->seal());
    auto copied = destination->readback();
    REQUIRE(copied.size() == image.size());
    CHECK(std::memcmp(copied.data(), image.data(), image.size()) == 0);
}

#endif
