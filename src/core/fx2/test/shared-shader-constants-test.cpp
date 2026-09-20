// Tests for GN::fx2::SharedShaderConstants.
// Requires a Vulkan GPU context; each test skips gracefully if unavailable.

#include <catch2/catch_test_macros.hpp>
#include <garnet/GNfx2.h>
#include "../vk-shaders/camera-ubo.h"
#include "../vk-shaders/scene-ubo.h"
#include <cstring>

using namespace GN;
using namespace GN::fx2;
using namespace GN::gpu2;

static const GpuContext::Verbosity kGpuVerbosity = GpuContext::Verbosity::SILENCE;

static AutoRef<GpuContext> makeGpu(const char * name) { return GpuContext::create(name, GpuContext::CreateParameters {.howToPrintDeviceCaps = kGpuVerbosity}); }

TEST_CASE("fx2::SharedShaderConstants: create() returns non-null with valid params", "[fx2][ssc][gpu]") {
    auto gpu = makeGpu("ssc_create_gpu");
    if (!gpu) SKIP("No Vulkan GPU context available");

    auto ssc = SharedShaderConstants::create({.gpu = gpu});
    REQUIRE(ssc);
}

TEST_CASE("fx2::SharedShaderConstants: snapshot contains env resources", "[fx2][ssc][gpu]") {
    auto gpu = makeGpu("ssc_env_gpu");
    if (!gpu) SKIP("No Vulkan GPU context available");

    auto ssc = SharedShaderConstants::create({.gpu = gpu});
    REQUIRE(ssc);

    ssc->set0.envLighting.environmentLuminanceScale = 1.f;

    auto snapshot = ssc->takeSnapshot();
    CHECK(snapshot.set0Resources.size() == 6u);
}

TEST_CASE("fx2::SharedShaderConstants: first snapshot includes fallback + env upload payloads", "[fx2][ssc][gpu]") {
    auto gpu = makeGpu("ssc_fallback_payload_gpu");
    if (!gpu) SKIP("No Vulkan GPU context available");

    auto ssc = SharedShaderConstants::create({.gpu = gpu});
    REQUIRE(ssc);

    // First snapshot must carry at least two uploads: fallback textures + env v1 blue-sky.
    auto snapshot = ssc->takeSnapshot();
    CHECK(snapshot.set0Payloads.size() >= 2u);
}

TEST_CASE("fx2::SharedShaderConstants: takeSnapshot() returns resources", "[fx2][ssc][gpu]") {
    auto gpu = makeGpu("ssc_snap_gpu");
    if (!gpu) SKIP("No Vulkan GPU context available");

    auto ssc = SharedShaderConstants::create({.gpu = gpu});
    REQUIRE(ssc);

    auto snapshot = ssc->takeSnapshot();
    CHECK(snapshot.set0Resources.size() == 6u);
}

TEST_CASE("fx2::SharedShaderConstants: snapshot returns payloads", "[fx2][ssc][gpu]") {
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

TEST_CASE("fx2::SharedShaderConstants: two snapshots are independent values", "[fx2][ssc][gpu]") {
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

TEST_CASE("fx2::SharedShaderConstants: camera exposure and debug environment floor reach the GPU and update between snapshots", "[fx2][ssc][gpu]") {
    auto gpu = makeGpu("ssc_exposure_gpu");
    if (!gpu) SKIP("No Vulkan GPU context available");
    auto ssc = SharedShaderConstants::create({.gpu = gpu});
    REQUIRE(ssc);
    CHECK(ssc->set0.camera.exposure == 0.002f);
    CHECK(ssc->set0.envLighting.environmentAmbientFloor == 0.f);

    for (float exposure : {ssc->set0.camera.exposure, 1.f, 0.f}) {
        CAPTURE(exposure);
        ssc->set0.envLighting.environmentAmbientFloor = exposure == 1.f ? 5.f : 0.f;
        ssc->set0.camera.exposure                     = exposure;
        auto                         snapshot         = ssc->takeSnapshot();
        GpuContext::SubmitParameters submission("ssc-exposure");
        for (const auto & payload : snapshot.set0Payloads) submission.appendWork(payload);
        gpu->submit(submission);
        gpu->waitForIdle();

        REQUIRE(snapshot.set0Resources.size() > 1);
        REQUIRE(snapshot.set0Resources[1].size() == 1);
        auto buffer = snapshot.set0Resources[1][0].buffer();
        REQUIRE(buffer);
        auto bytes = buffer->readContent();
        REQUIRE(bytes.size() == sizeof(shader::CameraUBO));
        shader::CameraUBO camera {};
        std::memcpy(&camera, bytes.data(), sizeof(camera));
        CHECK(camera.exposure == exposure);
        CHECK(camera.nearPlane == ssc->set0.camera.nearPlane);
        CHECK(camera.farPlane == ssc->set0.camera.farPlane);

        REQUIRE(snapshot.set0Resources[0].size() == 1);
        auto sceneBuffer = snapshot.set0Resources[0][0].buffer();
        REQUIRE(sceneBuffer);
        auto sceneBytes = sceneBuffer->readContent();
        REQUIRE(sceneBytes.size() == sizeof(shader::SceneUBO));
        shader::SceneUBO scene {};
        std::memcpy(&scene, sceneBytes.data(), sizeof(scene));
        CHECK(scene.environmentAmbientFloor == ssc->set0.envLighting.environmentAmbientFloor);
        CHECK(scene.environmentLuminanceScale == ssc->set0.envLighting.environmentLuminanceScale);
    }
}
