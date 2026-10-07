#include <catch2/catch_test_macros.hpp>
#include "gpu2-test-helpers.h"
#include <garnet/GNgpu2.h>
#include <thread>
#include <vector>

using namespace GN;
using namespace GN::gpu2;

TEST_CASE("bindless::DescriptorHeap: creation and capacity", "[gpu2][bindless]") {
    auto gpu = makeGpu();
    REQUIRE(gpu);

    SECTION("Null GPU fails") {
        auto heap = bindless::DescriptorHeap::create("null_heap", {.gpu = nullptr});
        REQUIRE_FALSE(heap);
    }

    SECTION("Default creation") {
        bindless::DescriptorHeap::CreateParameters cp;
        cp.gpu          = gpu;
        cp.capacity     = 1024;
        cp.bindingIndex = 1;

        auto heap = bindless::DescriptorHeap::create("heap_test", cp);
        REQUIRE(heap);
        CHECK(heap->capacity() == 1024);
        CHECK(heap->bindingIndex() == 1);
        CHECK(heap->size() == 0);
        CHECK(heap->gpu().get() == gpu.get());
        const auto material = heap->allocateMaterial(4 * 1024 * 1024);
        REQUIRE(material != bindless::DescriptorHeap::INVALID_MATERIAL_TOKEN);
        CHECK(heap->materialView(material).bufferView.size == 4 * 1024 * 1024);
        CHECK_FALSE(heap->allocateMaterial(1));
        heap->freeMaterial(material);
    }
}

TEST_CASE("bindless::DescriptorHeap: allocation, update, and free recycling", "[gpu2][bindless]") {
    auto gpu = makeGpu();
    REQUIRE(gpu);

    bindless::DescriptorHeap::CreateParameters cp;
    cp.gpu      = gpu;
    cp.capacity = 16;

    auto heap = bindless::DescriptorHeap::create("heap_ops", cp);
    REQUIRE(heap);

    auto tex1 = makeRgba8Tex(gpu, "tex1", 16, 16);
    auto tex2 = makeRgba8Tex(gpu, "tex2", 16, 16);
    auto tex3 = makeRgba8Tex(gpu, "tex3", 16, 16);
    REQUIRE(tex1);
    REQUIRE(tex2);
    REQUIRE(tex3);

    // 1. Sequential allocation
    auto slot0 = heap->allocate(bindless::DescriptorHeap::SAMPLED_TEXTURE, GpuResourceView {tex1});
    CHECK(slot0.slot == 0);
    CHECK(heap->size() == 1);

    auto slot1 = heap->allocate(bindless::DescriptorHeap::SAMPLED_TEXTURE, GpuResourceView {tex2});
    CHECK(slot1.slot == 1);
    CHECK(heap->size() == 2);

    // 2. In-place update
    bool updateOk = heap->update(slot0, GpuResourceView {tex3});
    CHECK(updateOk);
    CHECK(heap->size() == 2);

    bool badUpdate = heap->update(bindless::DescriptorHeap::DescriptorIndex {999}, GpuResourceView {tex3});
    CHECK_FALSE(badUpdate);

    // 3. Freeing and recycling
    heap->free(slot0);
    CHECK(heap->size() == 1);

    // Next allocation should reuse slot0
    auto recycledSlot = heap->allocate(bindless::DescriptorHeap::SAMPLED_TEXTURE, GpuResourceView {tex1});
    CHECK(recycledSlot == slot0);
    CHECK(heap->size() == 2);

    heap->free(slot1);
    heap->free(recycledSlot);
    CHECK(heap->size() == 0);
}

TEST_CASE("bindless::DescriptorHeap: concurrent multi-threaded allocation", "[gpu2][bindless]") {
    auto gpu = makeGpu();
    REQUIRE(gpu);

    bindless::DescriptorHeap::CreateParameters cp;
    cp.gpu      = gpu;
    cp.capacity = 256;

    auto heap = bindless::DescriptorHeap::create("heap_concurrent", cp);
    REQUIRE(heap);

    auto tex = makeRgba8Tex(gpu, "thread_tex", 8, 8);
    REQUIRE(tex);

    constexpr int kNumThreads     = 4;
    constexpr int kItersPerThread = 20;

    std::vector<std::thread> threads;
    for (int t = 0; t < kNumThreads; ++t) {
        threads.emplace_back([&heap, &tex]() {
            for (int i = 0; i < kItersPerThread; ++i) {
                auto slot = heap->allocate(bindless::DescriptorHeap::SAMPLED_TEXTURE, GpuResourceView {tex});
                if (slot != bindless::DescriptorHeap::INVALID_DESCRIPTOR_INDEX) {
                    heap->update(slot, GpuResourceView {tex});
                    heap->free(slot);
                }
            }
        });
    }

    for (auto & th : threads) { th.join(); }

    CHECK(heap->size() == 0);
}
