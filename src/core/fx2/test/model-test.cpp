#include <catch2/catch_test_macros.hpp>
#include <garnet/GNfx2.h>

using namespace GN::fx2;

TEST_CASE("fx2::ModelScene classifies supported source formats", "[fx2][model]") {
    CHECK(ModelScene::sourceFormatFromPath("scene.fbx") == ModelScene::SourceFormat::FBX);
    CHECK(ModelScene::sourceFormatFromPath("scene.gltf") == ModelScene::SourceFormat::GLTF);
    CHECK(ModelScene::sourceFormatFromPath("scene.glb") == ModelScene::SourceFormat::GLB);
    CHECK(ModelScene::sourceFormatFromPath("scene.stl") == ModelScene::SourceFormat::STL);
}

TEST_CASE("fx2::ModelScene source classification is case insensitive", "[fx2][model]") {
    CHECK(ModelScene::sourceFormatFromPath("scene.FBX") == ModelScene::SourceFormat::FBX);
    CHECK(ModelScene::sourceFormatFromPath("scene.GlTf") == ModelScene::SourceFormat::GLTF);
    CHECK(ModelScene::sourceFormatFromPath("scene.GLB") == ModelScene::SourceFormat::GLB);
    CHECK(ModelScene::sourceFormatFromPath("scene.StL") == ModelScene::SourceFormat::STL);
}

TEST_CASE("fx2::ModelScene rejects unsupported source extensions", "[fx2][model]") {
    CHECK(ModelScene::sourceFormatFromPath("") == ModelScene::SourceFormat::UNKNOWN);
    CHECK(ModelScene::sourceFormatFromPath("scene") == ModelScene::SourceFormat::UNKNOWN);
    CHECK(ModelScene::sourceFormatFromPath("scene.obj") == ModelScene::SourceFormat::UNKNOWN);
    CHECK(ModelScene::sourceFormatFromPath("scene.fbx.backup") == ModelScene::SourceFormat::UNKNOWN);
}
