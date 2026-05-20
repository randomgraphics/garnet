/*
 * GpuUploadSlot unit tests — Phase 1.1
 *
 * GpuUploadSlot is a CPU-only lifecycle helper. No GPU context required.
 * We verify:
 *   - initial state: isReady() == true
 *   - notifyCompletion(): slot transitions to not-ready
 *   - recycle(): clears keepAlive and resets submissionId
 *   - keepAlive refcount semantics
 *   - addKeepAlive() retains reference; recycle() drops it
 */

#include <catch2/catch_test_macros.hpp>
#include "../vk-upload-slot.h"

using namespace GN;
using namespace GN::rdg;

// Minimal RefCounter subclass to track destruction.
struct TrackingResource : public RefCounter {
    bool & destroyed;
    TrackingResource(bool & flag): destroyed(flag) { flag = false; }
    ~TrackingResource() override { destroyed = true; }
};

// Build a synthetic (non-empty) SubmissionID without a real queue.
// We set only the index so the empty() check returns false.
// We never call .wait() on it in these tests.
static rapid_vulkan::CommandQueue::SubmissionID makeFakeSubmissionId() {
    rapid_vulkan::CommandQueue::SubmissionID id;
    id.queue = 1; // non-zero pointer value (not dereferenced in these tests)
    id.index = 1;
    return id;
}

TEST_CASE("GpuUploadSlot: initial state is ready with no keepAlive", "[rdg][upload-slot]") {
    GpuUploadSlot slot;
    CHECK(slot.isReady());
    CHECK(slot.keepAlive.empty());
    CHECK(slot.submissionId.empty());
}

TEST_CASE("GpuUploadSlot: notifyCompletion makes slot not-ready", "[rdg][upload-slot]") {
    GpuUploadSlot slot;
    slot.notifyCompletion(makeFakeSubmissionId());
    CHECK_FALSE(slot.isReady());
    CHECK_FALSE(slot.submissionId.empty());
}

TEST_CASE("GpuUploadSlot: recycle clears submissionId and keepAlive", "[rdg][upload-slot]") {
    bool          destroyed = false;
    GpuUploadSlot slot;
    slot.notifyCompletion(makeFakeSubmissionId());
    slot.addKeepAlive(AutoRef<TrackingResource>::make(destroyed));

    CHECK_FALSE(slot.isReady());
    CHECK_FALSE(slot.keepAlive.empty());
    CHECK_FALSE(destroyed);

    slot.recycle();

    CHECK(slot.isReady());
    CHECK(slot.keepAlive.empty());
    CHECK(slot.submissionId.empty());
    CHECK(destroyed); // keepAlive ref dropped → destructor called
}

TEST_CASE("GpuUploadSlot: addKeepAlive retains multiple resources", "[rdg][upload-slot]") {
    bool          d1 = false, d2 = false;
    GpuUploadSlot slot;
    slot.addKeepAlive(AutoRef<TrackingResource>::make(d1));
    slot.addKeepAlive(AutoRef<TrackingResource>::make(d2));

    CHECK(slot.keepAlive.size() == 2);
    CHECK_FALSE(d1);
    CHECK_FALSE(d2);

    slot.recycle();

    CHECK(slot.keepAlive.empty());
    CHECK(d1);
    CHECK(d2);
}

TEST_CASE("GpuUploadSlot: recycle on ready slot is a no-op", "[rdg][upload-slot]") {
    GpuUploadSlot slot;
    REQUIRE(slot.isReady());
    slot.recycle(); // should not crash or change anything meaningful
    CHECK(slot.isReady());
    CHECK(slot.keepAlive.empty());
}

TEST_CASE("GpuUploadSlot: waitUntilReady on ready slot does not block", "[rdg][upload-slot]") {
    GpuUploadSlot slot;
    REQUIRE(slot.isReady());
    slot.waitUntilReady("test-slot"); // must not block or crash
    CHECK(slot.isReady());
}

TEST_CASE("GpuUploadSlot: keepAlive resource is not destroyed before recycle", "[rdg][upload-slot]") {
    bool          destroyed = false;
    GpuUploadSlot slot;
    {
        auto res = AutoRef<TrackingResource>::make(destroyed);
        slot.addKeepAlive(res); // slot holds a ref
        // local ref goes out of scope here
    }
    CHECK_FALSE(destroyed); // slot still holds a ref
    slot.recycle();
    CHECK(destroyed); // slot released its ref
}
