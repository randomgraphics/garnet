#include <catch2/catch_test_macros.hpp>

#include "gpu2-test-helpers.h"
#include "bindless-material-comp.spv.h"

#include <array>

using namespace GN;
using namespace GN::gpu2;
using Heap = bindless::DescriptorHeap;

TEST_CASE("heap material chunks are aligned, opaque and immediately reusable", "[gpu2][bindless][material]") {
    auto gpu = makeGpu();
    REQUIRE(gpu);
    auto heap = Heap::create("material-allocation", {.gpu = gpu, .capacity = 4, .materialCapacity = 128});
    REQUIRE(heap);
    const auto a = heap->allocateMaterial(12, 4);
    const auto b = heap->allocateMaterial(16, 32);
    const auto c = heap->allocateMaterial(64, 64);
    REQUIRE(a);
    REQUIRE(b);
    REQUIRE(c);
    CHECK(heap->materialView(a).bufferView.offset == 0);
    CHECK(heap->materialView(b).bufferView.offset == 32);
    CHECK(heap->materialView(c).bufferView.offset == 64);
    CHECK(heap->materialView(a).bufferView.size == 12);
    CHECK(heap->materialView(b).bufferView.type == GpuResourceView::BufferView::STORAGE);
    CHECK(heap->size() == 0); // Material chunks do not consume typed descriptor-array slots.
    CHECK_FALSE(heap->allocateMaterial(129));
    CHECK_FALSE(heap->allocateMaterial(0));
    CHECK_FALSE(heap->allocateMaterial(4, 0));
    CHECK_FALSE(heap->allocateMaterial(4, 3));
    CHECK_FALSE(heap->allocateMaterial(UINT64_MAX));
    CHECK_FALSE(heap->allocateMaterial(1, uint64_t(1) << 63));
    CHECK(heap->materialView(Heap::INVALID_MATERIAL_TOKEN).empty());
    auto foreignHeap = Heap::create("foreign-material", {.gpu = gpu, .capacity = 1, .materialCapacity = 128});
    REQUIRE(foreignHeap);
    auto foreign = foreignHeap->allocateMaterial(12);
    REQUIRE(foreign);
    CHECK(heap->materialView(foreign).empty());
    heap->freeMaterial(foreign);
    CHECK_FALSE(heap->materialView(a).empty());
    heap->freeMaterial(b);
    const auto reused = heap->allocateMaterial(16, 32);
    REQUIRE(reused);
    CHECK(reused != b);
    CHECK(heap->materialView(b).empty());
    CHECK(heap->materialView(reused).bufferView.offset == 32);
    heap->freeMaterial(b); // A stale free must not reclaim the new range.
    CHECK_FALSE(heap->materialView(reused).empty());
    heap->freeMaterial(a);
    heap->freeMaterial(c);
    heap->freeMaterial(reused);
    const auto full = heap->allocateMaterial(128);
    REQUIRE(full); // Both neighbours and alignment gaps have coalesced.
    CHECK(heap->materialView(full).bufferView.offset == 0);
    CHECK_FALSE(heap->allocateMaterial(1));
    auto retainedView = heap->materialView(full);
    heap.clear();
    CHECK(retainedView.buffer());
    CHECK(retainedView.bufferView.size == 128);
    CHECK_FALSE(Heap::create("zero-material", {.gpu = gpu, .capacity = 1, .materialCapacity = 0}));
    CHECK_FALSE(Heap::create("oversized-material", {.gpu = gpu, .capacity = 1, .materialCapacity = UINT64_MAX}));
}

TEST_CASE("heap material binding is readable after caller-recorded uploads", "[gpu2][bindless][material]") {
    auto gpu = makeGpu();
    REQUIRE(gpu);
    auto heap = Heap::create("material-gpu", {.gpu = gpu, .capacity = 8, .materialCapacity = 256});
    REQUIRE(heap);
    auto uploads = GpuCnC::create({.gpu = gpu});
    REQUIRE(uploads);
    auto output = Buffer::create("material-output", {.context = gpu, .size = 48});
    REQUIRE(output);
    std::array<Heap::MaterialToken, 3>     chunks;
    std::array<std::array<uint32_t, 4>, 3> data {{{1, 17, 29, 43}, {71, 83, 97, 101}, {113, 127, 131, 149}}};
    for (size_t i = 0; i < chunks.size(); ++i) {
        chunks[i] = heap->allocateMaterial(sizeof(data[i]), uint64_t(16) << i);
        REQUIRE(chunks[i]);
        auto view = heap->materialView(chunks[i]);
        uploads->recordUploadBuffer(view.buffer(), view.bufferView.offset, {reinterpret_cast<const uint8_t *>(data[i].data()), sizeof(data[i])});
    }
    auto shader = makeShader(gpu, "material-reader", kBindlessMaterialCompSpv, sizeof(kBindlessMaterialCompSpv));
    REQUIRE(shader);
    GpuResourceTable resources;
    resources.resize(2);
    resources[1].resize(1);
    resources[1][0].append(GpuResourceView(output).setBufferViewType(GpuResourceView::BufferView::STORAGE));
    auto compute = bindless::CnC::create("material-consumers", {.gpu = gpu, .heap = heap, .passResources = resources});
    REQUIRE(compute);
    for (uint32_t i = 0; i < chunks.size(); ++i) {
        std::array<uint32_t, 2> args {uint32_t(heap->materialView(chunks[i]).bufferView.offset / sizeof(uint32_t)), i * 4};
        compute->recordCompute({.cs = shader, .immediates = {reinterpret_cast<const uint8_t *>(args.data()), sizeof(args)}});
    }
    auto producer = uploads->seal();
    auto consumer = compute->seal();
    heap.clear();
    submitAndWait(gpu, "material-upload-and-consume", producer, consumer);
    auto bytes = output->readContent();
    REQUIRE(bytes.size() == sizeof(data));
    CHECK(std::memcmp(bytes.data(), data.data(), bytes.size()) == 0);
}
