#if GN_BUILD_HAS_VULKAN
    #include <catch2/catch_test_macros.hpp>
    // Include internal header first so rapid-vulkan is pulled in before any other TU might do so.
    #include "../vk-gpu-payload.h"

    #include <atomic>
    #include <vector>

using namespace GN;
using namespace GN::gpu2;

// A minimal subclass that makes setSemaphore() publicly accessible for testing,
// and counts how many times recordForVulkanSubmit() is called so tests can
// verify the double-submit guard works.
struct InstrumentedPayload : GpuPayloadVulkan {
    GN_REGISTER_RUNTIME_TYPE(GpuPayloadVulkan);
    using GpuPayloadVulkan::GpuPayloadVulkan;
    using GpuPayloadVulkan::setSemaphore;

    int                recordCallCount = 0;
    int                orderId         = 0;
    std::vector<int> * recordOrder     = nullptr;

    void recordForVulkanSubmit(const RecordContext &) override {
        ++recordCallCount;
        if (recordOrder) recordOrder->push_back(orderId);
    }
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
    vk::Semaphore fake {(VkSemaphore) (intptr_t) 0xDEAD0001};
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

TEST_CASE("GpuPayload: combine empty and single payload cases", "[gpu2][payload]") {
    gpu2::ArrayContainer<AutoRef<GpuPayload>> empty;
    CHECK(!GpuPayload::combine("empty", empty));

    auto                                      p = AutoRef<InstrumentedPayload>::make("single");
    gpu2::ArrayContainer<AutoRef<GpuPayload>> single;
    single.append(p);

    auto combined = GpuPayload::combine("single-group", single);
    CHECK(combined.get() == p.get());
}

TEST_CASE("GpuPayload: combine rejects duplicate input payloads", "[gpu2][payload]") {
    auto p = AutoRef<InstrumentedPayload>::make("duplicate");

    gpu2::ArrayContainer<AutoRef<GpuPayload>> payloads;
    payloads.append(p);
    payloads.append(p);

    CHECK(!GpuPayload::combine("bad-duplicate", payloads));
}

TEST_CASE("GpuPayload: combine rejects payload already owned by another group", "[gpu2][payload]") {
    auto p1 = AutoRef<InstrumentedPayload>::make("p1");
    auto p2 = AutoRef<InstrumentedPayload>::make("p2");
    auto p3 = AutoRef<InstrumentedPayload>::make("p3");

    gpu2::ArrayContainer<AutoRef<GpuPayload>> first;
    first.append(p1);
    first.append(p2);
    auto group = GpuPayload::combine("group", first);
    REQUIRE(group);

    gpu2::ArrayContainer<AutoRef<GpuPayload>> second;
    second.append(p2);
    second.append(p3);
    CHECK(!GpuPayload::combine("bad-shared", second));
}

TEST_CASE("GpuPayload: combine rejects sharing a leaf from a nested group", "[gpu2][payload]") {
    auto p1 = AutoRef<InstrumentedPayload>::make("p1");
    auto p2 = AutoRef<InstrumentedPayload>::make("p2");

    gpu2::ArrayContainer<AutoRef<GpuPayload>> first;
    first.append(p1);
    first.append(p2);
    auto group = GpuPayload::combine("group", first);
    REQUIRE(group);

    gpu2::ArrayContainer<AutoRef<GpuPayload>> parent;
    parent.append(group);
    parent.append(p1);
    CHECK(!GpuPayload::combine("bad-parent", parent));
}

TEST_CASE("GpuPayload: group destruction releases child membership", "[gpu2][payload]") {
    auto p1 = AutoRef<InstrumentedPayload>::make("p1");
    auto p2 = AutoRef<InstrumentedPayload>::make("p2");

    {
        gpu2::ArrayContainer<AutoRef<GpuPayload>> payloads;
        payloads.append(p1);
        payloads.append(p2);
        REQUIRE(GpuPayload::combine("temporary", payloads));
    }

    gpu2::ArrayContainer<AutoRef<GpuPayload>> payloads;
    payloads.append(p1);
    payloads.append(p2);
    CHECK(GpuPayload::combine("regrouped", payloads));
}

// ---------------------------------------------------------------------------
// GPU integration tests
// ---------------------------------------------------------------------------

TEST_CASE("GpuPayloadVulkan: grouped payloads record leaves in order", "[gpu2][payload][gpu]") {
    auto gpu = makeGpu();
    if (!gpu) SKIP("No GPU context available");

    std::vector<int> order;
    auto             p1 = AutoRef<InstrumentedPayload>::make("p1");
    auto             p2 = AutoRef<InstrumentedPayload>::make("p2");
    p1->orderId         = 1;
    p2->orderId         = 2;
    p1->recordOrder     = &order;
    p2->recordOrder     = &order;

    gpu2::ArrayContainer<AutoRef<GpuPayload>> payloads;
    payloads.append(p1);
    payloads.append(p2);
    auto group = GpuPayload::combine("ordered-group", payloads);
    REQUIRE(group);

    std::atomic<bool> done = false;
    gpu->submit(GpuContext::SubmitParameters("grouped").appendWork(group).setOnComplete([&] { done = true; }));
    while (!done) gpu->pump();

    CHECK(p1->recordCallCount == 1);
    CHECK(p2->recordCallCount == 1);
    REQUIRE(order.size() == 2u);
    CHECK(order[0] == 1);
    CHECK(order[1] == 2);
}

TEST_CASE("GpuPayloadVulkan: duplicate work through group and leaf is skipped", "[gpu2][payload][gpu]") {
    auto gpu = makeGpu();
    if (!gpu) SKIP("No GPU context available");

    auto p1 = AutoRef<InstrumentedPayload>::make("p1");
    auto p2 = AutoRef<InstrumentedPayload>::make("p2");

    gpu2::ArrayContainer<AutoRef<GpuPayload>> payloads;
    payloads.append(p1);
    payloads.append(p2);
    auto group = GpuPayload::combine("group", payloads);
    REQUIRE(group);

    std::atomic<bool> done = false;
    gpu->submit(GpuContext::SubmitParameters("duplicate-work").appendWork(group).appendWork(p1).setOnComplete([&] { done = true; }));
    while (!done) gpu->pump();

    CHECK(p1->recordCallCount == 1);
    CHECK(p2->recordCallCount == 1);
}

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
