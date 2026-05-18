/**
 * PBR shading unit tests (migrated from src/test/ut/rdg/pbr-shading-test.cpp).
 * GPU-dependent tests use SKIP() when no Vulkan context is available.
 */

#include "common.h"
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <garnet/GNbase.h>
#include <garnet/GNrdg.h>
#include <garnet/base/filesys.h>
#include <algorithm>

using namespace GN;
using namespace GN::rdg;

TEST_CASE("PBR: type IDs non-zero", "[rdg][pbr]") {
    CHECK(PbrShading::TYPE_ID != 0);
    CHECK(PbrShading::Material::TYPE_ID != 0);
    CHECK(SharedShaderConstants::TYPE_ID != 0);
}

TEST_CASE("PBR: create and build without graph", "[rdg][pbr][gpu]") {
    auto gpuContext = GpuContext::create("gpu_context", {.howToPrintDeviceCaps = gpuVerbosity});
    if (!gpuContext) SKIP("No Vulkan GPU context available");

    auto pbr = PbrShading::create("pbr", PbrShading::CreateParameters {.gpu = gpuContext});
    REQUIRE(pbr != nullptr);

    PbrShading::BuildParameters params;
    params.renderGraph = nullptr;
    auto sg            = pbr->build(params);
    CHECK(sg.builtResult == Action::ExecutionResult::PASSED);
    CHECK(sg.workflows.empty());
}

TEST_CASE("PBR: build with render graph returns workflow", "[rdg][pbr][gpu]") {
    auto renderGraph = RenderGraph::create({});
    REQUIRE(renderGraph != nullptr);
    auto gpuContext = GpuContext::create("gpu_context", {.howToPrintDeviceCaps = gpuVerbosity});
    if (!gpuContext) SKIP("No Vulkan GPU context available");

    auto pbr = PbrShading::create("pbr_build", PbrShading::CreateParameters {.gpu = gpuContext});
    REQUIRE(pbr != nullptr);

    PbrShading::BuildParameters params;
    params.renderGraph = renderGraph;
    auto sg            = pbr->build(params);
    CHECK(sg.builtResult == Action::ExecutionResult::PASSED);
    REQUIRE_FALSE(sg.workflows.empty());
    CHECK_FALSE(sg.workflows[0].tasks().empty());
}

TEST_CASE("PBR: SubGraph::drop clears workflows", "[rdg][pbr][gpu]") {
    auto renderGraph = RenderGraph::create({});
    REQUIRE(renderGraph != nullptr);
    auto gpuContext = GpuContext::create("gpu_context", {.howToPrintDeviceCaps = gpuVerbosity});
    if (!gpuContext) SKIP("No Vulkan GPU context available");

    auto pbr = PbrShading::create("pbr_drop", PbrShading::CreateParameters {.gpu = gpuContext});
    REQUIRE(pbr != nullptr);
    PbrShading::BuildParameters params;
    params.renderGraph = renderGraph;
    auto sg            = pbr->build(params);
    REQUIRE_FALSE(sg.workflows.empty());

    sg.drop();
    CHECK(sg.workflows.empty());
}

TEST_CASE("PBR: Material::load from empty MemFile", "[rdg][pbr][gpu]") {
    auto gpuContext = GpuContext::create("gpu_context", {.howToPrintDeviceCaps = gpuVerbosity});
    if (!gpuContext) SKIP("No Vulkan GPU context available");

    static const char empty[1] = {};
    auto              memFile  = AutoRef<MemFile>::make(const_cast<char *>(empty), 0, "pbr_material");
    REQUIRE(memFile->readable());
    auto mat = PbrShading::Material::load("test_material_empty", PbrShading::Material::LoadParameters {.gpu = gpuContext, .source = memFile});
    REQUIRE(mat != nullptr);
    CHECK(mat->isKindOf<PbrShading::Material>());
}

TEST_CASE("PBR: Material::load resolves texture path", "[rdg][pbr][gpu]") {
    auto gpuContext = GpuContext::create("gpu_context", {.howToPrintDeviceCaps = gpuVerbosity});
    if (!gpuContext) SKIP("No Vulkan GPU context available");

    static const char content[] = "baseColorTexture=texture/earth.jpg\n";
    auto              memFile   = AutoRef<MemFile>::make(const_cast<char *>(content), sizeof(content) - 1, "test.material");
    REQUIRE(memFile->readable());
    StrA basePath = "media::";
    auto mat =
        PbrShading::Material::load("test_material_tex", PbrShading::Material::LoadParameters {.gpu = gpuContext, .source = memFile, .basePath = basePath});
    REQUIRE(mat != nullptr);
    CHECK(mat->getBaseColorTexture() != nullptr);
}

TEST_CASE("PBR: Material::load lined-metal-sheeting", "[rdg][pbr][gpu][media]") {
    auto gpuContext = GpuContext::create("gpu_context", {.howToPrintDeviceCaps = gpuVerbosity});
    if (!gpuContext) SKIP("No Vulkan GPU context available");

    auto fp = fs::openFile("media::pbr/lined-metal-sheeting/lined-metal-sheeting.material", std::ios::in);
    if (!fp) SKIP("Media not mounted (media::pbr/lined-metal-sheeting not found)");

    auto mat = PbrShading::Material::load("lined_metal_sheeting", PbrShading::Material::LoadParameters {
                                                                      .gpu      = gpuContext,
                                                                      .source   = fp,
                                                                      .basePath = "media::pbr/lined-metal-sheeting",
                                                                  });
    REQUIRE(mat != nullptr);
    Texture * baseColorTex = mat->getBaseColorTexture();
    REQUIRE(baseColorTex != nullptr);

    gfx::img::Image readback = baseColorTex->readback();
    CHECK_FALSE(readback.empty());
    CHECK(readback.width() > 0);
    CHECK(readback.height() > 0);

    bool hasNonZero = false;
    for (uint32_t y = 0; y < readback.height() && !hasNonZero; y += std::max(1u, readback.height() / 16))
        for (uint32_t x = 0; x < readback.width() && !hasNonZero; x += std::max(1u, readback.width() / 16))
            for (size_t c = 0; c < readback.step(); ++c)
                if (readback.at({}, x, y, 0)[c] != 0) hasNonZero = true;
    CHECK(hasNonZero);
}

TEST_CASE("PBR: SharedShaderConstants view round-trip", "[rdg][pbr][gpu]") {
    auto gpuContext = GpuContext::create("gpu_context", {.howToPrintDeviceCaps = gpuVerbosity});
    if (!gpuContext) SKIP("No Vulkan GPU context available");

    auto shared = SharedShaderConstants::create("shared", SharedShaderConstants::CreateParameters {.gpu = gpuContext});
    REQUIRE(shared != nullptr);

    SharedShaderConstants::ViewInformation view;
    view.cameraPosition    = Location {1.f, 2.f, 3.f};
    view.cameraOrientation = glm::quat(1.f, 0.f, 0.f, 0.f);
    view.cameraFov         = Degree(60.f);
    view.aspectRatio       = 16.f / 9.f;
    view.nearPlane         = 0.1f;
    view.farPlane          = 500.f;
    shared->setViewInformation(view);

    const auto & got = shared->getViewInformation();
    CHECK_THAT(got.cameraPosition.x, Catch::Matchers::WithinAbs(1.f, 1e-5f));
    CHECK_THAT(got.cameraPosition.y, Catch::Matchers::WithinAbs(2.f, 1e-5f));
    CHECK_THAT(got.cameraPosition.z, Catch::Matchers::WithinAbs(3.f, 1e-5f));
    CHECK_THAT(got.cameraFov.value, Catch::Matchers::WithinAbs(Radian(Degree(60.f)).value, 1e-5f));
    CHECK_THAT(got.aspectRatio, Catch::Matchers::WithinAbs(16.f / 9.f, 1e-5f));
}
