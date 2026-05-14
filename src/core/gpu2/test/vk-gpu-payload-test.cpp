#if GN_BUILD_HAS_VULKAN
    #include <catch2/catch_test_macros.hpp>
    // Include internal header first so rapid-vulkan is pulled in before any other TU might do so.
    #include "../vk-gpu-payload.h"

    #include <atomic>

using namespace GN;
using namespace GN::gpu2;

// A minimal subclass that makes setSemaphore() publicly accessible for testing,
// and counts how many times recordForVulkanSubmit() is called so tests can
// verify the double-submit guard works.
struct InstrumentedPayload : GpuPayloadVulkan {
    GN_REGISTER_RUNTIME_TYPE(GpuPayloadVulkan);
    using GpuPayloadVulkan::GpuPayloadVulkan;
    using GpuPayloadVulkan::setSemaphore;

    int  recordCallCount = 0;
    void recordForVulkanSubmit(const RecordContext &) override { ++recordCallCount; }
};

static AutoRef<GpuContext> makeGpu() {
    return GpuContext::create("gpu", GpuContext::CreateParameters {.howToPrintDeviceCaps = GpuContext::Verbosity::SILENCE});
}

// ---------------------------------------------------------------------------
// Pure state-machine tests — no GPU required
// ---------------------------------------------------------------------------

TEST_CASE("GpuPayloadVulkan: semaphore() is null before any setSemaphore call", "[gpu2][payload]") {
    auto p = AutoRef<InstrumentedPayload>::make("unsubmitted");
    CHECK(!p->semaphore()); // default variant holds empty AutoRef → null handle
}

TEST_CASE("GpuPayloadVulkan: setSemaphore(vk::Semaphore) sets semaphore()", "[gpu2][payload]") {
    auto          p = AutoRef<InstrumentedPayload>::make("native-sem");
    vk::Semaphore fake {(VkSemaphore) 0xDEAD0001};
    p->setSemaphore(fake);
    CHECK(p->semaphore() == fake);
}

TEST_CASE("GpuPayloadVulkan: setSemaphore(vk::Semaphore) can overwrite each frame (swapchain reuse)", "[gpu2][payload]") {
    // The swapchain calls setSemaphore on the same readyPayload every frame with the
    // new imageAvailable handle — this must not assert.
    auto          p = AutoRef<InstrumentedPayload>::make("swapchain-reuse");
    vk::Semaphore s1 {(VkSemaphore) 0x1000};
    vk::Semaphore s2 {(VkSemaphore) 0x2000};
    p->setSemaphore(s1);
    REQUIRE(p->semaphore() == s1);
    p->setSemaphore(s2); // must not assert on second call
    CHECK(p->semaphore() == s2);
}

// ---------------------------------------------------------------------------
// GPU integration tests
// ---------------------------------------------------------------------------

TEST_CASE("GpuPayloadVulkan: double-submit is detected and skipped", "[gpu2][payload][gpu]") {
    auto gpu = makeGpu();
    if (!gpu) SKIP("No GPU context available");

    auto p = AutoRef<InstrumentedPayload>::make("payload");

    // First submit: recordForVulkanSubmit should be called once.
    std::atomic<bool> done1 = false;
    gpu->submit(GpuContext::SubmitParameters("first").appendWork(p).setOnComplete([&] { done1 = true; }));
    while (!done1) gpu->pump();
    REQUIRE(p->recordCallCount == 1);
    REQUIRE(p->semaphore()); // semaphore is set — payload is marked as "submitted"

    // Second submit of the same payload: semaphore() is non-null so the guard must
    // skip recordForVulkanSubmit. The submission still completes (with empty work).
    std::atomic<bool> done2 = false;
    gpu->submit(GpuContext::SubmitParameters("second").appendWork(p).setOnComplete([&] { done2 = true; }));
    while (!done2) gpu->pump();
    CHECK(p->recordCallCount == 1); // guard worked — not called a second time
}

TEST_CASE("GpuPayloadVulkan: unsubmitted dependency is skipped but submission still completes", "[gpu2][payload][gpu]") {
    auto gpu = makeGpu();
    if (!gpu) SKIP("No GPU context available");

    auto work = AutoRef<InstrumentedPayload>::make("work");
    auto dep  = AutoRef<InstrumentedPayload>::make("dep"); // intentionally never submitted

    // dep has a null semaphore → must be silently skipped, not block the submission.
    std::atomic<bool> done = false;
    gpu->submit(GpuContext::SubmitParameters("main").appendWork(work).waitFor(dep).setOnComplete([&] { done = true; }));
    while (!done) gpu->pump();

    CHECK(work->recordCallCount == 1); // work ran despite the invalid dependency
    CHECK(!dep->semaphore());          // dep remains unsubmitted
}

#endif // GN_BUILD_HAS_VULKAN
