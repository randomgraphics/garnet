// Tests for GN::rdg2::SharedShaderConstants.
// Requires a Vulkan GPU context; each test skips gracefully if unavailable.

#include <catch2/catch_test_macros.hpp>
#include <garnet/GNrdg2.h>

using namespace GN;
using namespace GN::rdg2;
using namespace GN::gpu2;

static const GpuContext::Verbosity kGpuVerbosity = GpuContext::Verbosity::SILENCE;

static AutoRef<GpuContext> makeGpu(const char * name) { return GpuContext::create(name, GpuContext::CreateParameters {.howToPrintDeviceCaps = kGpuVerbosity}); }

TEST_CASE("rdg2::SharedShaderConstants: create() returns non-null with valid params", "[rdg2][ssc][gpu]") {
    auto gpu = makeGpu("ssc_create_gpu");
    if (!gpu) SKIP("No Vulkan GPU context available");

    auto ssc = SharedShaderConstants::create({.gpu = gpu});
    REQUIRE(ssc);
}

TEST_CASE("rdg2::SharedShaderConstants: snapshot contains env resources", "[rdg2][ssc][gpu]") {
    auto gpu = makeGpu("ssc_env_gpu");
    if (!gpu) SKIP("No Vulkan GPU context available");

    auto ssc = SharedShaderConstants::create({.gpu = gpu});
    REQUIRE(ssc);

    ssc->set0.envLighting.environmentRadianceScale = 1.f;

    auto snapshot = ssc->takeSnapshot();
    CHECK(snapshot.set0Resources.size() == 6u);
}

TEST_CASE("rdg2::SharedShaderConstants: first snapshot includes fallback + env upload payloads", "[rdg2][ssc][gpu]") {
    auto gpu = makeGpu("ssc_fallback_payload_gpu");
    if (!gpu) SKIP("No Vulkan GPU context available");

    auto ssc = SharedShaderConstants::create({.gpu = gpu});
    REQUIRE(ssc);

    // First snapshot must carry at least two uploads: fallback textures + env v1 blue-sky.
    auto snapshot = ssc->takeSnapshot();
    CHECK(snapshot.set0Payloads.size() >= 2u);
}

TEST_CASE("rdg2::SharedShaderConstants: takeSnapshot() returns resources", "[rdg2][ssc][gpu]") {
    auto gpu = makeGpu("ssc_snap_gpu");
    if (!gpu) SKIP("No Vulkan GPU context available");

    auto ssc = SharedShaderConstants::create({.gpu = gpu});
    REQUIRE(ssc);

    auto snapshot = ssc->takeSnapshot();
    CHECK(snapshot.set0Resources.size() == 6u);
}

TEST_CASE("rdg2::SharedShaderConstants: snapshot returns payloads", "[rdg2][ssc][gpu]") {
    auto gpu = makeGpu("ssc_content_gpu");
    if (!gpu) SKIP("No Vulkan GPU context available");

    auto ssc = SharedShaderConstants::create({.gpu = gpu});
    REQUIRE(ssc);

    auto snapshot = ssc->takeSnapshot();
    // set0Resources must have 6 bindings: scene UBO, camera UBO, 4 env textures.
    CHECK(snapshot.set0Resources.size() == 6u);
    // set0Payloads must be non-empty — it carries UBO + any pending texture uploads.
    CHECK(!snapshot.set0Payloads.empty());
}

TEST_CASE("rdg2::SharedShaderConstants: two snapshots are independent values", "[rdg2][ssc][gpu]") {
    auto gpu = makeGpu("ssc_evict_gpu");
    if (!gpu) SKIP("No Vulkan GPU context available");

    auto ssc = SharedShaderConstants::create({.gpu = gpu});
    REQUIRE(ssc);

    auto s1 = ssc->takeSnapshot();
    CHECK(s1.set0Resources.size() == 6u);

    ssc->set0.frameConstants.frameCounter = 2;
    auto s2                               = ssc->takeSnapshot();
    CHECK(s2.set0Resources.size() == 6u);
    CHECK(!s2.set0Payloads.empty());
}
