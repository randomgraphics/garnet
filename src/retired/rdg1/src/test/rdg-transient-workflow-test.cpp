/**
 * Task 4.1 — Sample workflow using transient buffer + copy.
 * Task 4.2 — Per-draw dynamic-offset packing experiment.
 *
 * Demonstrates the intended "allocate transient on caller thread, map/write/
 * unmap, then copy to device buffer via submission" pattern.
 */

#include "common.h"
#include <catch2/catch_test_macros.hpp>
#include <garnet/GNrdg.h>
#include <cstring>

using namespace GN;
using namespace GN::rdg;

// ---------------------------------------------------------------------------
// 4.1 — UBO-style upload via transient buffer + CopyBuffer
// ---------------------------------------------------------------------------

struct FakeCameraUBO {
    float viewMatrix[16];
    float projMatrix[16];
};

TEST_CASE("Transient workflow: UBO upload via transient + copy", "[rdg][transient-workflow][gpu]") {
    auto rg = std::unique_ptr<RenderGraph>(RenderGraph::create({}));
    REQUIRE(rg);
    auto gpu = GpuContext::create("gpu_tw", {.howToPrintDeviceCaps = gpuVerbosity});
    if (!gpu) SKIP("No Vulkan GPU context available");

    constexpr uint64_t kUboSize = sizeof(FakeCameraUBO);

    // 1. Create the device-local UBO that shaders will bind to.
    auto ubo =
        PersistentBuffer::create("camera_ubo", {.context = gpu, .size = kUboSize, .usage = BufferUsageFlags(static_cast<uint32_t>(BufferUsageBits::UNIFORM))});
    REQUIRE(ubo);

    // 2. On the caller thread: create arena and allocate transient, write structured data.
    auto arena = TransientArena::create("arena_ubo", TransientArena::CreateParameters {.context = gpu});
    REQUIRE(arena);
    auto tb = arena->allocate(kUboSize, "tb");
    REQUIRE(tb);
    {
        auto mapped = tb->map();
        REQUIRE(mapped.size() >= sizeof(FakeCameraUBO));
        auto * ptr = static_cast<FakeCameraUBO *>(mapped.data());
        for (int i = 0; i < 16; ++i) ptr->viewMatrix[i] = float(i);
        for (int i = 0; i < 16; ++i) ptr->projMatrix[i] = float(i + 100);
    }

    // 3. Create a stateless copy action and wire arguments.
    auto copy = GpuCopy::create("ubo_copy", {.gpu = gpu});
    REQUIRE(copy);

    auto args  = AutoRef<GpuCopy::BufferToBuffer>::make();
    args->src  = tb;
    args->dst  = ubo;
    args->size = kUboSize;

    // 4. Submit a workflow that performs the copy.
    auto wf = rg->createWorkflow("ubo-upload-wf");
    wf.appendTask("camera-copy", copy, args);

    auto sub = rg->submit({.workflows = SafeArrayAccessor<Workflow>(&wf, 1)});
    REQUIRE(sub);
    auto result = sub->result();
    CHECK(result.executionResult == Action::PASSED);

    // After submission completes, the caller can drop the transient reference.
    tb.clear();
}

// ---------------------------------------------------------------------------
// 4.1b — Two UBOs uploaded in a single submission (camera + lighting)
// ---------------------------------------------------------------------------

struct FakeLightingUBO {
    float color[4];
    float direction[4];
};

TEST_CASE("Transient workflow: multiple UBO copies in one submission", "[rdg][transient-workflow][gpu]") {
    auto rg = std::unique_ptr<RenderGraph>(RenderGraph::create({}));
    REQUIRE(rg);
    auto gpu = GpuContext::create("gpu_tw2", {.howToPrintDeviceCaps = gpuVerbosity});
    if (!gpu) SKIP("No Vulkan GPU context available");

    const BufferUsageFlags kDstUsage(static_cast<uint32_t>(BufferUsageBits::UNIFORM));

    auto cameraUbo   = PersistentBuffer::create("cam_ubo", {.context = gpu, .size = sizeof(FakeCameraUBO), .usage = kDstUsage});
    auto lightingUbo = PersistentBuffer::create("light_ubo", {.context = gpu, .size = sizeof(FakeLightingUBO), .usage = kDstUsage});
    REQUIRE(cameraUbo);
    REQUIRE(lightingUbo);

    auto arena = TransientArena::create("arena_multi", TransientArena::CreateParameters {.context = gpu});
    REQUIRE(arena);

    // Caller thread: write both UBOs into separate transient allocations.
    auto tbCam = arena->allocate(sizeof(FakeCameraUBO), "tb_cam");
    REQUIRE(tbCam);
    {
        auto m = tbCam->map();
        std::memset(m.data(), 0xAA, sizeof(FakeCameraUBO));
    }

    auto tbLight = arena->allocate(sizeof(FakeLightingUBO), "tb_light");
    REQUIRE(tbLight);
    {
        auto m = tbLight->map();
        std::memset(m.data(), 0xBB, sizeof(FakeLightingUBO));
    }

    // Reuse one stateless CopyBuffer action for both tasks.
    auto copy = GpuCopy::create("multi_copy", {.gpu = gpu});
    REQUIRE(copy);

    auto camArgs  = AutoRef<GpuCopy::BufferToBuffer>::make();
    camArgs->src  = tbCam;
    camArgs->dst  = cameraUbo;
    camArgs->size = sizeof(FakeCameraUBO);

    auto lightArgs  = AutoRef<GpuCopy::BufferToBuffer>::make();
    lightArgs->src  = tbLight;
    lightArgs->dst  = lightingUbo;
    lightArgs->size = sizeof(FakeLightingUBO);

    auto wf = rg->createWorkflow("multi-ubo-wf");
    wf.appendTask("camera-copy", copy, camArgs);
    wf.appendTask("lighting-copy", copy, lightArgs);

    auto sub = rg->submit({.workflows = SafeArrayAccessor<Workflow>(&wf, 1)});
    REQUIRE(sub);
    auto result = sub->result();
    CHECK(result.executionResult == Action::PASSED);
}

// ---------------------------------------------------------------------------
// 4.2 — Per-draw dynamic-offset packing in a single transient buffer
// ---------------------------------------------------------------------------

struct PerDrawPayload {
    float modelMatrix[16];
    float tintColor[4];
};

TEST_CASE("Transient workflow: per-draw payloads packed in one transient buffer", "[rdg][transient-workflow][gpu]") {
    auto rg = std::unique_ptr<RenderGraph>(RenderGraph::create({}));
    REQUIRE(rg);
    auto gpu = GpuContext::create("gpu_dyn", {.howToPrintDeviceCaps = gpuVerbosity});
    if (!gpu) SKIP("No Vulkan GPU context available");

    constexpr uint32_t     kDrawCount   = 4;
    constexpr uint64_t     kPayloadSize = sizeof(PerDrawPayload);
    constexpr uint64_t     kTotalSize   = kPayloadSize * kDrawCount;
    const BufferUsageFlags kDstUsage(static_cast<uint32_t>(BufferUsageBits::STORAGE));

    auto arena = TransientArena::create("arena_dyn", TransientArena::CreateParameters {.context = gpu});
    REQUIRE(arena);

    // Allocate one transient buffer large enough for all draws.
    auto tb = arena->allocate(kTotalSize, "tb");
    REQUIRE(tb);

    auto mapped = tb->map();
    REQUIRE(mapped.size() >= kTotalSize);
    auto * base = static_cast<PerDrawPayload *>(mapped.data());
    for (uint32_t i = 0; i < kDrawCount; ++i) {
        auto & pd = base[i];
        for (int j = 0; j < 16; ++j) pd.modelMatrix[j] = float(i * 100 + j);
        for (int j = 0; j < 4; ++j) pd.tintColor[j] = float(i) / float(kDrawCount);
    }

    // Copy the entire packed region into a device-local storage buffer.
    auto dst = PersistentBuffer::create("per_draw_buf", {.context = gpu, .size = kTotalSize, .usage = kDstUsage});
    REQUIRE(dst);

    auto copy = GpuCopy::create("dyn_copy", {.gpu = gpu});
    REQUIRE(copy);

    auto args  = AutoRef<GpuCopy::BufferToBuffer>::make();
    args->src  = tb;
    args->dst  = dst;
    args->size = kTotalSize;

    auto wf = rg->createWorkflow("dyn-offset-wf");
    wf.appendTask("pack-copy", copy, args);

    auto sub = rg->submit({.workflows = SafeArrayAccessor<Workflow>(&wf, 1)});
    REQUIRE(sub);
    auto result = sub->result();
    CHECK(result.executionResult == Action::PASSED);

    // At this point, a real renderer would bind `dst` with dynamic offsets
    // [0, kPayloadSize, 2*kPayloadSize, ...] for each draw call.
}
