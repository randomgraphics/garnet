/**
 * Tests for TransientArena allocation and recycling behavior.
 * Uses internal vk-transient-buffer.h to verify backing buffer states.
 */

#include <catch2/catch_test_macros.hpp>
#include <garnet/GNrdg.h>
#include "rdg/vk-transient-buffer.h"
#include <cstring>

using namespace GN;
using namespace GN::rdg;

static TransientArenaVulkan & requireVulkanArena(TransientArena & arena) {
    REQUIRE(arena.isKindOf<TransientArena>());
    auto * vk = dynamic_cast<TransientArenaVulkan *>(&arena);
    REQUIRE(vk != nullptr);
    return *vk;
}

// ---------------------------------------------------------------------------
// Allocation: default capacity, single backing
// ---------------------------------------------------------------------------

TEST_CASE("TransientArena: first allocation creates 16MB backing", "[rdg][transient-arena][gpu]") {
    auto db = std::unique_ptr<ArtifactDatabase>(ArtifactDatabase::create({}));
    REQUIRE(db);
    auto gpu = GpuContext::create(*db, "gpu_ta1", {});
    if (!gpu) SKIP("No Vulkan GPU context available");

    auto arena = TransientArena::create(*db, "arena_ta1", TransientArena::CreateParameters {.context = gpu});
    REQUIRE(arena);
    auto & vk = requireVulkanArena(*arena);

    CHECK(vk.numBackingBuffers() == 0);

    auto tb = arena->allocate(256, "tb1");
    REQUIRE(tb);
    CHECK(static_cast<const TransientBufferVulkan &>(*tb).size() >= 256);

    REQUIRE(vk.numBackingBuffers() >= 1);
    CHECK(vk.backingCapacity(0) == 16u * 1024u * 1024u); // 16MB
    CHECK(vk.backingOffset(0) >= 256);
    CHECK(vk.backingLiveCount(0) == 1);
    CHECK(vk.activeBackingIndex() == 0);
}

// ---------------------------------------------------------------------------
// Allocation: multiple buffers from same backing
// ---------------------------------------------------------------------------

TEST_CASE("TransientArena: multiple allocs bump offset within same backing", "[rdg][transient-arena][gpu]") {
    auto db = std::unique_ptr<ArtifactDatabase>(ArtifactDatabase::create({}));
    REQUIRE(db);
    auto gpu = GpuContext::create(*db, "gpu_ta2", {});
    if (!gpu) SKIP("No Vulkan GPU context available");

    auto arena = TransientArena::create(*db, "arena_ta2", TransientArena::CreateParameters {.context = gpu});
    REQUIRE(arena);
    auto & vk = requireVulkanArena(*arena);

    auto t1 = arena->allocate(512, "t1");
    auto t2 = arena->allocate(1024, "t2");
    auto t3 = arena->allocate(256, "t3");
    REQUIRE(t1);
    REQUIRE(t2);
    REQUIRE(t3);

    REQUIRE(vk.numBackingBuffers() >= 1);
    CHECK(vk.backingLiveCount(0) == 3);
    CHECK(vk.backingOffset(0) >= 512 + 1024 + 256);
}

// ---------------------------------------------------------------------------
// Map/unmap: mappedCount
// ---------------------------------------------------------------------------

TEST_CASE("TransientArena: map increments mappedCount, unmap decrements", "[rdg][transient-arena][gpu]") {
    auto db = std::unique_ptr<ArtifactDatabase>(ArtifactDatabase::create({}));
    REQUIRE(db);
    auto gpu = GpuContext::create(*db, "gpu_ta3", {});
    if (!gpu) SKIP("No Vulkan GPU context available");

    auto arena = TransientArena::create(*db, "arena_ta3", TransientArena::CreateParameters {.context = gpu});
    REQUIRE(arena);
    auto tb = arena->allocate(64, "tb");
    REQUIRE(tb);
    auto & vk         = requireVulkanArena(*arena);
    size_t backingIdx = vk.activeBackingIndex();

    CHECK(vk.backingMappedCount(backingIdx) == 0);

    {
        auto m = tb->map();
        REQUIRE(m.data());
        CHECK(vk.backingMappedCount(backingIdx) == 1);
    }
    CHECK(vk.backingMappedCount(backingIdx) == 0);
}

// ---------------------------------------------------------------------------
// Recycling: liveCount and offset reset
// ---------------------------------------------------------------------------

TEST_CASE("TransientArena: recycling reuses backing when all refs dropped", "[rdg][transient-arena][gpu]") {
    auto db = std::unique_ptr<ArtifactDatabase>(ArtifactDatabase::create({}));
    REQUIRE(db);
    auto gpu = GpuContext::create(*db, "gpu_ta4", {});
    if (!gpu) SKIP("No Vulkan GPU context available");

    auto arena = TransientArena::create(*db, "arena_ta4", TransientArena::CreateParameters {.context = gpu});
    REQUIRE(arena);
    auto & vk = requireVulkanArena(*arena);

    {
        auto tb = arena->allocate(1024, "tb_phase1");
        REQUIRE(tb);
        CHECK(vk.numBackingBuffers() >= 1);
        CHECK(vk.backingLiveCount(0) == 1);
        CHECK(vk.backingOffset(0) >= 1024);
    }

    CHECK(vk.backingLiveCount(0) == 0);

    {
        auto tb2 = arena->allocate(512, "tb_phase2");
        REQUIRE(tb2);
        CHECK(static_cast<const TransientBufferVulkan &>(*tb2).size() >= 512);
        CHECK(vk.backingLiveCount(0) == 1);
        CHECK(vk.backingOffset(0) >= 512);
        CHECK(vk.backingOffset(0) < 1024u * 1024u); // reused, offset reset
    }
}

// ---------------------------------------------------------------------------
// Recycling: no recycle while mapped
// ---------------------------------------------------------------------------

TEST_CASE("TransientArena: recyclable only when liveCount and mappedCount are 0", "[rdg][transient-arena][gpu]") {
    auto db = std::unique_ptr<ArtifactDatabase>(ArtifactDatabase::create({}));
    REQUIRE(db);
    auto gpu = GpuContext::create(*db, "gpu_ta5", {});
    if (!gpu) SKIP("No Vulkan GPU context available");

    auto arena = TransientArena::create(*db, "arena_ta5", TransientArena::CreateParameters {.context = gpu});
    REQUIRE(arena);
    auto & vk = requireVulkanArena(*arena);

    AutoRef<TransientBuffer> tb = arena->allocate(64, "tb");
    REQUIRE(tb);
    {
        auto m = tb->map();
        REQUIRE(m.data());
        CHECK(vk.backingLiveCount(0) == 1);
        CHECK(vk.backingMappedCount(0) == 1);
    }
    CHECK(vk.backingMappedCount(0) == 0);

    tb.clear();
    CHECK(vk.backingLiveCount(0) == 0);

    auto tb2 = arena->allocate(64, "tb2");
    REQUIRE(tb2);
    CHECK(vk.backingLiveCount(0) == 1);
}

// ---------------------------------------------------------------------------
// Geometric growth: second backing when first full
// ---------------------------------------------------------------------------

TEST_CASE("TransientArena: geometric growth creates larger second backing", "[rdg][transient-arena][gpu]") {
    auto db = std::unique_ptr<ArtifactDatabase>(ArtifactDatabase::create({}));
    REQUIRE(db);
    auto gpu = GpuContext::create(*db, "gpu_ta6", {});
    if (!gpu) SKIP("No Vulkan GPU context available");

    constexpr uint64_t MB = 1024u * 1024u;

    auto arena = TransientArena::create(*db, "arena_ta6", TransientArena::CreateParameters {.context = gpu});
    REQUIRE(arena);
    auto & vk = requireVulkanArena(*arena);

    auto t1 = arena->allocate(16 * MB - 256, "fill"); // nearly fill first 16MB
    REQUIRE(t1);
    CHECK(vk.numBackingBuffers() == 1);
    CHECK(vk.backingCapacity(0) == 16 * MB);

    auto t2 = arena->allocate(512, "overflow");
    REQUIRE(t2);
    REQUIRE(vk.numBackingBuffers() >= 2);
    CHECK(vk.backingCapacity(1) >= 32 * MB); // geometric growth: 2x
}

// ---------------------------------------------------------------------------
// suggestedArenaSize
// ---------------------------------------------------------------------------

TEST_CASE("TransientArena: suggestedArenaSize used when larger than default", "[rdg][transient-arena][gpu]") {
    auto db = std::unique_ptr<ArtifactDatabase>(ArtifactDatabase::create({}));
    REQUIRE(db);
    auto gpu = GpuContext::create(*db, "gpu_ta7", {});
    if (!gpu) SKIP("No Vulkan GPU context available");

    constexpr uint64_t MB = 1024u * 1024u;

    auto arena = TransientArena::create(*db, "arena_ta7", TransientArena::CreateParameters {.context = gpu, .suggestedArenaSize = 32 * MB});
    REQUIRE(arena);
    auto & vk = requireVulkanArena(*arena);

    auto tb = arena->allocate(64, "tb");
    REQUIRE(tb);
    REQUIRE(vk.numBackingBuffers() >= 1);
    CHECK(vk.backingCapacity(0) == 32 * MB);
}
