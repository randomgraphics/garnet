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

TEST_CASE("rdg2::SharedShaderConstants: env textures load async via set0.envLighting paths", "[rdg2][ssc][gpu]") {
    auto gpu = makeGpu("ssc_env_gpu");
    if (!gpu) SKIP("No Vulkan GPU context available");
    auto graph = Graph::create();
    REQUIRE(graph);

    auto ssc = SharedShaderConstants::create({.gpu = gpu, .graph = graph});
    REQUIRE(ssc);

    // Setting envLighting paths triggers async IO on the next takeSnapshot().
    ssc->set0.envLighting.environmentRadianceScale = 1.f;

    // After takeSnapshot + wait, set0Resources must be populated (blue-sky defaults at minimum).
    VersionedArtifact snapshot = ssc->takeSnapshot();
    REQUIRE(snapshot.artifact);
    REQUIRE(snapshot.version);
    graph->waitForToken(snapshot.version);
    auto content = ssc->getContent(snapshot.artifact);
    REQUIRE(content);
    CHECK(content->set0Resources.size() == 6u);
}

TEST_CASE("rdg2::SharedShaderConstants: first snapshot includes fallback + env upload payloads", "[rdg2][ssc][gpu]") {
    auto gpu = makeGpu("ssc_fallback_payload_gpu");
    if (!gpu) SKIP("No Vulkan GPU context available");
    auto graph = Graph::create();
    REQUIRE(graph);

    auto ssc = SharedShaderConstants::create({.gpu = gpu, .graph = graph});
    REQUIRE(ssc);

    VersionedArtifact snapshot = ssc->takeSnapshot();
    REQUIRE(snapshot.artifact);
    REQUIRE(snapshot.version);
    graph->waitForToken(snapshot.version);

    auto content = ssc->getContent(snapshot.artifact);
    REQUIRE(content);
    // First snapshot must carry at least two uploads: fallback textures + env v1 blue-sky.
    CHECK(content->set0Payloads.size() >= 2u);
}

TEST_CASE("rdg2::SharedShaderConstants: takeSnapshot() returns non-null token", "[rdg2][ssc][gpu]") {
    auto gpu = makeGpu("ssc_snap_gpu");
    if (!gpu) SKIP("No Vulkan GPU context available");
    auto graph = Graph::create();
    REQUIRE(graph);

    auto ssc = SharedShaderConstants::create({.gpu = gpu, .graph = graph});
    REQUIRE(ssc);

    VersionedArtifact snapshot = ssc->takeSnapshot();
    REQUIRE(snapshot.artifact != nullptr);
    REQUIRE(snapshot.version != nullptr);
}

TEST_CASE("rdg2::SharedShaderConstants: getContent() returns valid content after snapshot", "[rdg2][ssc][gpu]") {
    auto gpu = makeGpu("ssc_content_gpu");
    if (!gpu) SKIP("No Vulkan GPU context available");
    auto graph = Graph::create();
    REQUIRE(graph);

    auto ssc = SharedShaderConstants::create({.gpu = gpu, .graph = graph});
    REQUIRE(ssc);

    VersionedArtifact snapshot = ssc->takeSnapshot();
    REQUIRE(snapshot.artifact);
    REQUIRE(snapshot.version);

    // Block until the upload node completes.
    graph->waitForToken(snapshot.version);

    auto content = ssc->getContent(snapshot.artifact);
    REQUIRE(content);
    // set0Resources must have 6 bindings: scene UBO, camera UBO, 4 env textures.
    CHECK(content->set0Resources.size() == 6u);
    // set0Payloads must be non-empty — it carries UBO + any pending texture uploads.
    CHECK(!content->set0Payloads.empty());
}

TEST_CASE("rdg2::SharedShaderConstants: two snapshots evict stale map entries", "[rdg2][ssc][gpu]") {
    auto gpu = makeGpu("ssc_evict_gpu");
    if (!gpu) SKIP("No Vulkan GPU context available");
    auto graph = Graph::create();
    REQUIRE(graph);

    auto ssc = SharedShaderConstants::create({.gpu = gpu, .graph = graph});
    REQUIRE(ssc);

    // Frame 1
    VersionedArtifact s1 = ssc->takeSnapshot();
    REQUIRE(s1.artifact);
    REQUIRE(s1.version);
    graph->waitForToken(s1.version);
    auto c1 = ssc->getContent(s1.artifact);
    REQUIRE(c1);

    // Frame 2 publishes a newer content version to the same artifact.
    ssc->set0.frameConstants.frameCounter = 2;
    VersionedArtifact s2                  = ssc->takeSnapshot();
    REQUIRE(s2.artifact);
    REQUIRE(s2.version);
    CHECK(s2.artifact == s1.artifact);
    graph->waitForToken(s2.version);

    // getContent returns the latest published content of the artifact.
    auto c2 = ssc->getContent(s2.artifact);
    CHECK(c2);
    CHECK(c2->set0Parameters.frameConstants.frameCounter == 2);
}
