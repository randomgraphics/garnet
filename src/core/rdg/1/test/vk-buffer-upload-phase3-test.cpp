/*
 * GpuBufferUploadVulkan Phase 3 tests — GPU completion token distribution.
 *
 * Verifies that after a full submission round-trip (upload + draw), the upload
 * action's current slot receives a non-empty SubmissionID from the command
 * buffer manager.  This is the mechanism that makes ring-slot reuse safe without
 * a global waitForIdle() on the hot path.
 *
 * Tests access vk-buffer-upload.h internals directly (Tier 2 / white-box).
 *
 * Disabled: GpuBufferUpload / vk-buffer-upload not yet implemented.
 */
#if 0
    #include <catch2/catch_test_macros.hpp>
    #include <garnet/GNrdg.h>

using namespace GN;
using namespace GN::rdg;

// ─────────────────────────────────────────────────────────────────────────────
// Helpers
// ─────────────────────────────────────────────────────────────────────────────

/// Submit one upload frame through the render graph and wait for it to finish.
/// Returns the submission result.
static Submission::Result submitUploadFrame(RenderGraph & rg, AutoRef<GpuBufferUpload> upload, const uint8_t * data, uint64_t size) {
    auto wf = rg.createWorkflow("upload");
    wf.appendTask(Workflow::Task("write", upload, GpuBufferUpload::A::make(data, size)));
    auto sub = rg.submit({.workflows = SafeArrayAccessor<Workflow>(&wf, 1)});
    return sub->result();
}

/// Cast the public GpuBufferUpload to the internal Vulkan subclass.
static GpuBufferUploadVulkan * asVulkan(GpuBufferUpload * upload) { return static_cast<GpuBufferUploadVulkan *>(upload); }

// ─────────────────────────────────────────────────────────────────────────────
// Phase 3.1 / 3.2: Slot receives submissionId after GPU submission
// ─────────────────────────────────────────────────────────────────────────────

TEST_CASE("GpuBufferUpload Phase3: slot is ready before any execute", "[rdg][upload-phase3][gpu]") {
    auto gpu = GpuContext::create("gpu_p3_ready", {.howToPrintDeviceCaps = gpuVerbosity});
    if (!gpu) SKIP("No Vulkan GPU context available");

    GpuBufferUpload::CreateParameters cp;
    cp.gpu       = gpu;
    cp.size      = 64;
    cp.mechanism = GpuBufferUpload::Mechanism::HOST_MAP;
    cp.ringSlots = 2;
    auto upload  = GpuBufferUpload::create("upl_p3_ready", cp);
    REQUIRE(upload);

    auto * vkUpload = asVulkan(upload.get());

    // Before any execute, slot 0 is the current slot and is immediately ready.
    CHECK(vkUpload->mSlots[0].isReady());
    CHECK(vkUpload->mSlots[1].isReady());
}

TEST_CASE("GpuBufferUpload Phase3: upload-only submission — slot stays ready (no draw CB)", "[rdg][upload-phase3][gpu]") {
    // An upload-only workflow has no GPU command buffer, so CommandBufferManagerVulkan::submit()
    // is never called → no SubmissionID distributed.  The slot should remain isReady().
    auto rg  = std::unique_ptr<RenderGraph>(RenderGraph::create({}));
    auto gpu = GpuContext::create("gpu_p3_nocd", {.howToPrintDeviceCaps = gpuVerbosity});
    if (!gpu) SKIP("No Vulkan GPU context available");

    GpuBufferUpload::CreateParameters cp;
    cp.gpu       = gpu;
    cp.size      = 64;
    cp.mechanism = GpuBufferUpload::Mechanism::HOST_MAP;
    cp.ringSlots = 2;
    auto upload  = GpuBufferUpload::create("upl_p3_nocd", cp);
    REQUIRE(upload);

    const uint8_t data[64] = {};
    auto          res      = submitUploadFrame(*rg, upload, data, 64);
    CHECK(res.executionResult != Action::ExecutionResult::FAILED);

    auto * vkUpload = asVulkan(upload.get());
    // After execute(), mCurrentSlot is 1 (advanced from 0).
    // Slot 1 is the one that was just written.
    CHECK(vkUpload->mCurrentSlot == 1u);
    // Without a draw command buffer, no SubmissionID is distributed → slot 1 is still ready.
    CHECK(vkUpload->mSlots[1].isReady());
}

TEST_CASE("GpuBufferUpload Phase3: with draw command — slot gets submissionId", "[rdg][upload-phase3][gpu]") {
    // A workflow that combines an upload with a draw (ClearRenderTarget is enough to
    // produce a GPU command buffer submission) should result in the upload slot
    // receiving a non-empty SubmissionID.
    auto rg = std::unique_ptr<RenderGraph>(RenderGraph::create({}));
    REQUIRE(rg);
    auto gpu = GpuContext::create("gpu_p3_draw", {.howToPrintDeviceCaps = gpuVerbosity});
    if (!gpu) SKIP("No Vulkan GPU context available");

    // Build a minimal renderable target (headless backbuffer).
    auto backbuffer =
        Backbuffer::create("bb_p3", Backbuffer::CreateParameters {.context = gpu, .descriptor = Backbuffer::Descriptor {}.setDimensions(64, 64)});
    REQUIRE(backbuffer);
    auto rt = RenderTarget::create("rt_p3", RenderTarget::CreateParameters {});
    REQUIRE(rt);
    rt->addColorTarget(backbuffer);

    auto prepareAction = PrepareBackbuffer::create("prepare_p3", PrepareBackbuffer::CreateParameters {.gpu = gpu});
    auto clearAction   = ClearRenderTarget::create("clear_p3", ClearRenderTarget::CreateParameters {.gpu = gpu});
    auto presentAction = PresentBackbuffer::create("present_p3", PresentBackbuffer::CreateParameters {.gpu = gpu});
    REQUIRE(prepareAction);
    REQUIRE(clearAction);
    REQUIRE(presentAction);

    // Create the upload.
    GpuBufferUpload::CreateParameters cp;
    cp.gpu       = gpu;
    cp.size      = 64;
    cp.mechanism = GpuBufferUpload::Mechanism::HOST_MAP;
    cp.ringSlots = 2;
    auto upload  = GpuBufferUpload::create("upl_p3_draw", cp);
    REQUIRE(upload);

    // Build one workflow: prepare → upload → clear → present.
    const uint8_t data[64] = {};
    auto          wf       = rg->createWorkflow("frame");
    {
        auto prepA        = AutoRef<PrepareBackbuffer::A>(new PrepareBackbuffer::A());
        prepA->backbuffer = backbuffer;
        wf.appendTask(Workflow::Task("prepare", prepareAction, prepA));
    }
    wf.appendTask(Workflow::Task("upload", upload, GpuBufferUpload::A::make(data, sizeof(data))));
    {
        auto clearA          = AutoRef<ClearRenderTarget::A>(new ClearRenderTarget::A());
        clearA->renderTarget = rt;
        wf.appendTask(Workflow::Task("clear", clearAction, clearA));
    }
    {
        auto presentA        = AutoRef<PresentBackbuffer::A>(new PresentBackbuffer::A());
        presentA->backbuffer = backbuffer;
        wf.appendTask(Workflow::Task("present", presentAction, presentA));
    }

    auto sub = rg->submit({.workflows = SafeArrayAccessor<Workflow>(&wf, 1)});
    REQUIRE(sub);
    auto res = sub->result();
    CHECK(res.executionResult != Action::ExecutionResult::FAILED);

    auto * vkUpload = asVulkan(upload.get());
    // Slot 1 was written (mCurrentSlot advanced from 0 → 1).
    CHECK(vkUpload->mCurrentSlot == 1u);
    // With a draw command buffer submitted, the slot should have a non-empty SubmissionID.
    CHECK_FALSE(vkUpload->mSlots[1].isReady());
}

TEST_CASE("GpuBufferUpload Phase3: slot becomes ready after recycle", "[rdg][upload-phase3]") {
    // Pure CPU test — no GPU needed. Verify recycle() resets the state.
    GpuBufferUploadVulkan::Slot slot;
    CHECK(slot.isReady());

    rapid_vulkan::CommandQueue::SubmissionID fakeId;
    fakeId.queue = 1;
    fakeId.index = 42;
    slot.notifyCompletion(fakeId);
    CHECK_FALSE(slot.isReady());

    slot.recycle();
    CHECK(slot.isReady());
}
#else
    #include <catch2/catch_test_macros.hpp>
    #include <garnet/GNrdg.h>
TEST_CASE("GpuBufferUpload Phase3: skipped (not implemented)", "[rdg][upload-phase3]") { SKIP("GpuBufferUpload/vk-buffer-upload not yet implemented"); }
#endif
