/*
 * Tests for Skybox + IBL infrastructure (Phase 1 + Phase 3).
 *
 * Verifies:
 *  - Texture::create with faces=6 creates a cube-compatible Vulkan image.
 *  - createDefault1x1CubemapTexture succeeds and reports faces==6.
 *  - SharedShaderConstants: setEnvironmentLightingInformation round-trip.
 *  - After build(), getSet0Resources() has 7 bindings (camera, lighting, 4 env textures, env UBO).
 *  - SkyBox::create succeeds and build() produces a non-empty SubGraph.
 */

#include "common.h"
#include <catch2/catch_test_macros.hpp>
#include <garnet/GNrdg.h>
#include "rdg/vk-texture.h"

using namespace GN;
using namespace GN::rdg;

TEST_CASE("Texture: create with faces=6 succeeds (cubemap)", "[rdg][texture][cubemap][gpu]") {
    auto gpu = GpuContext::create("gpu_cube", {.howToPrintDeviceCaps = gpuVerbosity});
    if (!gpu) SKIP("No Vulkan GPU context available");

    Texture::Descriptor desc;
    desc.setDimensions(64, 64).setFaces(6).setLevels(1).setFormat(gfx::img::PixelFormat::RGBA_8_8_8_8_UNORM());
    auto tex = Texture::create("cube64", Texture::CreateParameters {.context = gpu, .descriptor = desc});
    REQUIRE(tex);
    CHECK(tex->descriptor().faces == 6u);
    CHECK(tex->descriptor().width == 64u);
    CHECK(tex->descriptor().height == 64u);
}

TEST_CASE("Texture: createDefault1x1CubemapTexture succeeds", "[rdg][texture][cubemap][gpu]") {
    auto gpu = GpuContext::create("gpu_cube1x1", {.howToPrintDeviceCaps = gpuVerbosity});
    if (!gpu) SKIP("No Vulkan GPU context available");

    auto tex = createDefault1x1CubemapTexture(gpu, "cube_fallback", 0, 0, 0, 255);
    REQUIRE(tex);
    CHECK(tex->descriptor().faces == 6u);
    CHECK(tex->descriptor().width == 1u);
    CHECK(tex->descriptor().height == 1u);
}

TEST_CASE("SharedShaderConstants: setEnvironmentLightingInformation round-trip", "[rdg][shared-constants][ibl][gpu]") {
    auto gpu = GpuContext::create("gpu_env", {.howToPrintDeviceCaps = gpuVerbosity});
    if (!gpu) SKIP("No Vulkan GPU context available");

    auto ssc = SharedShaderConstants::create("ssc_env", {.gpu = gpu});
    REQUIRE(ssc);

    // Initially all textures are null.
    CHECK(ssc->getEnvironmentLightingInformation().skyboxCubemap == nullptr);
    CHECK(ssc->getEnvironmentLightingInformation().irradianceMap == nullptr);

    // Set a cubemap and verify it round-trips.
    auto cube = createDefault1x1CubemapTexture(gpu, "test_cube", 128, 128, 255, 255);
    REQUIRE(cube);

    SharedShaderConstants::EnvironmentLightingInformation env;
    env.skyboxCubemap = cube;
    ssc->setEnvironmentLightingInformation(env);
    CHECK(ssc->getEnvironmentLightingInformation().skyboxCubemap.get() == cube.get());
    CHECK(ssc->getEnvironmentLightingInformation().irradianceMap == nullptr);
    CHECK(ssc->getEnvironmentLightingInformation().environmentRadianceScale == 1.0f);
}

TEST_CASE("SharedShaderConstants: environmentRadianceScale round-trip", "[rdg][shared-constants][ibl][gpu]") {
    auto gpu = GpuContext::create("gpu_env_scale", {.howToPrintDeviceCaps = gpuVerbosity});
    if (!gpu) SKIP("No Vulkan GPU context available");

    auto ssc = SharedShaderConstants::create("ssc_env_scale", {.gpu = gpu});
    REQUIRE(ssc);

    SharedShaderConstants::EnvironmentLightingInformation env;
    env.environmentRadianceScale = 2.5f;
    ssc->setEnvironmentLightingInformation(env);
    CHECK(ssc->getEnvironmentLightingInformation().environmentRadianceScale == 2.5f);
}

TEST_CASE("SharedShaderConstants: build() provides 7-binding Set 0 after env lighting set", "[rdg][shared-constants][ibl][gpu]") {
    auto rg = std::unique_ptr<RenderGraph>(RenderGraph::create({}));
    REQUIRE(rg);
    auto gpu = GpuContext::create("gpu_env2", {.howToPrintDeviceCaps = gpuVerbosity});
    if (!gpu) SKIP("No Vulkan GPU context available");

    auto ssc = SharedShaderConstants::create("ssc_env2", {.gpu = gpu});
    REQUIRE(ssc);

    // Even without calling setEnvironmentLightingInformation, build() must provide 7 bindings
    // (fallback textures fill slots 2-5, env UBO at 6).
    auto sg = ssc->build(*rg);
    REQUIRE(!sg.workflows.empty());
    CHECK(ssc->getSet0Resources().size() == 7u);
}

TEST_CASE("SkyBox: create and build produce a non-empty SubGraph", "[rdg][skybox][gpu]") {
    auto rg = std::unique_ptr<RenderGraph>(RenderGraph::create({}));
    REQUIRE(rg);
    auto gpu = GpuContext::create("gpu_skybox", {.howToPrintDeviceCaps = gpuVerbosity});
    if (!gpu) SKIP("No Vulkan GPU context available");

    auto skybox = SkyBox::create("test_skybox", {.gpu = gpu});
    REQUIRE(skybox);

    auto ssc = SharedShaderConstants::create("ssc_skybox", {.gpu = gpu});
    REQUIRE(ssc);
    ssc->build(*rg);

    SkyBox::BuildParameters bp;
    bp.renderGraph           = rg.get();
    bp.sharedShaderConstants = ssc;
    auto sg                  = skybox->build(bp);

    CHECK(sg.builtResult == Action::ExecutionResult::PASSED);
    CHECK(!sg.workflows.empty());
}

TEST_CASE("SharedShaderConstants: Set 0 binding 2 uses provided skyboxCubemap", "[rdg][shared-constants][ibl][gpu]") {
    auto rg = std::unique_ptr<RenderGraph>(RenderGraph::create({}));
    REQUIRE(rg);
    auto gpu = GpuContext::create("gpu_env3", {.howToPrintDeviceCaps = gpuVerbosity});
    if (!gpu) SKIP("No Vulkan GPU context available");

    auto ssc = SharedShaderConstants::create("ssc_env3", {.gpu = gpu});
    REQUIRE(ssc);

    auto cube = createDefault1x1CubemapTexture(gpu, "skybox_cube", 0, 128, 255, 255);
    REQUIRE(cube);

    SharedShaderConstants::EnvironmentLightingInformation env;
    env.skyboxCubemap = cube;
    ssc->setEnvironmentLightingInformation(env);

    auto sg = ssc->build(*rg);
    REQUIRE(!sg.workflows.empty());

    const auto & set0 = ssc->getSet0Resources();
    REQUIRE(set0.size() >= 3u);                     // at least bindings 0..2
    REQUIRE(set0[2].size() == 1u);                  // binding 2 has one view
    CHECK(set0[2][0].artifact.get() == cube.get()); // binding 2 points to our cubemap
}
