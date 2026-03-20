/*
 * Tests for Skybox + IBL infrastructure (Tasks 2.1a-c, 1.2).
 *
 * Verifies:
 *  - Texture::create with faces=6 creates a cube-compatible Vulkan image.
 *  - createDefault1x1CubemapTexture succeeds and reports faces==6.
 *  - SharedShaderConstants: setEnvironmentLightingInformation round-trip.
 *  - After build(), getSet0Resources() has 6 bindings (camera, lighting, 4 env slots).
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
}

TEST_CASE("SharedShaderConstants: build() provides 6-binding Set 0 after env lighting set", "[rdg][shared-constants][ibl][gpu]") {
    auto rg = std::unique_ptr<RenderGraph>(RenderGraph::create({}));
    REQUIRE(rg);
    auto gpu = GpuContext::create("gpu_env2", {.howToPrintDeviceCaps = gpuVerbosity});
    if (!gpu) SKIP("No Vulkan GPU context available");

    auto ssc = SharedShaderConstants::create("ssc_env2", {.gpu = gpu});
    REQUIRE(ssc);

    // Even without calling setEnvironmentLightingInformation, build() must provide 6 bindings
    // (fallback textures fill slots 2-5).
    auto sg = ssc->build(*rg);
    REQUIRE(!sg.workflows.empty());
    CHECK(ssc->getSet0Resources().size() == 6u);
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
    REQUIRE(set0.size() >= 3u);                          // at least bindings 0..2
    REQUIRE(set0[2].size() == 1u);                       // binding 2 has one view
    CHECK(set0[2][0].artifact.get() == cube.get());      // binding 2 points to our cubemap
}
