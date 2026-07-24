// Regression test for engine2 quit-order safety: the VisualDomain (which owns the Vulkan
// instance) is commonly destroyed while the OperatingDomain (whose window provided the
// render surface) is still alive — exactly what the simple-world sample does. The render
// surface must not outlive the instance, or the validation layer reports leaked objects
// at vkDestroyInstance time.

#include <catch2/catch_test_macros.hpp>

#include <garnet/GNengine2.h>

using namespace GN;
using namespace GN::e2;

namespace {

// Counts ERROR-level messages on the Vulkan logger; validation-layer complaints
// (e.g. "VkSurfaceKHR has not been destroyed") are routed there.
struct ErrorCounter : Logger::Receiver {
    int errors = 0;

    void onLog(Logger &, const Logger::LogLocation & loc, const char *) override {
        if (loc.level <= Logger::ERROR_) ++errors;
    }
    void onLog(Logger &, const Logger::LogLocation & loc, const wchar_t *) override {
        if (loc.level <= Logger::ERROR_) ++errors;
    }
};

} // namespace

TEST_CASE("E2 visual: destroying the visual domain before the OS domain leaves no live Vulkan objects", "[e2][gpu]") {
    Universe u;

    auto os = OperatingDomain::create({.universe = u, .caption = "GNtest-e2-teardown", .width = 320, .height = 240});
    if (!os) SKIP("Window creation failed (no display?)");

    auto visual = VisualDomain::create({.universe = u, .os = os});
    if (!visual) SKIP("No Vulkan-capable GPU context");

    ErrorCounter counter;
    auto *       vkLogger = getLogger("GN.gpu2.vk");
    vkLogger->addReceiver(&counter);

    // Mirror the sample's quit order: the visual domain goes away first, while the OS domain
    // still holds the window. Any Vulkan object left alive at instance destruction (or any
    // other teardown error) surfaces as an ERROR log from the validation layer.
    visual.clear();

    vkLogger->removeReceiver(&counter);
    CHECK(counter.errors == 0);
}
