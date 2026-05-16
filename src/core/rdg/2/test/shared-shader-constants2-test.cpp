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
    auto graph = Graph::create();
    REQUIRE(graph);

    auto ssc = SharedShaderConstants::create({.gpu = gpu, .graph = graph});
    REQUIRE(ssc);
}

TEST_CASE("rdg2::SharedShaderConstants: fallback env textures are non-null after create", "[rdg2][ssc][gpu]") {
    auto gpu = makeGpu("ssc_fallback_gpu");
    if (!gpu) SKIP("No Vulkan GPU context available");
    auto graph = Graph::create();
    REQUIRE(graph);

    auto ssc = SharedShaderConstants::create({.gpu = gpu, .graph = graph});
    REQUIRE(ssc);
    CHECK(ssc->set0.environmentLighting.skyboxCubemap);
    CHECK(ssc->set0.environmentLighting.irradianceMap);
    CHECK(ssc->set0.environmentLighting.prefilteredEnvMap);
    CHECK(ssc->set0.environmentLighting.brdfLut);
}

TEST_CASE("rdg2::SharedShaderConstants: takeSnapshot() returns non-null token", "[rdg2][ssc][gpu]") {
    auto gpu = makeGpu("ssc_snap_gpu");
    if (!gpu) SKIP("No Vulkan GPU context available");
    auto graph = Graph::create();
    REQUIRE(graph);

    auto ssc = SharedShaderConstants::create({.gpu = gpu, .graph = graph});
    REQUIRE(ssc);

    TokenPtr token = ssc->takeSnapshot();
    REQUIRE(token != nullptr);
}

TEST_CASE("rdg2::SharedShaderConstants: getContent() returns valid content after snapshot", "[rdg2][ssc][gpu]") {
    auto gpu = makeGpu("ssc_content_gpu");
    if (!gpu) SKIP("No Vulkan GPU context available");
    auto graph = Graph::create();
    REQUIRE(graph);

    auto ssc = SharedShaderConstants::create({.gpu = gpu, .graph = graph});
    REQUIRE(ssc);

    TokenPtr token = ssc->takeSnapshot();
    REQUIRE(token);

    // Block until the upload node completes.
    graph->waitForToken(token);

    auto content = ssc->getContent(token);
    REQUIRE(content);
    // set0Resources must have 6 bindings: scene UBO, camera UBO, 4 env textures.
    CHECK(content->set0Resources.size() == 6u);
    // Payload must be non-null — it carries the GPU copy command.
    CHECK(content->set0Payload);
}

TEST_CASE("rdg2::SharedShaderConstants: two snapshots evict stale map entries", "[rdg2][ssc][gpu]") {
    auto gpu = makeGpu("ssc_evict_gpu");
    if (!gpu) SKIP("No Vulkan GPU context available");
    auto graph = Graph::create();
    REQUIRE(graph);

    auto ssc = SharedShaderConstants::create({.gpu = gpu, .graph = graph});
    REQUIRE(ssc);

    // Frame 1
    TokenPtr t1 = ssc->takeSnapshot();
    REQUIRE(t1);
    graph->waitForToken(t1);
    auto c1 = ssc->getContent(t1);
    REQUIRE(c1);

    // Frame 2 — takeSnapshot() should evict the satisfied token from frame 1.
    ssc->set0.frameConstants.frameCounter = 2;
    TokenPtr t2                           = ssc->takeSnapshot();
    REQUIRE(t2);
    graph->waitForToken(t2);

    // t1's entry was evicted; getContent must return empty for it.
    auto c1again = ssc->getContent(t1);
    CHECK(!c1again);

    // t2's content is still accessible.
    auto c2 = ssc->getContent(t2);
    CHECK(c2);
}
