#include <catch2/catch_test_macros.hpp>
#include <garnet/GNfx2.h>
#include "fx2/model-internal.h"
#include <garnet/base/filesys.h>

using namespace GN;
using namespace GN::fx2;

static StrA repositoryPath(const char * relativePath) {
    const StrA root = getEnv("GARNET_ROOT");
    return root.empty() ? StrA(relativePath) : fs::joinPath(root, relativePath);
}

TEST_CASE("fx2::CelModelShading default config has valid anime parameters", "[fx2][cel]") {
    CelModelShading::Config config;
    CHECK(config.shadowThreshold > 0.0f);
    CHECK(config.shadowThreshold < 1.0f);
    CHECK(config.shadowFeather > 0.0f);
    CHECK(config.deepShadowThreshold < config.shadowThreshold);
    CHECK(config.specularThreshold > 0.0f);
    CHECK(config.rimIntensity > 0.0f);
    CHECK(config.outlineWidth > 0.0f);
    CHECK(config.outlineColor.a == 1.0f);
}

TEST_CASE("fx2::CelModelShading rejects null GPU context", "[fx2][cel]") {
    CHECK_FALSE(CelModelShading::create(nullptr));
}

TEST_CASE("fx2::CelModelShading builds surface and inverted-hull outline draws", "[fx2][cel][gpu]") {
    const StrA path = repositoryPath("src/3rdparty/assimp/test/models/glTF2/BoxTextured-glTF-Embedded/BoxTextured.gltf");
    if (!fs::isFile(path)) SKIP("Assimp model corpus is not initialized");

    const auto gpu = gpu2::GpuContext::create("cel-shading-test",
                                              gpu2::GpuContext::CreateParameters {.howToPrintDeviceCaps = gpu2::GpuContext::Verbosity::SILENCE});
    if (!gpu) SKIP("No gpu2 context is available");

    const auto scene = ModelScene::load({.path = path});
    REQUIRE(scene);
    const auto model = ModelAsset::create(gpu, scene);
    REQUIRE(model);

    CelModelShading::Config config;
    config.outlineWidth = 0.005f;
    const auto shading  = CelModelShading::create(gpu, config);
    REQUIRE(shading);
    REQUIRE(shading->uploadPayload());
    CHECK(shading->config().outlineWidth == 0.005f);

    const auto ssc = SharedShaderConstants::create({.gpu = gpu});
    REQUIRE(ssc);
    const auto snapshot = ssc->takeSnapshot();

    // 1. Surface draw check
    const auto surfaceDraw = CelModelShading::getDrawParams(snapshot, shading, model, 0, glm::mat4(1.0f));
    CHECK(surfaceDraw.vs);
    CHECK(surfaceDraw.ps);
    CHECK(surfaceDraw.geometry.vertexCount == scene->primitives[0].vertices.size());
    CHECK(surfaceDraw.geometry.indexCount == scene->primitives[0].indices.size());
    CHECK(surfaceDraw.states.cullMode == gpu2::RasterState::CULL_BACK);
    CHECK(surfaceDraw.states.depthState->func == gpu2::RasterState::Compare::LESS);
    REQUIRE(surfaceDraw.resources.size() == 2);
    REQUIRE(surfaceDraw.resources[1].size() == 7);
    CHECK(surfaceDraw.resources[1][6].size() == 1); // cel UBO

    // 2. Inverted-hull outline draw check
    const auto outlineDraw = CelModelShading::getOutlineDrawParams(snapshot, shading, model, 0, glm::mat4(1.0f));
    CHECK(outlineDraw.vs);
    CHECK(outlineDraw.ps);
    CHECK(outlineDraw.geometry.vertexCount == scene->primitives[0].vertices.size());
    CHECK(outlineDraw.geometry.indexCount == scene->primitives[0].indices.size());
    CHECK(outlineDraw.states.cullMode == gpu2::RasterState::CULL_FRONT);
    CHECK(outlineDraw.states.depthState->func == gpu2::RasterState::Compare::LESS_EQUAL);
    REQUIRE(outlineDraw.resources.size() == 2);
    REQUIRE(outlineDraw.resources[1].size() == 7);

    // 3. Runtime updateConfig check
    CelModelShading::Config updatedConfig = config;
    updatedConfig.outlineWidth            = 0.0f; // Disable outline
    shading->updateConfig(updatedConfig);
    CHECK(shading->config().outlineWidth == 0.0f);

    const auto disabledOutline = CelModelShading::getOutlineDrawParams(snapshot, shading, model, 0, glm::mat4(1.0f));
    CHECK_FALSE(disabledOutline.vs);
}
