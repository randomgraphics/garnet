/*
 * GpuBufferUpload unit tests — Phase 1.2 / 1.3 / 2
 *
 * Tests cover:
 *   Phase 1.2 — GpuBufferUpload public API: TYPE_ID, Mechanism enum, A::make factories.
 *   Phase 1.3 — GpuBufferUpload::create() with Vulkan backend.
 *   Phase 2   — HOST_MAP execute() ring slot rotation, data write, currentBufferView().
 *
 * Disabled: GpuBufferUpload action is not yet implemented.
 */
#if 0
    #include <catch2/catch_test_macros.hpp>
    #include <garnet/GNrdg.h>
    #include <memory>

using namespace GN;
using namespace GN::rdg;

// ─────────────────────────────────────────────────────────────────────────────
// Phase 1.2: public API (no GPU required)
// ─────────────────────────────────────────────────────────────────────────────

TEST_CASE("GpuBufferUpload: TYPE_ID is non-zero and distinct", "[rdg][gpu-buffer-upload]") {
    CHECK(GpuBufferUpload::TYPE_ID != 0);
    CHECK(GpuBufferUpload::A::TYPE_ID != 0);
    CHECK(GpuBufferUpload::TYPE_ID != GpuBufferUpload::A::TYPE_ID);
}

TEST_CASE("GpuBufferUpload::A: make(ptr, size) stores data and size", "[rdg][gpu-buffer-upload]") {
    const uint8_t dummy[16] = {};
    auto          a         = GpuBufferUpload::A::make(dummy, sizeof(dummy));
    REQUIRE(a != nullptr);
    CHECK(a->data == dummy);
    CHECK(a->dataSize == sizeof(dummy));
    CHECK_FALSE(a->writeFn); // no callback
}

TEST_CASE("GpuBufferUpload::A: make(writeFn) stores callback, no raw pointer", "[rdg][gpu-buffer-upload]") {
    bool called = false;
    auto a      = GpuBufferUpload::A::make([&](void *, uint64_t) { called = true; });
    REQUIRE(a != nullptr);
    CHECK(a->data == nullptr);
    CHECK(a->dataSize == 0);
    REQUIRE(static_cast<bool>(a->writeFn));
    // Invoke the callback to confirm it works.
    a->writeFn(nullptr, 0);
    CHECK(called);
}

TEST_CASE("GpuBufferUpload::CreateParameters: defaults are sensible", "[rdg][gpu-buffer-upload]") {
    GpuBufferUpload::CreateParameters cp;
    CHECK(cp.size == 0);
    CHECK(cp.mechanism == GpuBufferUpload::Mechanism::HOST_MAP);
    CHECK(cp.ringSlots == 2);
    CHECK(cp.gpu == nullptr);
}

// ─────────────────────────────────────────────────────────────────────────────
// Phase 1.3: create() with GPU (Vulkan backend stub)
// ─────────────────────────────────────────────────────────────────────────────

TEST_CASE("GpuBufferUpload::create: returns non-null for valid HOST_MAP params", "[rdg][gpu-buffer-upload][gpu]") {
    auto db = std::unique_ptr<ArtifactDatabase>(ArtifactDatabase::create({}));
    REQUIRE(db != nullptr);
    auto gpu = GpuContext::create(*db, "gpu", GpuContext::CreateParameters {});
    if (!gpu) SKIP("No Vulkan GPU context available");

    GpuBufferUpload::CreateParameters cp;
    cp.gpu       = gpu;
    cp.size      = 256;
    cp.mechanism = GpuBufferUpload::Mechanism::HOST_MAP;
    cp.ringSlots = 2;
    auto upload  = GpuBufferUpload::create(*db, "test_upload", cp);
    REQUIRE(upload != nullptr);
    CHECK(upload->typeId == GpuBufferUpload::TYPE_ID);
}

TEST_CASE("GpuBufferUpload::create: fails when gpu is null", "[rdg][gpu-buffer-upload]") {
    auto db = std::unique_ptr<ArtifactDatabase>(ArtifactDatabase::create({}));
    REQUIRE(db != nullptr);

    GpuBufferUpload::CreateParameters cp;
    cp.gpu      = nullptr;
    cp.size     = 256;
    auto upload = GpuBufferUpload::create(*db, "test_upload_no_gpu", cp);
    CHECK(upload == nullptr);
}

TEST_CASE("GpuBufferUpload::create: fails when size is 0", "[rdg][gpu-buffer-upload][gpu]") {
    auto db = std::unique_ptr<ArtifactDatabase>(ArtifactDatabase::create({}));
    REQUIRE(db != nullptr);
    auto gpu = GpuContext::create(*db, "gpu2", GpuContext::CreateParameters {});
    if (!gpu) SKIP("No Vulkan GPU context available");

    GpuBufferUpload::CreateParameters cp;
    cp.gpu      = gpu;
    cp.size     = 0;
    auto upload = GpuBufferUpload::create(*db, "test_upload_zero", cp);
    CHECK(upload == nullptr);
}

TEST_CASE("GpuBufferUpload::create: currentBufferView returns empty before first execute", "[rdg][gpu-buffer-upload][gpu]") {
    auto db = std::unique_ptr<ArtifactDatabase>(ArtifactDatabase::create({}));
    REQUIRE(db != nullptr);
    auto gpu = GpuContext::create(*db, "gpu3", GpuContext::CreateParameters {});
    if (!gpu) SKIP("No Vulkan GPU context available");

    GpuBufferUpload::CreateParameters cp;
    cp.gpu       = gpu;
    cp.size      = 64;
    cp.mechanism = GpuBufferUpload::Mechanism::HOST_MAP;
    cp.ringSlots = 2;
    auto upload  = GpuBufferUpload::create(*db, "test_upload_view", cp);
    REQUIRE(upload != nullptr);

    // After construction, currentBufferView() returns a valid view pointing to slot 0.
    auto view = upload->currentBufferView();
    CHECK(view.artifact != nullptr);
    CHECK(view.isBuffer());
}

// ─────────────────────────────────────────────────────────────────────────────
// Phase 2: HOST_MAP execute() via RenderGraph submission
// ─────────────────────────────────────────────────────────────────────────────

/// Utility: create a GpuBufferUpload, submit one frame of data through the render graph,
/// and verify execute() succeeded.
static void runUploadFrame(RenderGraph & rg, GpuBufferUpload & upload, const void * data, uint64_t size) {
    auto wf = rg.createWorkflow("upload");
    REQUIRE(wf);
    wf->tasks.append(Workflow::Task("write", AutoRef<GpuBufferUpload>(&upload), GpuBufferUpload::A::make(data, size)));
    auto sub = rg.submit({.workflows = {&wf, 1}});
    REQUIRE(sub);
    auto res = sub->result();
    CHECK(res.executionResult != Action::ExecutionResult::FAILED);
}

TEST_CASE("GpuBufferUpload HOST_MAP: execute writes data; view stays valid", "[rdg][gpu-buffer-upload][gpu]") {
    auto db = std::unique_ptr<ArtifactDatabase>(ArtifactDatabase::create({}));
    REQUIRE(db);
    auto rg = std::unique_ptr<RenderGraph>(RenderGraph::create({}));
    REQUIRE(rg);
    auto gpu = GpuContext::create(*db, "gpu_exec", GpuContext::CreateParameters {});
    if (!gpu) SKIP("No Vulkan GPU context available");

    constexpr uint64_t                kSize = 64;
    GpuBufferUpload::CreateParameters cp;
    cp.gpu       = gpu;
    cp.size      = kSize;
    cp.mechanism = GpuBufferUpload::Mechanism::HOST_MAP;
    cp.ringSlots = 2;
    auto upload  = GpuBufferUpload::create(*db, "upl_exec", cp);
    REQUIRE(upload);

    const uint8_t data[kSize] = {};
    runUploadFrame(*rg, *upload, data, kSize);

    // After first execute, view still points to a valid buffer.
    auto view = upload->currentBufferView();
    CHECK(view.artifact != nullptr);
    CHECK(view.isBuffer());
}

TEST_CASE("GpuBufferUpload HOST_MAP: currentBufferView rotates slot on each execute", "[rdg][gpu-buffer-upload][gpu]") {
    auto db = std::unique_ptr<ArtifactDatabase>(ArtifactDatabase::create({}));
    REQUIRE(db);
    auto rg = std::unique_ptr<RenderGraph>(RenderGraph::create({}));
    REQUIRE(rg);
    auto gpu = GpuContext::create(*db, "gpu_rot", GpuContext::CreateParameters {});
    if (!gpu) SKIP("No Vulkan GPU context available");

    constexpr uint64_t                kSize = 64;
    GpuBufferUpload::CreateParameters cp;
    cp.gpu       = gpu;
    cp.size      = kSize;
    cp.mechanism = GpuBufferUpload::Mechanism::HOST_MAP;
    cp.ringSlots = 2;
    auto upload  = GpuBufferUpload::create(*db, "upl_rot", cp);
    REQUIRE(upload);

    auto view0 = upload->currentBufferView(); // slot 0 initially
    REQUIRE(view0.artifact != nullptr);

    const uint8_t data[kSize] = {};
    runUploadFrame(*rg, *upload, data, kSize);
    auto view1 = upload->currentBufferView(); // slot 1 after first execute
    CHECK(view1.artifact != nullptr);
    // The artifact pointer must have changed (different ring slot).
    CHECK(view0.artifact.get() != view1.artifact.get());

    runUploadFrame(*rg, *upload, data, kSize);
    auto view2 = upload->currentBufferView(); // slot 0 again after second execute (ringSlots=2)
    CHECK(view2.artifact != nullptr);
    CHECK(view0.artifact.get() == view2.artifact.get());
}

TEST_CASE("GpuBufferUpload HOST_MAP: writeFn callback executes correctly", "[rdg][gpu-buffer-upload][gpu]") {
    auto db = std::unique_ptr<ArtifactDatabase>(ArtifactDatabase::create({}));
    REQUIRE(db);
    auto rg = std::unique_ptr<RenderGraph>(RenderGraph::create({}));
    REQUIRE(rg);
    auto gpu = GpuContext::create(*db, "gpu_fn", GpuContext::CreateParameters {});
    if (!gpu) SKIP("No Vulkan GPU context available");

    constexpr uint64_t                kSize = 64;
    GpuBufferUpload::CreateParameters cp;
    cp.gpu       = gpu;
    cp.size      = kSize;
    cp.mechanism = GpuBufferUpload::Mechanism::HOST_MAP;
    cp.ringSlots = 2;
    auto upload  = GpuBufferUpload::create(*db, "upl_fn", cp);
    REQUIRE(upload);

    bool callbackInvoked = false;
    auto args            = GpuBufferUpload::A::make([&](void * dst, uint64_t cap) {
        callbackInvoked = true;
        CHECK(dst != nullptr);
        CHECK(cap == kSize);
        memset(dst, 0xAB, cap);
    });

    auto wf = rg->createWorkflow("upload_fn");
    REQUIRE(wf);
    wf->tasks.append(Workflow::Task("write_fn", upload, args));
    auto sub = rg->submit({.workflows = {&wf, 1}});
    REQUIRE(sub);
    auto res = sub->result();
    CHECK(res.executionResult != Action::ExecutionResult::FAILED);
    CHECK(callbackInvoked);
}
#else
    #include <catch2/catch_test_macros.hpp>
    #include <garnet/GNrdg.h>
TEST_CASE("GpuBufferUpload: skipped (not implemented)", "[rdg][gpu-buffer-upload]") { SKIP("GpuBufferUpload not yet implemented"); }
#endif
