#include <catch2/catch_test_macros.hpp>
#include "gpu2-test-helpers.h"
#include "bindless-types-comp.spv.h"
#include <array>
#include <cmath>

using namespace GN;
using namespace GN::gpu2;
using namespace GN::gpu2::test;
using Heap = bindless::DescriptorHeap;

TEST_CASE("bindless heap enforces descriptor type and view agreement", "[gpu2][bindless][typed]") {
    auto gpu = makeGpu();
    REQUIRE(gpu);
    CHECK(Heap::DescriptorIndex {}.u32 == 0);
    CHECK(Heap::INVALID_DESCRIPTOR_INDEX.u32 == 0);
    auto heap = Heap::create("typed-validation", {.gpu = gpu, .capacity = 5});
    REQUIRE(heap);
    auto texture = makeRgba8Tex(gpu, "texture", 2, 1);
    auto buffer  = Buffer::create("buffer", {.context = gpu, .size = 128});
    auto sampler = Sampler::create("sampler", {.context = gpu});
    REQUIRE(texture);
    REQUIRE(buffer);
    REQUIRE(sampler);
    std::array<GpuResourceView, 5> views = {GpuResourceView {texture}, GpuResourceView {texture}.setImageViewType(GpuResourceView::ImageView::STORAGE),
                                            GpuResourceView {buffer}, GpuResourceView {buffer}.setBufferViewType(GpuResourceView::BufferView::STORAGE),
                                            GpuResourceView {sampler}};
    for (uint32_t type = 0; type < views.size(); ++type) {
        for (uint32_t other = 0; other < views.size(); ++other) {
            if (type == other) continue;
            CHECK(heap->allocate(Heap::DescriptorType(type), views[other]) == Heap::INVALID_DESCRIPTOR_INDEX);
            CHECK(heap->size() == type);
        }
        auto index = heap->allocate(Heap::DescriptorType(type), views[type]);
        REQUIRE(index != Heap::INVALID_DESCRIPTOR_INDEX);
        CHECK(index.type == type);
        CHECK(index.slot == type);
        CHECK(index.tag == 1);
        CHECK(index.u32 == (0x80000000u | (type << 28) | type));
        auto untagged = index;
        untagged.tag  = 0;
        CHECK_FALSE(heap->update(untagged, views[type]));
        heap->free(untagged);
        CHECK(heap->size() == type + 1);
        for (uint32_t other = 0; other < views.size(); ++other) CHECK(heap->update(index, views[other]) == (type == other));
        auto wrongType = index;
        wrongType.type = (type + 1) % 5;
        CHECK_FALSE(heap->update(wrongType, views[wrongType.type]));
        heap->free(wrongType);
        CHECK(heap->size() == type + 1);
    }
    CHECK(heap->allocate(Heap::SAMPLER, views[4]) == Heap::INVALID_DESCRIPTOR_INDEX);
    std::array<Heap::DescriptorIndex, 5> indices;
    for (uint32_t type = 0; type < indices.size(); ++type) indices[type] = Heap::DescriptorIndex {0x80000000u | (type << 28) | type};
    CHECK(heap->update({indices.data(), indices.size()}, {views.data(), views.size()}) == 5);
    auto mixed = views;
    mixed[4]   = views[0];
    CHECK(heap->update({indices.data(), indices.size()}, {mixed.data(), mixed.size()}) == 4);
    CHECK(heap->update({indices.data(), 1}, {views.data(), views.size()}) == 0);
    auto invalid = Heap::INVALID_DESCRIPTOR_INDEX;
    CHECK_FALSE(heap->update(invalid, views[0]));
    heap->free(invalid);
    CHECK(heap->size() == 5);
    heap->free({indices.data(), indices.size()});
    CHECK(heap->size() == 0);
    heap->free({indices.data(), indices.size()});
    CHECK(heap->size() == 0);
    auto recycled = heap->allocate(Heap::SAMPLER, views[4]);
    CHECK(recycled.slot == 4);
    CHECK(heap->size() == 1);
    heap->free(recycled);
    CHECK(heap->allocate(Heap::DescriptorType(15), views[0]) == Heap::INVALID_DESCRIPTOR_INDEX);
    CHECK(heap->allocate(Heap::SAMPLED_TEXTURE, GpuResourceView {texture}.setCombinedTextureSampler(sampler)) == Heap::INVALID_DESCRIPTOR_INDEX);
    CHECK(heap->allocate(Heap::UNIFORM_BUFFER, GpuResourceView {buffer}.setBufferViewOffset(1)) == Heap::INVALID_DESCRIPTOR_INDEX);
    CHECK(heap->allocate(Heap::STORAGE_BUFFER, GpuResourceView {buffer}.setBufferViewType(GpuResourceView::BufferView::STORAGE).setBufferViewSize(129)) ==
          Heap::INVALID_DESCRIPTOR_INDEX);
    auto otherGpu = makeGpu();
    REQUIRE(otherGpu);
    auto foreignTexture = makeRgba8Tex(otherGpu, "foreign", 2, 1);
    auto foreignBuffer  = Buffer::create("foreign", {.context = otherGpu, .size = 128});
    auto foreignSampler = Sampler::create("foreign", {.context = otherGpu});
    REQUIRE(foreignTexture);
    REQUIRE(foreignBuffer);
    REQUIRE(foreignSampler);
    CHECK(heap->allocate(Heap::SAMPLED_TEXTURE, GpuResourceView {foreignTexture}) == Heap::INVALID_DESCRIPTOR_INDEX);
    CHECK(heap->allocate(Heap::UNIFORM_BUFFER, GpuResourceView {foreignBuffer}) == Heap::INVALID_DESCRIPTOR_INDEX);
    CHECK(heap->allocate(Heap::SAMPLER, GpuResourceView {foreignSampler}) == Heap::INVALID_DESCRIPTOR_INDEX);
    CHECK_FALSE(Heap::create("zero", {.gpu = gpu, .capacity = 0}));
    CHECK_FALSE(Heap::create("overflow", {.gpu = gpu, .capacity = 1, .bindingIndex = UINT32_MAX}));
}

// The folowing unit test is disabled since it replies on nonUniformEXT fetch that is not property supported by mesa driver yet.
// it runs fine on real GPU with proper divergence support.
#if 0

TEST_CASE("bindless heap uses all five resource types and four samplers for 1024 textures", "[gpu2][bindless][typed][gpu]") {
    auto gpu = makeGpu();
    REQUIRE(gpu);
    constexpr uint32_t COUNT = 1024;
    auto               heap  = Heap::create("all-types", {.gpu = gpu, .capacity = COUNT + 8, .bindingIndex = 3});
    REQUIRE(heap);
    using Filter  = Sampler::Descriptor::Filter;
    using Address = Sampler::Descriptor::Address;
    for (uint32_t i = 0; i < 4; ++i) {
        Sampler::CreateParameters cp;
        cp.context              = gpu;
        cp.descriptor.minFilter = cp.descriptor.magFilter = i < 2 ? Filter::NEAREST : Filter::LINEAR;
        cp.descriptor.addressU                            = i % 2 ? Address::CLAMP_TO_EDGE : Address::REPEAT;
        auto sampler                                      = Sampler::create("shared", cp);
        REQUIRE(sampler);
        REQUIRE(heap->allocate(Heap::SAMPLER, GpuResourceView {sampler}).slot == i);
    }
    auto uniform = Buffer::create("uniform", {.context = gpu, .size = 16});
    auto input   = Buffer::create("input", {.context = gpu, .size = 16});
    auto output  = Buffer::create("output", {.context = gpu, .size = COUNT * 6 * 16});
    auto image   = makeRgba8Tex(gpu, "storage-image", COUNT, 1);
    REQUIRE(uniform);
    REQUIRE(input);
    REQUIRE(output);
    REQUIRE(image);
    const auto uniformIndex = heap->allocate(Heap::UNIFORM_BUFFER, GpuResourceView {uniform});
    const auto inputView    = GpuResourceView {input}.setBufferViewType(GpuResourceView::BufferView::STORAGE);
    const auto outputView   = GpuResourceView {output}.setBufferViewType(GpuResourceView::BufferView::STORAGE);
    const auto imageView    = GpuResourceView {image}.setImageViewType(GpuResourceView::ImageView::STORAGE);
    const auto inputIndex   = heap->allocate(Heap::STORAGE_BUFFER, inputView);
    const auto outputIndex  = heap->allocate(Heap::STORAGE_BUFFER, outputView);
    const auto imageIndex   = heap->allocate(Heap::STORAGE_TEXTURE, imageView);
    REQUIRE(uniformIndex != Heap::INVALID_DESCRIPTOR_INDEX);
    REQUIRE(inputIndex != Heap::INVALID_DESCRIPTOR_INDEX);
    REQUIRE(outputIndex != Heap::INVALID_DESCRIPTOR_INDEX);
    REQUIRE(imageIndex != Heap::INVALID_DESCRIPTOR_INDEX);
    auto uploads = GpuCnC::create({.gpu = gpu});
    REQUIRE(uploads);
    const float a[4] = {1, 2, 3, 4}, b[4] = {2, 3, 4, 5};
    uploads->recordUploadBuffer(uniform, 0, {reinterpret_cast<const uint8_t *>(a), sizeof(a)});
    uploads->recordUploadBuffer(input, 0, {reinterpret_cast<const uint8_t *>(b), sizeof(b)});
    for (uint32_t i = 0; i < COUNT; ++i) {
        auto texture = makeRgba8Tex(gpu, "sampled", 2, 1);
        REQUIRE(texture);
        const uint8_t  pixels[8] = {uint8_t(i), 64, 32, 255, uint8_t(255 - uint8_t(i)), 192, 96, 255};
        GpuCnC::Region region;
        region.imageExtent = {2, 1, 1};
        uploads->recordUploadImage(texture, {pixels, sizeof(pixels)}, {&region, 1});
        REQUIRE(heap->allocate(Heap::SAMPLED_TEXTURE, GpuResourceView {texture}).slot == i + 8);
    }
    CHECK(heap->size() == COUNT + 8);
    submitAndWait(gpu, "initialize-typed-heap", uploads->seal());
    // Pass resources declare writable hazards; the shader accesses the same resources through heap indices.
    GpuResourceTable resources;
    resources.resize(2);
    resources[1].resize(2);
    resources[1][0].append(outputView);
    resources[1][1].append(imageView);
    auto compute = bindless::CnC::create("all-types", {.gpu = gpu, .heap = heap, .passResources = resources});
    REQUIRE(compute);
    auto shader = makeShader(gpu, "all-types", kBindlessTypesCompSpv, sizeof(kBindlessTypesCompSpv));
    REQUIRE(shader);
    const uint32_t p[] = {8, COUNT, uniformIndex.slot, inputIndex.slot, outputIndex.slot, imageIndex.slot};
    compute->recordCompute({.cs = shader, .x = COUNT / 64, .immediates = {reinterpret_cast<const uint8_t *>(p), sizeof(p)}});
    submitAndWait(gpu, "typed-dispatch", compute->seal());
    auto downloads = GpuCnC::create({.gpu = gpu});
    REQUIRE(downloads);
    auto download = downloads->recordDownloadBuffer(output);
    submitAndWait(gpu, "typed-readback", downloads->seal());
    auto data = download.get();
    REQUIRE(data);
    REQUIRE(data->size() == COUNT * 6 * 16);
    const auto * values = reinterpret_cast<const float *>(data->data());
    for (uint32_t i = 0; i < COUNT; ++i) {
        const float left[4]  = {float(uint8_t(i)) / 255, 64.0f / 255, 32.0f / 255, 1};
        const float right[4] = {float(255 - uint8_t(i)) / 255, 192.0f / 255, 96.0f / 255, 1};
        for (uint32_t c = 0; c < 4; ++c) {
            const float expected[6] = {left[c], left[c], right[c], (left[c] + right[c]) * 0.5f, right[c], float(3 + 2 * c)};
            for (uint32_t result = 0; result < 6; ++result) CHECK(std::abs(values[(i * 6 + result) * 4 + c] - expected[result]) < 0.005f);
        }
    }
    auto pixels = image->readback();
    REQUIRE_FALSE(pixels.empty());
    const auto * bytes = static_cast<const uint8_t *>(pixels.at({0, 0, 0}));
    for (uint32_t i = 0; i < COUNT; ++i) {
        CHECK(bytes[i * 4] == uint8_t(i));
        CHECK(bytes[i * 4 + 1] == 64);
        CHECK(bytes[i * 4 + 2] == 32);
        CHECK(bytes[i * 4 + 3] == 255);
    }
}

#endif
